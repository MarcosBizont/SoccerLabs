<?php print render($title_prefix); ?>
<?php if ($header): ?>
<?php print $header; ?>
<?php endif; ?>
<!--<div class="b_block b_4">-->
	<div id="block_carousel" class="carousel_posts_block owl-carousel">
		<?php if ($rows): ?>
			<?php print $rows; ?>
		<?php endif; ?>
	</div>
<!--</div>-->