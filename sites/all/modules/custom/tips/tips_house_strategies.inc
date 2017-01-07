<?php

function tips_check_house_tips_creation($daystocheck)
{
	module_load_include('inc', 'tips', 'tips_poisson');
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
	
		
	$from = new DateTime('now',new DateTimeZone('UTC'));
	$from = $from->modify('+24 hours');
	
	$to = new DateTime('now',new DateTimeZone('UTC'));
	$to = $to->modify('+' . $daystocheck . ' days');
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_match_date', 'value', $to->format('Y-m-d H:i:s'), '<')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')		
		  ->fieldOrderBy('field_match_date', 'value', 'ASC')->range(0,150);
	
	$result = $query->execute();
	
	$return_array = array();
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$match_items = entity_load('node', $matchids);
		foreach($match_items as $match)
		{	
			$posson_stats = tips_poisson_distribution($match,false,5);
			$posson_stats2 = tips_poisson_distribution_both($match,false,5);
			$posson_stats3 = tips_poisson_distribution($match,false,10);
			$posson_stats4 = tips_poisson_distribution_both($match,false,10);
			if($posson_stats && $posson_stats2)
			{
				//dpm($match->title);
				//dpm($posson_stats);
				//dpm($posson_stats2);
				//dpm($posson_stats3);
				//dpm($posson_stats4);
				
				
				$probability = array();
				// 40% a Poisson
				
					// 60% as Local vs Visitante
						// 60% 5 juegos
				$probability['local']['LV']['POI'][] = tips_poisson_winner_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], '1');
				$probability['visitor']['LV']['POI'][] = tips_poisson_winner_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], '2');
					 
						// 40% 10 juegos
				if($posson_stats3&&$posson_stats4)
				{
					$probability['local']['LV']['POI'][] = tips_poisson_winner_match($posson_stats3['proximity_local'], $posson_stats3['proximity_visitor'], '1');
					$probability['visitor']['LV']['POI'][] = tips_poisson_winner_match($posson_stats3['proximity_local'], $posson_stats3['proximity_visitor'], '2');	
				}
				
					// 40% as juegos sin importar si es local o fisitante
						// 60% 5 juegos
				$probability['local']['BOTH']['POI'][] = tips_poisson_winner_match($posson_stats2['proximity_local'], $posson_stats2['proximity_visitor'], '1');
				$probability['visitor']['BOTH']['POI'][] = tips_poisson_winner_match($posson_stats2['proximity_local'], $posson_stats2['proximity_visitor'], '2');
						// 40% 10 juegos
				if($posson_stats3&&$posson_stats4)
				{
					$probability['local']['BOTH']['POI'][] = tips_poisson_winner_match($posson_stats4['proximity_local'], $posson_stats4['proximity_visitor'], '1');
					$probability['visitor']['BOTH']['POI'][] = tips_poisson_winner_match($posson_stats4['proximity_local'], $posson_stats4['proximity_visitor'], '2');	
				}
						
				// 60% a puntos en juegos
					// 60% as Local vs Visitante
						// 60% 5 juegos
				$probability['local']['LV']['PUNTS'][] = ((($posson_stats['local_stats']['wins_as_local'] * 3) + ($posson_stats['local_stats']['draws_as_local'] * 1)) / ($posson_stats['local_stats']['local_count'] * 3));
				$probability['visitor']['LV']['PUNTS'][] = ((($posson_stats['visitor_stats']['wins_as_visitor'] * 3) + ($posson_stats['visitor_stats']['draws_as_visitor'] * 1)) / ($posson_stats['visitor_stats']['visitor_count'] * 3));
				
						// 40% 10 juegos
				if($posson_stats3&&$posson_stats4)
				{
					$probability['local']['LV']['PUNTS'][] = ((($posson_stats3['local_stats']['wins_as_local'] * 3) + ($posson_stats3['local_stats']['draws_as_local'] * 1)) / ($posson_stats3['local_stats']['local_count'] * 3));
					$probability['visitor']['LV']['PUNTS'][] = ((($posson_stats3['visitor_stats']['wins_as_visitor'] * 3) + ($posson_stats3['visitor_stats']['draws_as_visitor'] * 1)) / ($posson_stats3['visitor_stats']['visitor_count'] * 3));
				}
				
					// 40% as juegos sin importar
						// 60% 5 juegos
				$probability['local']['BOTH']['PUNTS'][] = ((($posson_stats2['local_stats']['wins'] * 3) + ($posson_stats2['local_stats']['draws'] * 1)) / ($posson_stats2['local_stats']['count'] * 3));
				$probability['visitor']['BOTH']['PUNTS'][] = ((($posson_stats2['visitor_stats']['wins'] * 3) + ($posson_stats2['visitor_stats']['draws'] * 1)) / ($posson_stats2['visitor_stats']['count'] * 3));
						// 40% 10 juegos
				if($posson_stats3&&$posson_stats4)
				{
					$probability['local']['BOTH']['PUNTS'][] = ((($posson_stats4['local_stats']['wins'] * 3) + ($posson_stats4['local_stats']['draws'] * 1)) / ($posson_stats4['local_stats']['count'] * 3));
					$probability['visitor']['BOTH']['PUNTS'][] = ((($posson_stats4['visitor_stats']['wins'] * 3) + ($posson_stats4['visitor_stats']['draws'] * 1)) / ($posson_stats4['visitor_stats']['count'] * 3));
				}		
				
				
				if($posson_stats3&&$posson_stats4)
				{
					$poisson_local = (((($probability['local']['LV']['POI'][0] * .6) + ($probability['local']['LV']['POI'][1] * .4)) * .6) +
					((($probability['local']['BOTH']['POI'][0] * .6) + ($probability['local']['BOTH']['POI'][1] * .4)) * .4)) * .4;	
					
					$punts_local = (((($probability['local']['LV']['PUNTS'][0] * .6) + ($probability['local']['LV']['PUNTS'][1] * .4)) * .6) +
					((($probability['local']['BOTH']['PUNTS'][0] * .6) + ($probability['local']['BOTH']['PUNTS'][1] * .4)) * .4)) * .6;
					
					$poisson_visitor = (((($probability['visitor']['LV']['POI'][0] * .6) + ($probability['visitor']['LV']['POI'][1] * .4)) * .6) +
					((($probability['visitor']['BOTH']['POI'][0] * .6) + ($probability['visitor']['BOTH']['POI'][1] * .4)) * .4)) * .4;	
					
					$punts_visitor = (((($probability['visitor']['LV']['PUNTS'][0] * .6) + ($probability['visitor']['LV']['PUNTS'][1] * .4)) * .6) +
					((($probability['visitor']['BOTH']['PUNTS'][0] * .6) + ($probability['visitor']['BOTH']['PUNTS'][1] * .4)) * .4)) * .6;
					
					
				}
				else
				{
					$poisson_local = (((($probability['local']['LV']['POI'][0])) * .6) + ((($probability['local']['BOTH']['POI'][0])) * .4)) * .4;	
					
					$punts_local = (((($probability['local']['LV']['PUNTS'][0])) * .6) + ((($probability['local']['BOTH']['PUNTS'][0])) * .4)) * .6;
					
					$poisson_visitor = (((($probability['visitor']['LV']['POI'][0])) * .6) + ((($probability['visitor']['BOTH']['POI'][0])) * .4)) * .4;	
					
					$punts_visitor = (((($probability['visitor']['LV']['PUNTS'][0])) * .6) + ((($probability['visitor']['BOTH']['PUNTS'][0])) * .4)) * .6;
					
					
				}		
				
				$local = node_load($match->field_local_team[LANGUAGE_NONE][0]['target_id']);
				$visitor = node_load($match->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
				$tiptype = taxonomy_term_load(785);
				if(($poisson_local + $punts_local) > ($poisson_visitor + $punts_visitor + .1))
				{
					$title = 'Win or Draw House : Local - ' . $local->title . ' : ' . intval(($poisson_local + $punts_local)*100) . '% ';
					$params = array(($poisson_local + $punts_local), '', '', $title,785, $local->nid);
					tips_house_strategies_check_odds($match, 1.29, $tiptype, $local, $params);
				}
				
				if(($poisson_local + $punts_local + .1) < ($poisson_visitor + $punts_visitor))
				{
					$title = 'Win or Draw House : Visitor - ' . $visitor->title . ' : ' . intval(($poisson_visitor + $punts_visitor)*100) . '% ';
					$params = array(($poisson_visitor + $punts_visitor), '', '', $title,785, $visitor->nid);
					tips_house_strategies_check_odds($match, 1.29, $tiptype, $visitor, $params);
				}
				
				//$title = 'Over 0.5 by match after 0-0 In Play : '  . intval($params[0]*100) . '% ';	
				//dpm($probability);
				
				
			}
			
			/*
			$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
			$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
			
			$local_win_probability =
				 tips_poisson_winner_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], '1');
			
			$visitor_win_probability =
				 tips_poisson_winner_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], '2');
			
			$draw_probability = 
				 tips_poisson_winner_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], 'X');
			*/
							
			/*
			if(is_array($posson_stats))
			{
				
				
				if( ($local_win_probability) >= WIN_DRAW_POISSON_PROBABILITY)
				{
					$team = node_load($lid);
					$title = 
						'Win or Draw by poisson : Local - ' . $team->title . ' : ' . intval(($local_win_probability+$draw_probability)*100) . '% ';
					tips_create_tip($match, json_encode($posson_stats),$title, $lid, ($local_win_probability+$draw_probability), 674);	
				}
				if( ($visitor_win_probability) >= WIN_DRAW_POISSON_PROBABILITY)
				{
					$team = node_load($vid);
					$title = 'Win or Draw by poisson : Visitor - ' . $team->title . ' : '  . intval(($visitor_win_probability+$draw_probability)*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, $vid, ($visitor_win_probability+$draw_probability), 674);	
				}
				
				
				if($under_1_5_by_match >= UNDER_1_5_BY_MATCH_PROBABILITY)
				{
					$title = 'Under 1.5 by match : '  . intval($under_1_5_by_match*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, 0, $under_1_5_by_match, 578);	
				}
				
				if($under_2_5_by_match >= UNDER_2_5_BY_MATCH_PROBABILITY)
				{
					$title = 'Under 2.5 by match : '  . intval($under_2_5_by_match*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, 0, $under_2_5_by_match, 576);	
				}
				
				if($over_2_5_by_match >= OVER_2_5_BY_MATCH_PROBABILITY)
				{
					$title = 'Over 2.5 by match : '  . intval($over_2_5_by_match*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, 0, $over_2_5_by_match, 577);	
				}
				
				
			}
			*/
		}
	}
	
	return $return_array;
}


function tips_close_tips_house($tip)
{
	$match = node_load($tip->field_match[LANGUAGE_NONE][0]['target_id']);
	switch($tip->field_tip_type[LANGUAGE_NONE][0]['target_id'])
	{
		case 571:
			if($tip->field_team[LANGUAGE_NONE][0]['target_id'] == $match->field_local_team[LANGUAGE_NONE][0]['target_id'])
				$score = $match->field_local_team_score[LANGUAGE_NONE][0]['value'];
			else
				$score = $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'];
			
			if($score < 1.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			
			node_save($tip);
		break;
		case 572:
			if($tip->field_team[LANGUAGE_NONE][0]['target_id'] == $match->field_local_team[LANGUAGE_NONE][0]['target_id'])
				$score = $match->field_local_team_score[LANGUAGE_NONE][0]['value'];
			else
				$score = $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'];
			
			if($score > 1.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			
			node_save($tip);
		break;
		case 573:
			if($match->field_local_team_score[LANGUAGE_NONE][0]['value'] > 0 && $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'] > 0) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		
		case 578:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] + $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']) < 1.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		case 772:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] + $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']) > 0.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		case 773:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] + $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']) > 1.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		case 753:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] + $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']) > 1.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		case 576:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] + $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']) < 2.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		case 577:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] + $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']) > 2.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		case 754:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] + $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']) < 3.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		case 755:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] + $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']) > 3.5) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;
		case 575:
			if($tip->field_team[LANGUAGE_NONE][0]['target_id'] == $match->field_local_team[LANGUAGE_NONE][0]['target_id'])
				$score = $match->field_local_team_score[LANGUAGE_NONE][0]['value'];
			else
				$score = $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'];
			
			if($score == 0) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			
			node_save($tip);
		break;
		case 593:
			if($tip->field_team[LANGUAGE_NONE][0]['target_id'] == $match->field_local_team[LANGUAGE_NONE][0]['target_id'])
			{	
				$score = $match->field_local_team_score[LANGUAGE_NONE][0]['value'];
				$scorevs = $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'];
			}
			else
			{
				$score = $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'];
				$scorevs = $match->field_local_team_score[LANGUAGE_NONE][0]['value'];
			}
			
			if($score > $scorevs) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			
			node_save($tip);
		break;	
		case 674:
			if($tip->field_team[LANGUAGE_NONE][0]['target_id'] == $match->field_local_team[LANGUAGE_NONE][0]['target_id'])
			{	
				$score = $match->field_local_team_score[LANGUAGE_NONE][0]['value'];
				$scorevs = $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'];
			}
			else
			{
				$score = $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'];
				$scorevs = $match->field_local_team_score[LANGUAGE_NONE][0]['value'];
			}
			
			if($score > $scorevs || $score == $scorevs) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			
			node_save($tip);
		break;	
		
		
	}
				
		
}


function tips_house_strategies_check_odds($matchnode, $oddssuggested, $tiptype, $team, $params)
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
							if($type['@attributes']['name']=='Double Chance' && $tiptype->tid == 785)
								tips_house_strategies_check_to_create_simple_bet($matchnode, $type, 
									($team->nid==$matchnode->field_local_team['und'][0]['target_id']) ? '1X' : '2X',
										$oddssuggested, $odds['@attributes']['updated'], $params);
							
						}	
					}
					
				}
			}
		}	
			
	
}	

function tips_house_strategies_check_to_create_simple_bet($matchnode, $type, $namebet,$oddssuggested, $updated, $params)
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
		tips_house_strategies_create_bet($matchnode, $bookmakername, $betterodd, $allbookmakername . ' ' . $updated, $params);
}


function tips_house_strategies_create_bet($matchnode, $bookmaker, $odds, $otherbookmakers = '', $params)
{
	//$title = 'Over 0.5 by match after 0-0 In Play : '  . intval($params[0]*100) . '% ';	
	$title = $params[3];
	$params[1]['match_status'] = $params[2];
	$tip = tips_create_tip($matchnode, json_encode($params[1]), $title, $params[5], $params[0], $params[4]);	
	
	if(!$tip) return false;
	
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


