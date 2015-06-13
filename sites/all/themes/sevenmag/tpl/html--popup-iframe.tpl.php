<?php

/**
 * @file html.tpl.php
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 * @see nucleus_preprocess_html()
 */
?>
<!doctype html>
<!--[if IE 8 ]><html class="ie8" lang="<?php print $language->language; ?>"><![endif]-->
<!--[if IE 9 ]><html class="ie9" lang="<?php print $language->language; ?>"><![endif]-->
<!--[if (gte IE 10)|!(IE)]><!-->
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>">
<!--<![endif]-->
<head>
<title><?php print $head_title; ?></title>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1" />
<?php print $styles; ?><?php print $scripts; ?><?php print $head; ?>
<!--[if IE]>
		<meta http-equiv="X-UA-Compatible" content="IE=9; IE=8; IE=EmulateIE8; IE=EDGE" />
		<script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
	<![endif]-->
<?php
	//Tracking code
	$tracking_code = theme_get_setting('general_setting_tracking_code', 'sevenmag');
	print $tracking_code;
	//Custom css
	$custom_css = theme_get_setting('custom_css', 'sevenmag');
	if(!empty($custom_css)):
?>
	<style type="text/css" media="all">
	<?php print $custom_css;
	 ?>
	</style>
<?php
	endif;
	
	//color scheme
	$color_scheme = theme_get_setting('color_scheme', 'sevenmag');
	if(isset($color_scheme) && !empty($color_scheme)){
?>
	<style type="text/css" media="all">
		a:hover, .sf-menu li li:hover > a, .sf-menu li li > a:hover, #block_carousel .details a:hover, .wgr .details a:hover, .sf-menu li li.current_page_item > a, .sf-menu li li.current-menu-item > a, .sec_head .sf-menu li a:hover, .sf-menu > li:hover > a, .sf-menu > li > a:hover, .post_meta a:hover, .widget_archive li.current a, .widget_categories li.current a, .widget_nav_menu li.current a, .widget_meta li.current a, .widget_pages li.current a, .widget_archive li:hover a, .widget_pages li:hover a, .widget_meta li:hover a, .widget_nav_menu li:hover a, .widget_categories li:hover a, #footer .sf-menu a:hover, #footer .sf-menu .current-menu-item a, #footer .sf-menu .current_page_item a, #header .search button:hover, #footer a:hover, .r_post .more_meta a:hover, .r_post .s_category a:hover, .sf-menu li.current_page_item > a, .sf-menu li.current-menu-item > a, .medium_thumb .s_category a:hover {
	color: <?php print $color_scheme;?>!important;
}
	</style>
<?php
	}
	$layout = 'full';
	$layout_temp = theme_get_setting('layout', 'sevenmag');
	if(!empty($layout_temp))
		$layout = $layout_temp;

?>
</head>
<body class="iframe <?php print $classes;?>"  <?php print $attributes;?>>

<div id="layout" class="<?php print $layout;?>"><?php print $page; ?></div>
<?php include_once('switcher.tpl.php');?>

</body>
</html>
