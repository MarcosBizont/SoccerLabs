<?php

header('content-type: application/json; charset=utf-8');
header("access-control-allow-origin: *");

$post_data =
	array(
		'in_progress' => '1'
	);

$post_data = http_build_query($post_data, '', '&');

$process = curl_init('https://soccer-labs.com/api/slb/get_games?api-key=testkey');
curl_setopt($process, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($process, CURLOPT_POST, TRUE);
curl_setopt($process, CURLOPT_POSTFIELDS, $post_data);
curl_setopt($process, CURLOPT_VERBOSE, TRUE);
curl_setopt($process, CURLOPT_FAILONERROR, TRUE);

$return = curl_exec($process);
curl_close($process);

//codificamos en json:
$json = json_encode($return);

echo $json;

?>