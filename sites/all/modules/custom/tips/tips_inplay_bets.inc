<?php

define("OVER_0_5_BY_MATCH_INPLAY_PROBABILITY", .7);
define("OVER_0_5_BY_MATCH_INPLAY_MINUTE", 60);
define("OVER_0_5_BY_MATCH_INPLAY_MIN_ODDS", 1.2);

// Function to check games in play
function tips_inplay_bets_check()
{
	libraries_load('tipgin-api');
	$tipgin = new TipGinApi(variable_get('tipgin_api_feed_apikey', ''));
	$tipgin->getOddsFile();
	
	$from = new DateTime('now',new DateTimeZone('UTC'));
	$from = $from->modify('-2 hours');
	
	$to = new DateTime('now',new DateTimeZone('UTC'));
	$to = $to->modify('+2 hours');
	
	// ToDo: Check games
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_match_status', 'value', 'FT', '<>')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>')
		  ->fieldCondition('field_match_date', 'value', $to->format('Y-m-d H:i:s'), '<');
		  
	$result = $query->execute();
	$return_array = array();
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$match_items = entity_load('node', $matchids);
		foreach($match_items as $match)
		{
			tips_inplay_bets_check_to_create($match);
		}
		watchdog('inplaygames','inplaygames ' . count($match_items));
	}
}


// Check by match
function tips_inplay_bets_check_to_create($matchnode)
{
	module_load_include('inc', 'tips', 'tips_tipfin_odds');
	module_load_include('inc', 'tips', 'tips_poisson');
	
	if($matchnode->field_local_team_score[LANGUAGE_NONE][0]['value'] > 0 || $matchnode->field_visitor_team_score[LANGUAGE_NONE][0]['value'] > 0) return false;
	
	$match_status = $matchnode->field_match_status[LANGUAGE_NONE][0]['value'];
	if(stripos($match_status, ':')!==false) return true;
	if($match_status=='HT') $match_status = '45';
	$remaining_time = intval($match_status);
	if($remaining_time > OVER_0_5_BY_MATCH_INPLAY_MINUTE) return false;
	$remaining_time = ( 90 - $remaining_time ) / 90 ;
	$posson_stats = tips_poisson_distribution($matchnode,$remaining_time);
	$lid = $matchnode->field_local_team[LANGUAGE_NONE][0]['target_id'];
	$vid = $matchnode->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
	$tipginapi_id = $matchnode->field_tipginapi_id[LANGUAGE_NONE][0]['value'];
	$over_0_5_by_match = tips_poisson_over_under_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], 'over', 0.5);
	watchdog('inplaygames','Over 0.5 prob : ' . $matchnode->nid . ' ' . $over_0_5_by_match . ' ' . $remaining_time);
	if($over_0_5_by_match >= OVER_0_5_BY_MATCH_INPLAY_PROBABILITY)
	{
		$params = array($over_0_5_by_match, $posson_stats, $match_status);
		tips_inplay_bets_check_odds($matchnode, OVER_0_5_BY_MATCH_INPLAY_MIN_ODDS,$params);
	}
	
	
}


function tips_inplay_bets_check_odds($matchnode, $oddssuggested,$params)
{
	$competition = node_load($matchnode->field_competition[LANGUAGE_NONE][0]['target_id']);
	$country = $competition->field_country[LANGUAGE_NONE][0]['value'];
	$node_local_team = node_load($matchnode->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$node_visitor_team = node_load($matchnode->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	
	$tipginapi_id = 'noid';
	if(isset($matchnode->field_tipginapi_id[LANGUAGE_NONE][0]['value']))
		if(trim($matchnode->field_tipginapi_id[LANGUAGE_NONE][0]['value'])!='')
			$tipginapi_id = $matchnode->field_tipginapi_id[LANGUAGE_NONE][0]['value'];
	
	libraries_load('tipgin-api');
	$tipgin = new TipGinApi(variable_get('tipgin_api_feed_apikey', ''));
	$odds = $tipgin->getOdds($country);
	
	
	if(isset($odds['league']['@attributes'])) $odds['league'] = array($odds['league']);
		
	if(isset($odds['league']))
		foreach($odds['league'] as $league)
		{
			if(isset($league['match']['@attributes'])) $league['match'] = array($league['match']);
			foreach($league['match'] as $match) 
			{
				if($tipginapi_id=='noid')
				{
					$localteam = _tipgin_api_feed_team_exists($match['home'],$league);
					$visitorteam = _tipgin_api_feed_team_exists($match['away'],$league);
					if($localteam&&$visitorteam)
					{
						if($node_local_team->nid==$localteam->nid&&$node_visitor_team->nid==$visitorteam->nid)
							$tipginapi_id=$match['@attributes']['id'];		
					}
				}
				if($tipginapi_id==$match['@attributes']['id'])
				{
					if(isset($match['odds']['type']))
					{
						foreach($match['odds']['type'] as $type)
						{
							if($type['@attributes']['name']=='Over/Under')
								tips_inplay_bets_check_to_create_overunder_bet($matchnode, $type, '0.5', 'Over', $oddssuggested, $odds['@attributes']['updated'], $params);
							
						}	
					}
					
				}
			}
		}	
			
	
}	


function tips_inplay_bets_check_to_create_overunder_bet($matchnode, $type, $namebet, $overunderbet, $oddssuggested, $updated, $params)
{
	$bookmakername = '';
	$allbookmakername = '';
	$betterodd = 0;
	if(isset($type['bookmaker']))
	{
		foreach($type['bookmaker'] as $bookmaker)
		{
			if(isset($bookmaker['total']))
			{ 
				if(isset($bookmaker['total']['@attributes'])) $bookmaker['total'] = array($bookmaker['total']);
				foreach($bookmaker['total'] as $total)
				{
					foreach($total['odd'] as $odd)
					{
						if($namebet==$total['@attributes']['name']
							&&$overunderbet==$odd['@attributes']['name']) 
							watchdog('inplaygames','Over 0.5 odds : ' . $matchnode->nid . ' ' . $oddssuggested . ' ' . $odd['@attributes']['value'] . ' ' . $bookmaker['@attributes']['name']);
						
						if($namebet==$total['@attributes']['name']
						&&$overunderbet==$odd['@attributes']['name']
						&&doubleval($oddssuggested)<=doubleval($odd['@attributes']['value'])) 
						{
							if(doubleval($odd['@attributes']['value'])>$betterodd)
							{
								$bookmakername = $bookmaker['@attributes']['name'];
								$allbookmakername .= $bookmakername . ' ';
								$betterodd = $odd['@attributes']['value'];	
							}
						}	
					}
				}
			}
		}
	}
	if($betterodd>0) 
		tips_inplay_bets_create_bet($matchnode, $bookmakername, $betterodd, 
			$allbookmakername . ' ' . $updated, $params);	
}



function tips_inplay_bets_create_bet($matchnode, $bookmaker, $odds, $otherbookmakers = '', $params)
{
	$title = 'Over 0.5 by match after 0-0 In Play : '  . intval($params[0]*100) . '% ';	
	$params[1]['match_status'] = $params[2];
	$tip = tips_create_tip($matchnode, json_encode($params[1]), $title, 0, $params[0], 777);	
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'bet')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_tip', 'target_id', $tip->nid, '=');
	
	$result = $query->execute();
	
	$bet = false;
	if (!isset($result['node'])) 
	{
		$bet = new stdClass();
		$bet->type = 'bet';
		$bet->status = 1;
		$bet->uid = 1;
		$bet->title = $tip->title;
		$bet->promote = 0;
		$bet->created = time();
		$bet->timestamp = $bet->created;
		$bet->changed = $bet->created;
		$bet->sticky = 0;
		$bet->revision = 0;
		$bet->language = 'und';
		
		$bet->body[LANGUAGE_NONE][0]['value'] = $otherbookmakers; 
		$bet->field_tip[LANGUAGE_NONE][0]['target_id'] = $tip->nid;
		$bet->field_bet_odds[LANGUAGE_NONE][0]['value'] = $odds;
		createTermsFromString($bookmaker, ',' , 'bookmaker', $bet->field_bookmaker);
		node_save($bet);
				
	}
	return $bet;	

}



	


?>