/**
 * Gutenberg registration for the SimonSays Board Editor block (no build step;
 * classic script). The block is dynamic: the front end renders the editor via
 * the PHP render_callback; inside the post editor only a placeholder shows.
 * Strings arrive pre-localized through SSBE_BLOCK (wp_localize_script).
 */
( function ( blocks, element ) {
	'use strict';
	var data = window.SSBE_BLOCK || { title: 'SimonSays Board Editor', description: '', placeholder: '' };

	blocks.registerBlockType( 'simonsays/board-editor', {
		apiVersion: 2,
		title: data.title,
		description: data.description,
		icon: 'grid-view',
		category: 'widgets',
		supports: { html: false, multiple: false },
		edit: function () {
			return element.createElement(
				'div',
				{ style: { border: '1px dashed #757575', padding: '16px', borderRadius: '4px' } },
				data.placeholder
			);
		},
		save: function () {
			return null; // dynamic block — rendered server-side
		},
	} );
} )( window.wp.blocks, window.wp.element );
