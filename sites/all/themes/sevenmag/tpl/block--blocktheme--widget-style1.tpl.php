<?php
$out = '';
$out .= '<div id="'.$block_html_id.'" class="'.$classes.' widget_style1 clearfix" '.$attributes.'>';
	$out .= render($title_prefix);
	if ($block->subject):
		$out .= '<span>'.$block->subject.'</span>';
	endif;
	$out .= $content;
$out .= '</div>';

print $out;