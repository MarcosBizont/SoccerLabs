<?php
	
	
/**
 * Prepare data before sending to Buffer
 * See README.txt for more informations.
 * @param bool $send
 * @param object $node
 * @param string $text
 * @param array $media
 * @param bool $now 
 */
function tips_buffer_bufferapp_data($send = TRUE, $node = NULL, $text, $media = array(), $now = FALSE, $scheduled_at = FALSE, $profile_only = false) {
	$data['text'] = $text;
	$data['media'] = $media;
	$data['now'] = $now;
	
	if ($scheduled_at > 1) {
		$data['now'] = FALSE;
		$data['scheduled_at'] = $scheduled_at;
	}
	
	// Get enabled profiles
	$bufferapp_profiles = bufferapp_profiles();
	if(!$profile_only)
	{
		$bufferapp_enabled = variable_get('bufferapp_selected_profiles');
		foreach ($bufferapp_profiles as $profile) {
			if ($bufferapp_enabled[$profile->id] == '0') {
				unset($bufferapp_enabled[$profile->id]);
			}
		}
	}
	else
	{
		$bufferapp_enabled = array();
		foreach ($bufferapp_profiles as $profile)
			if($profile->formatted_username == $profile_only)
				$bufferapp_enabled[$profile->id] = $profile->id;
				
	}
	$data['profile_ids'] = array_keys($bufferapp_enabled);
	
	// Call hooks from other modules
	//drupal_alter('bufferapp_data', $data, $node);
	
	if ($send) {
		bufferapp_send_update($data);
	} else {
		return $data;
	}
}

	
function tips_buffer_generate()
{
	$from = new DateTime('now',new DateTimeZone('UTC'));
	$from = $from->modify('-1 day');
	
	$to = new DateTime('now',new DateTimeZone('UTC'));
	$to = $to->modify('+2 days');
	
	
	// ToDo : En lugar de buscar los partidos 
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_buffer_match', 'value', 1, '=')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')
		  ->fieldCondition('field_match_date', 'value', $to->format('Y-m-d H:i:s'), '<=');
		  
	$result = $query->execute();
	
	$return_array = array();
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$match_items = entity_load('node', $matchids);
		
		foreach($match_items as $match)
		{
			//if(tips_match_have_tip($match->nid,$accuracy))
			if (!cache_get('tips_buffer_generate_' . $match->nid, 'cache')) 
			{
				
				$matchdate = $match->field_match_date[LANGUAGE_NONE][0]['value'];
				
				$texten = array();
				$textes = array();
				$date = new DateTime($matchdate,new DateTimeZone('UTC'));
				$date->setTimezone(new DateTimeZone('America/Mexico_City'));
				$date930am = new DateTime($date->format('Y-m-d') . ' 09:30',new DateTimeZone('America/Mexico_City'));
				if($date > $date930am)
				{
					// ToDo : Random between 1 y 20 para los minutos
					$date900am = new DateTime($date->format('Y-m-d') . ' 09:' . getRandomMinutesStr(0,20) ,new DateTimeZone('America/Mexico_City'));
					$date900am->setTimezone(new DateTimeZone('UTC'));
					tips_buffer_bufferapp_data(TRUE,NULL, getTweetTextES($match,$textes), getImagesByMatch($match), FALSE, $date900am->getTimestamp(), '@soccer_labs');
					//tips_buffer_bufferapp_data(TRUE,NULL, getTweetTextES($match,$textes), getImagesByMatch($match), FALSE, $date900am->getTimestamp(), 'Soccer Labs');
				}
				
				$date->setTimezone(new DateTimeZone('Europe/London'));
				$date930am = new DateTime($date->format('Y-m-d') . ' 09:30',new DateTimeZone('Europe/London'));
				if($date > $date930am)
				{
					// ToDo : Random between 1 y 20 para los minutos
					$date900am = new DateTime($date->format('Y-m-d') . ' 09:' . getRandomMinutesStr(0,20) ,new DateTimeZone('Europe/London'));
					$date900am->setTimezone(new DateTimeZone('UTC'));
					tips_buffer_bufferapp_data(TRUE,NULL, getTweetTextEN($match,$texten), getImagesByMatch($match), FALSE, $date900am->getTimestamp(), '@soccer_labsen');
				}
				
				$date = new DateTime($matchdate,new DateTimeZone('UTC'));
/*
				tips_buffer_bufferapp_data(TRUE,NULL, getTweetTextES($match,$textes), getImagesByMatch($match), FALSE, $date->getTimestamp(), '@soccer_labs');
				tips_buffer_bufferapp_data(TRUE,NULL, getTweetTextEN($match,$texten), getImagesByMatch($match), FALSE, $date->getTimestamp(), '@soccer_labsen');
*/
				
				
				$date = $date->modify('-15 minutes');
				tips_buffer_bufferapp_data(TRUE,NULL, getTweetTextES($match,$textes), getImagesByMatch($match), FALSE, $date->getTimestamp(), '@soccer_labs');
				//tips_buffer_bufferapp_data(TRUE,NULL, getTweetTextES($match,$textes), getImagesByMatch($match), FALSE, $date->getTimestamp(), 'Soccer Labs');
				tips_buffer_bufferapp_data(TRUE,NULL, getTweetTextEN($match,$texten), getImagesByMatch($match), FALSE, $date->getTimestamp(), '@soccer_labsen');
				
				cache_set('tips_buffer_generate_' . $match->nid, 1, 'cache', time() + (7 * 24 * 60 * 60) );	
			}	
			
			
		}
	}
	
	
	 
}


function getRandomMinutesStr($from,$to)
{
	$randomint = rand($from,$to);
	return str_pad($randomint, 2, "0", STR_PAD_LEFT);
}


function getImagesByMatch($match)
{
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	
	
	$images = array();
	if(isset($local_team->field_image['und'][0]['uri']))
	{
		$img_url_local = $local_team->field_image['und'][0]['uri'];
		$img_url_local = image_style_url("medium_circle", $img_url_local);
		$images[] = array(
			'picture' => $img_url_local,
			'thumbnail' => $img_url_local,
		);
		
	}

	if(isset($visitor_team->field_image['und'][0]['uri']))
	{
		$img_url_visitor = $visitor_team->field_image['und'][0]['uri'];
		$img_url_visitor = image_style_url("medium_circle", $img_url_visitor);
		$images[] = array(
			'picture' => $img_url_visitor,
			'thumbnail' => $img_url_visitor,
		);
		
	}
	
	//$images[] = array();
						
	if(count($images)==0) return $images;					
	$tweet = $images[array_rand($images)];	
	return $tweet;
}


function getMsgTextES($match,&$oldtexts,$nourl=false)
{
	$arraystr = array();
	$arraystr[] = '[local_team] vs [visitor_team] [url]';
	$arraystr[] = 'Descubre los tips para el partido [local_team] vs [visitor_team] [url]';
	$arraystr[] = 'Revisa los tips para [local_team] vs [visitor_team] [url]';
	$arraystr[] = 'Pronósticos [local_team] vs [visitor_team] [url]';
	$arraystr[] = 'Tenemos tips esperando para ti en el [local_team] contra [visitor_team] [url]';
	$arraystr[] = 'Nuestros tips en el [local_team] contra [visitor_team] [url]';
	$arraystr[] = 'Ya te enteraste de los tips de [local_team] vs [visitor_team] ? [url]';
	
	foreach($arraystr as $id => $str)
		if(is_array($oldtexts)) 
			if(in_array($str, $oldtexts))
				unset($arraystr[$id]);
	
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/es', 
						'absolute' => true,
						'query' => array('utm_source' => 'soccer_labs_es',
											'utm_medium' => 'twitter',
											'utm_campaign' => 'buffer' ),
						'language' => (object)array(
							  'language' => 'es',
							)
						));	
	
	
	
	$tweet = $arraystr[array_rand($arraystr)];
	$oldtexts[] = $tweet;
	$tweet = str_ireplace('[local_team]', $local_team->title, $tweet);
	$tweet = str_ireplace('[visitor_team]', $visitor_team->title, $tweet);
	if($nourl)
		$tweet = str_ireplace('[url]', '', $tweet);
	else
		$tweet = str_ireplace('[url]', $url, $tweet);
	return $tweet;
}

function getMsgTextEN($match,&$oldtexts,$nourl=false)
{
	$arraystr = array();
	$arraystr[] = '[local_team] vs [visitor_team] [url]';
	$arraystr[] = 'Check tips for [local_team] vs [visitor_team] [url]';
	$arraystr[] = 'Amazing tips for [local_team] vs [visitor_team] [url]';
	
	foreach($arraystr as $id => $str)
		if(is_array($oldtexts)) 
			if(in_array($str, $oldtexts))
				unset($arraystr[$id]);
			
			
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/en', 
						'absolute' => true,
						'query' => array('utm_source' => 'soccer_labs_en',
											'utm_medium' => 'twitter',
											'utm_campaign' => 'buffer' ),
						'language' => (object)array(
							  'language' => 'en',
							)
						));	
						
	$tweet = $arraystr[array_rand($arraystr)];
	$oldtexts[] = $tweet;
	$tweet = str_ireplace('[local_team]', $local_team->title, $tweet);
	$tweet = str_ireplace('[visitor_team]', $visitor_team->title, $tweet);
	if($nourl)
		$tweet = str_ireplace('[url]', '', $tweet);
	else
		$tweet = str_ireplace('[url]', $url, $tweet);
	return $tweet;
}



function getTweetTextES($match,&$oldtexts,$nourl=false)
{
	$arraystr = array();
/*
	$arraystr[] = '[local_team] vs [visitor_team] [url] #pronosticos #futbol #apuestas RT pls';
	$arraystr[] = 'Descubre los tips para el partido [local_team] vs [visitor_team] [url] #pronósticos #futbol #apuestas';
	$arraystr[] = 'Revisa los tips para [local_team] vs [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Pronósticos [local_team] vs [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = '[local_team] vs [visitor_team] se enfrentan hoy [url] RT pls #pronosticos #futbol #apuestas';
	$arraystr[] = 'Tenemos tips esperando para ti en el [local_team] contra [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Nuestros tips en el [local_team] contra [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Hoy [local_team] contra [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Ya te enteraste de los tips de [local_team] vs [visitor_team] ? [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Dale click [url] para ver los tips, [local_team] vs [visitor_team] #pronosticos #futbol #apuestas RT pls';
	$arraystr[] = '[url] , [local_team] vs [visitor_team] #pronosticos #futbol #apuestas RT pls';
	$arraystr[] = '[url] Pronósticos [local_team] vs [visitor_team] #pronosticos #futbol #apuestas RT pls';
	$arraystr[] = '[url] Tips, [local_team] contra [visitor_team] #pronosticos #futbol #apuestas RT pls';
	
*/
	$arraystr[] = '[local_team] vs [visitor_team] [url] atrévete a apostar gratis #pronosticos #futbol #apuestas RT pls';
	$arraystr[] = 'Apuesta en el siguiente partido y gana [local_team] vs [visitor_team] [url] #pronósticos #futbol #apuestas';
	$arraystr[] = 'Revisa los tips para [local_team] vs [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Tramos los pronósticos para que ganes [local_team] vs [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = '[local_team] vs [visitor_team] se enfrentan hoy [url] Apuesta gratis #futbol #apuestas';
	$arraystr[] = 'Te damos los mejores tips para apostar en [local_team] contra [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Tips en el [local_team] vs [visitor_team] [url] para apostar gratis #pronosticos #futbol #apuestas';
	$arraystr[] = 'Hoy [local_team] vs [visitor_team] [url] apuesta gratis #futbol #apuestas';
	$arraystr[] = 'Los mejores tips en [local_team] vs [visitor_team] [url] para apostar #futbol #apuestas';
	$arraystr[] = 'Dale click [url] para ver los tips, [local_team] vs [visitor_team] y apostar #futbol #apuestas RT pls';
	$arraystr[] = '[url] , [local_team] vs [visitor_team] aprovecha tips y apuesta gratis #futbol #apuestas RT pls';
	$arraystr[] = '[url] Pronósticos [local_team] vs [visitor_team] aprovecha, apuesta y gana #futbol #apuestas RT pls';
	$arraystr[] = '[url] Tips, [local_team] contra [visitor_team] apostar gratis está a tu alcance #futbol #apuestas RT pls';
	
	foreach($arraystr as $id => $str)
		if(is_array($oldtexts)) 
			if(in_array($str, $oldtexts))
				unset($arraystr[$id]);
	
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/es', 
						'absolute' => true,
						'query' => array('utm_source' => 'soccer_labs_es',
											'utm_medium' => 'twitter',
											'utm_campaign' => 'buffer' ),
						'language' => (object)array(
							  'language' => 'es',
							)
						));	
	
	
	
	$tweet = $arraystr[array_rand($arraystr)];
	$oldtexts[] = $tweet;
	$tweet = str_ireplace('[local_team]', $local_team->title, $tweet);
	$tweet = str_ireplace('[visitor_team]', $visitor_team->title, $tweet);
	if($nourl)
		$tweet = str_ireplace('[url]', '', $tweet);
	else
		$tweet = str_ireplace('[url]', $url, $tweet);
	return $tweet;
}

function getTweetTextEN($match,&$oldtexts,$nourl=false)
{
	$arraystr = array();
	$arraystr[] = '[local_team] vs [visitor_team] [url] #soccer #football';
	$arraystr[] = '[url] , [local_team] vs [visitor_team] RT pls';
	$arraystr[] = '[url] Tips, [local_team] vs [visitor_team] RT pls';
	$arraystr[] = 'Check tips for [local_team] vs [visitor_team] [url] #soccer #football';
	$arraystr[] = 'Today [local_team] vs [visitor_team] [url] RT pls';
	$arraystr[] = 'Amazing tips for [local_team] vs [visitor_team] [url] RT pls';
	$arraystr[] = '[local_team] is playing vs [visitor_team] today! [url] #soccer #football';
	$arraystr[] = 'Who are you backing today? [local_team] vs [visitor_team] ! [url] RT pls';
	
	foreach($arraystr as $id => $str)
		if(is_array($oldtexts)) 
			if(in_array($str, $oldtexts))
				unset($arraystr[$id]);
			
			
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/en', 
						'absolute' => true,
						'query' => array('utm_source' => 'soccer_labs_en',
											'utm_medium' => 'twitter',
											'utm_campaign' => 'buffer' ),
						'language' => (object)array(
							  'language' => 'en',
							)
						));	
						
	$tweet = $arraystr[array_rand($arraystr)];
	$oldtexts[] = $tweet;
	$tweet = str_ireplace('[local_team]', $local_team->title, $tweet);
	$tweet = str_ireplace('[visitor_team]', $visitor_team->title, $tweet);
	if($nourl)
		$tweet = str_ireplace('[url]', '', $tweet);
	else
		$tweet = str_ireplace('[url]', $url, $tweet);
	return $tweet;
}

function getTweetTextAccuracyES($match,&$oldtexts)
{
	$arraystr = array();
	$arraystr[] = 'Revisa los tips exitosos de ayer para [local_team] vs [visitor_team] [url] #pronosticos #futbol #apuestas RT';
	$arraystr[] = 'Pronósticos acertados de ayer en [local_team] vs [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Nuestros tips fueron acertados en el [local_team] contra [visitor_team] [url] #pronosticos #futbol #apuestas RT';
	$arraystr[] = 'Ya te enteraste de los tips exitosos de ayer en [local_team] vs [visitor_team] ? [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Dale click [url] para ver los tips acertados, [local_team] vs [visitor_team] #pronosticos #futbol #apuestas RT pls';
	$arraystr[] = '[url] Pronósticos acertados [local_team] vs [visitor_team] #pronosticos #futbol #apuestas';
	$arraystr[] = '[url] Tips acertados, [local_team] contra [visitor_team] #pronosticos #futbol #apuestas ';
	
	foreach($arraystr as $id => $str)
		if(is_array($oldtexts)) 
			if(in_array($str, $oldtexts))
				unset($arraystr[$id]);
	
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/es', 
						'absolute' => true,
						'query' => array('utm_source' => 'soccer_labs_es',
											'utm_medium' => 'twitter',
											'utm_campaign' => 'buffer' ),
						'language' => (object)array(
							  'language' => 'es',
							)
						));	
	
	
	
	$tweet = $arraystr[array_rand($arraystr)];
	$oldtexts[] = $tweet;
	$tweet = str_ireplace('[local_team]', $local_team->title, $tweet);
	$tweet = str_ireplace('[visitor_team]', $visitor_team->title, $tweet);
	$tweet = str_ireplace('[url]', $url, $tweet);
	return $tweet;
}

function getTweetTextAccuracyEN($match,&$oldtexts)
{
	$arraystr = array();
	$arraystr[] = '[url] Successful Tips, [local_team] vs [visitor_team] #soccer #football';
	$arraystr[] = 'Check our successful tips for [local_team] vs [visitor_team] [url] RT pls';
	$arraystr[] = 'Awesome successful tips yesterday for [local_team] vs [visitor_team] [url] #soccer #football';
	$arraystr[] = 'Do you know about our successful tips yesterday? [local_team] vs [visitor_team] ! [url]';
	
	foreach($arraystr as $id => $str)
		if(is_array($oldtexts)) 
			if(in_array($str, $oldtexts))
				unset($arraystr[$id]);
			
			
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/en', 
						'absolute' => true,
						'query' => array('utm_source' => 'soccer_labs_en',
											'utm_medium' => 'twitter',
											'utm_campaign' => 'buffer' ),
						'language' => (object)array(
							  'language' => 'en',
							)
						));	
						
	$tweet = $arraystr[array_rand($arraystr)];
	$oldtexts[] = $tweet;
	$tweet = str_ireplace('[local_team]', $local_team->title, $tweet);
	$tweet = str_ireplace('[visitor_team]', $visitor_team->title, $tweet);
	$tweet = str_ireplace('[url]', $url, $tweet);
	return $tweet;
}

function getTweetTextAccuracyNowES($match,&$oldtexts)
{
	$arraystr = array();
	$arraystr[] = 'Revisa los tips exitosos de para [local_team] vs [visitor_team] [url] #pronosticos #futbol #apuestas';
	$arraystr[] = 'Pronósticos acertados en [local_team] vs [visitor_team] [url] RT';
	$arraystr[] = 'Nuestros tips fueron acertados en el [local_team] contra [visitor_team] [url] #pronosticos #futbol #apuestas RT pls';
	$arraystr[] = 'Ya te enteraste de los tips exitosos en [local_team] vs [visitor_team] ? [url]';
	$arraystr[] = 'Dale click [url] para ver los tips acertados, [local_team] vs [visitor_team]';
	$arraystr[] = '[url] Pronósticos acertados [local_team] vs [visitor_team] #pronosticos #futbol #apuestas';
	$arraystr[] = '[url] Tips acertados, [local_team] contra [visitor_team] RT pls';
	
	foreach($arraystr as $id => $str)
		if(is_array($oldtexts)) 
			if(in_array($str, $oldtexts))
				unset($arraystr[$id]);
	
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/es', 
						'absolute' => true,
						'query' => array('utm_source' => 'soccer_labs_es',
											'utm_medium' => 'twitter',
											'utm_campaign' => 'buffer' ),
						'language' => (object)array(
							  'language' => 'es',
							)
						));	
	
	
	
	$tweet = $arraystr[array_rand($arraystr)];
	$oldtexts[] = $tweet;
	$tweet = str_ireplace('[local_team]', $local_team->title, $tweet);
	$tweet = str_ireplace('[visitor_team]', $visitor_team->title, $tweet);
	$tweet = str_ireplace('[url]', $url, $tweet);
	return $tweet;
}

function getTweetTextAccuracyNowEN($match,&$oldtexts)
{
	$arraystr = array();
	$arraystr[] = '[url] Successful Tips, [local_team] vs [visitor_team] #soccer #football';
	$arraystr[] = 'Check our successful tips for [local_team] vs [visitor_team] [url] #soccer #football';
	$arraystr[] = 'Awesome successful tips for [local_team] vs [visitor_team] [url]';
	$arraystr[] = 'Do you know about our successful tips? [local_team] vs [visitor_team] ! [url] RT pls';
	
	foreach($arraystr as $id => $str)
		if(is_array($oldtexts)) 
			if(in_array($str, $oldtexts))
				unset($arraystr[$id]);
			
			
	$local_team = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitor_team = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/en', 
						'absolute' => true,
						'query' => array('utm_source' => 'soccer_labs_en',
											'utm_medium' => 'twitter',
											'utm_campaign' => 'buffer' ),
						'language' => (object)array(
							  'language' => 'en',
							)
						));	
						
	$tweet = $arraystr[array_rand($arraystr)];
	$oldtexts[] = $tweet;
	$tweet = str_ireplace('[local_team]', $local_team->title, $tweet);
	$tweet = str_ireplace('[visitor_team]', $visitor_team->title, $tweet);
	$tweet = str_ireplace('[url]', $url, $tweet);
	return $tweet;
}
