<?php include_once('header.tpl.php');?>
<?php
	$page_type = NULL;
	if(!empty($node->field_page_type['und'][0]['value']))
		$page_type = $node->field_page_type['und'][0]['value'];

	
	
	/*if ($page['topnews']): //slideshow
		print render($page['topnews']);
	endif;*/
	
	if ($page['big_carousel_full']): //slideshow Big carousel full
		print render($page['big_carousel_full']);
	endif;
	
	
	
?>
<div class="page-content">
	<div class="row clearfix">
		<?php
			if ($page['sidebar_first'] && $page['sidebar_second']){
				$c = '6';
			}elseif($page['sidebar_first'] or $page['sidebar_second'] or $page['tabs_sidebar_first'] or $page['tabs_sidebar_second']){
				$c = '9';
			}else{
				$c = '12';
			}
			
			if((isset($page_type) && $page_type=='homepage_style_2') or (isset($page_type) && $page_type=='homepage_style_3')){ //home page style 2 and 3
				//Content
				if ($page['content'] || isset($messages)):
					print '<div class="grid_'.$c.' alpha">';
						print '<div class="post clearfix">';
							
							if($page['slideshow_homepage_style2']){
								print '<div class="b_5 mbf">';
									print render($page['slideshow_homepage_style2']);
								print '</div>';
							}
							
							print '<div class="row clearfix">';
								if(drupal_is_front_page()) {
									unset($page['content']['system_main']['default_message']);
								}
								print $messages;
								print render($page['content']); 
								//print $node->field_page_type['und'][0]['value'];
								 
							print '</div>';
						print '</div>';
					print '</div>';
				endif;
				
				//Sidebar first
				
				if ($page['sidebar_first'] or $page['tabs_sidebar_first']):
					print '<div class="grid_3">';
						//Tab sidebar
						if ($page['tabs_sidebar_first']):
							print render($page['tabs_sidebar_first']);
						endif;
						
						print render($page['sidebar_first']);
					print '</div>';
				endif;
				
				//Sidebar second
				
				if ($page['sidebar_second'] or $page['tabs_sidebar_second']):
					print '<div class="grid_3 omega">';
						//Tab sidebar
						if ($page['tabs_sidebar_second']):
							print render($page['tabs_sidebar_second']);
						endif;
						print render($page['sidebar_second']);
					print '</div>';
				endif;
			}elseif(isset($page_type) && $page_type=='homepage_style_4'){ //home page style 4
				//Sidebar first
				
				if ($page['sidebar_first'] or $page['tabs_sidebar_first']):
					print '<div class="grid_3 alpha">';
						
						if ($page['tabs_sidebar_first']):
							print render($page['tabs_sidebar_first']);
						endif;
						
						print render($page['sidebar_first']);
					print '</div>';
				endif;
				
				//Sidebar second
				
				
				if ($page['sidebar_second'] or $page['tabs_sidebar_second']):
					print '<div class="grid_3">';// righter omega
						if ($page['tabs_sidebar_second']):
							print render($page['tabs_sidebar_second']);
						endif;
						print render($page['sidebar_second']);
					print '</div>';
				endif;
				
				//Content
				if ($page['content'] || isset($messages)):
					print '<div class="grid_'.$c.' righter omega">';
						print '<div class="post clearfix">';
							print '<div class="row clearfix">';
								if(drupal_is_front_page()) {
									unset($page['content']['system_main']['default_message']);
								}
								print $messages;
								print render($page['content']); 
								//print $node->field_page_type['und'][0]['value'];
								 
							print '</div>';
						print '</div>';
					print '</div>';
				endif;
				
			}elseif(isset($page_type) && $page_type=='masonry'){ //home page masonry style
				//Content
				if ($page['content'] || isset($messages)):
					print '<div id="masonry-container" class="three_col_mas transitions-enabled centered clearfix">';
					/*print '<div class="grid_9 alpha">';
						print '<div class="post clearfix">';
							print '<div class="row clearfix">';*/
								if(drupal_is_front_page()) {
									unset($page['content']['system_main']['default_message']);
								}
								print $messages;
								print render($page['content']); 
								 
							/*print '</div>';
						print '</div>';
					print '</div>';*/
					
					print '</div>';
				endif;
				?>
	
				<script type="text/javascript">	
		/* <![CDATA[ */
			jQuery(document).ready(function ($) {
				var $container = jQuery('#masonry-container');
				$container.imagesLoaded( function() {
					$container.masonry({
						itemSelector: '.post',
						isRTL: false,
						columnWidth: 1,
						isAnimated: true,
						animationOptions: {
							duration: 300,
							easing: 'easeInExpo',
							queue: true
						}
					});
				});
			});
		/* ]]> */
	</script>
				<?php
				
			}elseif(isset($page_type) && $page_type=='fullwidth'){ //home page masonry style
				//Content
				if ($page['content'] || isset($messages)):
					if(drupal_is_front_page()) {
						unset($page['content']['system_main']['default_message']);
					}
					print $messages;
					print render($page['content']); 
				endif;
				?>
	
				
				<?php
				
			}else{ //Default home page
//			(empty($page_type) or $page_type=='homepage_style_1' or $page_type == '' or $page_type == null){ //Default home page
		
				
			
				if ($page['sidebar_first'] or $page['tabs_sidebar_first']):
					print '<div class="grid_3 alpha">';
						if ($page['tabs_sidebar_first']):
							print render($page['tabs_sidebar_first']);
						endif;
						print render($page['sidebar_first']);
					print '</div>';
				endif;
			
			
				
				if ($page['content'] || isset($messages)):
					print '<div class="grid_'.$c.'">';
						print '<div class="post clearfix">';
							print '<div class="row clearfix">';
								if(drupal_is_front_page()) {
									unset($page['content']['system_main']['default_message']);
								}
								print $messages;
								print render($page['content']); 
								//print $node->field_page_type['und'][0]['value'];
								 
							print '</div>';
						print '</div>';
					print '</div>';
				endif;
				
				
				
				if ($page['sidebar_second'] or $page['tabs_sidebar_second']):
					print '<div class="grid_3 righter omega">';
						if ($page['tabs_sidebar_second']):
							print render($page['tabs_sidebar_second']);
						endif;
						print render($page['sidebar_second']);
					print '</div>';
				endif;
			}
		?>
	
	</div>
</div>
<div class="clearfix"></div>
<?php include_once('footer.tpl.php');?>