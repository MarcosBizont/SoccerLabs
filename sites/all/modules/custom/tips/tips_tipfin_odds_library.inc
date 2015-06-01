<?php
	
	
function tips_tipfin_odds_library_check_odds_for_match($matchid, $tiptypestocheck, $options)
{
	// Tip type to check
	// Cache de 10 minutos
	
	$alltips = array();
	foreach($tiptypestocheck as $tiptype) 
	{	
		$obj = tips_tipfin_odds_library_check_odds_for_match_for_tiptype($matchid, $tiptype);

		$odd = array();
		if($tiptype == 832)
		{
			foreach($obj as $key => $objitem)
			{
				foreach ($objitem as $row)
				{
				    $odd[$key][$row['bookmakername']] = $row['odd'];
				}
				array_multisort($odd[$key], SORT_DESC, $objitem);		
			}
			
		}
		else
		{
			foreach ($obj as $row)
			{
			    $odd[$row['bookmakername']] = $row['odd'];
			}
			array_multisort($odd, SORT_DESC, $obj);
		}
		
		if(count($odd)>0) $alltips[$tiptype] = $odd;

	}
	
	return $alltips;
}

function tips_tipfin_odds_library_check_odds_for_match_for_tiptype($matchid, $tiptypeid)
{
	$matchnode = node_load($matchid);
	$competition = node_load($matchnode->field_competition[LANGUAGE_NONE][0]['target_id']);
	$country = $competition->field_country[LANGUAGE_NONE][0]['value'];
	$tiptype = taxonomy_term_load($tiptypeid);
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
								return tips_tipfin_odds_library_check_options_simple_bet(FALSE, $type, 'Yes', $oddssuggested, $odds['@attributes']['updated']);
								
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 578 || $tiptype->tid == 753))
								return tips_tipfin_odds_library_check_options_overunder_bet(FALSE, $type, '1.5', ($tiptype->tid == 753) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 577 || $tiptype->tid == 576))
								return tips_tipfin_odds_library_check_options_overunder_bet(FALSE, $type, '2.5', ($tiptype->tid == 577) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 754 || $tiptype->tid == 755) )
								return tips_tipfin_odds_library_check_options_overunder_bet(FALSE, $type, '3.5', ($tiptype->tid == 755) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Double Chance' && $tiptype->tid == 674)
							{
								$retarray = array();
								$retarray['1X'] =  
									tips_tipfin_odds_library_check_options_simple_bet(FALSE, 
										$type,'1X',$oddssuggested, $odds['@attributes']['updated']);
								$retarray['X2'] =  
									tips_tipfin_odds_library_check_options_simple_bet(FALSE, 
										$type,'X2',$oddssuggested, $odds['@attributes']['updated']);
											
								return $retarray;
							}
								
							
							if($type['@attributes']['name']=='1x2' && $tiptype->tid == 787)
								return tips_tipfin_odds_library_check_options_simple_bet(FALSE, $type, 'X', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='1x2' && $tiptype->tid == 832)
							{
								$retarray = array();
								$retarray['1'] =  
									tips_tipfin_odds_library_check_options_simple_bet(FALSE, 
										$type, '1', $oddssuggested, $odds['@attributes']['updated']);
								$retarray['2'] =  
									tips_tipfin_odds_library_check_options_simple_bet(FALSE, 
										$type, '2', $oddssuggested, $odds['@attributes']['updated']);
											
								return $retarray;
							}
						}	
					}
				}
			}
		}	
			
	return array();
}

function tips_tipfin_odds_library_check_odds_for_tip($tip)
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
								return tips_tipfin_odds_library_check_options_simple_bet($tip, $type, 'Yes', $oddssuggested, $odds['@attributes']['updated']);
								
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 578 || $tiptype->tid == 753))
								return tips_tipfin_odds_library_check_options_overunder_bet($tip, $type, '1.5', ($tiptype->tid == 753) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 577 || $tiptype->tid == 576))
								return tips_tipfin_odds_library_check_options_overunder_bet($tip, $type, '2.5', ($tiptype->tid == 577) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Over/Under' && ($tiptype->tid == 754 || $tiptype->tid == 755) )
								return tips_tipfin_odds_library_check_options_overunder_bet($tip, $type, '3.5', ($tiptype->tid == 755) ? 'Over' : 'Under', $oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='Double Chance' && $tiptype->tid == 674)
								return tips_tipfin_odds_library_check_options_simple_bet($tip, $type, 
									($tip->field_team['und'][0]['target_id']==$matchnode->field_local_team['und'][0]['target_id']) ? '1X' : 'X2',$oddssuggested, $odds['@attributes']['updated']);
							
							if($type['@attributes']['name']=='1x2' && $tiptype->tid == 787)
								return tips_tipfin_odds_library_check_options_simple_bet($tip, $type, 'X', $oddssuggested, $odds['@attributes']['updated']);
								
							if($type['@attributes']['name']=='1x2' && $tiptype->tid == 832)
								return tips_tipfin_odds_library_check_options_simple_bet($tip, $type, ($tip->field_team['und'][0]['target_id']==$matchnode->field_local_team['und'][0]['target_id']) ? '1' : '2', $oddssuggested, $odds['@attributes']['updated']);
						}	
					}
				}
			}
		}	
			
	return array();
}	


function tips_tipfin_odds_library_check_options_overunder_bet($tip, $type, $namebet, $overunderbet, $oddssuggested, $updated)
{
	$tipoptions = array();
	
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
						if($namebet==$total['@attributes']['name']&&$overunderbet==$odd['@attributes']['name']) 
							$tipoptions[] = array('bookmakername' => $bookmaker['@attributes']['name'], 'odd' => $odd['@attributes']['value']);
					}
				}
			}
		}
	}
	
	return $tipoptions;	
}

function tips_tipfin_odds_library_check_options_simple_bet($tip, $type, $namebet,$oddssuggested, $updated)
{
	$tipoptions = array();
	if(isset($type['bookmaker']))
	{
		foreach($type['bookmaker'] as $bookmaker)
		{
			if(isset($bookmaker['odd'])) 
			foreach($bookmaker['odd'] as $odd)
			{
				if(isset($odd['@attributes']['name']))
					if($namebet==$odd['@attributes']['name']) 
						$tipoptions[] = array('bookmakername' => $bookmaker['@attributes']['name'], 'odd' => $odd['@attributes']['value']);
			}
		}
	}
	
	return $tipoptions;
}

	
?>