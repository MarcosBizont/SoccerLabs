<?php
	

/*
$process = curl_init('http://soccer-labs.com/api/slb/get_competitions?api-key=testkey');
curl_setopt($process, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($process, CURLOPT_POST, TRUE);
curl_setopt($process, CURLOPT_VERBOSE, TRUE);
curl_setopt($process, CURLOPT_FAILONERROR, TRUE);

$return = curl_exec($process);
curl_close($process);

*/


/*
$post_data = 
  	array(
		'competition' => '1806',
		);
$post_data = http_build_query($post_data, '', '&');



$process = curl_init('http://soccer-labs.com/api/slb/get_teams?api-key=testkey');
curl_setopt($process, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($process, CURLOPT_POST, TRUE);
curl_setopt($process, CURLOPT_POSTFIELDS, $post_data);
curl_setopt($process, CURLOPT_VERBOSE, TRUE);
curl_setopt($process, CURLOPT_FAILONERROR, TRUE);

$return = curl_exec($process);
curl_close($process);

*/


/*
$post_data = 
  	array(
		'competition' => '1806',
		'from_match_date' => '2015-03-01' 
		);
$post_data = http_build_query($post_data, '', '&');



$process = curl_init('http://soccer-labs.com/api/slb/get_games?api-key=testkey');
curl_setopt($process, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($process, CURLOPT_POST, TRUE);
curl_setopt($process, CURLOPT_POSTFIELDS, $post_data);
curl_setopt($process, CURLOPT_VERBOSE, TRUE);
curl_setopt($process, CURLOPT_FAILONERROR, TRUE);

$return = curl_exec($process);
curl_close($process);

*/


/*
$post_data = 
  	array(
		'in_progress' => '1' 
		);
$post_data = http_build_query($post_data, '', '&');



$process = curl_init('http://soccer-labs.com/api/slb/get_games?api-key=testkey');
curl_setopt($process, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($process, CURLOPT_POST, TRUE);
curl_setopt($process, CURLOPT_POSTFIELDS, $post_data);
curl_setopt($process, CURLOPT_VERBOSE, TRUE);
curl_setopt($process, CURLOPT_FAILONERROR, TRUE);

$return = curl_exec($process);
curl_close($process);

*/