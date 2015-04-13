<?php print render($title_prefix); ?>
<?php if ($header): ?>
<?php print $header; ?>
<?php endif; ?>
<div id="big_carousel_full" class="owl-carousel owl-theme">
<?php if ($rows): ?>
	<?php print $rows; ?>
<?php endif; ?>
</div>