<div class="comment_wrap" <?php if(!$picture) print 'style="padding-left:0px; margin-left:0px;"';?>>
	<?php 
		if($picture):
			print '<div class="gravatar">'.$picture.'</div>';
		endif;
	?>
	<div class="comment_content" <?php if(!$picture) print 'style="padding-left:0px;"';?>>
		<div class="comment_meta">
			<div class="comment_author"><?php print theme('username', array('account' => $content['comment_body']['#object'], 'attributes' => array('class' => 'url'))) ?> - <i><?php print t('!time ago',array('!time' => format_interval(time() - $content['comment_body']['#object']->created))); ?></i></div>
		</div>
		<div class="comment_text">
			<?php hide($content['links']); print render($content) ?>
			<?php print render($content['links']) ?></div>
	</div>
</div>