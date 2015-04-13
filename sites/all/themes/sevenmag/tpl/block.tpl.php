<?php 
$out = '';

if ($block->region == 'news_trending') { 
	$out .= '<div class="row clearfix '.$classes.'" id="'.$block_html_id.'" '.$attributes.'>';
		$out .= render($title_suffix);
		if ($block->subject && !empty($block->subject)):
			$out .= '<span class="breaking">'.$block->subject.'</span>';
		endif;
		$out .= $content;
	$out .= '</div>';
}elseif ($block->region == 'search') { 
	$out .= '<div class="search_place '.$classes.'" id="'.$block_html_id.'" '.$attributes.'>';
		$out .= render($title_suffix);
		if ($block->subject && !empty($block->subject)):
			$out .= '<h4>'.$block->subject.'</h4>';
		endif;
		$out .= '<div class="s_form">';
			$out .= $content;
		$out .= '</div>';
	$out .= '</div>';
}elseif ($block->region == 'topnews') { 
	$out .= '<div class="big_carousel in-view-5 '.$classes.'" id="'.$block_html_id.'" '.$attributes.'>';
		$out .= '<div id="big_carousel" class="owl-carousel">';
			$out .= render($title_suffix);
			if ($block->subject && !empty($block->subject)):
				$out .= '<h4>'.$block->subject.'</h4>';
			endif;
			$out .= $content;
		$out .= '</div>';
	$out .= '</div>';
}elseif ($block->region == 'sidebar_first' or $block->region == 'sidebar_second') { 
	$out .= '<div class="clearfix '.$classes.'" id="'.$block_html_id.'" '.$attributes.'>';
		$out .= render($title_suffix);
		if ($block->subject && !empty($block->subject)):
			$out .= '<div class="b_title"><h4>'.$block->subject.'</h4></div>';
		endif;
		$out .= '<div class="widget clearfix">';
			$out .= $content;
		$out .= '</div>';
	$out .= '</div>';
	
	
}elseif ($block->region == 'tabs_sidebar_second' or $block->region=='tabs_sidebar_first') { 
	$out .= '<div class="clearfix '.$classes.'" id="'.$block_html_id.'" '.$attributes.'>';
		$out .= render($title_suffix);
		//if ($block->subject && !empty($block->subject)):
			//$out .= '<div class="b_title"><h4>'.$block->subject.'</h4></div>';
		//endif;
		$out .= '<div class="widget clearfix">';
			$out .= $content;
		$out .= '</div>';
	$out .= '</div>';
	
	
}elseif($block->region == 'content'){
	if ($block->subject){
		$out .= '<div class="b_title"><h4 class="section_title" '.$title_attributes.'>'.$block->subject.'</h4></div>';
	}
	if($classes=='block block-block no-border-widget')
		$a = ' no-border-widget-d';
	else
		$a = '';
	$out .= '<div class="b_block clearfix'.$a.'">';
		$out .= '<div id="'.$block_html_id.'" class="'.$classes.'" '.$attributes.'>';
			$out .= render($title_suffix);
			$out .= $content;
		$out .= '</div>';
	$out .= '</div>';
	
}elseif($block->region == 'main_menu' || $block->region == 'top_bar_left' || $block->region == 'top_bar_right' || $block->region == 'slideshow'){
	if ($block->subject)
		$out .= '<h4 '.$title_attributes.'>'.$block->subject.'</h4>';
	$out .= $content;
}elseif($block->region == 'footer_col_one' || $block->region == 'footer_col_two' || $block->region == 'footer_col_three' || $block->region == 'footer_col_four'){

	$out .= '<div class="'.$classes.'" id="'.$block_html_id.'" '.$attributes.'>';
		$out .= render($title_suffix);
		if ($block->subject):
			$out .= '<div class="b_title"><h4 '.$title_attributes.'>'.$block->subject.'</h4></div>';
		endif;
		$out .= '<div class="widget clearfix">';
			$out .= $content;
		$out .= '</div>';
	$out .= '</div>';

	
	
	
}else{
	$out .= '<div id="'.$block_html_id.'" class="'.$classes.'" '.$attributes.'>';
		$out .= render($title_prefix);
		if ($block->subject):
			$out .= '<div class="content-header widget-header v3"><h4 '.$title_attributes.'>'.$block->subject.'</h4></div>';
		endif;
		$out .= $content;
	$out .= '</div>';
}
print $out;
?>