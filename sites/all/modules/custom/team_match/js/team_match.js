(function($) {
	
		function getUrlParameter(sParam)
		{
		    var sPageURL = window.location.search.substring(1);
		    var sURLVariables = sPageURL.split('&');
		    for (var i = 0; i < sURLVariables.length; i++) 
		    {
		        var sParameterName = sURLVariables[i].split('=');
		        if (sParameterName[0] == sParam) 
		        {
		            return sParameterName[1];
		        }
		    }
		}    
	
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
		$('.view-tips-blocks .views-field-field-why-this-match-and-or-team .views-label,.view-tips-blocks .views-field-field-how-is-this-bet-works- .views-label,.view-tips-blocks .views-field-nothing .views-label').live( "click",function()
		{
			$(this).next().toggle();		
		});
		
		
		if(getUrlParameter('backtoinplay')=='1' && $('.backtoinplay').size() > 0)
		{
			$('.backtoinplay').css('margin-top','100px');
			$('.backtoinplay').show();	
			$(document).scroll(function() {
			  var y = $(this).scrollTop();
			  if (y > 100) {
			    $('.backtoinplay').css('margin-top','0px');
			  } else {
				var marginTop = 100 - y;  
			    $('.backtoinplay').css('margin-top', marginTop + 'px');
			  }
			});
			$('.backtoinplay').click(function(){
		        parent.history.back();
		        return false;
		    });
		}
		
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

		



})(jQuery);


