<?php
/**
 * ARASAAC license gate (asked on install).
 *
 * ARASAAC pictograms are CC BY-NC-SA (owner: Government of Aragón; author:
 * Sergio Palao). The pictogram search stays disabled until the site owner
 * explicitly affirms compliant (non-commercial) use. The affirmation — who
 * and when — is stored and shown in Settings, where it can also be revoked.
 */

defined( 'ABSPATH' ) || exit;

class SSBE_Activation {

	const OPTION_ONBOARDING  = 'ssbe_needs_onboarding';
	const OPTION_AFFIRMATION = 'ssbe_arasaac_affirmation';
	const TERMS_URL          = 'https://arasaac.org/terms-of-use';

	public static function on_activate() {
		if ( ! get_option( self::OPTION_AFFIRMATION ) ) {
			update_option( self::OPTION_ONBOARDING, 1, false );
		}
	}

	public static function is_affirmed(): bool {
		$a = get_option( self::OPTION_AFFIRMATION );
		return is_array( $a ) && ! empty( $a['time'] );
	}

	public static function affirmation(): ?array {
		$a = get_option( self::OPTION_AFFIRMATION );
		return is_array( $a ) ? $a : null;
	}

	/** First-activation onboarding notice with the explicit affirmation form. */
	public static function maybe_show_onboarding() {
		if ( ! get_option( self::OPTION_ONBOARDING ) || ! current_user_can( 'manage_options' ) ) {
			return;
		}
		$action = esc_url( admin_url( 'admin-post.php' ) );
		?>
		<div class="notice notice-info">
			<h2><?php esc_html_e( 'SimonSays Board Editor — ARASAAC pictograms', 'simonsays-board-editor' ); ?></h2>
			<p>
				<?php esc_html_e( 'The editor can search and insert ARASAAC pictograms. ARASAAC pictograms are licensed CC BY-NC-SA (owner: Government of Aragón; author: Sergio Palao): attribution is mandatory and commercial use is not permitted by that license.', 'simonsays-board-editor' ); ?>
				<a href="<?php echo esc_url( self::TERMS_URL ); ?>" target="_blank" rel="noopener noreferrer"><?php esc_html_e( 'Read the ARASAAC terms of use.', 'simonsays-board-editor' ); ?></a>
			</p>
			<form method="post" action="<?php echo $action; ?>">
				<?php wp_nonce_field( 'ssbe_affirm_license' ); ?>
				<input type="hidden" name="action" value="ssbe_affirm_license" />
				<p>
					<label>
						<input type="checkbox" name="ssbe_affirm" value="1" required />
						<?php esc_html_e( 'I affirm that the use of ARASAAC pictograms on this site complies with their license (including the non-commercial condition).', 'simonsays-board-editor' ); ?>
					</label>
				</p>
				<p>
					<button type="submit" class="button button-primary"><?php esc_html_e( 'Enable ARASAAC pictogram search', 'simonsays-board-editor' ); ?></button>
					<button type="submit" class="button" name="ssbe_skip" value="1" formnovalidate><?php esc_html_e( 'Skip — keep pictogram search disabled', 'simonsays-board-editor' ); ?></button>
				</p>
			</form>
		</div>
		<?php
	}

	public static function handle_affirmation() {
		if ( ! current_user_can( 'manage_options' ) ) {
			wp_die( esc_html__( 'Not allowed.', 'simonsays-board-editor' ) );
		}
		check_admin_referer( 'ssbe_affirm_license' );

		if ( empty( $_POST['ssbe_skip'] ) && ! empty( $_POST['ssbe_affirm'] ) ) {
			$user = wp_get_current_user();
			update_option( self::OPTION_AFFIRMATION, array(
				'user_id' => $user->ID,
				'login'   => $user->user_login,
				'time'    => time(),
			), false );
		}
		delete_option( self::OPTION_ONBOARDING );
		wp_safe_redirect( wp_get_referer() ?: admin_url() );
		exit;
	}

	public static function handle_revoke() {
		if ( ! current_user_can( 'manage_options' ) ) {
			wp_die( esc_html__( 'Not allowed.', 'simonsays-board-editor' ) );
		}
		check_admin_referer( 'ssbe_revoke_license' );
		delete_option( self::OPTION_AFFIRMATION );
		wp_safe_redirect( wp_get_referer() ?: admin_url() );
		exit;
	}
}
