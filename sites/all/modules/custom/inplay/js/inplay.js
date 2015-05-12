(function($) {
	$('.sidebarleft').addClass('loadingsidebar');
	$('#block-views-real-time-block-1, #block-views-real-time-block-2, .page-node-4629 #block-system-main').parent().hide();
	$('#block-views-real-time-block-3').parent().addClass('inplayoutside').parent().addClass('b_block');
	
	
	this.preProcessInplay = function()
	{
		containter = $('#block-views-real-time-block-3').isotope('destroy').isotope({
		  itemSelector: '.gamerow',
		  layoutMode: 'fitRows',
		});
		filterInplay();
		
		$('.grid_8.alpha').scrollTop(containter.parent().attr('offsettop'));
	}
	
	this.filterInplayItem = function(item)
	{
		containter = $('#block-views-real-time-block-3');
		var isvalid = true;
		if(containter.parent().hasClass('onlytips'))
			isvalid = (item.find('.views-field-field-tip-available .field-content i').size() > 0);
		
		var keywords = containter.parent().attr('keywords');
		if (typeof keywords !== typeof undefined && keywords !== false && isvalid)
			isvalid = 
				(item.find('.views-field-field-local-team span').text().toUpperCase().indexOf(keywords.toUpperCase()) > -1) || 
				(item.find('.views-field-field-visitor-team span').text().toUpperCase().indexOf(keywords.toUpperCase()) > -1) ||
				(item.find('.views-field-field-competition span').text().toUpperCase().indexOf(keywords.toUpperCase()) > -1);
		
		return isvalid; 
	}
	this.filterInplay = function()
	{
		containter = $('#block-views-real-time-block-3');
		/* Add gamerows for loading */
		containter.isotope({filter:function()
				{
					return filterInplayItem($(this)); 
				}
		});
		
			
		/*containter.isotope( 'on', 'layoutComplete', 
			function()
			{
				setTimeout(function()
				{
					if($('.gamerow:visible').size() > 0)
						$('#block-block-17').hide();
					else
					{	
						$('#block-block-17').show();
					}
				},400);
			} 
		);
		$('#block-block-17 a').click(function()
		{
			window.location.href = $('#block-block-17 a').attr('href') + '?search_api_views_fulltext=' + $('#autocompletetextbox').val();
			return false;
		});*/
		
	}
	
	containter = $('#block-views-real-time-block-3');
	containter.find('.view-real-time').prepend('<div class="gamerow toploadingsidebarsmall"><div class=""></div></div>');
	containter.find('.view-real-time').append('<div class="gamerow bottomloadingsidebarsmall"><div class=""></div></div>');
		
	/* Default isotope */
	$('#block-views-real-time-block-3').isotope({
	  itemSelector: '.gamerow',
	  layoutMode: 'fitRows',
	});
	filterInplay();
	
	// Filter buttons
	$('input[name=tipcheckbox]').attr('checked', false).click(function(){
		containter = $('#block-views-real-time-block-3');
		if($(this).is(':checked'))
			containter.parent().addClass('onlytips');
		else
			containter.parent().removeClass('onlytips');
		filterInplay();
	});
	
	
	$('input[name=autocompletetextbox]').val('').on('input', function(){
		containter = $('#block-views-real-time-block-3');
		containter.parent().attr('keywords', $(this).val());
		filterInplay();
	});
	
	
	$('.resetlink').click(function(){
		containter = $('#block-views-real-time-block-3');
		$('input[name=tipcheckbox]').attr('checked', false);
		containter.parent().removeClass('onlytips');
		$('input[name=autocompletetextbox]').val('');
		containter.parent().attr('keywords', '');
		filterInplay();
	});	
	
	////////////////////////////////////////////////////////
	// Load sidebar
	this.loadSidebar = function(urltocheck)
	{
		localStorage['urltocheck'] = urltocheck;
		setTimeout(function() {
			var nid = Drupal.settings.block_refresh.args[1];
			var pathbase = Drupal.settings.basePath + Drupal.settings.pathPrefix + 'block_refresh/';
			
			BlockRefreshContentInPlay(pathbase + 'inplay/inplay_header/node/' + nid, '#block-inplay-inplay-header', '.content', false);
			BlockRefreshContentInPlay(pathbase + 'inplay/inplay_tips/node/' + nid, '#block-inplay-inplay-tips', '.content', false);
			BlockRefreshContentInPlay(pathbase + 'inplay/inplay_last5_game_stats_lv/node/' + nid, '#block-inplay-inplay-last5-game-stats-lv', '.content', false);
			BlockRefreshContentInPlay(pathbase + 'inplay/inplay_last5_game_stats/node/' + nid, '#block-inplay-inplay-last5-game-stats', '.content', false);
			BlockRefreshContentInPlay(pathbase + 'inplay/inplay_game_stats/node/' + nid, '#block-inplay-inplay-game-stats', '.content', false);
		
		},50);
	}
	
	// Click on row
	// ToDo : Best way of doing this?
	$('#block-views-real-time-block-3 .gamerow.views-row').live("click", function() {
		var originalurl = document.URL.split("#"); 
		window.location.href = originalurl[0] + '#' + $(this).find('.views-field-nid a').text().trim();
		loadSidebar(window.location.href);
	});
		
	
	// Function after all sidebar are loaded
	this.preProcessSidebar = function()
	{
		$('.sidebarleft').removeClass('loadingsidebar');
		
		// Sidebar Left funcionality
		var windowheight = $(window).height();
		var settings = {
						trigger:'click',
						title:'',
						content:'',
						width:750,
						height:windowheight,						
						multi:false,						
						closeable:true,
						style:'',
						delay:300,
						padding:true,
						placement: 'right',
						arrow:true,
						animation:'pop'
				};
		
		// Load tips by group 
		var id = 0;
		$('.view-tips-blocks h3').each(function()
		{
			// clean any 
			var isvalid = false;
			var showlink = false;
			var firstelement = $(this).next();
			var originalurl = document.URL.split("#"); 
			var idcurrent = originalurl[1];
			id++;
			$("div.container_group.container_group_" + idcurrent + '_' + id).remove();
			firstelement.attr('openclass','container_group_' + idcurrent + '_' + id);
			var element = firstelement.show();
			while(!isvalid)
			{	
				element = element.next();
				if(element.hasClass('views-row')) 
				{
					element.addClass('indent').addClass('group_' + idcurrent + '_' + id);
					showlink = true;
				}
				else
					break;
			}
			$( '.group_' + idcurrent + '_' + id).wrapAll( "<div class='container_group container_group_" + idcurrent + '_' + id + "' />");
			if(showlink) firstelement.find('.views-field-nothing-1').show();
			
		});
		$( '.container_group').hide();
		
		
		$( ".sidebarleft .content-header" ).each(function() {
			mydiv = $(this).next();
			if(mydiv.find('div.b_block').size()==0) 
			{
				$(this).hide();	
			}
			else
			{
				settings.height = ((windowheight / 16) * 14) - 50 ;
				settings.content = mydiv.html();
				settings.title = $(this).html();
				$(this).webuiPopover(settings).click(function(){
					var posheader = $(this).position();
					$('.webui-popover .arrow').css({ top: (posheader.top + 50) + 'px' }); 
				});	
			}
						
		});
		
		$(".sidebarleft .content-inblock").hide();
		
		
	}
	// Add betclicks if links are availables
	$('a.simbetpopover').live('click',function()
	{
		var anchor = $(this);
		anchor.parent().find('.containerforsimbet').addClass('loadingsidebarsimbet').show();
		var urltobet = anchor.attr('url');
		data_to_send = '';
		$.post(urltobet, { data: data_to_send } ,function(data) {
			anchor.parent().find('.containerforsimbet').html(data);
			anchor.parent().find('.containerforsimbet').removeClass('loadingsidebarsimbet');	
		});
	});
	$('a.simbetaccept').live('click',function()
	{
		var anchor = $(this);
		anchor.parent().parent().find('.containerforsimbet').addClass('loadingsidebarsimbet');
		var urltobet = anchor.attr('url');
		data_to_send = '';
		$.post(urltobet, { data: data_to_send } ,function(data) {
			anchor.parent().parent().find('.containerforsimbet').removeClass('loadingsidebarsimbet');
			anchor.parent().parent().find('.containerforsimbet').html(data);
			setTimeout(function()
			{
				$('.containerforsimbet').slideUp();	
			},2000);
			
		});
	});	
	
	
	$('.views-field-nothing-1 .views-label').live('click',function()
	{
		$('.webui-popover-inner .' + $(this).parent().parent().attr('openclass')).slideToggle('slow');
	});
	
	function BlockRefreshContentInPlay(path, element, element_content, panels) {
		if($(element).parent().hasClass('sidebarleft'))
	    {
		    if(!$('.sidebarleft').hasClass('loadingsidebar_' + element.replace('#','')))
		    	$('.sidebarleft').addClass('loadingsidebar_' + element.replace('#',''));
		    if(!$('.sidebarleft').hasClass('loadingsidebar'))
		    	$('.sidebarleft').addClass('loadingsidebar');
	    }
	    var urltocheck = window.location.href;
	    if(localStorage['urltocheck']) urltocheck = localStorage['urltocheck'];
        $.post(path, { url: urltocheck } ,function(data) {
          var contents = $(data).html();
          // if this is a panel, preserve panel title.
          var oldh2 = $(element + ' h2.pane-title');
          $(element).html(contents);
          if (panels) {
            if (oldh2.length) {
              $(element + ' h2:first-child').replaceWith(oldh2);
            }
            else {
              $(element + ' h2:first-child').remove();
            }
            //panels renders block content in a 'pane-content' wrapper.
            $(element + ' .content').removeClass('content').addClass('pane-content');
          }
          $(element).removeClass('block-refresh-processed');
          Drupal.attachBehaviors(this);
          if($(element).parent().hasClass('sidebarleft'))
		  {
          	$('.sidebarleft').removeClass('loadingsidebar_' + element.replace('#',''));
		  	if($(".sidebarleft[class*='loadingsidebar_']").size()==0) preProcessSidebar();
          }
          else
          {
	          preProcessInplay();
          }
        });
      }
    
    
    // Autosize inplay
    function adjustGridSize(mainwindow)
    {
	    $('.grid_8.alpha').height(mainwindow.height()-95);
    }
    $(window).resize(function() {
	    
	    adjustGridSize($(this));
	});
	adjustGridSize($(window));
	
	// Refresh content clicks  
	$( ".refreshfrommatch" ).live( "click", function() {
		loadSidebar(this.href);
	});
	
	
	// Link toggle for label in every tip
	$('.view-tips-blocks .views-field-field-why-this-match-and-or-team .views-label,.view-tips-blocks .views-field-field-how-is-this-bet-works- .views-label,.view-tips-blocks .views-field-nothing .views-label').live( "click",function()
	{
		$(this).next().toggle();		
	});
	
	$('.webui-popover-content .collapsible .fieldset-legend').live( "click",function()
	{
		$(this).parent().next().slideToggle();		
	});
	
	
	
	//$('.grid_8.alpha').animate({
	//		scrollTop: $('.view-grouping:eq(1)').find('.competitiontitle:eq(0)').position().top
	//	}, 2000);
	
	
	/* Controls */
	
	function updateInplayGameRows()
	{
		var data_to_send = '';
		$.post('/inplay/loadinplay/gamerows', { data: data_to_send } ,function(data) {
			if(data=='') return;
			data = jQuery.parseHTML(data);
			$('.gamerow',data).each(function()
			{
				var growclass = '';
				var val = $(this).attr('class').match(/\bgrow-(\d+)\b/)[1];
				$('#block-views-real-time-block-3 .view-content .grow-' + val).html($(this).html());
			});
		});
	};
	
	function addNewGameRows()
	{
		var data_to_send = '';
		$('.toploadingsidebarsmall').addClass('loadingnow');
		$.post('/inplay/loadnew/gamerows', { data: data_to_send } ,function(data) {
			data = jQuery.parseHTML(data);
			var first_element = $('#block-views-real-time-block-3 .view-content .views-row:eq(0)');
			$('#block-views-real-time-block-3 .view-content').prepend($('.view-content',data).html());
			
			containter = $('#block-views-real-time-block-3').isotope('destroy').isotope({
			  itemSelector: '.gamerow',
			  layoutMode: 'fitRows',
			});
			filterInplay();
			$('.toploadingsidebarsmall').removeClass('loadingnow');
			if(first_element.size()>0)
				$('.grid_8.alpha').scrollTop(first_element.position().top);
		});
	};
	
	function addOldGameRows()
	{
		var data_to_send = '';
		
		if(localStorage['addingoldgames'] != 1)
		{
			$('.bottomloadingsidebarsmall').addClass('loadingnow');
			localStorage['addingoldgames'] = 1;
			$.post('/inplay/loadold/gamerows', { data: data_to_send } ,function(data) {
				localStorage['addingoldgames'] = 1;
				data = jQuery.parseHTML(data);
				currenttop = $('.grid_8.alpha').scrollTop();
				$('#block-views-real-time-block-3 .view-content').append($('.view-content',data).html());
				
				containter = $('#block-views-real-time-block-3').isotope('destroy').isotope({
				  itemSelector: '.gamerow',
				  layoutMode: 'fitRows',
				});
				filterInplay();
				$('.bottomloadingsidebarsmall').removeClass('loadingnow');
				localStorage['addingoldgames'] = 0;
				$('.grid_8.alpha').scrollTop(currenttop);
			});
		}
	};
	
	function defaultLoadMore()
	{
		if($('#block-views-real-time-block-3 .view-empty').size() >= 1 || ( $('#block-views-real-time-block-3 .view-content .gamerow').size() < 20 ) )
		{
			$('#block-views-real-time-block-3 .view-empty').addClass('view-content');
			//addNewGameRows();
			addOldGameRows();	
		}
	}	
	
	$("#edit-competition").chosen().change(function()
	{
		var data_to_send = new Object();
		data_to_send.data_to_send = $(this).val();
		$('#block-views-real-time-block-3').addClass('loadingsidebar');
		$.post('/inplay/refresh/competition', { data: data_to_send } ,function(data) {
			$('#block-views-real-time-block-3').html(data);
			containter = $('#block-views-real-time-block-3').isotope('destroy').isotope({
			  itemSelector: '.gamerow',
			  layoutMode: 'fitRows',
			});
			defaultLoadMore();
			filterInplay();
			$('#block-views-real-time-block-3').removeClass('loadingsidebar');
		});
	});
	$('.grid_8.prealpha .fa-cog').click(function()
	{
		$('#block-inplay-inplay-controls').slideToggle();	
	});
	
	/* Scroll */
	$('.grid_8.alpha').scroll(function()
	{
		$('#block-views-real-time-block-3').parent().attr('offsettop', $(this).scrollTop());
		if ($(this).scrollTop() == 0) {
			addNewGameRows();
		}
		else if( ($(this).find('.post').height() - $(this).scrollTop()) <= ( $(window).height() + 180))
		{
			addOldGameRows();
		}
		
		if ($(this).scrollTop() < 400) {
			$("#toTop").addClass("hidett").removeClass("showtt");
		} else {
			$("#toTop").removeClass("hidett").addClass("showtt");
		}
	});
	// ToDo : Remove Live from here
	$('#toTop').live( "click", function () {
		$('.grid_8.alpha').animate({
			scrollTop: 0
		}, 1000);
	});
	$("#toTop").addClass("hidett");
	
	localStorage['addingoldgames'] = 0;
	defaultLoadMore();
	
	setInterval(function() {
	    updateInplayGameRows();
	}, 120 * 1000);
	
	
	
})(jQuery);



