<?php
/**
 * Admin-managed templates: upload / rename / delete, each tagged with a board
 * language. Files live in wp-content/uploads/simonsays-board-templates/ and
 * metadata in one option. This is an ADMIN-ONLY surface (capability + nonce
 * on every action) — visitor boards remain fully stateless.
 *
 * Uploaded files are validated server-side before being stored: extension
 * whitelist (.ssc/.ssz), size caps from Settings, the exact .ssc header, and
 * for .ssz the same structural rules the client enforces (one root
 * categories.ssc, flat whitelisted resources/, no traversal), via ZipArchive.
 */

defined( 'ABSPATH' ) || exit;

class SSBE_Templates {

	const OPTION   = 'ssbe_templates';
	const DIR_NAME = 'simonsays-board-templates';
	const SSC_HEADER = 'SIMONSAYS_CATEGORIES_V1';
	const RESOURCE_EXTS = array( 'ico', 'png', 'jpg', 'jpeg', 'wav', 'mp3' );

	public static function dir_path(): string {
		$up = wp_upload_dir();
		return trailingslashit( $up['basedir'] ) . self::DIR_NAME . '/';
	}

	public static function dir_url(): string {
		$up = wp_upload_dir();
		return trailingslashit( $up['baseurl'] ) . self::DIR_NAME . '/';
	}

	/** Custom templates metadata: [{id, name, language, file}]. */
	public static function all(): array {
		$list = get_option( self::OPTION, array() );
		return is_array( $list ) ? $list : array();
	}

	private static function save_all( array $list ): void {
		update_option( self::OPTION, array_values( $list ), false );
	}

	private static function language_codes(): array {
		return array_map( fn( $l ) => $l['code'], SSBE_Embed::languages() );
	}

	private static function redirect_with( string $code ): void {
		wp_safe_redirect( add_query_arg( 'ssbe_msg', $code,
			admin_url( 'options-general.php?page=' . SSBE_Settings::PAGE ) ) );
		exit;
	}

	private static function guard(): void {
		if ( ! current_user_can( 'manage_options' ) ) {
			wp_die( esc_html__( 'Not allowed.', 'simonsays-board-editor' ) );
		}
	}

	// ------------------------------------------------------------------
	// Validation
	// ------------------------------------------------------------------

	private static function is_valid_ssc( string $path ): bool {
		$head = file_get_contents( $path, false, null, 0, 64 );
		if ( false === $head ) {
			return false;
		}
		if ( str_starts_with( $head, "\xEF\xBB\xBF" ) ) {
			$head = substr( $head, 3 );
		}
		$line = strtok( $head, "\r\n" );
		return self::SSC_HEADER === $line;
	}

	private static function is_safe_entry_name( string $name ): bool {
		if ( '' === $name || str_starts_with( $name, '/' ) ) {
			return false;
		}
		if ( str_contains( $name, '..' ) || str_contains( $name, '\\' ) || str_contains( $name, ':' ) ) {
			return false;
		}
		return 1 !== preg_match( '/[\x00-\x1f]/', $name );
	}

	private static function is_valid_ssz( string $path, int $max_file_bytes, int $max_total_bytes ): bool {
		if ( ! class_exists( 'ZipArchive' ) ) {
			return false; // handled separately with a clearer message
		}
		$zip = new ZipArchive();
		if ( true !== $zip->open( $path, ZipArchive::RDONLY ) ) {
			return false;
		}
		$ssc_count = 0;
		$total     = 0;
		$ok        = true;
		for ( $i = 0; $i < $zip->numFiles; $i++ ) {
			$st = $zip->statIndex( $i );
			if ( false === $st ) {
				$ok = false;
				break;
			}
			$name = (string) $st['name'];
			if ( str_ends_with( $name, '/' ) ) {
				continue; // directory placeholder
			}
			if ( ! self::is_safe_entry_name( $name ) ) {
				$ok = false;
				break;
			}
			$total += (int) $st['size'];
			if ( (int) $st['size'] > $max_file_bytes || $total > $max_total_bytes ) {
				$ok = false;
				break;
			}
			if ( 'categories.ssc' === $name ) {
				$ssc_count++;
				continue;
			}
			if ( ! str_starts_with( $name, 'resources/' ) ) {
				$ok = false;
				break;
			}
			$leaf = substr( $name, strlen( 'resources/' ) );
			$ext  = strtolower( (string) pathinfo( $leaf, PATHINFO_EXTENSION ) );
			if ( '' === $leaf || str_contains( $leaf, '/' ) || ! in_array( $ext, self::RESOURCE_EXTS, true ) ) {
				$ok = false;
				break;
			}
		}
		$zip->close();
		return $ok && 1 === $ssc_count;
	}

	// ------------------------------------------------------------------
	// admin-post handlers
	// ------------------------------------------------------------------

	public static function handle_upload() {
		self::guard();
		check_admin_referer( 'ssbe_template_upload' );

		$name = trim( wp_strip_all_tags( (string) ( $_POST['ssbe_tpl_name'] ?? '' ) ) );
		$name = mb_substr( $name, 0, 60 );
		$lang = (string) ( $_POST['ssbe_tpl_language'] ?? 'en' );
		if ( '' === $name || ! in_array( $lang, self::language_codes(), true ) ) {
			self::redirect_with( 'tpl-invalid' );
		}

		$file = $_FILES['ssbe_tpl_file'] ?? null;
		if ( ! $file || UPLOAD_ERR_OK !== (int) $file['error'] || ! is_uploaded_file( $file['tmp_name'] ) ) {
			self::redirect_with( 'tpl-invalid' );
		}

		$max_total = 1024 * (int) SSBE_Settings::get( 'ssbe_max_total_kb' );
		$max_file  = 1024 * (int) SSBE_Settings::get( 'ssbe_max_file_kb' );
		if ( (int) $file['size'] > $max_total ) {
			self::redirect_with( 'tpl-too-large' );
		}

		$ext = strtolower( (string) pathinfo( (string) $file['name'], PATHINFO_EXTENSION ) );
		if ( 'ssc' === $ext ) {
			if ( ! self::is_valid_ssc( $file['tmp_name'] ) ) {
				self::redirect_with( 'tpl-invalid' );
			}
		} elseif ( 'ssz' === $ext ) {
			if ( ! class_exists( 'ZipArchive' ) ) {
				self::redirect_with( 'tpl-no-zip' );
			}
			if ( ! self::is_valid_ssz( $file['tmp_name'], $max_file, $max_total ) ) {
				self::redirect_with( 'tpl-invalid' );
			}
		} else {
			self::redirect_with( 'tpl-invalid' );
		}

		$dir = self::dir_path();
		if ( ! wp_mkdir_p( $dir ) ) {
			self::redirect_with( 'tpl-save-failed' );
		}
		$id       = 'tpl_' . strtolower( wp_generate_password( 10, false, false ) );
		$filename = $id . '.' . $ext;
		if ( ! move_uploaded_file( $file['tmp_name'], $dir . $filename ) ) {
			self::redirect_with( 'tpl-save-failed' );
		}

		$list   = self::all();
		$list[] = array( 'id' => $id, 'name' => $name, 'language' => $lang, 'file' => $filename );
		self::save_all( $list );
		self::redirect_with( 'tpl-uploaded' );
	}

	public static function handle_update() {
		self::guard();
		check_admin_referer( 'ssbe_template_update' );

		$id   = (string) ( $_POST['ssbe_tpl_id'] ?? '' );
		$name = trim( wp_strip_all_tags( (string) ( $_POST['ssbe_tpl_name'] ?? '' ) ) );
		$name = mb_substr( $name, 0, 60 );
		$lang = (string) ( $_POST['ssbe_tpl_language'] ?? 'en' );
		if ( '' === $name || ! in_array( $lang, self::language_codes(), true ) ) {
			self::redirect_with( 'tpl-invalid' );
		}

		$list  = self::all();
		$found = false;
		foreach ( $list as &$tpl ) {
			if ( $tpl['id'] === $id ) {
				$tpl['name']     = $name;
				$tpl['language'] = $lang;
				$found           = true;
				break;
			}
		}
		unset( $tpl );
		if ( ! $found ) {
			self::redirect_with( 'tpl-invalid' );
		}
		self::save_all( $list );
		self::redirect_with( 'tpl-updated' );
	}

	public static function handle_delete() {
		self::guard();
		check_admin_referer( 'ssbe_template_delete' );

		$id   = (string) ( $_POST['ssbe_tpl_id'] ?? '' );
		$list = self::all();
		$kept = array();
		foreach ( $list as $tpl ) {
			if ( $tpl['id'] === $id ) {
				// The stored file name is generated by us (id.ext) — safe to unlink.
				$path = self::dir_path() . $tpl['file'];
				if ( is_file( $path ) ) {
					wp_delete_file( $path );
				}
				continue;
			}
			$kept[] = $tpl;
		}
		self::save_all( $kept );
		self::redirect_with( 'tpl-deleted' );
	}

	// ------------------------------------------------------------------
	// Settings-page UI
	// ------------------------------------------------------------------

	public static function maybe_notice() {
		$code = (string) ( $_GET['ssbe_msg'] ?? '' );
		if ( '' === $code ) {
			return;
		}
		$map = array(
			'tpl-uploaded'    => array( 'success', __( 'Template uploaded.', 'simonsays-board-editor' ) ),
			'tpl-updated'     => array( 'success', __( 'Template updated.', 'simonsays-board-editor' ) ),
			'tpl-deleted'     => array( 'success', __( 'Template deleted.', 'simonsays-board-editor' ) ),
			'tpl-invalid'     => array( 'error', __( 'The template file is not valid.', 'simonsays-board-editor' ) ),
			'tpl-too-large'   => array( 'error', __( 'The template file is too large.', 'simonsays-board-editor' ) ),
			'tpl-save-failed' => array( 'error', __( 'The template could not be saved.', 'simonsays-board-editor' ) ),
			'tpl-no-zip'      => array( 'error', __( 'ZipArchive is not available on this server; only .ssc templates can be uploaded.', 'simonsays-board-editor' ) ),
		);
		if ( ! isset( $map[ $code ] ) ) {
			return;
		}
		printf(
			'<div class="notice notice-%1$s is-dismissible"><p>%2$s</p></div>',
			esc_attr( $map[ $code ][0] ),
			esc_html( $map[ $code ][1] )
		);
	}

	private static function language_select( string $field_name, string $selected ): void {
		echo '<select name="' . esc_attr( $field_name ) . '">';
		foreach ( SSBE_Embed::languages() as $l ) {
			printf(
				'<option value="%1$s" %2$s>%3$s</option>',
				esc_attr( $l['code'] ),
				selected( $selected, $l['code'], false ),
				esc_html( $l['name'] )
			);
		}
		echo '</select>';
	}

	public static function render_admin_section() {
		$action = esc_url( admin_url( 'admin-post.php' ) );
		$langs  = array();
		foreach ( SSBE_Embed::languages() as $l ) {
			$langs[ $l['code'] ] = $l['name'];
		}
		?>
		<h2><?php esc_html_e( 'Templates', 'simonsays-board-editor' ); ?></h2>
		<table class="widefat striped" style="max-width:760px">
			<thead>
				<tr>
					<th><?php esc_html_e( 'Template name', 'simonsays-board-editor' ); ?></th>
					<th><?php esc_html_e( 'Template language', 'simonsays-board-editor' ); ?></th>
					<th><?php esc_html_e( 'Actions', 'simonsays-board-editor' ); ?></th>
				</tr>
			</thead>
			<tbody>
				<?php foreach ( SSBE_Embed::builtin_templates() as $tpl ) : ?>
					<tr>
						<td><?php echo esc_html( $tpl['name'] ); ?></td>
						<td><?php echo esc_html( $langs[ $tpl['language'] ] ?? $tpl['language'] ); ?></td>
						<td><em><?php esc_html_e( 'Built-in', 'simonsays-board-editor' ); ?></em></td>
					</tr>
				<?php endforeach; ?>
				<?php foreach ( self::all() as $tpl ) : ?>
					<tr>
						<form method="post" action="<?php echo $action; ?>">
							<?php wp_nonce_field( 'ssbe_template_update' ); ?>
							<input type="hidden" name="action" value="ssbe_template_update" />
							<input type="hidden" name="ssbe_tpl_id" value="<?php echo esc_attr( $tpl['id'] ); ?>" />
							<td><input type="text" name="ssbe_tpl_name" value="<?php echo esc_attr( $tpl['name'] ); ?>" required maxlength="60" /></td>
							<td><?php self::language_select( 'ssbe_tpl_language', $tpl['language'] ); ?></td>
							<td>
								<button type="submit" class="button"><?php esc_html_e( 'Save', 'simonsays-board-editor' ); ?></button>
						</form>
								<form method="post" action="<?php echo $action; ?>" style="display:inline"
									onsubmit="return confirm('<?php echo esc_js( __( 'Delete this template?', 'simonsays-board-editor' ) ); ?>');">
									<?php wp_nonce_field( 'ssbe_template_delete' ); ?>
									<input type="hidden" name="action" value="ssbe_template_delete" />
									<input type="hidden" name="ssbe_tpl_id" value="<?php echo esc_attr( $tpl['id'] ); ?>" />
									<button type="submit" class="button button-link-delete"><?php esc_html_e( 'Delete', 'simonsays-board-editor' ); ?></button>
								</form>
							</td>
					</tr>
				<?php endforeach; ?>
			</tbody>
		</table>

		<h3><?php esc_html_e( 'Upload template', 'simonsays-board-editor' ); ?></h3>
		<form method="post" action="<?php echo $action; ?>" enctype="multipart/form-data">
			<?php wp_nonce_field( 'ssbe_template_upload' ); ?>
			<input type="hidden" name="action" value="ssbe_template_upload" />
			<p>
				<label><?php esc_html_e( 'Template name', 'simonsays-board-editor' ); ?><br />
					<input type="text" name="ssbe_tpl_name" required maxlength="60" /></label>
			</p>
			<p>
				<label><?php esc_html_e( 'Template language', 'simonsays-board-editor' ); ?><br />
					<?php self::language_select( 'ssbe_tpl_language', 'en' ); ?></label>
			</p>
			<p>
				<label><?php esc_html_e( 'Template file (.ssc or .ssz)', 'simonsays-board-editor' ); ?><br />
					<input type="file" name="ssbe_tpl_file" accept=".ssc,.ssz" required /></label>
			</p>
			<p><button type="submit" class="button button-primary"><?php esc_html_e( 'Upload', 'simonsays-board-editor' ); ?></button></p>
		</form>
		<?php
	}
}
