<div class="widget_T20_posts_slideshow">
<?php print render($title_prefix); ?>
<?php if ($header): ?>
<?php print $header; ?>
<?php endif; ?>
<div class="gallery_widget gallery_widget_posts owl-carousel owl-theme">
	<?php if ($rows): ?>
		<?php print $rows; ?>
	<?php endif; ?>
</div>
</div>