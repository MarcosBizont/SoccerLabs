<?php
	
	
function commentaries_inplaytips_getlastgamesgoals_team_both($teamid, $matchdate, $gamestocheck, $tostrong = 1)
{
	if ($variable = variable_get('getlastgamesgoals_team_both_' . $teamid . '_' . $matchdate . '_' . $gamestocheck . '_' . $tostrong)) {
	  	$arrtoreturn =  $variable;
	}
	else 
	{	
		$or = db_or();
		$or->condition('a.field_local_team_target_id', $teamid, '=');
		$or->condition('c.field_visitor_team_target_id', $teamid, '=');

		$query = db_select('node', 'n');
		$query->join('field_data_field_local_team', 'a', 'n.nid = a.entity_id'); 
		$query->join('field_data_field_visitor_team', 'c', 'n.nid = c.entity_id');
		$query->join('field_data_field_match_date', 'b', 'n.nid = b.entity_id');
		$query->fields('n',array('nid'))
			->condition('a.bundle', 'match')->condition('b.bundle', 'match')->condition('c.bundle', 'match')
			->condition($or)->condition('b.field_match_date_value', $matchdate, '<')
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
		$stats = array();
		foreach($match_items as $match)
		{
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
						
						'count' => count($match_items),
						
						'times_w_comments' => $times_w_comments,
						'times_w_stats' => $times_w_stats,
						
						);
			
		foreach($stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_w_stats;
				
		variable_set('getlastgamesgoals_team_both_' . $teamid . '_' . $matchdate . '_' . $gamestocheck . '_' . $tostrong, $arrtoreturn);	
	}
					
	return $arrtoreturn;
}

function commentaries_inplaytips_getlastgamesgoals_team($teamid, $matchdate, $gamestocheck = 5, $tostrong = 1, $competitionid = 'NO')
{
	if ($variable = variable_get('getlastgamesgoals_team_' . $teamid . '_' . $matchdate . '_' . $gamestocheck . '_' . $tostrong . '_' . $competitionid)) {
	  	$arrtoreturn =  $variable;
	}
	else 
	{	
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node')
			  ->entityCondition('bundle', 'match')
			  ->addMetaData('account', user_load(1))
			  ->fieldCondition('field_local_team', 'target_id', $teamid, '=')
			  ->fieldCondition('field_match_date', 'value', $matchdate, '<')
			  ->fieldOrderBy('field_match_date', 'value', 'DESC')
			  ->range(0, $gamestocheck);
		
		if($competitionid!=='NO') $query->fieldCondition('field_competition', 'target_id', $competitionid, '=');
		
		
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
		
		if($competitionid!=='NO') $query->fieldCondition('field_competition', 'target_id', $competitionid, '=');
		
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
		$local_stats = array();
		foreach($match_items_local as $matchlocal)
		{
			$gl = intval($matchlocal->field_local_team_score['und'][0]['value']);
			$gv = intval($matchlocal->field_visitor_team_score['und'][0]['value']);
			$gt = $gl + $gv;
			$goals_as_local += $gl;
			$goals_against_as_local += $gv;
			
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
		$visitor_stats = array();
		foreach($match_items_visitor as $matchvisitor)
		{
			$gl = intval($matchvisitor->field_local_team_score['und'][0]['value']);
			$gv = intval($matchvisitor->field_visitor_team_score['und'][0]['value']);
			$gt = $gl + $gv;
			$goals_as_visitor += $gv;
			$goals_against_as_visitor += $gl;
			
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
						
						'local_count' => count($match_items_local),
						'visitor_count' => count($match_items_visitor),
						
						'times_local_w_comments' => $times_local_w_comments,
						'times_visitor_w_comments' => $times_visitor_w_comments,
						'times_local_w_stats' => $times_local_w_stats,
						'times_visitor_w_stats' => $times_visitor_w_stats
						
						);
			
		foreach($local_stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_local_w_stats;
		foreach($visitor_stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_visitor_w_stats;	
				
		variable_set('getlastgamesgoals_team_' . $teamid . '_' . $matchdate . '_' . $gamestocheck . '_' . $tostrong . '_' . $competitionid, $arrtoreturn );	
	}
					
	return $arrtoreturn;
}

function commentaries_get_competition_stats($competitionid, $matchdate, $gamestocheck = 100)
{
	if ($variable = variable_get('commentaries_get_competition_stats' . $competitionid . '_' . $matchdate . '_' . $gamestocheck)) {
	  	$arrtoreturn =  $variable;
	}
	else 
	{	
		
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node')
			  ->entityCondition('bundle', 'match')
			  ->addMetaData('account', user_load(1))
			  ->fieldCondition('field_competition', 'target_id', $competitionid, '=')
			  ->fieldCondition('field_match_date', 'value', $matchdate, '<')
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
		
		foreach($match_items as $match)
		{
			$gl = intval($match->field_local_team_score['und'][0]['value']);
			$gv = intval($match->field_visitor_team_score['und'][0]['value']);
			$gt = $gl + $gv;
			$goals_as_local += $gl;
			$goals_as_visitor += $gv;
			
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
		$arrtoreturn =  array('goals_as_local' => $goals_as_local, 
						'goals_as_visitor' => $goals_as_visitor, 
						'goals_as_local_avg' => ($goals_as_local/$gamestocheck), 
						'goals_as_visitor_avg' => ($goals_as_visitor/$gamestocheck), 
						
						'times_over_0_5' => intval(($times_over_0_5/$gamestocheck)*100),
						'times_over_1_5' => intval(($times_over_1_5/$gamestocheck)*100),
						'times_over_2_5' => intval(($times_over_2_5/$gamestocheck)*100),
						'times_over_3_5' => intval(($times_over_3_5/$gamestocheck)*100),
						
						'count' => $gamestocheck,
						
						'times_w_comments' => $times_w_comments,
						'times_w_stats' => $times_w_stats,
						
						);
			
		foreach($local_stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_w_stats;
		foreach($visitor_stats as $statindex => $stat) $arrtoreturn[$statindex] = $stat/$times_w_stats;	
				
		variable_set('commentaries_get_competition_stats' . $competitionid . '_' . $matchdate . '_' . $gamestocheck, $arrtoreturn );	
	}
					
	return $arrtoreturn;
}	

function commentaries_inplaytips_getStats($node)
{
	dpm($node);
	return array();
	
	$statstoreturn = false;
	if(isset($node->field_football_api_['und'][0]['value']))
  	{
	  	$commentaries = json_decode($node->field_football_api_['und'][0]['value']);
	  	$local = node_load($node->field_local_team[LANGUAGE_NONE][0]['target_id']);
	  	$visitor = node_load($node->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	  	
	  	
	  	if(isset($commentaries[0]->comm_match_stats->localteam) && isset($commentaries[0]->comm_match_stats->visitorteam))
	  	{
			$commentaries[0]->comm_match_stats->localteam->myname = $local->title;
		  	$commentaries[0]->comm_match_stats->visitorteam->myname = $visitor->title;
		  	$arrstats = array($commentaries[0]->comm_match_stats->localteam, $commentaries[0]->comm_match_stats->visitorteam);
		  	$array_prefix = array('local_','visitor_');
		  	
		  	$statstoreturn = array();
		  	foreach($arrstats as $idnum => $statobj)
		  	{
		  	  	foreach($statobj as $key => $stat)
			  	{
				  	switch($key)
				  	{
					  	case 'shots';
					  		$statstoreturn[$array_prefix[$idnum] . 'shots'] = $stat->total;
					  		$statstoreturn[$array_prefix[$idnum] . 'shots_ongoal'] = $stat->ongoal;	
					  	break;
					  	case 'fouls';
					  	case 'corners';
					  	case 'offsides';
					  	case 'possestiontime';
					  	case 'yellowcards';
					  	case 'redcards';
					  	case 'saves';
					  		$statstoreturn[$array_prefix[$idnum] . $key] = $stat->total;
					  	break;
				  	}
			  	}
			}
		}
	}
	return $statstoreturn;	
}


function commentaries_inplaytips_have_commentaries_general($node)
{
	if(!isset($node->field_football_api_['und'][0]['value'])) return false;
	return ($node->field_football_api_['und'][0]['value'] != '');
	
}

function commentaries_inplaytips_have_commentaries($node)
{
	$commentaries = json_decode($node->field_football_api_['und'][0]['value']);
	return (isset($commentaries[0]->comm_commentaries->comment) 
	  		&& !is_string($commentaries[0]->comm_commentaries->comment)
	  		&& !isset($commentaries[0]->comm_commentaries->comment->comment));
	
}

function commentaries_inplaytips_have_stats($node)
{
	if(isset($node->field_football_api_['und'][0]['value']))
  	{
	  	$commentaries = json_decode($node->field_football_api_['und'][0]['value']);
	  	if(isset($commentaries[0]->comm_match_stats->localteam) && isset($commentaries[0]->comm_match_stats->visitorteam))
	  		return true;
	}
	return false;		
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
		$header = array(t('Stat'), $node->title);
		
		foreach($team_numbers as $key => &$value) $rows[] = array($key, commentaries_format_row($key,$value,'none'));
		
		$block .= theme('table', array('header' => $header, 'rows'=> $rows));
	}
	
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
	foreach($arr_days as $dayam)
	{
		$local_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($lid,$matchdate,$dayam,1);
		$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($vid,$matchdate,$dayam,2);
		
		$rows = array();
		$header = array(t('Stat'), $localteam->title, $visitorteam->title);
		
		foreach($local_numbers as $key => &$value) $rows[] = array($key, 
																	commentaries_format_row($key,$value,'none') , 
																	commentaries_format_row($key,$visitor_numbers[$key],'none'));
																	
		
		$block .= theme('table', array('header' => $header, 'rows'=> $rows));
	}
	
	return $block;	
}


function commentaries_format_row($key,$value,$str)
{
	if(is_double($value)) $value = number_format($value,2);
	return ( stripos($key, $str) === false ? $value : '<strong><font color="red">' . $value . '</font></strong>' );
}



