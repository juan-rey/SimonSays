<?php
/**
 * Settings page (admin-only): exposure, ARASAAC, size limits, default
 * template. Boards are never stored server-side — these options are the
 * plugin's only persistent state (see uninstall.php).
 */

defined( 'ABSPATH' ) || exit;

class SSBE_Settings {

	const PAGE = 'ssbe-settings';

	public static function defaults(): array {
		return array(
			'ssbe_exposure'         => 'public',   // public | logged_in
			'ssbe_arasaac_enabled'  => 1,
			'ssbe_arasaac_locale'   => 'en',
			'ssbe_max_file_kb'      => 5120,       // per resource file
			'ssbe_max_total_kb'     => 51200,      // whole bundle
			'ssbe_default_template' => 'blank',
		);
	}

	public static function get( string $key ) {
		$defaults = self::defaults();
		return get_option( $key, $defaults[ $key ] ?? null );
	}

	public static function register_menu() {
		add_options_page(
			__( 'SimonSays Board Editor', 'simonsays-board-editor' ),
			__( 'SimonSays Boards', 'simonsays-board-editor' ),
			'manage_options',
			self::PAGE,
			array( __CLASS__, 'render_page' )
		);
	}

	public static function register_settings() {
		register_setting( self::PAGE, 'ssbe_exposure', array(
			'type'              => 'string',
			'sanitize_callback' => fn( $v ) => in_array( $v, array( 'public', 'logged_in' ), true ) ? $v : 'public',
			'default'           => 'public',
		) );
		register_setting( self::PAGE, 'ssbe_arasaac_enabled', array(
			'type'              => 'boolean',
			'sanitize_callback' => fn( $v ) => $v ? 1 : 0,
			'default'           => 1,
		) );
		register_setting( self::PAGE, 'ssbe_arasaac_locale', array(
			'type'              => 'string',
			'sanitize_callback' => fn( $v ) => preg_match( '/^[a-z]{2,3}$/', (string) $v ) ? $v : 'en',
			'default'           => 'en',
		) );
		register_setting( self::PAGE, 'ssbe_max_file_kb', array(
			'type'              => 'integer',
			'sanitize_callback' => fn( $v ) => max( 16, min( 65536, (int) $v ) ),
			'default'           => 5120,
		) );
		register_setting( self::PAGE, 'ssbe_max_total_kb', array(
			'type'              => 'integer',
			'sanitize_callback' => fn( $v ) => max( 64, min( 262144, (int) $v ) ),
			'default'           => 51200,
		) );
		register_setting( self::PAGE, 'ssbe_default_template', array(
			'type'              => 'string',
			'sanitize_callback' => fn( $v ) => preg_match( '/^[a-z0-9\-]{1,40}$/', (string) $v ) ? $v : 'blank',
			'default'           => 'blank',
		) );
	}

	public static function render_page() {
		if ( ! current_user_can( 'manage_options' ) ) {
			return;
		}
		$affirmation = SSBE_Activation::affirmation();
		$templates   = SSBE_Embed::templates();
		?>
		<div class="wrap">
			<h1><?php esc_html_e( 'SimonSays Board Editor', 'simonsays-board-editor' ); ?></h1>
			<?php SSBE_Templates::maybe_notice(); ?>
			<p>
				<?php esc_html_e( 'The editor is stateless: boards are created, edited and downloaded entirely in the visitor\'s browser, and are never uploaded to or stored on this site.', 'simonsays-board-editor' ); ?>
				<?php esc_html_e( 'Embed it with the "SimonSays Board Editor" block or the [simonsays_board_editor] shortcode.', 'simonsays-board-editor' ); ?>
			</p>
			<form method="post" action="options.php">
				<?php settings_fields( self::PAGE ); ?>
				<table class="form-table" role="presentation">
					<tr>
						<th scope="row"><label for="ssbe_exposure"><?php esc_html_e( 'Who can use the editor', 'simonsays-board-editor' ); ?></label></th>
						<td>
							<select id="ssbe_exposure" name="ssbe_exposure">
								<option value="public" <?php selected( self::get( 'ssbe_exposure' ), 'public' ); ?>><?php esc_html_e( 'Everyone (anonymous visitors included)', 'simonsays-board-editor' ); ?></option>
								<option value="logged_in" <?php selected( self::get( 'ssbe_exposure' ), 'logged_in' ); ?>><?php esc_html_e( 'Logged-in users only', 'simonsays-board-editor' ); ?></option>
							</select>
						</td>
					</tr>
					<tr>
						<th scope="row"><?php esc_html_e( 'ARASAAC pictogram search', 'simonsays-board-editor' ); ?></th>
						<td>
							<label>
								<input type="checkbox" name="ssbe_arasaac_enabled" value="1" <?php checked( self::get( 'ssbe_arasaac_enabled' ) ); ?> <?php disabled( ! $affirmation ); ?> />
								<?php esc_html_e( 'Enable the ARASAAC pictogram panel in the editor', 'simonsays-board-editor' ); ?>
							</label>
							<p class="description">
								<?php if ( $affirmation ) : ?>
									<?php
									printf(
										/* translators: 1: user login, 2: date */
										esc_html__( 'License compliance affirmed by %1$s on %2$s.', 'simonsays-board-editor' ),
										esc_html( $affirmation['login'] ),
										esc_html( wp_date( get_option( 'date_format' ), (int) $affirmation['time'] ) )
									);
									?>
									<a href="<?php echo esc_url( wp_nonce_url( admin_url( 'admin-post.php?action=ssbe_revoke_license' ), 'ssbe_revoke_license' ) ); ?>"><?php esc_html_e( 'Revoke affirmation', 'simonsays-board-editor' ); ?></a>
								<?php else : ?>
									<?php esc_html_e( 'Disabled until the ARASAAC license affirmation is completed (see the activation notice on the Plugins screen, or reactivate the plugin).', 'simonsays-board-editor' ); ?>
								<?php endif; ?>
							</p>
							<p class="description"><?php echo esc_html( SSBE_Embed::attribution_text() ); ?> <em>(<?php esc_html_e( 'attribution is always shown and cannot be disabled', 'simonsays-board-editor' ); ?>)</em></p>
						</td>
					</tr>
					<tr>
						<th scope="row"><label for="ssbe_arasaac_locale"><?php esc_html_e( 'Default ARASAAC search language', 'simonsays-board-editor' ); ?></label></th>
						<td><input id="ssbe_arasaac_locale" name="ssbe_arasaac_locale" type="text" class="small-text" value="<?php echo esc_attr( self::get( 'ssbe_arasaac_locale' ) ); ?>" pattern="[a-z]{2,3}" /> <span class="description"><?php esc_html_e( 'ISO code, e.g. en, es, fr, it, de', 'simonsays-board-editor' ); ?></span></td>
					</tr>
					<tr>
						<th scope="row"><label for="ssbe_max_file_kb"><?php esc_html_e( 'Maximum size per resource file (KB)', 'simonsays-board-editor' ); ?></label></th>
						<td><input id="ssbe_max_file_kb" name="ssbe_max_file_kb" type="number" min="16" max="65536" value="<?php echo esc_attr( self::get( 'ssbe_max_file_kb' ) ); ?>" /></td>
					</tr>
					<tr>
						<th scope="row"><label for="ssbe_max_total_kb"><?php esc_html_e( 'Maximum total board size (KB)', 'simonsays-board-editor' ); ?></label></th>
						<td><input id="ssbe_max_total_kb" name="ssbe_max_total_kb" type="number" min="64" max="262144" value="<?php echo esc_attr( self::get( 'ssbe_max_total_kb' ) ); ?>" /></td>
					</tr>
					<tr>
						<th scope="row"><label for="ssbe_default_template"><?php esc_html_e( 'Default template', 'simonsays-board-editor' ); ?></label></th>
						<td>
							<select id="ssbe_default_template" name="ssbe_default_template">
								<?php foreach ( $templates as $tpl ) : ?>
									<option value="<?php echo esc_attr( $tpl['id'] ); ?>" <?php selected( self::get( 'ssbe_default_template' ), $tpl['id'] ); ?>><?php echo esc_html( $tpl['name'] ); ?></option>
								<?php endforeach; ?>
							</select>
						</td>
					</tr>
				</table>
				<?php submit_button(); ?>
			</form>
			<?php SSBE_Templates::render_admin_section(); ?>
		</div>
		<?php
	}
}
