<?php
$out = '';
$out .= '<div id="'.$block_html_id.'" class="'.$classes.' clearfix" '.$attributes.'>';
	$out .= render($title_prefix);
	if ($block->subject):
		$out .= '<div class="b_title"><h4>'.$block->subject.'</h4></div>';
	endif;
	$out .= '<div class="widget clearfix">';
		$out .= $content;
	$out .= '</div>';
$out .= '</div>';

print $out;