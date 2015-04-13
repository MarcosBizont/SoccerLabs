<?php include_once('header.tpl.php');?>
<?php
	$page_type = NULL;
	if(!empty($node->field_page_type['und'][0]['value']))
		$page_type = $node->field_page_type['und'][0]['value'];

	$nodenid = NULL;
	if(!empty($node->nid))
		$nodenid = $node->nid;
		
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
								print render($page['precontent']);
								print render($page['content']); 
								//print $node->field_page_type['und'][0]['value'];
								 
							print '</div>';
						print '</div>';
					print '</div>';
				endif;
				
				//Sidebar first
				
				if ($page['sidebar_first'] or $page['tabs_sidebar_first']):
					print '<div class="sidebarleft grid_3">';
						//Tab sidebar
						if ($page['tabs_sidebar_first']):
							print render($page['tabs_sidebar_first']) . render($page['tabs_sidebar_second']) . render($page['tabs_sidebar_third']);
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
					print '<div class="sidebarleft grid_3 alpha">';
						
						if ($page['tabs_sidebar_first']):
							print render($page['tabs_sidebar_first']) . render($page['tabs_sidebar_second']) . render($page['tabs_sidebar_third']);
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
								print render($page['precontent']);
								print render($page['content']); 
								//print $node->field_page_type['und'][0]['value'];
								 
							print '</div>';
						print '</div>';
					print '</div>';
				endif;
				
			}elseif(isset($page_type) && $page_type=='homepage_style_5'){ //home page style 5
				//Content
				if ($page['content'] || isset($messages)):
					print '<div class="grid_9 alpha">';
						print '<div class="post clearfix">';
							print '<div class="row clearfix">';
								if(drupal_is_front_page()) {
									unset($page['content']['system_main']['default_message']);
								}
								print $messages;
								print render($page['precontent']);
								print render($page['content']); 
								//print $node->field_page_type['und'][0]['value'];
								 
							print '</div>';
						print '</div>';
					print '</div>';
				endif;
				
				//Sidebar first
				
				if ($page['sidebar_first'] or $page['tabs_sidebar_first'] or $page['tabs_sidebar_second'] or $page['sidebar_second']):
					print '<div class="sidebarleft grid_3 omega">';
						
						if ($page['tabs_sidebar_first']):
							print render($page['tabs_sidebar_first']) . render($page['tabs_sidebar_second']) . render($page['tabs_sidebar_third']);
						endif;
						
						print render($page['sidebar_first']);
						
						
						//Sidebar second
						if ($page['tabs_sidebar_second']):
							print render($page['tabs_sidebar_second']);
						endif;
						print render($page['sidebar_second']);
					print '</div>';
				endif;
				
			}
			elseif($nodenid==4629){ //Default home page
//			(empty($page_type) or $page_type=='homepage_style_1' or $page_type == '' or $page_type == null){ //Default home page
		
				
				$cs = 4;
				//if (!$page['sidebar_second'] and !$page['tabs_sidebar_second']) $cs = 4;
				if ($page['sidebar_first'] or $page['tabs_sidebar_first']):
					print '<div class="sidebarleft grid_'.$cs.' ">';
						if ($page['tabs_sidebar_first']):
							print render($page['tabs_sidebar_first']) . render($page['tabs_sidebar_second']) . render($page['tabs_sidebar_third']);
						endif;
						print render($page['sidebar_first']);
					print '</div>';
				endif;
				
				$c = 8;
				//if (!$page['sidebar_second'] and !$page['tabs_sidebar_second']) $c = 8;
				
			
				if ($page['precontentinside']):
					print '<div class="grid_'.$c.' prealpha">';
						print '<div class="post clearfix">';
							print '<div class="row clearfix">';
								print render($page['precontentinside']); 
							print '</div>';
						print '</div>';
					print '</div>';
				endif;
				
				
				if ($page['content'] || isset($messages)):
					
					print '<div class="grid_'.$c.' alpha">';
						if (!empty($tabs['#primary']) || !empty($tabs['#secondary'])):
							print render($tabs);
						endif;
						
						if ($breadcrumb):
						//print '<div class="b_title"><h3><i class="icon-folder-open mi"></i>';
						print '<div class="b_title"><h3><i class="mi"></i>';
							//print $breadcrumb;
							//if(!isset($node))
								print ' '.drupal_get_title();
							print '</h3></div>';
						endif; 
						
						
						if(drupal_get_title() && isset($node))
							//print '<h1 class="title-p">'.drupal_get_title().'</h1>';
						print '<div class="post clearfix">';
							print '<div class="row clearfix">';
								if(drupal_is_front_page()) {
									unset($page['content']['system_main']['default_message']);
								}
								print $messages;
								print render($page['precontent']);
								print render($page['content']); 
								//print $node->field_page_type['und'][0]['value'];
								 
							print '</div>';
						print '</div>';
					print '</div>';
				endif;
				
				
				

				/*
				if ($page['sidebar_second'] or $page['tabs_sidebar_second']):
					print '<div class="grid_3 righter omega">';
						if ($page['tabs_sidebar_second']):
							print render($page['tabs_sidebar_second']);
						endif;
						print render($page['sidebar_second']);
					print '</div>';
				endif;
				*/

			}
			else{ //Default home page
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
					print '<div class="grid_'.$c.' alpha ">';
						if (!empty($tabs['#primary']) || !empty($tabs['#secondary'])):
							print render($tabs);
						endif;
						if ($breadcrumb):
						print '<div class="b_title"><h3><i class="fa-angle-right fa"></i>';
							print $breadcrumb;
							if(!isset($node))
								print ' '.drupal_get_title();
							print '</h3></div>';
						endif; 
						
						if(drupal_get_title() && isset($node))
							print '<h1 class="title-p">'.drupal_get_title().'</h1>';
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