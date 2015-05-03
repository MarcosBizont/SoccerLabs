(function($) {
	
	containter = $('#block-views-in-play-mobile-block-3');
	containter.find('.view-in-play-mobile').prepend('<div class="gamerow toploadingsidebarsmall"><div class=""></div></div>');
	containter.find('.view-in-play-mobile').append('<div class="gamerow bottomloadingsidebarsmall"><div class=""></div></div>');
	
	this.preProcessInplay = function()
	{
	
	}
	
	
	$('.view-in-play-mobile .gamerow .main-fieldset').live("click", function() {
		$('.view-in-play-mobile .gamerow .tips-fieldset').hide('400');
		$(this).next().toggle();
	});
	
	
	/* Controls */
	
	function updateInplayGameRows()
	{
		var data_to_send = '';
		$.post('/inplaym/loadinplay/gamerows', { data: data_to_send } ,function(data) {
			if(data=='') return;
			data = jQuery.parseHTML(data);
			$('.gamerow',data).each(function()
			{
				var growclass = '';
				var val = $(this).attr('class').match(/\bgrow-(\d+)\b/)[1];
				$('#block-views-in-play-mobile-block-3 .view-content .grow-' + val).html($(this).html());
			});
		});
	};
	
	function addNewGameRows()
	{
		var data_to_send = '';
		$('.toploadingsidebarsmall').addClass('loadingnow');
		$.post('/inplaym/loadnew/gamerows', { data: data_to_send } ,function(data) {
			data = jQuery.parseHTML(data);
			var first_element = $('#block-views-in-play-mobile-block-3 .view-content .views-row:eq(0)');
			$('#block-views-in-play-mobile-block-3 .view-content').prepend($('.view-content',data).html());
			
			/*containter = $('#block-views-in-play-mobile-block-3').isotope('destroy').isotope({
			  itemSelector: '.gamerow',
			  layoutMode: 'fitRows',
			});
			filterInplay();*/
			$('.toploadingsidebarsmall').removeClass('loadingnow');
			if(first_element.size()>0)
				$(window).scrollTop(first_element.position().top);
		});
	};
	
	function addOldGameRows()
	{
		var data_to_send = '';
		
		if(localStorage['addingoldgames'] != 1)
		{
			$('.bottomloadingsidebarsmall').addClass('loadingnow');
			localStorage['addingoldgames'] = 1;
			$.post('/inplaym/loadold/gamerows', { data: data_to_send } ,function(data) {
				localStorage['addingoldgames'] = 1;
				data = jQuery.parseHTML(data);
				currenttop = $('.grid_8.alpha').scrollTop();
				$('#block-views-in-play-mobile-block-3 .view-content').append($('.view-content',data).html());
				
				/*containter = $('#block-views-in-play-mobile-block-3').isotope('destroy').isotope({
				  itemSelector: '.gamerow',
				  layoutMode: 'fitRows',
				});
				filterInplay();*/
				$('.bottomloadingsidebarsmall').removeClass('loadingnow');
				localStorage['addingoldgames'] = 0;
				$('.grid_8.alpha').scrollTop(currenttop);
			});
		}
	};
	
	function defaultLoadMore()
	{
		if($('#block-views-in-play-mobile-block-3 .view-empty').size() >= 1 || ( $('#block-views-in-play-mobile-block-3 .view-content .gamerow').size() < 20 ) )
		{
			$('#block-views-in-play-mobile-block-3 .view-empty').addClass('view-content');
			addOldGameRows();	
		}
	}
	
	$(window).scroll(function()
	{
		//console.log($(this).scrollTop());
		if ($(this).scrollTop() == 0) {
			addNewGameRows();
		}
		else if( ($(this).find('.post').height() - $(this).scrollTop()) <= ( $(window).height() + 180))
		{
			addOldGameRows();
		}
	});
	
	
	localStorage['addingoldgames'] = 0;
	defaultLoadMore();
	
	setInterval(function() {
	    updateInplayGameRows();
	}, 120 * 1000);
	
	

})(jQuery);