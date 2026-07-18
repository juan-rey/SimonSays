<?php
/**
 * Plugin Name:       SimonSays Board Editor
 * Description:       Stateless in-browser editor for SimonSays communication boards (.ssc / .ssz) with live preview, templates and ARASAAC pictogram search. Board files never leave the visitor's browser.
 * Version:           0.1.0
 * Requires at least: 6.0
 * Requires PHP:      8.0
 * Author:            Juan Rey
 * License:           GPL-2.0-or-later
 * License URI:       https://www.gnu.org/licenses/gpl-2.0.html
 * Text Domain:       simonsays-board-editor
 * Domain Path:       /languages
 */

defined( 'ABSPATH' ) || exit;

define( 'SSBE_VERSION', '0.1.0' );
define( 'SSBE_PLUGIN_FILE', __FILE__ );
define( 'SSBE_PLUGIN_DIR', plugin_dir_path( __FILE__ ) );
define( 'SSBE_PLUGIN_URL', plugin_dir_url( __FILE__ ) );

require_once SSBE_PLUGIN_DIR . 'includes/class-ssbe-activation.php';
require_once SSBE_PLUGIN_DIR . 'includes/class-ssbe-settings.php';
require_once SSBE_PLUGIN_DIR . 'includes/class-ssbe-templates.php';
require_once SSBE_PLUGIN_DIR . 'includes/class-ssbe-embed.php';

register_activation_hook( __FILE__, array( 'SSBE_Activation', 'on_activate' ) );

add_action( 'plugins_loaded', function () {
	load_plugin_textdomain( 'simonsays-board-editor', false, dirname( plugin_basename( __FILE__ ) ) . '/languages' );
} );

add_action( 'init', array( 'SSBE_Embed', 'register' ) );
add_action( 'admin_menu', array( 'SSBE_Settings', 'register_menu' ) );
add_action( 'admin_init', array( 'SSBE_Settings', 'register_settings' ) );
add_action( 'admin_notices', array( 'SSBE_Activation', 'maybe_show_onboarding' ) );
add_action( 'admin_post_ssbe_affirm_license', array( 'SSBE_Activation', 'handle_affirmation' ) );
add_action( 'admin_post_ssbe_revoke_license', array( 'SSBE_Activation', 'handle_revoke' ) );
add_action( 'admin_post_ssbe_template_upload', array( 'SSBE_Templates', 'handle_upload' ) );
add_action( 'admin_post_ssbe_template_update', array( 'SSBE_Templates', 'handle_update' ) );
add_action( 'admin_post_ssbe_template_delete', array( 'SSBE_Templates', 'handle_delete' ) );
