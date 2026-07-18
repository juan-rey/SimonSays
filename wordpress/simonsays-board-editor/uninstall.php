<?php
/**
 * Uninstall cleanup. The plugin stores no board data — only these options.
 */

defined( 'WP_UNINSTALL_PLUGIN' ) || exit;

$ssbe_options = array(
	'ssbe_exposure',
	'ssbe_arasaac_enabled',
	'ssbe_arasaac_locale',
	'ssbe_max_file_kb',
	'ssbe_max_total_kb',
	'ssbe_default_template',
	'ssbe_needs_onboarding',
	'ssbe_arasaac_affirmation',
);

foreach ( $ssbe_options as $ssbe_option ) {
	delete_option( $ssbe_option );
}
