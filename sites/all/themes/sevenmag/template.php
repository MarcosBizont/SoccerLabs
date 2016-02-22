<?php 
global $base_url;

function sevenmag_preprocess_html(&$variables) {
	//drupal_add_css('http://fonts.googleapis.com/css?family=Open+Sans:300,300italic,400,400italic,600,600italic,700,700italic,800,800italic', array('type' => 'external'));
	
	
	if (isset($_GET['as_iframe']) && $_GET['as_iframe'] == 1) {
		$variables['theme_hook_suggestions'][] = 'html__popup_iframe';
	}
	
	
	drupal_add_css(base_path().path_to_theme().'/css/symple_shortcodes_styles.css', array('type' => 'external'));
	drupal_add_css(base_path().path_to_theme().'/css/style.css', array('type' => 'external'));
	drupal_add_css(base_path().path_to_theme().'/css/stylev2.css', array('type' => 'external'));
	
	
	//drupal_add_css(base_path().path_to_theme().'/css/style.css', array('type' => 'external'));
	drupal_add_css(base_path().path_to_theme().'/css/icons.css', array('type' => 'external'));
	drupal_add_css(base_path().path_to_theme().'/css/animate.css', array('type' => 'external'));
	drupal_add_css(base_path().path_to_theme().'/css/responsive.css', array('type' => 'external'));
	drupal_add_css('//fonts.googleapis.com/css?family=Montserrat', array('type' => 'external'));
	
	//RTL version
	$styling = theme_get_setting('styling', 'sevenmag');
	if($styling=='rtl')
		drupal_add_css(base_path().path_to_theme().'/css/rtl.css', array('type' => 'external'));
	//End RTL Version
	
	//Version background
	$version = theme_get_setting('version_c', 'sevenmag');
	if($version=='dark')
		drupal_add_css(base_path().path_to_theme().'/css/dark.css', array('type' => 'external'));
	
	drupal_add_css(base_path().path_to_theme().'/css/update.css', array('type' => 'external'));

	//JS Footer
	drupal_add_js(path_to_theme().'/js/jquery.js', array('type' => 'file', 'scope' => 'footer'));
	drupal_add_js(path_to_theme().'/js/seven.min.js', array('type' => 'file', 'scope' => 'footer'));
	drupal_add_js(path_to_theme().'/js/owl.carousel.min.js', array('type' => 'file', 'scope' => 'footer'));
	
	if($styling=='rtl') //RTL version
		drupal_add_js(path_to_theme().'/js/jquery.li-scroller-rtl.1.0.js', array('type' => 'file', 'scope' => 'footer'));
	else //LTR Version
		drupal_add_js(path_to_theme().'/js/jquery.li-scroller.1.0.js', array('type' => 'file', 'scope' => 'footer'));
	
	drupal_add_js(path_to_theme().'/js/jquery.fitvids.js', array('type' => 'file', 'scope' => 'footer'));
	
	drupal_add_js(path_to_theme().'/js/custom.js', array('type' => 'file', 'scope' => 'footer'));
	drupal_add_js(path_to_theme().'/js/update.js', array('type' => 'file', 'scope' => 'footer'));
	
}



function sevenmag_form_comment_form_alter(&$form, &$form_state) {
  $form['comment_body']['#after_build'][] = 'sevenmag_customize_comment_form';
}

function sevenmag_customize_comment_form(&$form) {
  $form[LANGUAGE_NONE][0]['format']['#access'] = FALSE;
  return $form;
}

function sevenmag_preprocess_page(&$vars) {
	
	if (isset($vars['node'])) {  
		$vars['theme_hook_suggestions'][] = 'page__'. $vars['node']->type;
	}
	if (isset($_GET['as_iframe']) && $_GET['as_iframe'] == 1) {
		$vars['theme_hook_suggestions'][] = 'page__popup_iframe';
	}
	
	//404 page
	$status = drupal_get_http_header("status");  
	if($status == "404 Not Found") {      
		$vars['theme_hook_suggestions'][] = 'page__404';
	}
	if (isset($vars['node'])) :
        if($vars['node']->type == 'page'):
            $node = node_load($vars['node']->nid);
            $output = field_view_field('node', $node, 'field_show_page_title', array('label' => 'hidden'));
            $vars['field_show_page_title'] = $output;
			//sidebar
			$output = field_view_field('node', $node, 'field_sidebar', array('label' => 'hidden'));
            $vars['field_sidebar'] = $output;
        endif;
    endif;
	
	
}


// Remove superfish css files.
function sevenmag_css_alter(&$css) {
	unset($css[drupal_get_path('module', 'system') . '/system.menus.css']);
	unset($css[drupal_get_path('module', 'system') . '/system.theme.css']);
	
//	unset($css[drupal_get_path('module', 'system') . '/system.base.css']);
}

function sevenmag_form_alter(&$form, &$form_state, $form_id) {
	if ($form_id == 'search_block_form') {
		$form['search_block_form']['#title_display'] = 'invisible'; // Toggle label visibilty
		$form['search_block_form']['#default_value'] = t('Search'); // Set a default value for the textfield
		$form['search_block_form']['#attributes']['id'] = array("mod-search-searchword");
		//disabled submit button
		//unset($form['actions']['submit']);
		unset($form['search_block_form']['#title']);
		$form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search';}";
		$form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search') {this.value = '';}";
	}
	if($form_id == 'contact_site_form'){
		$form['mail']['#attributes']['class'] = array("input-contact-form");
		$form['name']['#attributes']['class'] = array("input-contact-form");
		$form['subject']['#attributes']['class'] = array("input-contact-form");
		$form['message']['#attributes']['class'] = array("message-contact-form");
		$form['actions']['submit']['#attributes']['class'] = array('btn btn-success contact-form-button'); 
	}
	if ($form_id == 'comment_form') {
		$form['comment_filter']['format'] = array(); // nuke wysiwyg from comments
	}
}
function sevenmag_textarea($variables) {
  $element = $variables['element'];
  $element['#attributes']['name'] = $element['#name'];
  $element['#attributes']['id'] = $element['#id'];
  $element['#attributes']['cols'] = $element['#cols'];
  $element['#attributes']['rows'] = $element['#rows'];
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    $wrapper_attributes['class'][] = 'resizable';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}
function sevenmag_breadcrumb($variables) {
	$crumbs ='';
	$breadcrumb = $variables['breadcrumb'];
	if (!empty($breadcrumb)) {
		
		foreach($breadcrumb as $value) {
			$crumbs .= '&nbsp;'.$value.' <i>/</i>';
		}
		return $crumbs;
	}else{
		return NULL;
	}
}
//custom main menu
function sevenmag_menu_tree__main_menu($variables) {
	$str = '';
	$str .= '<ul class="sf-menu res_mode">';
		$str .= $variables['tree'];
	$str .= '</ul>';
	
	return $str;
}
//custom footer menu
function sevenmag_menu_tree__menu_footer_menu($variables) {
	$str = '';
	$str .= '<ul class="sf-menu">';
		$str .= $variables['tree'];
	$str .= '</ul>';
	
	return $str;
}

function hook_preprocess_page(&$variables) {
       if (arg(0) == 'node' && is_numeric($nid)) {
    if (isset($variables['page']['content']['system_main']['nodes'][$nid])) {
      $variables['node_content'] =& $variables['page']['content']['system_main']['nodes'][$nid];
      if (empty($variables['node_content']['field_show_page_title'])) {
        $variables['node_content']['field_show_page_title'] = NULL;
      }
    }
  }
}

