<?php

function odds_pinnaclesports_test1()
{
	// This fetches the initial feed from the Pinnacle Sports API
	$feedUrl = 'https://api.pinnaclesports.com/v1/feed?sportid=29';
	
	// Set your credentials here, format = clientid:password from your account.
	$credentials = base64_encode("justmad:dar9ched");
	
	// Build the header, the content-type can also be application/json if needed
	$header[] = 'Content-length: 0';
	$header[] = 'Content-type: application/json';
	$header[] = 'Authorization: Basic ' . $credentials;
	
	// Set up a CURL channel.
	$httpChannel = curl_init();
	// Prime the channel
	curl_setopt($httpChannel, CURLOPT_URL, $feedUrl);
	curl_setopt($httpChannel, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($httpChannel, CURLOPT_HTTPHEADER, $header);
	// Unless you have all the CA certificates installed in your trusted root authority, this should be left as false.
	curl_setopt($httpChannel, CURLOPT_SSL_VERIFYPEER, false);
	
	// This fetches the initial feed result. Next we will fetch the update using the fdTime value and the last URL parameter
	$initialFeed = curl_exec($httpChannel);
	echo $initialFeed;
	
	// You need to pick an XML library that is suitable for you, in this case i am using the built-in simple XML feature of PHP.
	//$xmlDocument = simplexml_load_string($initialFeed);
	// Simple XML has now build an array of arrays or a dictionary of values, you may access this information by index or name.
	//$feedTime = $xmlDocument->rsp->fd[0]->fdTime;
	//echo $fdTime;
	
	// Now we simply alter the URL with the last parameter and feed in the value of fdTime
	//$feedUrl = 'https://api.pinnaclesports.com/v1/feed?sportid=29&last=' . $fdTime;
	
	// Now we can fetch the updates.
	//$updates = curl_exec($httpChannel);
	
	// Build an XML document from simple XML to read your data as an object again.
	// ...
}
	
	
?>