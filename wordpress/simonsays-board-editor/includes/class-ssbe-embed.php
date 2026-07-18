<?php
/**
 * Front-end embedding: Gutenberg block + shortcode, conditional asset
 * enqueueing (only on pages that embed the editor), and the localized data
 * passed to the client app (settings, strings, templates, languages).
 *
 * There are NO server endpoints for boards: the client modules do all the
 * parsing/serializing/zipping in the browser and ARASAAC is called directly
 * from the visitor's browser (CORS-verified).
 */

defined( 'ABSPATH' ) || exit;

class SSBE_Embed {

	const APP_HANDLE   = 'ssbe-app';
	const STYLE_HANDLE = 'ssbe-editor';
	const BLOCK_HANDLE = 'ssbe-block';

	public static function register() {
		wp_register_style( self::STYLE_HANDLE, SSBE_PLUGIN_URL . 'assets/css/editor.css', array(), SSBE_VERSION );
		wp_register_script( self::APP_HANDLE, SSBE_PLUGIN_URL . 'assets/js/app.js', array(), SSBE_VERSION, true );
		wp_register_script( self::BLOCK_HANDLE, SSBE_PLUGIN_URL . 'block/editor.js', array( 'wp-blocks', 'wp-element' ), SSBE_VERSION, true );
		wp_localize_script( self::BLOCK_HANDLE, 'SSBE_BLOCK', array(
			'title'       => __( 'SimonSays Board Editor', 'simonsays-board-editor' ),
			'description' => __( 'In-browser editor for SimonSays communication boards (.ssc / .ssz).', 'simonsays-board-editor' ),
			'placeholder' => __( 'SimonSays Board Editor — shown on the site front end.', 'simonsays-board-editor' ),
		) );

		register_block_type( 'simonsays/board-editor', array(
			'api_version'     => 2,
			'editor_script'   => self::BLOCK_HANDLE,
			'render_callback' => array( __CLASS__, 'render_editor' ),
		) );
		add_shortcode( 'simonsays_board_editor', array( __CLASS__, 'render_editor' ) );

		// The app is a native ES module (imports the format modules directly).
		add_filter( 'script_loader_tag', function ( $tag, $handle ) {
			if ( self::APP_HANDLE === $handle ) {
				$tag = str_replace( ' src=', ' type="module" src=', $tag );
			}
			return $tag;
		}, 10, 2 );
	}

	/** Block/shortcode renderer — the mount node plus the enqueued app. */
	public static function render_editor(): string {
		if ( 'logged_in' === SSBE_Settings::get( 'ssbe_exposure' ) && ! is_user_logged_in() ) {
			return '<p>' . esc_html__( 'Please log in to use the board editor.', 'simonsays-board-editor' ) . '</p>';
		}
		wp_enqueue_style( self::STYLE_HANDLE );
		wp_enqueue_script( self::APP_HANDLE );
		wp_localize_script( self::APP_HANDLE, 'SSBE_DATA', self::client_data() );
		return '<div class="ssbe-mount"></div>';
	}

	public static function attribution_text(): string {
		// Fixed credit line (proper nouns; deliberately not translated). Must
		// never contain ';' or '|' — it is appended to the board's `credits`.
		return 'Pictograms: Sergio Palao / ARASAAC (CC BY-NC-SA) - Government of Aragon';
	}

	/** Bundled, non-deletable templates. */
	public static function builtin_templates(): array {
		return array(
			array( 'id' => 'blank', 'name' => __( 'Blank board', 'simonsays-board-editor' ), 'language' => 'en', 'url' => SSBE_PLUGIN_URL . 'templates/blank.ssc' ),
			array( 'id' => 'spc', 'name' => __( 'SPC adult board (Spanish)', 'simonsays-board-editor' ), 'language' => 'es', 'url' => SSBE_PLUGIN_URL . 'templates/spc-adult.ssc' ),
		);
	}

	/** Built-in + admin-uploaded templates, as delivered to the client. */
	public static function templates(): array {
		$list = self::builtin_templates();
		foreach ( SSBE_Templates::all() as $tpl ) {
			$list[] = array(
				'id'       => $tpl['id'],
				'name'     => $tpl['name'],
				'language' => $tpl['language'],
				'url'      => SSBE_Templates::dir_url() . $tpl['file'],
			);
		}
		return $list;
	}

	/** Board content languages (drive preview direction + export filename). */
	public static function languages(): array {
		return array(
			array( 'code' => 'en', 'name' => __( 'English', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'es', 'name' => __( 'Spanish', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'ca', 'name' => __( 'Catalan', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'val', 'name' => __( 'Valencian', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'eu', 'name' => __( 'Basque', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'gl', 'name' => __( 'Galician', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'fr', 'name' => __( 'French', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'de', 'name' => __( 'German', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'it', 'name' => __( 'Italian', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'pt', 'name' => __( 'Portuguese', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'ru', 'name' => __( 'Russian', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'ar', 'name' => __( 'Arabic', 'simonsays-board-editor' ), 'rtl' => true ),
			array( 'code' => 'he', 'name' => __( 'Hebrew', 'simonsays-board-editor' ), 'rtl' => true ),
			array( 'code' => 'zh', 'name' => __( 'Chinese (Simplified)', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'ja', 'name' => __( 'Japanese', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'ko', 'name' => __( 'Korean', 'simonsays-board-editor' ), 'rtl' => false ),
			array( 'code' => 'hi', 'name' => __( 'Hindi', 'simonsays-board-editor' ), 'rtl' => false ),
		);
	}

	private static function client_data(): array {
		return array(
			'strings'   => self::strings(),
			'settings'  => array(
				'maxResourceBytes' => 1024 * (int) SSBE_Settings::get( 'ssbe_max_file_kb' ),
				'maxTotalBytes'    => 1024 * (int) SSBE_Settings::get( 'ssbe_max_total_kb' ),
				'arasaac'          => array(
					'enabled'     => (bool) SSBE_Settings::get( 'ssbe_arasaac_enabled' ) && SSBE_Activation::is_affirmed(),
					'locale'      => (string) SSBE_Settings::get( 'ssbe_arasaac_locale' ),
					'attribution' => self::attribution_text(),
				),
				'defaultTemplate'  => (string) SSBE_Settings::get( 'ssbe_default_template' ),
			),
			'templates' => self::templates(),
			'baseUrl'   => SSBE_PLUGIN_URL,
			'languages' => self::languages(),
		);
	}

	/**
	 * Every user-visible editor string, keyed for the JS layer. Reused
	 * sentences share one msgid so the translation catalogs stay small.
	 */
	private static function strings(): array {
		$d = 'simonsays-board-editor';

		$reserved      = __( 'This text contains a character reserved by the board format (| ## :: = ♫).', $d );
		$empty         = __( 'This field cannot be empty.', $d );
		$bad_icon_ext  = __( 'Icons must be emoji or an .ico/.png/.jpg file name.', $d );
		$bad_audio_ext = __( 'Audio must be a .wav or .mp3 file name.', $d );
		$file_too_big  = __( 'The file exceeds the maximum size allowed by this site.', $d );
		$total_too_big = __( 'The board exceeds the maximum total size allowed by this site.', $d );
		$bad_bundle    = __( 'The file was rejected: it is not a valid SimonSays board bundle.', $d );
		$unsafe_bundle = __( 'The file was rejected: it contains unsafe or unexpected entries.', $d );
		$dangling      = __( 'The referenced file is not among the board resources.', $d );

		return array(
			// Toolbar & layout
			'toolbar-label'             => __( 'Board editor toolbar', $d ),
			'tb-new'                    => __( 'New', $d ),
			'tb-open'                   => __( 'Open…', $d ),
			'tb-export'                 => __( 'Export', $d ),
			'tb-export-ssc'             => __( 'Export .ssc', $d ),
			'tb-export-ssz'             => __( 'Export .ssz', $d ),
			'tb-validate'               => __( 'Validate', $d ),
			'template-label'            => __( 'Template', $d ),
			'language-label'            => __( 'Board language', $d ),
			'panel-preview'             => __( 'Live preview', $d ),
			'preview-hint'              => __( 'Phrases', $d ),

			// Panels
			'panel-categories'          => __( 'Categories', $d ),
			'panel-phrases'             => __( 'Phrases', $d ),
			'panel-board-style'         => __( 'Board style', $d ),
			'panel-category-style'      => __( 'Selected category style', $d ),
			'panel-resources'           => __( 'Resources (icons & sounds)', $d ),
			'panel-arasaac'             => __( 'ARASAAC pictograms', $d ),
			'panel-findings'            => __( 'Validation results', $d ),
			'findings-none'             => __( 'No problems found.', $d ),

			// Category / phrase editing
			'category-name'             => __( 'Category name', $d ),
			'add-category'              => __( 'Add category', $d ),
			'delete-category'           => __( 'Delete category', $d ),
			'confirm-delete-category'   => __( 'Delete this category and all its phrases?', $d ),
			'new-category-name'         => __( 'New category', $d ),
			'phrase-text'               => __( 'Phrase text', $d ),
			'add-phrase'                => __( 'Add phrase', $d ),
			'delete-phrase'             => __( 'Delete phrase', $d ),
			'new-phrase-text'           => __( 'New phrase', $d ),
			'move-up'                   => __( 'Move up', $d ),
			'move-down'                 => __( 'Move down', $d ),
			'icon-placeholder'          => __( 'emoji or file.png', $d ),
			'audio-placeholder'         => __( 'sound.mp3', $d ),

			// Style forms
			'style-scope-window'        => __( 'Window', $d ),
			'style-scope-categories'    => __( 'All category buttons', $d ),
			'style-scope-phrases'       => __( 'Phrase buttons', $d ),
			'style-scope-own'           => __( 'This category\'s button', $d ),
			'style-unset'               => __( '(not set)', $d ),
			'style-raw-label'           => __( 'Raw style list (advanced)', $d ),
			'color-picker-for'          => __( 'Color picker for', $d ),

			// Resources
			'add-resources'             => __( 'Add files (.ico .png .jpg .wav .mp3)', $d ),
			'delete-resource'           => __( 'Remove resource', $d ),
			'res-used'                  => __( 'used', $d ),
			'res-unused'                => __( 'unused', $d ),
			'confirm-replace-resource'  => __( 'A resource with this name already exists. Replace it?', $d ),

			// Icon pickers & renaming
			'emoji-picker-open'         => __( 'Insert an emoji', $d ),
			'pictogram-picker-open'     => __( 'Choose a pictogram for this icon', $d ),
			'arasaac-target'            => __( 'Assigning to:', $d ),
			'arasaac-target-category'   => __( 'category icon', $d ),
			'arasaac-target-phrase'     => __( 'phrase icon', $d ),
			'rename-resource'           => __( 'Rename resource', $d ),
			'prompt-new-name'           => __( 'New name (the extension is kept):', $d ),
			'err-name-exists'           => __( 'That name is already in use.', $d ),
			'err-speech-unavailable'    => __( 'Speech synthesis is not available in this browser.', $d ),

			// ARASAAC
			'arasaac-search'            => __( 'Search', $d ),
			'arasaac-search-placeholder' => __( 'Search pictograms…', $d ),
			'arasaac-searching'         => __( 'Searching…', $d ),
			'arasaac-no-results'        => __( 'No pictograms found.', $d ),
			'arasaac-error'             => __( 'The ARASAAC service could not be reached.', $d ),
			'arasaac-added'             => __( 'Pictogram added to the board resources:', $d ),
			'arasaac-disabled'          => __( 'The ARASAAC pictogram search is disabled on this site.', $d ),

			// Lifecycle / dialogs
			'confirm-discard'           => __( 'Discard the current board and its unsaved changes?', $d ),
			'confirm-recover'           => __( 'An unsaved board from a previous session was found. Recover it?', $d ),

			// Generic + field errors
			'err-invalid'               => __( 'Invalid value.', $d ),
			'err-empty'                 => $empty,
			'err-reserved-prefix'       => __( 'Names starting with $$ are reserved by the board format.', $d ),
			'err-char-pipe'             => $reserved,
			'err-char-icon-sep'         => $reserved,
			'err-char-audio-sep'        => $reserved,
			'err-char-equals'           => $reserved,
			'err-char-note'             => $reserved,
			'err-icon-char-pipe'        => $reserved,
			'err-icon-char-icon-sep'    => $reserved,
			'err-icon-char-audio-sep'   => $reserved,
			'err-icon-bad-extension'    => $bad_icon_ext,
			'err-audio-bad-extension'   => $bad_audio_ext,
			'err-style-char-pipe'       => __( 'Style text cannot contain the | character.', $d ),
			'err-style-char-semicolon'  => __( 'Style text cannot contain the ; character.', $d ),

			// Import / export
			'err-import'                => __( 'The file could not be imported.', $d ),
			'err-export'                => __( 'The board could not be exported.', $d ),
			'err-fix-before-export'     => __( 'Fix the validation errors before exporting.', $d ),
			'err-template-load'         => __( 'The template could not be loaded.', $d ),
			'err-ssc-bad-header'        => __( 'This is not a SimonSays board file (missing header).', $d ),
			'err-resource-bad-extension' => __( 'Only .ico, .png, .jpg, .wav and .mp3 files can be added.', $d ),
			'err-resource-too-large'    => $file_too_big,
			'err-content-mismatch'      => __( 'The file content does not match its extension.', $d ),
			'err-audio-missing'         => $dangling,
			'err-audio-play'            => __( 'The sound could not be played.', $d ),

			// Bundle (.ssz) rejection codes
			'err-ssz-not-a-zip'         => $bad_bundle,
			'err-ssz-corrupt'           => $bad_bundle,
			'err-ssz-zip64-unsupported' => $bad_bundle,
			'err-ssz-unsupported-method' => $bad_bundle,
			'err-ssz-entry-count'       => $bad_bundle,
			'err-ssz-no-ssc'            => $bad_bundle,
			'err-ssz-multiple-ssc'      => $bad_bundle,
			'err-ssz-extract-failed'    => $bad_bundle,
			'err-ssz-unsafe-name'       => $unsafe_bundle,
			'err-ssz-symlink'           => $unsafe_bundle,
			'err-ssz-nested-resource'   => $unsafe_bundle,
			'err-ssz-unexpected-entry'  => $unsafe_bundle,
			'err-ssz-duplicate-resource' => $unsafe_bundle,
			'err-ssz-ratio'             => $unsafe_bundle,
			'err-ssz-bad-extension'     => __( 'The bundle contains a resource type that is not allowed.', $d ),
			'err-ssz-content-mismatch'  => __( 'The file content does not match its extension.', $d ),
			'err-ssz-entry-too-large'   => $file_too_big,
			'err-ssz-resource-too-large' => $file_too_big,
			'err-ssz-total-too-large'   => $total_too_big,

			// Validation findings
			'finding-empty-board'       => __( 'The board has no categories yet.', $d ),
			'finding-empty-category'    => __( 'This category has no phrases.', $d ),
			'finding-category-name-empty' => $empty,
			'finding-category-name-duplicate' => __( 'Two categories share the same name.', $d ),
			'finding-category-name-reserved-prefix' => __( 'Names starting with $$ are reserved by the board format.', $d ),
			'finding-category-name-char-pipe' => $reserved,
			'finding-category-name-char-icon-sep' => $reserved,
			'finding-category-name-char-audio-sep' => $reserved,
			'finding-category-name-char-equals' => $reserved,
			'finding-category-name-char-note' => $reserved,
			'finding-phrase-text-empty' => $empty,
			'finding-phrase-text-char-pipe' => $reserved,
			'finding-phrase-text-char-icon-sep' => $reserved,
			'finding-phrase-text-char-audio-sep' => $reserved,
			'finding-icon-char-pipe'    => $reserved,
			'finding-icon-char-icon-sep' => $reserved,
			'finding-icon-char-audio-sep' => $reserved,
			'finding-icon-bad-extension' => $bad_icon_ext,
			'finding-audio-bad-extension' => $bad_audio_ext,
			'finding-dangling-icon'     => $dangling,
			'finding-dangling-audio'    => $dangling,
			'finding-resource-bad-extension' => __( 'Only .ico, .png, .jpg, .wav and .mp3 files can be added.', $d ),
			'finding-resource-too-large' => $file_too_big,
			'finding-unreferenced-resource' => __( 'This resource is not referenced by any category or phrase (it will not be exported).', $d ),
			'finding-total-too-large'   => $total_too_big,
		);
	}
}
