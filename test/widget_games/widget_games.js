$(function(){

 $.ajax({
		type: 'GET',
        url: "https://soccer-labs.com/test/widget_games/api.php",
        dataType: "json",
		success: function (response) {

			var htmlContent = $('<div id="wrapSoccerLabsWidget" style="width: 300px; height:550px; overflow:auto; border: 1px solid #CCD0D3; font-family: Helvetica;">');

			var json = $.parseJSON(response);
			$(json).each(function(i,val){

				function isInt(n) { return n % 1 === 0; }
				var statusGame = "";

				if(val.match_status === "FT")
				{
					statusGame = "Final";
				}else if(val.match_status === "HT"){
					statusGame = "HT";
				}else if(isInt(val.match_status)){
					statusGame = val.match_status + "\"";
				}else{
					statusGame = val.match_date.time_formatted_value;
				}
				
				
				

				var golLocal = 0;

				if(val.local_team.score != "?")
				{
					golLocal = val.local_team.score;
				}

				var golVisitor = 0;

				if(val.visitor_team.score != "?")
				{
					golVisitor = val.visitor_team.score;
				}

				var item = '<div class="row" style="padding: 5px 5px 2px 5px; margin: 0; min-height: 60px;">' +
				'<div class="info" style="width: 60px; background-color: #E1E1E1; text-align: center; height: 62px;float: left">' +
					'<div class="status" style="margin-top: 5px; font-size: 18px; font-weight:500px;"  >'+ statusGame +'</div>' +
					'<div class="date" style="font-size: 13px;">' + val.match_date.formatted_value + '</div>' +
				'</div>' +
				'<div class="teams">' +
					'<div class="teamLocal" style="height: 30px; position: relative; overflow: hidden; background-color:#E9EEF4; border-bottom: 1px solid #CCD0D3;">'+
						'<div class="logo" style="position: absolute; top: 1px; left: 1px; height:26px; width: 26px;"><img src="' + val.local_team.imagethumbnail + '" width="25px" height="25px" /></div>'+
						'<span class="name" style="	display: block; margin-left: 28px; margin-right:45px; margin-top: 7px; font-size: 13px;">' + val.local_team.title + '</span>'+
						'<div class="scores">'+
							'<div class="goals" style="position: absolute; right:5px; top: 4px; height:21px; min-width:21px; border-radius:3px; border: 1px solid #1D242C; font-weight:500; font-size:18px; color: #FEFFFF; background-color:#2A333C; text-align: center;">' + golLocal + '</div>'+
						'</div>'+
					'</div>'+
					'<div class="teamVisitor" style="height: 30px; position: relative; overflow: hidden; background-color:#E9EEF4; border-top: 1px solid #FCFFFF;">'+
						'<div class="logo" style="position: absolute; top: 1px; left: 1px; height:26px; width: 26px;"><img src="' + val.visitor_team.imagethumbnail + '" width="25px" height="25px"/></div>'+
						'<span class="name" style="	display: block; margin-left: 28px; margin-right:45px; margin-top: 7px; font-size: 13px;">' + val.visitor_team.title + '</span>'+
						'<div class="scores" style="position: absolute; right:0px; top: 0px;">'+
							'<div class="goals" style="position: absolute; right:5px; top: 4px; height:21px; min-width:21px; border-radius:3px; border: 1px solid #1D242C; font-weight:500; font-size:18px; color: #FEFFFF; background-color:#2A333C; text-align: center;">' + golVisitor + '</div>'+
						'</div>'+
					'</div>'+
				'</div>'+

				'<a href="http://soccer-labs.com" target="_blank" style="text; text-decoration: none;"><div class="linkSoccerLabs" style="text-align: center; background-color: #2A333C; border-top: 1px solid #F5F8F8;  color: #91A8BF;">' +
				'<span style="position: relative; top: -9px; font-weight: bold;">Apuesta gratis en </span>' +
					'<img src="https://soccer-labs.com/test/widget_games/logoSoccerLabs.png" style="width: 120px; height: 30px;" />' +
				'</div>' +

			'</div></a>';

			htmlContent.append(item);

			});

			var wrapClose = '</div>';
		    htmlContent.append(wrapClose );

			$("#widgetSoccerLabsGames").html(htmlContent);
        }
  });
});