<?php print render($title_prefix); ?>
<?php if ($header): ?>
<?php print $header; ?>
<?php endif; ?>

<div class="big_carousel in-view-2">
	<div id="big_carousel" class="owl-carousel">
		<?php if ($rows): ?>
			<?php print $rows; ?>
		<?php endif; ?>
	</div>
</div>