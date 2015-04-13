<?php

function tips_check_poisson_tips_ht_creation()
{
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
		
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_match_status', 'value', 'HT', '=');
	
	$result = $query->execute();
	
	$return_array = array();
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$match_items = entity_load('node', $matchids);
		foreach($match_items as $match)
		{
			watchdog('trying a HT game', $match->nid);
			$posson_stats = tips_poisson_distribution($match);
			
			$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
			$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
	
			// Over under by match
			$over_0_5_by_match 
				= tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'over', 0.5);
					
			$over_1_5_by_match 
				= tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'over', 1.5);
			
			if(is_array($posson_stats))
			{	
				if($match->field_local_team_score[LANGUAGE_NONE][0]['value']==0 && $match->field_visitor_team_score[LANGUAGE_NONE][0]['value']==0)
				{
					if($over_0_5_by_match >= OVER_0_5_BY_MATCH_HT_PROBABILITY)
					{
						$title = 'Over 0.5 by match after 0-0 HT : '  . intval($over_0_5_by_match*100) . '% ';	
						tips_create_tip($match, json_encode($posson_stats), $title, 0, $over_0_5_by_match, 772);	
					}
					
					if($over_1_5_by_match >= OVER_1_5_BY_MATCH_HT_PROBABILITY)
					{
						$title = 'Over 1.5 by match after 0-0 HT : '  . intval($over_1_5_by_match*100) . '% ';	
						tips_create_tip($match, json_encode($posson_stats), $title, 0, $over_1_5_by_match, 773);	
					}
				}
				
			}
		}
	}
	
	return $return_array;
}



function tips_check_poisson_tips_creation($daystocheck)
{
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
		
	$from = new DateTime();
	//$from = $from->modify('-5 days');
	
	$to = new DateTime();
	$to = $to->modify('+' . $daystocheck . ' days');
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_match_date', 'value', $to->format('Y-m-d H:i:s'), '<')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')		
		  ->fieldOrderBy('field_match_date', 'value', 'ASC');
	
	$result = $query->execute();
	
	$return_array = array();
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$match_items = entity_load('node', $matchids);
		foreach($match_items as $match)
		{
			$posson_stats = tips_poisson_distribution($match);
			
			$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
			$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
	
			$over_1_5_local_probability = 
				$posson_stats['proximity_local'][2] + $posson_stats['proximity_local'][3] + 
				$posson_stats['proximity_local'][4] + $posson_stats['proximity_local'][5] + $posson_stats['proximity_local'][6];
			$over_1_5_visitor_probability = 
				$posson_stats['proximity_visitor'][2] + $posson_stats['proximity_visitor'][3] + 
				$posson_stats['proximity_visitor'][4] + $posson_stats['proximity_visitor'][5] + $posson_stats['proximity_visitor'][6];
			
			$over_0_5_local_probability = 
				$posson_stats['proximity_local'][1] + $over_1_5_local_probability;
			$over_0_5_visitor_probability = 
				$posson_stats['proximity_visitor'][1] + $over_1_5_visitor_probability;
			$both_to_score = $over_0_5_local_probability * $over_0_5_visitor_probability;
			
			
			// Over under by team
			$under_1_5_local_probability = $posson_stats['proximity_local'][0] + $posson_stats['proximity_local'][1];
			$under_1_5_visitor_probability = $posson_stats['proximity_visitor'][0] + $posson_stats['proximity_visitor'][1];
			
			
			// Over under by match
			
			$under_1_5_by_match 
				= tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'under', 1.5);
			$over_1_5_by_match 
				= tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'over', 1.5);
									
			$under_2_5_by_match = tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'under', 2.5);
			
			$over_2_5_by_match = tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'over', 2.5);
			
			$under_3_5_by_match = tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'under', 3.5);
			
			$over_3_5_by_match = tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'over', 3.5);
			
			$under_4_5_by_match = tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'under', 4.5);
			
			$over_4_5_by_match = tips_poisson_over_under_match($posson_stats['proximity_local'], 
					$posson_stats['proximity_visitor'], 'over', 4.5);
			
			// Win, loss, draw
			
			$local_win_probability =
				 tips_poisson_winner_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], '1');
			
			$visitor_win_probability =
				 tips_poisson_winner_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], '2');
			
			$draw_probability = 
				 tips_poisson_winner_match($posson_stats['proximity_local'], $posson_stats['proximity_visitor'], 'X');
								
			$no_goal_by_local_probability = $posson_stats['proximity_local'][0];
			$no_goal_by_visitor_probability = $posson_stats['proximity_visitor'][0];
			
			if(is_array($posson_stats))
			{
				
				
				if($over_1_5_local_probability >= OVER_1_5_BY_TEAM_PROBABILITY)
				{
					$team = node_load($lid);
					$title = 'Over 1.5 by team : Local - ' . $team->title . ' : ' . intval($over_1_5_local_probability*100) . '% ';
					tips_create_tip($match, json_encode($posson_stats),$title, $lid, $over_1_5_local_probability, 572);	
				}
				if($over_1_5_visitor_probability >= OVER_1_5_BY_TEAM_PROBABILITY)
				{
					$team = node_load($vid);
					$title = 'Over 1.5 by team : Visitor - ' . $team->title . ' : '  . intval($over_1_5_visitor_probability*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, $vid, $over_1_5_visitor_probability, 572);	
				}
				
				if($local_win_probability >= WIN_BY_POISSON_PROBABILITY)
				{
					$team = node_load($lid);
					$title = 'Winner by poisson : Local - ' . $team->title . ' : ' . intval($local_win_probability*100) . '% ';
					tips_create_tip($match, json_encode($posson_stats),$title, $lid, $local_win_probability, 593);	
				}
				if($visitor_win_probability >= WIN_BY_POISSON_PROBABILITY)
				{
					$team = node_load($vid);
					$title = 'Winner by poisson : Visitor - ' . $team->title . ' : '  . intval($visitor_win_probability*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, $vid, $visitor_win_probability, 593);	
				}
				
				
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
					$title = 
						'Win or Draw by poisson : Visitor - ' . $team->title . ' : '  . intval(($visitor_win_probability+$draw_probability)*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, $vid, ($visitor_win_probability+$draw_probability), 674);	
				}
				
				
				
				
				
				if($under_1_5_local_probability >= UNDER_1_5_BY_TEAM_PROBABILITY)
				{
					$team = node_load($lid);
					$title = 'Under 1.5 by team : Local - ' . $team->title . ' : ' . intval($under_1_5_local_probability*100) . '% ';
					tips_create_tip($match, json_encode($posson_stats),$title, $lid, $under_1_5_local_probability, 571);	
				}
				if($under_1_5_visitor_probability >= UNDER_1_5_BY_TEAM_PROBABILITY)
				{
					$team = node_load($vid);
					$title = 'Under 1.5 by team : Visitor - ' . $team->title . ' : '  . intval($under_1_5_visitor_probability*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, $vid, $under_1_5_visitor_probability, 571);	
				}
				
				if($both_to_score >= BOTH_TO_SCORE_PROBABILITY)
				{
					$title = 'Both to score : '  . intval($both_to_score*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, 0, $both_to_score, 573);	
				}
				
				
				if($under_1_5_by_match >= UNDER_1_5_BY_MATCH_PROBABILITY)
				{
					$title = 'Under 1.5 by match : '  . intval($under_1_5_by_match*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, 0, $under_1_5_by_match, 578);	
				}
				
				if($over_1_5_by_match >= OVER_1_5_BY_MATCH_PROBABILITY)
				{
					$title = 'Over 1.5 by match : '  . intval($over_1_5_by_match*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, 0, $over_1_5_by_match, 753);	
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
				
				if($under_3_5_by_match >= UNDER_3_5_BY_MATCH_PROBABILITY)
				{
					$title = 'Under 3.5 by match : '  . intval($under_3_5_by_match*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, 0, $under_3_5_by_match, 754);	
				}
				
				if($over_3_5_by_match >= OVER_3_5_BY_MATCH_PROBABILITY)
				{
					$title = 'Over 3.5 by match : '  . intval($over_3_5_by_match*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, 0, $over_3_5_by_match, 755);	
				}
				
				
				
				if($no_goal_by_local_probability >= NO_GOAL_BY_TEAM_PROBABILITY)
				{
					$team = node_load($lid);
					$title = 'No goal by team : Local - ' . $team->title . ' : ' . intval($no_goal_by_local_probability*100) . '% ';
					tips_create_tip($match, json_encode($posson_stats),$title, $lid, $no_goal_by_local_probability, 575);	
				}
				if($no_goal_by_visitor_probability >= NO_GOAL_BY_TEAM_PROBABILITY)
				{
					$team = node_load($vid);
					$title = 'No goal by team : Visitor - ' . $team->title . ' : '  . intval($no_goal_by_visitor_probability*100) . '% ';	
					tips_create_tip($match, json_encode($posson_stats), $title, $vid, $no_goal_by_visitor_probability, 575);	
				}
				
			}
		}
	}
	
	return $return_array;
}


function tips_close_tips_poisson($tip)
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

function tips_poisson_distribution($match, $remaining_time = false, $gamestocheck = false)
{
	$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
	$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
	$competitionid = $match->field_competition[LANGUAGE_NONE][0]['target_id'];
	$matchdate = $match->field_match_date[LANGUAGE_NONE][0]['value'];
	
	$competition_stats = commentaries_get_competition_stats($competitionid, $matchdate, MATCHES_TO_CHECK_IN_THE_PAST_COMPETITION);
	if(!$gamestocheck) $gamestocheck = MATCHES_TO_CHECK_IN_THE_PAST;
	$local_stats = commentaries_inplaytips_getlastgamesgoals_team($lid, $matchdate, $gamestocheck, 1, $competitionid);
	$visitor_stats = commentaries_inplaytips_getlastgamesgoals_team($vid, $matchdate, $gamestocheck, 1, $competitionid);
	
	//dpm($competition_stats['count']);
	//dpm($local_stats['local_count']);
	//dpm($visitor_stats['visitor_count']);
	
	if($competition_stats['count'] < MATCHES_TO_CHECK_IN_THE_PAST_COMPETITION_MIN || $local_stats['local_count'] < $gamestocheck || $visitor_stats['visitor_count'] < $gamestocheck)
		return false;
	
	$local_goals = ($local_stats['goals_as_local_avg']/$competition_stats['goals_as_local_avg']) *
					($visitor_stats['goals_against_as_visitor_avg']/$competition_stats['goals_as_local_avg']) *
					$competition_stats['goals_as_local_avg'];
	
	$visitor_goals = ($visitor_stats['goals_as_visitor_avg']/$competition_stats['goals_as_visitor_avg']) *
					($local_stats['goals_against_as_local_avg']/$competition_stats['goals_as_visitor_avg']) *
					$competition_stats['goals_as_visitor_avg'];
	
	
	if($remaining_time!==false)
	{
		$local_goals = $local_goals * pow($remaining_time, 0.85);
		$visitor_goals = $visitor_goals * pow($remaining_time, 0.85);
	}
	
	$proximity_local = array('0' => 0, '1' => 0, '2' => 0, '3' => 0, '4' => 0, '5' => 0, '6' => 0,); 
	$proximity_visitor = array('0' => 0, '1' => 0, '2' => 0, '3' => 0, '4' => 0, '5' => 0, '6' => 0,); 
	
	foreach($proximity_local as $id => &$value) $value = ( pow(2.71828,-$local_goals) * pow($local_goals,$id) ) / tips_poisson_factorial($id);
	foreach($proximity_visitor as $id => &$value) $value = ( pow(2.71828,-$visitor_goals) * pow($visitor_goals,$id) ) / tips_poisson_factorial($id);
	
	$arraytoreturn['local_goals'] = $local_goals;
	$arraytoreturn['visitor_goals'] = $visitor_goals;
	$arraytoreturn['proximity_local'] = $proximity_local;
	$arraytoreturn['proximity_visitor'] = $proximity_visitor;
	$arraytoreturn['competition_stats'] = $competition_stats;
	$arraytoreturn['local_stats'] = $local_stats;
	$arraytoreturn['visitor_stats'] = $visitor_stats;
	
	return $arraytoreturn;
}


function tips_poisson_distribution_both($match, $remaining_time = false, $gamestocheck = false)
{
	$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
	$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
	$competitionid = $match->field_competition[LANGUAGE_NONE][0]['target_id'];
	$matchdate = $match->field_match_date[LANGUAGE_NONE][0]['value'];
	
	
	$competition_stats = commentaries_get_competition_stats($competitionid, $matchdate, MATCHES_TO_CHECK_IN_THE_PAST_COMPETITION);
	if(!$gamestocheck) $gamestocheck = MATCHES_TO_CHECK_IN_THE_PAST;
	$local_stats = commentaries_inplaytips_getlastgamesgoals_team_both($lid, $matchdate, $gamestocheck, 1);
	$visitor_stats = commentaries_inplaytips_getlastgamesgoals_team_both($vid, $matchdate, $gamestocheck, 1);
	
	//dpm($competition_stats['count']);
	//dpm($local_stats['local_count']);
	//dpm($visitor_stats['visitor_count']);
	
	if($competition_stats['count'] < MATCHES_TO_CHECK_IN_THE_PAST_COMPETITION_MIN || $local_stats['count'] < $gamestocheck || $visitor_stats['count'] < $gamestocheck)
		return false;
	
	$local_goals = ($local_stats['goals_avg']/$competition_stats['goals_avg']) *
					($visitor_stats['goals_against_avg']/$competition_stats['goals_avg']) *
					$competition_stats['goals_avg'];
	
	$visitor_goals = ($visitor_stats['goals_avg']/$competition_stats['goals_avg']) *
					($local_stats['goals_against_avg']/$competition_stats['goals_avg']) *
					$competition_stats['goals_avg'];
	
	
	if($remaining_time!==false)
	{
		$local_goals = $local_goals * pow($remaining_time, 0.85);
		$visitor_goals = $visitor_goals * pow($remaining_time, 0.85);
	}
	
	$proximity_local = array('0' => 0, '1' => 0, '2' => 0, '3' => 0, '4' => 0, '5' => 0, '6' => 0,); 
	$proximity_visitor = array('0' => 0, '1' => 0, '2' => 0, '3' => 0, '4' => 0, '5' => 0, '6' => 0,); 
	
	foreach($proximity_local as $id => &$value) $value = ( pow(2.71828,-$local_goals) * pow($local_goals,$id) ) / tips_poisson_factorial($id);
	foreach($proximity_visitor as $id => &$value) $value = ( pow(2.71828,-$visitor_goals) * pow($visitor_goals,$id) ) / tips_poisson_factorial($id);
	
	$arraytoreturn['local_goals'] = $local_goals;
	$arraytoreturn['visitor_goals'] = $visitor_goals;
	$arraytoreturn['proximity_local'] = $proximity_local;
	$arraytoreturn['proximity_visitor'] = $proximity_visitor;
	$arraytoreturn['competition_stats'] = $competition_stats;
	$arraytoreturn['local_stats'] = $local_stats;
	$arraytoreturn['visitor_stats'] = $visitor_stats;
	
	return $arraytoreturn;
}



function tips_poisson_factorial($number) {
    if ($number == 0) return 1;
    return $number * tips_poisson_factorial($number - 1);
}


function tips_poisson_over_under_match($local_goals, $visitor_goals, $op, $amount)
{
	$poss_goals_l = array(0,1,2,3,4,5,6); 
	$poss_goals_v = array(0,1,2,3,4,5,6);
	$ret_probability = 0;
	foreach($poss_goals_l as $sim_goal_local)
	{
		foreach($poss_goals_v as $sim_goal_visitor)
		{
			if($op=='over' && (($sim_goal_local + $sim_goal_visitor) > $amount))
				$ret_probability += ($local_goals[$sim_goal_local] * $visitor_goals[$sim_goal_visitor]);
				
			if($op=='under' && (($sim_goal_local + $sim_goal_visitor) < $amount))
				$ret_probability += ($local_goals[$sim_goal_local] * $visitor_goals[$sim_goal_visitor]);
			
		}	
	}
	return $ret_probability;
	
}

function tips_poisson_winner_match($local_goals, $visitor_goals, $op)
{
	$poss_goals_l = array(0,1,2,3,4,5,6); 
	$poss_goals_v = array(0,1,2,3,4,5,6);
	$ret_probability = 0;
	foreach($poss_goals_l as $sim_goal_local)
	{
		foreach($poss_goals_v as $sim_goal_visitor)
		{
			if($op=='1' && ($sim_goal_local > $sim_goal_visitor))
				$ret_probability += ($local_goals[$sim_goal_local] * $visitor_goals[$sim_goal_visitor]);
			
			if($op=='2' && ($sim_goal_local < $sim_goal_visitor))
				$ret_probability += ($local_goals[$sim_goal_local] * $visitor_goals[$sim_goal_visitor]);
			
			if($op=='1X' && ($sim_goal_local >= $sim_goal_visitor))
				$ret_probability += ($local_goals[$sim_goal_local] * $visitor_goals[$sim_goal_visitor]);
			
			if($op=='X2' && ($sim_goal_local <= $sim_goal_visitor))
				$ret_probability += ($local_goals[$sim_goal_local] * $visitor_goals[$sim_goal_visitor]);
			
			if($op=='X' && ($sim_goal_local == $sim_goal_visitor))
				$ret_probability += ($local_goals[$sim_goal_local] * $visitor_goals[$sim_goal_visitor]);
		}	
	}
	return $ret_probability;
	
}


