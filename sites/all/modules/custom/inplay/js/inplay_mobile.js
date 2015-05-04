(function($) {
	
	containter = $('#block-views-in-play-mobile-block-3');
	containter.find('.view-in-play-mobile').prepend('<div class="gamerow toploadingsidebarsmall"><div class=""></div></div>');
	containter.find('.view-in-play-mobile').append('<div class="gamerow bottomloadingsidebarsmall"><div class=""></div></div>');
	
	/* Custom game window */
	function postLoadGameBox()
	{
		// Load tips by group 
		var id = 0;
		$('.view-tips-blocks h3').each(function()
		{
			var isvalid = false;
			var showlink = false;
			var firstelement = $(this).next();
			id++;
			firstelement.attr('openclass','container_group_' + id);
			var element = firstelement.show();
			while(!isvalid)
			{	
				element = element.next();
				if(element.hasClass('views-row')) 
				{
					element.addClass('indent').addClass('group_' + id);
					showlink = true;
				}
				else
					break;
			}
			$( '.group_' + id).wrapAll( "<div class='container_group container_group_" + id + "' />");
			if(showlink) firstelement.find('.views-field-nothing-1').show();
			
		});
		$( '.container_group').hide();
		
		$('.views-field-nothing-1 .views-label').click(function()
		{
			$('.' + $(this).parent().parent().attr('openclass')).slideToggle('slow');
		});
		
		// Link toggle for label in every tip
		$('.view-tips-blocks .views-field-field-why-this-match-and-or-team .views-label,.view-tips-blocks .views-field-field-how-is-this-bet-works- .views-label,.view-tips-blocks .views-field-nothing .views-label').click(function()
		{
			$(this).next().toggle();		
		});
		
		$('.webui-popover-content .fieldset-legend').click(function()
		{
			$(this).parent().next().toggle();		
		});

		
		
		// Add betclicks if links are availables

		$('a.simbetpopover').click(function()
		{

			var anchor = $(this);
			anchor.parent().find('.containerforsimbet').addClass('loadingsidebarsimbet').show();
			var urltobet = anchor.attr('url');
			data_to_send = '';
			$.ajax({
			  type: "POST",
			  url: urltobet,
			  data: data_to_send,
			  success: function(data) {
					anchor.parent().find('.containerforsimbet').html(data);
					anchor.parent().find('.containerforsimbet').removeClass('loadingsidebarsimbet');	
				},
			});

		});

		

	}
	
	$('a.simbetaccept').live('click', function()
	{
		var anchor = $(this);
		anchor.parent().parent().find('.containerforsimbet').addClass('loadingsidebarsimbet');
		var urltobet = anchor.attr('url');
		data_to_send = '';
		$.ajax({
		  type: "POST",
		  url: urltobet,
		  data: data_to_send,
		  success: function(data) {
				anchor.parent().parent().find('.containerforsimbet').removeClass('loadingsidebarsimbet');
				anchor.parent().parent().find('.containerforsimbet').html(data);
				setTimeout(function()
				{
					$('.containerforsimbet').slideUp();	
				},2000);
				
			},
		});
	});	
	
	
	
	function preLoadStream()
	{
		var settings = {
				trigger:'click',
				title:'',
				content:'',
				width:100,
				height:100,						
				multi:false,						
				closeable:true,
				style:'',
				delay:0,
				padding:true,
				placement: 'bottom',
				arrow:false,
				animation:'pop'
		};
		//
		$('.view-in-play-mobile .gamerow .main-fieldset:not(.bound)').addClass('bound').webuiPopover(settings).on('click', function(){ 
					var currenttop = $(window).scrollTop();
					var currentheight = $(window).height()+100;
					var currentwidth = $(window).width();
			
					$('.webui-popover .close').click(function()
					{
						$('.webui-popover').remove();	
					});
			
					$('.webui-popover').css({height: currentheight, width: currentwidth, top:currenttop, left:0});
					$('.webui-popover-content').html('').addClass('loadinggamebox').css({height: currentheight-100, width: currentwidth});
					
					var nid = $(this).parent().attr('class').match(/\bgrow-(\d+)\b/)[1];
					var urlgamebox = 'node/' + nid + '?onlycontent=1';
					data_to_send = '';
					$.ajax({
					  type: "POST",
					  url: urlgamebox,
					  data: data_to_send,
					  success: function(data) {
						  	data = jQuery.parseHTML(data);
						  	$('.webui-popover-content').removeClass('loadinggamebox').html($('.post.clearfix',data).html());
						  	postLoadGameBox();
						},
					});
		});
		
		
	}
	

	
	
	
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
			preLoadStream();
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
			preLoadStream();
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
				preLoadStream();
				$('.bottomloadingsidebarsmall').removeClass('loadingnow');
				localStorage['addingoldgames'] = 0;
				$('.grid_8.alpha').scrollTop(currenttop);
			});
		}
	};
	
	function defaultLoadMore()
	{
		if($('#block-views-in-play-mobile-block-3 .view-empty').size() >= 1 || ( $('#block-views-in-play-mobile-block-3 .view-content .gamerow').size() < 10 ) )
		{
			$('#block-views-in-play-mobile-block-3 .view-empty').addClass('view-content');
			addOldGameRows();	
		}
	}
	
	$(window).scroll(function()
	{
		//console.log($('#block-views-in-play-mobile-block-3').height());
		//console.log($(this).height());
		var currenttop = $(this).scrollTop();
		if (currenttop == 0) {
			addNewGameRows();
		}
		else if( ($('#block-views-in-play-mobile-block-3').height() - currenttop) <= ( $(this).height() + 180))
		{
			addOldGameRows();
		}
		$('.webui-popover').css({top:currenttop});
		
	});
	
	preLoadStream();
	localStorage['addingoldgames'] = 0;
	defaultLoadMore();
	
	
	setInterval(function() {
	    updateInplayGameRows();
	}, 120 * 1000);
	
	

})(jQuery);