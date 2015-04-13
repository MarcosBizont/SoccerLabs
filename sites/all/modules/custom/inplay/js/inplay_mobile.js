(function($) {
	
	this.preProcessInplay = function()
	{
	
	}
	
	
	$('.view-in-play-mobile .gamerow .main-fieldset').live("click", function() {
		$('.view-in-play-mobile .gamerow .tips-fieldset').hide('400');
		$(this).next().toggle();
	});
	
	

})(jQuery);