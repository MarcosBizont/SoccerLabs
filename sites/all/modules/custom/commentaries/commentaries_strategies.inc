<?php
	
	
function commentaries_inplaytips_getlastgamesgoals_team_both($teamid, $matchdate, $gamestocheck, $tostrong = 1, &$games = array())
{
	//if ($variable = variable_get('getlastgamesgoals_team_both_' . $teamid . '_' . $matchdate . '_' . $gamestocheck . '_' . $tostrong)) {
	//  	$arrtoreturn =  $variable;
	//}
	//else 
	//{	
		$or = db_or();
		$or->condition('a.field_local_team_target_id', $teamid, '=');
		$or->condition('c.field_visitor_team_target_id', $teamid, '=');
		
		$query = db_select('node', 'n');
		$query->join('field_data_field_local_team', 'a', 'n.nid = a.entity_id'); 
		$query->join('field_data_field_visitor_team', 'c', 'n.nid = c.entity_id');
		$query->join('field_data_field_match_date', 'b', 'n.nid = b.entity_id');
		$query->join('field_data_field_match_status', 'd', 'n.nid = d.entity_id');
		$query->fields('n',array('nid'))
			->condition('a.bundle', 'match')->condition('b.bundle', 'match')->condition('c.bundle', 'match')->condition('d.bundle', 'match')
			->condition($or)->condition('b.field_match_date_value', $matchdate, '<')
			->condition('d.field_match_status_value', 'FT', '=')
		    ->orderBy('b.field_match_date_value', 'DESC')
		    ->range(0,$gamestocheck);
		    
		
		$result = $query->execute();
		$match_items = array();
		while($record = $result->fetchAssoc())
	        $match_items[] = node_load($record['nid']);
	        
		$goals = 0;
		$goals_against = 0;
		$times_over_0_5 = 0;
		$times_over_1_5 = 0;
		$times_over_2_5 = 0;
		$times_over_3_5 = 0;
		$times_w_comments = 0;
		$times_w_stats = 0;
		$score = 0;
		$score_against = 0;
		$wins = 0;
		$draws = 0;
		$loss = 0;
		$stats = array();
		foreach($match_items as $match)
		{
			$games[] = $match->nid;
			$iamlocal = ($match->field_local_team['und'][0]['target_id'] == $teamid) ;
			$gme = 
				($iamlocal ? intval($match->field_local_team_score['und'][0]['value']) : 
					intval($match->field_visitor_team_score['und'][0]['value']) );
			$gthey = 
				($iamlocal ? intval($match->field_visitor_team_score['und'][0]['value']) : 
					intval($match->field_local_team_score['und'][0]['value']) );
			$gt = $gme + $gthey;
			$goals += $gme;
			$goals_against += $gthey;
			
			$wins += ($gme > $gthey) ? 1 : 0;
			$draws += ($gme == $gthey) ? 1 : 0;
			$loss += ($gme < $gthey) ? 1 : 0;
			
			$score += ($gme > 0.5 ? 1 : 0);
			$score_against += ($gthey > 0.5 ? 1 : 0);
		
			$times_over_0_5 += ($gt > 0.5 ? 1 : 0);
			$times_over_1_5 += ($gt > 1.5 ? 1 : 0);
			$times_over_2_5 += ($gt > 2.5 ? 1 : 0);
			$times_over_3_5 += ($gt > 3.5 ? 1 : 0);
			$times_w_comments += commentaries_inplaytips_have_commentaries_general($match);
			$times_w_stats += commentaries_inplaytips_have_stats($match);
			
			$strtocheck = ( $iamlocal ? 'local_' : 'visitor_' ); 
			$game_stats = commentaries_inplaytips_getStats($match);
			if(is_array($game_stats)) 
				foreach($game_stats as $keystate => $statvalue) 
					if(stripos($keystate, $strtocheck)!==false)
						if(isset($stats[str_ireplace($strtocheck, '', $keystate)]))
						{ $stats[str_ireplace($strtocheck, '', $keystate)] += $statvalue; } 
						else { $stats[str_ireplace($strtocheck, '', $keystate)] = $statvalue; }
		}
		
		$arrtoreturn =  array('goals' => $goals, 
						'goals_against' => $goals_against, 
						'goals_avg' => ($goals/$gamestocheck), 
						'goals_against_avg' => ($goals_against/$gamestocheck), 
						'times_over_0_5' => intval(($times_over_0_5/$gamestocheck)*100),
						'times_over_1_5' => intval(($times_over_1_5/$gamestocheck)*100),
						'times_over_2_5' => intval(($times_over_2_5/$gamestocheck)*100),
						'times_over_3_5' => intval(($times_over_3_5/$gamestocheck)*100),
						
						'score' => intval(($score/$gamestocheck)*100),
						'score_against' => intval(($score_against/$gamestocheck)*100),
						
						'wins' => $wins,
						'draws' => $draws,
						'loss' => $loss,
						
						'count' => count($match_items),
						
						'times_w_comments' => $times_w_comments,
						'times_w_stats' => $times_w_stats,
						
						);
			
		//foreach($stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_w_stats;
				
		//variable_set('getlastgamesgoals_team_both_' . $teamid . '_' . $matchdate . '_' . $gamestocheck . '_' . $tostrong, $arrtoreturn);	
	//}
					
	return $arrtoreturn;
}

function commentaries_inplaytips_getlastgamesgoals_team($teamid, $matchdate, $gamestocheck = 5, $tostrong = 1, $competitionid = 'NO', &$games = array())
{
	//if ($variable = variable_get('getlastgamesgoals_team_' . $teamid . '_' . $matchdate . '_' . $gamestocheck . '_' . $tostrong . '_' . $competitionid)) {
	//  	$arrtoreturn =  $variable;
	//}
	//else 
	//{	
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node')
			  ->entityCondition('bundle', 'match')
			  ->addMetaData('account', user_load(1))
			  ->fieldCondition('field_local_team', 'target_id', $teamid, '=')
			  ->fieldCondition('field_match_date', 'value', $matchdate, '<')
			  ->fieldCondition('field_match_status', 'value', 'FT', '=')
			  ->fieldOrderBy('field_match_date', 'value', 'DESC')
			  ->range(0, $gamestocheck);
		
		//if($competitionid!=='NO') $query->fieldCondition('field_competition', 'target_id', $competitionid, '=');
		
		
		$result = $query->execute();
		$match_items_local = array();
		if (isset($result['node'])) 
		{
			$match = array_keys($result['node']);
			$match_items_local = entity_load('node', $match);
		}
		
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node')
			  ->entityCondition('bundle', 'match')
			  ->addMetaData('account', user_load(1))
			  ->fieldCondition('field_visitor_team', 'target_id', $teamid, '=')
			  ->fieldCondition('field_match_date', 'value', $matchdate, '<')
			  ->fieldOrderBy('field_match_date', 'value', 'DESC')
			  ->range(0, $gamestocheck);
		
		//if($competitionid!=='NO') $query->fieldCondition('field_competition', 'target_id', $competitionid, '=');
		
		$result = $query->execute();
		$match_items_visitor = array();
		if (isset($result['node'])) 
		{
			$match = array_keys($result['node']);
			$match_items_visitor = entity_load('node', $match);
		}
		
		$goals_as_local = 0;
		$goals_against_as_local = 0;
		$times_as_local_over_0_5 = 0;
		$times_as_local_over_1_5 = 0;
		$times_as_local_over_2_5 = 0;
		$times_as_local_over_3_5 = 0;
		$times_local_w_comments = 0;
		$times_local_w_stats = 0;
		$score_as_local = 0;
		$score_against_as_local = 0;
		$wins_as_local = 0;
		$draws_as_local = 0;
		$loss_as_local = 0;
		$local_stats = array();
		foreach($match_items_local as $matchlocal)
		{
			$games['local'][] = $matchlocal->nid;
			$gl = intval($matchlocal->field_local_team_score['und'][0]['value']);
			$gv = intval($matchlocal->field_visitor_team_score['und'][0]['value']);
			$gt = $gl + $gv;
			$goals_as_local += $gl;
			$goals_against_as_local += $gv;
			
			$wins_as_local += ($gl > $gv) ? 1 : 0;
			$draws_as_local += ($gl == $gv) ? 1 : 0;
			$loss_as_local += ($gl < $gv) ? 1 : 0;
			
			$score_as_local += $gl > 0.5 ? 1 : 0;
			$score_against_as_local += $gv > 0.5 ? 1 : 0;
		
			$times_as_local_over_0_5 += $gt > 0.5 ? 1 : 0;
			$times_as_local_over_1_5 += $gt > 1.5 ? 1 : 0;
			$times_as_local_over_2_5 += $gt > 2.5 ? 1 : 0;
			$times_as_local_over_3_5 += $gt > 3.5 ? 1 : 0;
			$times_local_w_comments += commentaries_inplaytips_have_commentaries_general($matchlocal);
			$times_local_w_stats += commentaries_inplaytips_have_stats($matchlocal);
			
			$game_stats = commentaries_inplaytips_getStats($matchlocal);
			if(is_array($game_stats)) 
				foreach($game_stats as $keystate => $statvalue) 
					if(stripos($keystate, 'local_')!==false)
						if(isset($local_stats[$keystate]))
						{ $local_stats[$keystate] += $statvalue; } else { $local_stats[$keystate] = $statvalue; }
		}
		
		$goals_as_visitor = 0;
		$goals_against_as_visitor = 0;
		$times_as_visitor_over_0_5 = 0;
		$times_as_visitor_over_1_5 = 0;
		$times_as_visitor_over_2_5 = 0;
		$times_as_visitor_over_3_5 = 0;
		$times_visitor_w_comments= 0;
		$times_visitor_w_stats = 0;
		$score_as_visitor = 0;
		$score_against_as_visitor = 0;
		$wins_as_visitor = 0;
		$draws_as_visitor = 0;
		$loss_as_visitor = 0;
		$visitor_stats = array();
		foreach($match_items_visitor as $matchvisitor)
		{
			$games['visitor'][] = $matchvisitor->nid;
			$gl = intval($matchvisitor->field_local_team_score['und'][0]['value']);
			$gv = intval($matchvisitor->field_visitor_team_score['und'][0]['value']);
			$gt = $gl + $gv;
			$goals_as_visitor += $gv;
			$goals_against_as_visitor += $gl;
			
			$wins_as_visitor += ($gl < $gv) ? 1 : 0;
			$draws_as_visitor += ($gl == $gv) ? 1 : 0;
			$loss_as_visitor += ($gl > $gv) ? 1 : 0;
			
			$score_as_visitor += $gv > 0.5 ? 1 : 0;
			$score_against_as_visitor += $gl > 0.5 ? 1 : 0;
			
			$times_as_visitor_over_0_5 += $gt > 0.5 ? 1 : 0;
			$times_as_visitor_over_1_5 += $gt > 1.5 ? 1 : 0;
			$times_as_visitor_over_2_5 += $gt > 2.5 ? 1 : 0;
			$times_as_visitor_over_3_5 += $gt > 3.5 ? 1 : 0;
			$times_visitor_w_comments += commentaries_inplaytips_have_commentaries_general($matchvisitor);
			$times_visitor_w_stats += commentaries_inplaytips_have_stats($matchvisitor);
			
			$game_stats = commentaries_inplaytips_getStats($matchvisitor);
			if(is_array($game_stats)) 
				foreach($game_stats as $keystate => $statvalue) 
					if(stripos($keystate, 'visitor_')!==false) 
						if(isset($visitor_stats[$keystate]))
						{ $visitor_stats[$keystate] += $statvalue; } else { $visitor_stats[$keystate] = $statvalue; }
							
		}
		
		
		$arrtoreturn =  array('goals_as_local' => $goals_as_local, 
						'goals_against_as_local' => $goals_against_as_local, 
						'goals_as_visitor' => $goals_as_visitor, 
						'goals_against_as_visitor' => $goals_against_as_visitor,
						'goals_as_local_avg' => ($goals_as_local/$gamestocheck), 
						'goals_against_as_local_avg' => ($goals_against_as_local/$gamestocheck), 
						'goals_as_visitor_avg' => ($goals_as_visitor/$gamestocheck), 
						'goals_against_as_visitor_avg' => ($goals_against_as_visitor/$gamestocheck),
						'times_as_local_over_0_5' => intval(($times_as_local_over_0_5/$gamestocheck)*100),
						'times_as_local_over_1_5' => intval(($times_as_local_over_1_5/$gamestocheck)*100),
						'times_as_local_over_2_5' => intval(($times_as_local_over_2_5/$gamestocheck)*100),
						'times_as_local_over_3_5' => intval(($times_as_local_over_3_5/$gamestocheck)*100),
						'times_as_visitor_over_0_5' => intval(($times_as_visitor_over_0_5/$gamestocheck)*100),
						'times_as_visitor_over_1_5' => intval(($times_as_visitor_over_1_5/$gamestocheck)*100),
						'times_as_visitor_over_2_5' => intval(($times_as_visitor_over_2_5/$gamestocheck)*100),
						'times_as_visitor_over_3_5' => intval(($times_as_visitor_over_3_5/$gamestocheck)*100),
						
						'score_as_local' => intval(($score_as_local/$gamestocheck)*100),
						'score_against_as_local' => intval(($score_against_as_local/$gamestocheck)*100),
						'score_as_visitor' => intval(($score_as_visitor/$gamestocheck)*100),
						'score_against_as_visitor' => intval(($score_against_as_visitor/$gamestocheck)*100),
						
						'wins_as_local' => $wins_as_local,
						'draws_as_local' => $draws_as_local,
						'loss_as_local' => $loss_as_local,
						
						'wins_as_visitor' => $wins_as_visitor,
						'draws_as_visitor' => $draws_as_visitor,
						'loss_as_visitor' => $loss_as_visitor,
						
						'local_count' => count($match_items_local),
						'visitor_count' => count($match_items_visitor),
						
						'times_local_w_comments' => $times_local_w_comments,
						'times_visitor_w_comments' => $times_visitor_w_comments,
						'times_local_w_stats' => $times_local_w_stats,
						'times_visitor_w_stats' => $times_visitor_w_stats
						
						);
			
		//foreach($local_stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_local_w_stats;
		//foreach($visitor_stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_visitor_w_stats;	
				
		//variable_set('getlastgamesgoals_team_' . $teamid . '_' . $matchdate . '_' . $gamestocheck . '_' . $tostrong . '_' . $competitionid, $arrtoreturn );	
	//}
					
	return $arrtoreturn;
}

function commentaries_get_competition_stats($competitionid, $matchdate, $gamestocheck = 100)
{
	if ($cache = cache_get('commentaries_get_competition_stats' . $competitionid . '_' . $matchdate . '_' . $gamestocheck, 'cache')) {
	  	$arrtoreturn = $cache->data;
	}
	else 
	{	
	
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node')
			  ->entityCondition('bundle', 'match')
			  ->addMetaData('account', user_load(1))
			  ->fieldCondition('field_competition', 'target_id', $competitionid, '=')
			  ->fieldCondition('field_match_date', 'value', $matchdate, '<')
			  ->fieldCondition('field_match_status', 'value', 'FT', '=')
			  ->fieldOrderBy('field_match_date', 'value', 'DESC')
			  ->range(0, $gamestocheck);
		
		$result = $query->execute();
		$match_items = array();
		if (isset($result['node'])) 
		{
			$match = array_keys($result['node']);
			$match_items = entity_load('node', $match);
		}
		
		$goals_as_local = 0;
		$goals_as_visitor = 0;
		$times_over_0_5 = 0;
		$times_over_1_5 = 0;
		$times_over_2_5 = 0;
		$times_over_3_5 = 0;
		$times_w_comments = 0;
		$times_w_stats = 0;
		$local_stats = array();
		$visitor_stats = array();
		$draws = 0;
		foreach($match_items as $match)
		{
			$gl = intval($match->field_local_team_score['und'][0]['value']);
			$gv = intval($match->field_visitor_team_score['und'][0]['value']);
			$gt = $gl + $gv;
			$goals_as_local += $gl;
			$goals_as_visitor += $gv;
			
			
			$draws += ($gl == $gv) ? 1 : 0;  
			$times_over_0_5 += $gt > 0.5 ? 1 : 0;
			$times_over_1_5 += $gt > 1.5 ? 1 : 0;
			$times_over_2_5 += $gt > 2.5 ? 1 : 0;
			$times_over_3_5 += $gt > 3.5 ? 1 : 0;
			$times_w_comments += commentaries_inplaytips_have_commentaries_general($match);
			$times_w_stats += commentaries_inplaytips_have_stats($match);
			
			$game_stats = commentaries_inplaytips_getStats($match);
			if(is_array($game_stats)) 
				foreach($game_stats as $keystate => $statvalue) 
					if(stripos($keystate, 'local_')!==false)
						if(isset($local_stats[$keystate]))
						{ $local_stats[$keystate] += $statvalue; } else { $local_stats[$keystate] = $statvalue; }
			
			if(is_array($game_stats)) 
				foreach($game_stats as $keystate => $statvalue) 
					if(stripos($keystate, 'visitor_')!==false)
						if(isset($visitor_stats[$keystate]))
						{ $visitor_stats[$keystate] += $statvalue; } else { $visitor_stats[$keystate] = $statvalue; }
		}
		
		$gamestocheck = count($match_items);
		if($gamestocheck==0) $gamestocheck = -1;
		$arrtoreturn =  array('goals_as_local' => $goals_as_local, 
						'goals_as_visitor' => $goals_as_visitor,
						'goals' => $goals_as_local + $goals_as_visitor, 
						'goals_as_local_avg' => ($goals_as_local/$gamestocheck), 
						'goals_as_visitor_avg' => ($goals_as_visitor/$gamestocheck), 
						'goals_avg' => (($goals_as_local + $goals_as_visitor)/($gamestocheck * 2)), 
						
						'times_over_0_5' => intval(($times_over_0_5/$gamestocheck)*100),
						'times_over_1_5' => intval(($times_over_1_5/$gamestocheck)*100),
						'times_over_2_5' => intval(($times_over_2_5/$gamestocheck)*100),
						'times_over_3_5' => intval(($times_over_3_5/$gamestocheck)*100),
						
						'draws' => intval(($draws/$gamestocheck)*100),
						
						'count' => $gamestocheck,
						
						'times_w_comments' => $times_w_comments,
						'times_w_stats' => $times_w_stats,
						
						);
			
		foreach($local_stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_w_stats;
		foreach($visitor_stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_w_stats;	
				
		cache_set('commentaries_get_competition_stats' . $competitionid . '_' . $matchdate . '_' . $gamestocheck, $arrtoreturn, 'cache', time() + 60*60*168);
	}
					
	return $arrtoreturn;
}	

function commentaries_inplaytips_getStats($node)
{
	$statstoreturn = false;
	if(isset($node->field_stats['und']))
	if(is_array($node->field_stats['und']))
  	{
	  	//$commentaries = json_decode($node->field_football_api_['und'][0]['value']);
	  	$local = node_load($node->field_local_team[LANGUAGE_NONE][0]['target_id']);
	  	$visitor = node_load($node->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	  	
	  	foreach($node->field_stats['und'] as $stat)
	  	{
		 	$statobj = entity_load('field_collection_item', array($stat['value']));
		 	if(isset($statobj[$stat['value']]))
		 	{
			 	$statobj = $statobj[$stat['value']];
			 	$curr_array_prefix = ( $node->field_local_team[LANGUAGE_NONE][0]['target_id'] == $statobj->field_team[LANGUAGE_NONE][0]['target_id']) ? 'local_' : 'visitor_' ;
			 	$statstoreturn[$curr_array_prefix . $statobj->field_stat[LANGUAGE_NONE][0]['value']] = $statobj->field_value[LANGUAGE_NONE][0]['value'];
			}
	  	}
		
	}
	
	return $statstoreturn;	
}

function commentaries_inplaytips_getCards($node)
{
	$statstoreturn = false;
	if(isset($node->field_cards['und']))
	if(is_array($node->field_cards['und']))
  	{
	  	$local = node_load($node->field_local_team[LANGUAGE_NONE][0]['target_id']);
	  	$visitor = node_load($node->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	  	
	  	foreach($node->field_cards['und'] as $stat)
	  	{
		 	$statobj = entity_load('field_collection_item', array($stat['value']));
		 	if(isset($statobj[$stat['value']]))
		 	{
			 	$statobj = $statobj[$stat['value']];
			 	$curr_array_prefix = ( $node->field_local_team[LANGUAGE_NONE][0]['target_id'] == $statobj->field_team[LANGUAGE_NONE][0]['target_id']) ? 'local' : 'visitor' ;
			 	if(isset($statobj->field_player_name[LANGUAGE_NONE][0]['value']))
			 	$statstoreturn[$curr_array_prefix][] = array('color' => $statobj->field_color[LANGUAGE_NONE][0]['value'],
			 						'minute' => $statobj->field_minute_str[LANGUAGE_NONE][0]['value'],
			 						'player_name' => $statobj->field_player_name[LANGUAGE_NONE][0]['value'] );
			}
	  	}
		
	}
	
	return $statstoreturn;	
}

function commentaries_inplaytips_getGoals($node)
{
	$statstoreturn = false;
	if(isset($node->field_goals['und']))
	if(is_array($node->field_goals['und']))
  	{
	  	$local = node_load($node->field_local_team[LANGUAGE_NONE][0]['target_id']);
	  	$visitor = node_load($node->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	  	
	  	foreach($node->field_goals['und'] as $stat)
	  	{
		 	$statobj = entity_load('field_collection_item', array($stat['value']));
		 	if(isset($statobj[$stat['value']]))
		 	{
			 	$statobj = $statobj[$stat['value']];
			 	$curr_array_prefix = ( $node->field_local_team[LANGUAGE_NONE][0]['target_id'] == $statobj->field_team[LANGUAGE_NONE][0]['target_id']) ? 'local' : 'visitor' ;
			 	if(isset($statobj->field_player_name[LANGUAGE_NONE][0]['value'])) 
			 	$statstoreturn[$curr_array_prefix][] = array(
			 						'minute' => $statobj->field_minute_str[LANGUAGE_NONE][0]['value'],
			 						'player_name' => $statobj->field_player_name[LANGUAGE_NONE][0]['value'] );
			}
	  	}
		
	}
	
	return $statstoreturn;	
}



function commentaries_inplaytips_have_commentaries_general($node)
{
	return ((isset($node->field_stats['und'])) && (is_array($node->field_stats['und'])));
	
}

function commentaries_inplaytips_have_commentaries($node)
{
	return ((isset($node->field_stats['und'])) && (is_array($node->field_stats['und'])));
	
}

function commentaries_inplaytips_have_stats($node)
{
	return ((isset($node->field_stats['und'])) && (is_array($node->field_stats['und'])));		
}


function commentaries_teamstats_overview($nid, $arr_days = array(5) )
{
	$node = node_load($nid);
	
	$competitionid = $node->field_competition[LANGUAGE_NONE][0]['target_id']; // ???
	//$matchdate = $node->field_match_date[LANGUAGE_NONE][0]['value'];
	$matchdate = date('Y-m-d H:i:s'); 
	
	$block = '';
	foreach($arr_days as $dayam)
	{
		$team_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($nid,$matchdate,$dayam,1);
		
		$rows = array();
		$header = array(t('Stat'), '');
		
		foreach($team_numbers as $key => &$value) $rows[] = array($key, commentaries_format_row($key,$value,'none'));
		
		$block .= theme('table', array('header' => $header, 'rows'=> $rows));
	}
	
	return $block;	
}

function commentaries_matchstats_inplay_overview($nid)
{
	$node = node_load($nid);
	
	$localteam = node_load($node->field_local_team[LANGUAGE_NONE][0]['target_id']);
	$visitorteam = node_load($node->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	
	$block = '';
	
		
	$rows = array();
	$header = array($localteam->title, '', $visitorteam->title);
	
	
	$goals = commentaries_inplaytips_getGoals($node);
	if(is_array($goals))
	{
		if(count($goals)>0) $rows[] = array('',t('Goals'),'');
		foreach($goals as $key => &$value) 
			foreach($value as $goal)
				if($key=='local')
					$rows[] = array($goal['player_name'],$goal['minute'],'');
				else
					$rows[] = array('',$goal['minute'],$goal['player_name']); 
		if(count($goals)>0) $rows[] = array('','','');
	}
	
		
	
	$cards = commentaries_inplaytips_getCards($node);
	if(is_array($cards))
	{
		if(count($cards)>0) $rows[] = array('',t('Cards'),'');
		foreach($cards as $key => &$value) 
			foreach($value as $card)
				if($key=='local')
					$rows[] = array($card['player_name'] . ' (' . $card['color'] . ')' ,$card['minute'],'');
				else
					$rows[] = array('',$card['minute'],$card['player_name'] . ' (' . $card['color'] . ')' ); 
		if(count($cards)>0) $rows[] = array('','','');
	}
	
	$stats = commentaries_inplaytips_getStats($node);
	$statsshow = 0;	
	if(is_array($stats))
	foreach($stats as $key => &$value) 
	{
		if(stripos($key, 'local')!==false)
		{
			$term = taxonomy_get_term_by_name($key,'display_values');
			$term = reset($term);
			if(isset($term->name))
			{
				if($term->field_show_to_everybody[LANGUAGE_NONE][0]['value']==1)
				{
					$statsshow++;
					$visitorkey = str_ireplace('local', 'visitor', $key);
					$label = t($term->field_label[LANGUAGE_NONE][0]['value']);
					$rows[] = array(commentaries_format_row($key,$value,'none') , $label ,
						commentaries_format_row($key, $stats[$visitorkey]   ,'none'));
				}
			}	
		}
	}															
	
	if(count($rows)==0) return '';	
	$block .= theme('table', array('header' => $header, 'rows'=> $rows));
	
	
	return $block;	
}

function commentaries_matchstats_overview($nid, $arr_days = array(5) )
{
	$node = node_load($nid);
	
	$lid = $node->field_local_team[LANGUAGE_NONE][0]['target_id'];
	$localteam = node_load($lid);
	$vid = $node->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
	$visitorteam = node_load($vid);
	$competitionid = $node->field_competition[LANGUAGE_NONE][0]['target_id'];
	$matchdate = $node->field_match_date[LANGUAGE_NONE][0]['value'];
	
	
	$block = '';
	$blockgames = '';
	foreach($arr_days as $dayam)
	{
		$local_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($lid,$matchdate,$dayam,1,$local_games);
		$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($vid,$matchdate,$dayam,2,$visitor_games);
		
		
		$rows = array();
		$header = array($localteam->title, '', $visitorteam->title);
		
		foreach($local_numbers as $key => &$value) 
		{
			$term = taxonomy_get_term_by_name($key,'display_values');
			$term = reset($term);
			if(isset($term->name))
			{
				
				if($term->field_show_to_everybody[LANGUAGE_NONE][0]['value']==1)
				{
					$label = t($term->field_label[LANGUAGE_NONE][0]['value']);
					$rows[] = array(commentaries_format_row($key,$value,'none') , $label ,
								commentaries_format_row($key,$visitor_numbers[$key],'none'));
				}
			}
		}															
		// $block .= '<div class="statstable">' . theme('table', array('header' => $header, 'rows'=> $rows, )) . '</div>';
		$block .= '' .theme('table', array('header' => $header, 'rows'=> $rows, 'class' => array('statstable')  ));
		
		// 2 blocks for both teams last games
		if(count($local_games)>0)
		{
			$view = views_get_view('matches_by_team_in_play');
			$view->get_total_rows = TRUE;
			$view->set_display('block_3');
			$view->set_arguments(array(implode(',', $local_games),$lid));
			$view->pre_execute();
			$view->execute();
			$tipiblock = $view->render();
			if($view->total_rows > 0) $blockgames .= $tipiblock;
		}
		
		if(count($visitor_games)>0)
		{
			$view = views_get_view('matches_by_team_in_play');
			$view->get_total_rows = TRUE;
			$view->set_display('block_3');
			$view->set_arguments(array(implode(',', $visitor_games),$vid));
			$view->pre_execute();
			$view->execute();
			$tipiblock = $view->render();
			if($view->total_rows > 0) $blockgames .= $tipiblock;
		}
		
		if($blockgames!='')
		{
			drupal_add_library('system', 'drupal.collapse');
			$blockgames_element = array( 
				'#title'          => t('> View games'), 
				'#children'    => $blockgames, 
				'#collapsible' => true,   
				'#collapsed'  => true, 
				'#attributes'  => array(  
			                      'class' => array('collapsible', 'collapsed')
			                      ), 
					); 
			$blockgames = theme('fieldset', array('element' => $blockgames_element)); 
		}
	}
	
	return $block.$blockgames;	
}

function commentaries_matchstats_overview_lv($nid, $arr_days = array(5) )
{
	$node = node_load($nid);
	
	$lid = $node->field_local_team[LANGUAGE_NONE][0]['target_id'];
	$localteam = node_load($lid);
	$vid = $node->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
	$visitorteam = node_load($vid);
	$competitionid = $node->field_competition[LANGUAGE_NONE][0]['target_id'];
	$matchdate = $node->field_match_date[LANGUAGE_NONE][0]['value'];
	
	
	$block = '';
	$blockgames = '';
	foreach($arr_days as $dayam)
	{
		$local_numbers = commentaries_inplaytips_getlastgamesgoals_team($lid,$matchdate,$dayam,1,'NO', $local_games);
		$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team($vid,$matchdate,$dayam,2,'NO', $visitor_games);
		
		$rows = array();
		$header = array($localteam->title, '', $visitorteam->title);
		
		foreach($local_numbers as $key => &$value) 
		{
			if(stripos($key,'local') !== false)
			{
				$visitorkey = str_ireplace('local', 'visitor', $key);
				$term = taxonomy_get_term_by_name($key,'display_values');
				$term = reset($term);
				if(isset($term->name))
				{
					if($term->field_show_to_everybody[LANGUAGE_NONE][0]['value']==1)
					{
						$label = t($term->field_label[LANGUAGE_NONE][0]['value']);
						$rows[] = array(commentaries_format_row($key,$value,'none') , $label ,
									commentaries_format_row($visitorkey,$visitor_numbers[$visitorkey],'none'));
					}
				}
			}
		}
		$block .= theme('table', array('header' => $header, 'rows'=> $rows));
		
		// 2 blocks for both teams last games
		if(count($local_games['local'])>0)
		{
			$view = views_get_view('matches_by_team_in_play');
			$view->get_total_rows = TRUE;
			$view->set_display('block_3');
			$view->set_arguments(array(implode(',', $local_games['local']),$lid));
			$view->pre_execute();
			$view->execute();
			$tipiblock = $view->render();
			if($view->total_rows > 0) $blockgames .= $tipiblock;
		}
		
		if(count($visitor_games['visitor'])>0)
		{
			$view = views_get_view('matches_by_team_in_play');
			$view->get_total_rows = TRUE;
			$view->set_display('block_3');
			$view->set_arguments(array(implode(',', $visitor_games['visitor']),$vid));
			$view->pre_execute();
			$view->execute();
			$tipiblock = $view->render();
			if($view->total_rows > 0) $blockgames .= $tipiblock;
		}
		
		if($blockgames!='')
		{
			drupal_add_library('system', 'drupal.collapse');
			$blockgames_element = array( 
				'#title'          => t('> View games'), 
				'#children'    => $blockgames, 
				'#collapsible' => true,   
				'#collapsed'  => true, 
				'#attributes'  => array(  
			                      'class' => array('collapsible', 'collapsed')
			                      ), 
					); 
			$blockgames = theme('fieldset', array('element' => $blockgames_element)); 
		}
	}
	
	return $block.$blockgames;	
}


function commentaries_format_row($key,$value,$str)
{
	if(is_double($value)) $value = number_format($value,2);
	return ( stripos($key, $str) === false ? $value : '<strong><font color="red">' . $value . '</font></strong>' );
}



