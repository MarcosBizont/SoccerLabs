<?php

function tips_tipfin_odds_tips_checktips_ht()
{
	libraries_load('tipgin-api');
	$tipgin = new TipGinApi(variable_get('tipgin_api_feed_apikey', ''));
	$tipgin->getOddsFile();
	
	$from = new DateTime();
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'tip')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_tip_type', 'target_id', array(772,773), 'IN')
		  ->fieldCondition('field_win', 'value', 0, '=');
		  
	$result = $query->execute();
	$return_array = array();
	
	if (isset($result['node'])) 
	{
		$tipids = array_keys($result['node']);
		$tip_items = entity_load('node', $tipids);
		foreach($tip_items as $tip)
		{
			tips_tipfin_odds_check_odds_for_tip_ht($tip);
		}
	}
}

function tips_tipfin_odds_check_odds_for_tip_ht($tip)
{
	$matchnode = node_load($tip->field_match[LANGUAGE_NONE][0]['target_id']);
	if($matchnode->field_match_status[LANGUAGE_NONE][0]['value']!='HT') return false;
	
	$competition = node_load($tip->field_competition[LANGUAGE_NONE][0]['target_id']);
	$country = $competition->field_country[LANGUAGE_NONE][0]['value'];
	$tiptype = taxonomy_term_load($tip->field_tip_type['und'][0]['target_id']);
	$oddssuggested = $tiptype->field_odds_suggested['und'][0]['value'];
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
				// ToDo : El id puede no estar presente, buscar por ids de equipos con librerias
				if($tipginapi_id=='noid')
				{
					$localteam = _tipgin_api_feed_team_exists($match['home']);
					$visitorteam = _tipgin_api_feed_team_exists($match['away']);
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
							if( ($type['@attributes']['name']=='Over/Under' || $type['@attributes']['name']=='Over/Under 2nd Half') && $tiptype->tid == 772)
								tips_tipfin_odds_check_to_create_overunder_bet($tip, $type, '0.5', 'Over', $oddssuggested, $odds['@attributes']['updated']);
							
							if( ($type['@attributes']['name']=='Over/Under' || $type['@attributes']['name']=='Over/Under 2nd Half') && $tiptype->tid == 773 )
								tips_tipfin_odds_check_to_create_overunder_bet($tip, $type, '1.5', 'Over', $oddssuggested, $odds['@attributes']['updated']);
							
						}	
					}
					
				}
			}
		}
			
			
			
	
}

function tips_tipfin_odds_tips_checktips()
{
	libraries_load('tipgin-api');
	$tipgin = new TipGinApi(variable_get('tipgin_api_feed_apikey', ''));
	$tipgin->getOddsFile();
	
	$from = new DateTime('now',new DateTimeZone('UTC'));
	
	// ToDo : Right now only before game tips
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'tip')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_tip_type', 'target_id', array(674,593,577,576,578,753,754,755,573,787,832), 'IN')
		  ->fieldCondition('field_win', 'value', 0, '=')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>');
		  
	// ,577,576
	$result = $query->execute();
	
	$return_array = array();
	
	if (isset($result['node'])) 
	{
		$tipids = array_keys($result['node']);
		$tip_items = entity_load('node', $tipids);
		foreach($tip_items as $tip)
		{
			tips_tipfin_odds_check_odds_for_tip($tip);
		}
	}
}

function tips_tipfin_odds_check_odds_for_tip($tip)
{
	$competition = node_load($tip->field_competition[LANGUAGE_NONE][0]['target_id']);
	$matchnode = node_load($tip->field_match[LANGUAGE_NONE][0]['target_id']);
	$country = $competition->field_country[LANGUAGE_NONE][0]['value'];
	$tiptype = taxonomy_term_load($tip->field_tip_type['und'][0]['target_id']);
	$oddssuggested = $tiptype->field_odds_suggested['und'][0]['value'];
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
				// ToDo : El id puede no estar presente, buscar por ids de equipos con librerias
				if($tipginapi_id=='noid')
				{
					$localteam = _tipgin_api_feed_team_exists($match['home']);
					$visitorteam = _tipgin_api_feed_team_exists($match['away']);
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
							if($type['@attributes']['name']=='Both Teams to Score' && $tiptype->tid == 573)
								tips_tipfin_odds_check_to_create_simple_bet($tip, $type, 'Yes', $oddssuggested, $odds['@attributes']['updated']);
								
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 578 || $tiptype->tid == 753))
								tips_tipfin_odds_check_to_create_overunder_bet($tip, $type, '1.5', ($tiptype->tid == 753) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 577 || $tiptype->tid == 576))
								tips_tipfin_odds_check_to_create_overunder_bet($tip, $type, '2.5', ($tiptype->tid == 577) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 754 || $tiptype->tid == 755) )
								tips_tipfin_odds_check_to_create_overunder_bet($tip, $type, '3.5', ($tiptype->tid == 755) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Double Chance' && $tiptype->tid == 674)
								tips_tipfin_odds_check_to_create_simple_bet($tip, $type, 
									($tip->field_team['und'][0]['target_id']==$matchnode->field_local_team['und'][0]['target_id']) ? '1X' : 'X2',$oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='1x2' && $tiptype->tid == 787)
								tips_tipfin_odds_check_to_create_simple_bet($tip, $type, 'X', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='1x2' && $tiptype->tid == 832)
								return tips_tipfin_odds_library_check_options_simple_bet($tip, $type, ($tip->field_team['und'][0]['target_id']==$matchnode->field_local_team['und'][0]['target_id']) ? '1' : '2', $oddssuggested, $odds['@attributes']['updated']);
						}	
					}
					
				}
			}
		}	
			
	
}	

function tips_tipfin_odds_check_to_create_overunder_bet($tip, $type, $namebet, $overunderbet, $oddssuggested, $updated)
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
		tips_tipfin_odds_create_bet($tip, $bookmakername, $betterodd, 
			$allbookmakername . ' ' . $updated);	
}

function tips_tipfin_odds_check_to_create_simple_bet($tip, $type, $namebet,$oddssuggested, $updated)
{
	$bookmakername = '';
	$allbookmakername = '';
	$betterodd = 0;
	if(isset($type['bookmaker']))
	{
		foreach($type['bookmaker'] as $bookmaker)
		{
			if(isset($bookmaker['odd'])) 
			foreach($bookmaker['odd'] as $odd)
			{
				if(isset($odd['@attributes']))
					if($namebet==$odd['@attributes']['name']
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
	if($betterodd>0) 
		tips_tipfin_odds_create_bet($tip, $bookmakername, $betterodd, 
			$allbookmakername . ' ' . $updated);
}


function tips_tipfin_odds_create_bet($tip, $bookmaker, $odds, $otherbookmakers = '')
{
	if($bookmaker=='Bet Fred') return false;
	// Validate a bed before anything
	//$tiptype = taxonomy_term_load($tip->field_tip_type['und'][0]['target_id']);
	//$oddssuggested = $tiptype->field_odds_suggested['und'][0]['value'];
	
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