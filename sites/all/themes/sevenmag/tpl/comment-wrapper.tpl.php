<?php if ($content['#node']->comment and !($content['#node']->comment == 1 and $content['#node']->comment_count)) { ?>
<div class="b_title"><h3><?php print $content['#node']->comment_count.' '.t('Comments');?></h3></div>

<div class="mar_top_bottom_lines_small3"></div>
	<?php print render($content['comments']); ?>
	<div class="comment_form">
	<h4><?php print t('Leave a Comment');?></h4>
		<?php print str_replace('resizable', '', render($content['comment_form'])); ?>
	</div>
<?php } ?>
