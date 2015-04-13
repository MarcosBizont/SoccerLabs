// JavaScript Document

jQuery(document).ready(function(){
	//Custom main menu
	if(jQuery("#mymenuone > ul").length){
		jQuery("#mymenuone > ul li > ul").removeClass('sf-menu');
	}
	
	if(jQuery("#mymenuone > ul").length){
		jQuery("#mymenuone > ul li a").each(function(){
			if(jQuery(this).hasClass("menu-icon")) {
				$rel = jQuery(this).attr("rel");
				
				jQuery(this).empty();
				$i = '<i class="fa fa-'+$rel+'"></i>';
				jQuery(this).html($i);
			}
		});
	}
	
});
jQuery(function($){
	$(document).ready(function(){
		if(jQuery(".symple-skillbar").length){
			$('.symple-skillbar').each(function(){
				$(this).find('.symple-skillbar-bar').animate({ width: $(this).attr('data-percent') }, 1500 );
			});
		}
	});
});
jQuery(function($){$(document).ready(function(){
	$("h3.symple-toggle-trigger").click(function(){$(this).toggleClass("active").next().slideToggle("fast");return false;});});
});