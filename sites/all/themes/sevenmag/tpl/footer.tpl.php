<footer id="footer">
	<div class="row clearfix">
		<?php
			$col_setting = theme_get_setting('footer_columns', 'sevenmag');
			if($col_setting==3){
				$col = '4';
			}else{
				$col = '3';
			}
			if($page["footer_col_one"]):
				print '<div class="alpha grid_'.$col.' footer_w">';
					print render($page["footer_col_one"]);
				print '</div>';
			endif;
			if($page["footer_col_two"]):
				print '<div class="grid_'.$col.' footer_w">';
					print render($page["footer_col_two"]);
				print '</div>';
			endif;
			
			if($page["footer_col_three"]):
				print '<div class="grid_'.$col.' footer_w">';
					print render($page["footer_col_three"]);
				print '</div>';
			endif;
			if($page["footer_col_four"]):
				print '<div class="omega grid_'.$col.' footer_w">';
					print render($page["footer_col_four"]);
				print '</div>';
			endif;
			
		?>
	</div>
	<!-- /row -->
	<div class="row clearfix">
		<div class="footer_last"><span class="copyright"><?php print theme_get_setting('footer_copyright_message', 'sevenmag'); ?></span>
			<nav class="nav-footer" id="nav-footer">
				<?php 
					if ($page['footer_menu']): 
						print render($page['footer_menu']);
					endif;
				?>
			</nav>
			<!--/nav-footer-->
		</div>
		<!-- /last footer -->
	</div>
	<!-- /row -->
	
</footer>