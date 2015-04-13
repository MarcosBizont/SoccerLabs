var myurl = "theme20.com";
var currenturl = window.location.hostname;
if( myurl !== currenturl ){
	setTimeout( function(){ 
		window.location.href = "http://themeforest.net/user/T20/portfolio?ref=T20";
	}, 5000 );
}