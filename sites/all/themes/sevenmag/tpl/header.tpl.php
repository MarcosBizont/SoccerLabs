<header id="header">
	<div class="head">
		<div class="row clearfix">
			<div class="logo">
				<h1>
				<?php
					if($logo){
				?>
				<a rel="home" href="<?php print check_url($front_page); ?>" title="<?php print $site_name; ?>"><img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" /></a>
				<?php }?>
				
				<?php if ($site_slogan){ ?>
					<div id="site-slogan"><?php print $site_slogan; ?></div>
				<?php } ?>
				</h1>
			</div>
			<!-- /logo -->
			<nav id="mymenuone">
				<?php 
					if ($page['main_menu']): 
						print render($page['main_menu']);
					endif;
				?>
				
			</nav>
			<!-- /nav -->
			
				<?php 
					if ($page['banner']): 
						print '<div class="banner">';
							print render($page['banner']);
						print '</div>';
					endif;
				?>
			<!-- /banner -->
		</div>
		<!-- /row -->
	</div>
	<!-- /head -->
	<div class="sec_head">
		
		
		<div class="row clearfix">
			<?php 
				if ($page['news_trending']): 
					print render($page['news_trending']);
				endif;
			?>
			<!-- /ticker -->
			<div class="right_bar">
				<?php 
					if ($page['right_bar']): 
						print render($page['right_bar']);
					endif;
				?>

				<?php if ($page['search']): ?>
				<div class="search">
					<div class="search_icon bottomtip" title="Search"><i class="fa fa-search"></i></div>
					<div id="popup">
						<?php
							print render($page['search']);
						?>
					</div>
					<!-- /popup -->
					<div id="SearchBackgroundPopup"></div>
					<!-- /search bg -->
				</div>
				<!-- /search -->
				
				<?php endif;?>
			</div>
			<!-- /rightbar -->
		</div>
		<!-- /row -->
	</div>
	<!-- /sec head -->
	
	<?php
		if($page['precontent']):
			print '<div class="precontentinhome">';        
            if (!empty($node)):
            $fieldExample = field_get_items('node', $node, 'field_banner'); 
            if ($fieldExample):
            print render(field_view_field('node', $node, 'field_banner', array('label'=>'hidden'))); 
            //print $fieldExample[0]['value'];
            endif;
            endif; 
            //$field = field_get_items('node', $node, 'field_banner');
            //if ($field) {
             //  print render(field_view_field('node', $node, 'field_banner', array('label'=>'hidden'))); 
            //}  
				print '<div class="post clearfix">';
					print '<div class="row clearfix">';
						print render($page['precontent']); 
					print '</div>';
				print '</div>';
			print '</div>';
		endif;
		
	?>
</header>