<?php print render($title_prefix); ?>
<?php if ($header): ?>
<?php print $header; ?>
<?php endif; ?>
<div class="post_thumbnail fully">
	<div class="slideshow_b5 owl-carousel owl-theme">
		<?php if ($rows): ?>
			<?php print $rows; ?>
		<?php endif; ?>
	</div>
</div>