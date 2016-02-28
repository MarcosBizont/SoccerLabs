<?php

header('content-type: application/json; charset=utf-8');
header("access-control-allow-origin: *");

do {

	$diasInicio = " -1 days";
	$diasFin = "+36 hours";

	$fechaInicio = date("Y-m-d", strtotime($diasInicio));
	$fechaFin = date("Y-m-d", strtotime($diasFin));

	$post_data =
	array(
		'from_match_date' => $fechaInicio,
		'to_match_date' => $fechaFin,
		'bring_big_leagues_mexico' => '1'
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

	if($return != null)
	{
		$esData = true;
	}else{
		$esData = false;
	}

	$json = json_encode($return);
	echo $json;

	$diaFinal++;

} while ($esData == false);


?>