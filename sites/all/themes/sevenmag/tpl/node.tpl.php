<?php
/**
 * @file
 * Default theme implementation to display a node.
 */
global $base_root;
if($node->type=='blog'){
	$style = 'large'; //image style
	if($node->field_image){
	$imageone = $node->field_image['und'][0]['uri']; 
	}else{
	$imageone = '';
	}
	
	$post_type_a='export';//default post type
	
	if(!empty($node->field_post_type['und'][0]['value']))
			$post_type_a = $node->field_post_type['und'][0]['value'];
	?>
	<article>
		<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> b_block medium_thumb clearfix"<?php print $attributes; ?> >
			<?php
				if($node->field_image):
			?>
				<div class="post_thumbnail">
					<div class="item">
						<div class="featured_thumb">	
							<a class="first_A" href="<?php print $node_url; ?>" title="<?php print $title; ?>">
								<?php print theme('image', array('path' => $imageone, 'attributes'=>array('class'=>'mbt', 'alt'=>$title)));?>
								<!--<img src="img/assets/photodune-1333976-255x180.jpg" alt="4 essential rules of effective logo design">-->
								<span class="thumb-icon"><i class="icon-<?php print $post_type_a;?>"></i></span>	
							</a>
						</div>
					</div><!-- /item -->
				</div><!-- /thumbnail -->
			<?php
				endif;
			?>
			<h3><a href="<?php print $node_url; ?>" title="<?php print $title;?>"><?php print $title;?></a></h3>
			<div class="details mb">
				<span class="s_category">
					<a href="<?php print $node_url; ?>" rel="date"><i class="icon-calendar mi"></i><?php print format_date($node->created, 'custom', 'M d, Y');?></a>
					<a rel="author" href="#"><i class="icon-user mi"></i><?php print $node->name?></a>
					<span class="morely mid"><i class="icon-folder-open mi"></i><?php print render($content['field_categories']); //$content['field_categories'];?></span>
					
				</span>
				<span class="more_meta">
					<a class="post-comments" href="<?php print $node_url;?>"><span><i class="icon-message mi"></i><span><?php $comment_count=0;print $comment_count?></span></span></a>
					
				</span>
			</div>
			<!-- /details -->
	
			<div class="clearfix"><?php
			hide($content['comments']); 
			hide($content['links']); 
			hide($content['field_tags']); 
			hide($content['field_image']); 
			hide($content['field_categories']); 
			
			print render($content);
			
			if($page):
		?> 
		<p class="mtf tagcloud single_post">
		
			<?php 
			
				print t('<span>Tags: </span>');
				print render($content['field_tags']);
			
			?>
		</p>
		<?php
			endif;
		?>
		</div>
		<?php
			if(!$page){
		?>
		<a class="readmore" href="<?php print $node_url; ?>"><?php print t('Read More');?> </a>
		<?php
			}
		?>
		</div><!--/b block -->
	</article>
	
	
	
<?php

		print render($content['comments']);
}else{
	if(!empty($node)){
?>
<div id="node-a<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php print $user_picture; ?>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($display_submitted): ?>
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>

    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>

  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>

</div>
<?php
	}
}
?>