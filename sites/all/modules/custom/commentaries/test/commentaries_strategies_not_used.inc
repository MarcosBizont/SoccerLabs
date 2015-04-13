<?PHP
	
	
function commentaries_inplaytips_over_2_5_FSB_view()
{
	/*
	 * For the HOME TEAM what we need is the following:
	 *	The home team MUST have had 7 goals or MORE in their last 3 home games.
	 * 	2 or all 3 of the 3 previous games must have ended over 2.5
	 *	For the AWAY TEAM what we need is the following:
	
	 * The away team MUST have had 7 goals or MORE in their last 3 away games.
	 *	The PREVIOUS game... must have had 2 or more goals in total for the entire game.
	 *	The away team MUST have scored in 2 or 3 of the last 3 games.
	 *	2 or 3 of the 3 previous games must have ended over 2.5.
	 *
	 * 
	 * http://www.financial-spread-betting.com/sports/Goals-betting-system.html	
	 *	
	 */
	 
	$nid = is_numeric(arg(1)) ? arg(1) : func_get_arg(0);
	$node = node_load($nid);
	if($node->type!='match' && $node->type!='team') $nid = func_get_arg(0);
	$node = node_load($nid);
	

	$lid = $node->field_local_team[LANGUAGE_NONE][0]['target_id'];
	$vid = $node->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
	$matchdate = $node->field_match_date[LANGUAGE_NONE][0]['value'];
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_local_team', 'target_id', $lid, '=')
		  ->fieldCondition('field_match_date', 'value', $matchdate, '<')
		  ->fieldOrderBy('field_match_date', 'value', 'DESC')
		  ->range(0, 3);
	
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
		  ->fieldCondition('field_visitor_team', 'target_id', $vid, '=')
		  ->fieldCondition('field_match_date', 'value', $matchdate, '<')
		  ->fieldOrderBy('field_match_date', 'value', 'DESC')
		  ->range(0, 3);
	
	$result = $query->execute();
	$match_items_visitor = array();
	if (isset($result['node'])) 
	{
		$match = array_keys($result['node']);
		$match_items_visitor = entity_load('node', $match);
	}
	
	//dpm($match_items_local);
	//dpm($match_items_visitor);
	
	$local_goals = 0;
	$local_over_2_5 = 0;
	
	foreach($match_items_local as $matchlocal)
	{
		$local_goals += intval($matchlocal->field_local_team_score['und'][0]['value']); 	
		$local_over_2_5 += ( (intval($matchlocal->field_local_team_score['und'][0]['value']) 
								+ intval($matchlocal->field_visitor_team_score['und'][0]['value'])) > 2.5 );
	}
	
	$visitor_goals = 0;
	$visitor_over_2_5 = 0;
	$visitor_scored = 0;
	foreach($match_items_visitor as $matchvisitor)
	{
		$visitor_goals += intval($matchvisitor->field_visitor_team_score['und'][0]['value']); 	
		$visitor_over_2_5 += ( (intval($matchvisitor->field_local_team_score['und'][0]['value']) 
								+ intval($matchvisitor->field_visitor_team_score['und'][0]['value'])) > 2.5 );
		$visitor_scored += ( (intval($matchvisitor->field_visitor_team_score['und'][0]['value'])) > 0 );
	}
	
	$visitor_lastgame_over_2 = 0;
	if(is_array($match_items_visitor))
	{
		$visitor_lastgame = array_shift($match_items_visitor);
		if(is_object($visitor_lastgame))
			$visitor_lastgame_over_2 += ( (intval($visitor_lastgame->field_local_team_score['und'][0]['value']) 
										+ intval($visitor_lastgame->field_visitor_team_score['und'][0]['value'])) > 2 );
	}
		
		
	$block  = 'NO';
	if($local_goals>=7 && $local_over_2_5 >=2 && 
		$visitor_goals>=7 && $visitor_over_2_5 >=2 && $visitor_lastgame_over_2 > 0 && $visitor_scored >= 2) $block  = 'YES';
		  
	
	//$block  = $local_goals . ' - ' . $local_over_2_5 . ' - <br><br>' ;
	//$block  .= $visitor_goals . ' - ' . $visitor_over_2_5 . ' - ' . $visitor_scored . ' - ' . $visitor_lastgame_over_2 ; 
	
	return $block;
}

function commentaries_inplaytips_overview($nid, $arr_days = array(5) )
{
	/*
	 *  Tabla por equipo
	 *  Promedio de goles : Local y visitantes  3 stats
	 *  Promedio de goles recibidos : Local y visitantes 3 stats
	 *  Porcentajes de partidos que ha anotado gol : Local y visitantes : 0.5 / 1.5 / 2.5 / 3.5
	 *	Porcentajes de partidos que le ha anotado gol : Local y visitantes : 0.5 / 1.5 / 2.5 / 3.5
	 *	Porcentajes totales : Local y visitantes : 0.5 / 1.5 / 2.5 / 3.5
	 *  Promedio de corners, y numero de partidos en los que se toma
	 *
	 */
	 
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
		$block .= '<h2>' . $dayam . ' games</h2>';
		$local_numbers = commentaries_inplaytips_getlastgamesgoals_team($lid,$matchdate,$dayam,1);
		$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team($vid,$matchdate,$dayam,2);
		
		$rows = array();
		$header = array('Stat', $localteam->title, $visitorteam->title);
		
		foreach($local_numbers as $key => &$value) $rows[] = array($key, 
																	commentaries_format_row($key,$value,'local') , 
																	commentaries_format_row($key,$visitor_numbers[$key],'visitor'));
		
		
		
		$rows[] = array('prom_score_both', ($local_numbers['score_as_local'] + $local_numbers['score_against_as_local'] +
						$local_numbers['score_as_visitor'] + $local_numbers['score_against_as_visitor'] +
						$visitor_numbers['score_as_local'] + $visitor_numbers['score_against_as_local'] +
						$visitor_numbers['score_as_visitor'] + $visitor_numbers['score_against_as_visitor']) / 8, '');
					
		
		$pctover = commentaries_inplaytips_over_under_2_5__1__view($nid, 0.5);
		$rows[] = array('Over 0.5', commentaries_format_row('key', $pctover, 'key'), '');
		
		
		$pctover = commentaries_inplaytips_over_under_2_5__1__view($nid, 2.5);
		$pctover1 = commentaries_inplaytips_over_under_2_5__1__view($nid, 1.5);
		$odds = commentaries_inplaytips_getodds(2.5, 1.5, $pctover, 5, $competitionid);
		$odds1 = commentaries_inplaytips_getodds(1.5, 1.5, $pctover1, 5, $competitionid);
		
		$rows[] = array('Over 1.5', commentaries_format_row('key', $pctover . ' - ' . $pctover1 , 'key'), 
									commentaries_format_row('key', number_format($odds[0],2) . ' / ' . $odds[1] . ' / ' . number_format($odds[2],2) . ' - ' . 
																   number_format($odds1[0],2) . ' / ' . $odds1[1] . ' / ' . number_format($odds1[2],2) , 'key') );
		
		$pctover = commentaries_inplaytips_over_under_2_5__1__view($nid, 3.5);
		$pctover1 = commentaries_inplaytips_over_under_2_5__1__view($nid, 2.5);
		$odds = commentaries_inplaytips_getodds(3.5, 2.5, $pctover, 5, $competitionid);
		$odds1 = commentaries_inplaytips_getodds(2.5, 2.5, $pctover1, 5, $competitionid);
		$rows[] = array('Over 2.5', commentaries_format_row('key', $pctover . ' - ' . $pctover1 , 'key'), 
									commentaries_format_row('key', number_format($odds[0],2) . ' / ' . $odds[1] . ' / ' . number_format($odds[2],2) . ' - ' . 
																   number_format($odds1[0],2) . ' / ' . $odds1[1] . ' / ' . number_format($odds1[2],2) , 'key') );
		
		$pctover = commentaries_inplaytips_over_under_2_5__1__view($nid, 3.5);
		$odds = commentaries_inplaytips_getodds(3.5, 3.5, $pctover, 5, $competitionid);
		$rows[] = array('Over 3.5', commentaries_format_row('key', $pctover, 'key'), 
									commentaries_format_row('key', number_format($odds[0],2) . ' / ' . $odds[1] . ' / ' . number_format($odds[2],2) , 'key') );					 
																 
		$block .= theme('table', array('header' => $header, 'rows'=> $rows));
	}
	
	return $block;	
}


function commentaries_inplaytips_over_under_2_5__1__view($nid, $pct = 2.5, $arr_days = array(1,5))
{
	$pctstr = str_ireplace('.', '_', $pct.'');
	$arr_days_str = '';
	foreach($arr_days as $day_s) $arr_days_str .= $day_s . '_';
	
	if ($cache = cache_get('under_2_5__1__view' . $nid .'_' . $pctstr . '_' . $arr_days_str)) {
	  	$block =  $cache->data;
	}
	else 
	{		
		$node = node_load($nid);
		$lid = $node->field_local_team[LANGUAGE_NONE][0]['target_id'];
		$vid = $node->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
		$matchdate = $node->field_match_date[LANGUAGE_NONE][0]['value'];
		
		$proms = array();
		foreach($arr_days as $dayam)
		{
			$local_numbers = commentaries_inplaytips_getlastgamesgoals_team($lid,$matchdate,$dayam);
			$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team($vid,$matchdate,$dayam);
			if( $local_numbers['local_count'] == $dayam && $local_numbers['visitor_count'] == $dayam 
					&& $visitor_numbers['local_count'] == $dayam && $visitor_numbers['visitor_count'] == $dayam )
			{
				$proms[] = ($local_numbers['goals_as_local_avg'] + $local_numbers['goals_against_as_local_avg']) ;
				$proms[] = ($visitor_numbers['goals_as_visitor_avg'] + $visitor_numbers['goals_against_as_visitor_avg']) ;
				
				$proms[] = (($local_numbers['goals_as_local_avg'] > $visitor_numbers['goals_against_as_visitor_avg']) ? $local_numbers['goals_as_local_avg'] : $visitor_numbers['goals_against_as_visitor_avg']) + (($visitor_numbers['goals_as_visitor_avg'] > $local_numbers['goals_against_as_local_avg']) ? $visitor_numbers['goals_as_visitor_avg'] : $local_numbers['goals_against_as_local_avg']);
				
				$proms[] = (($local_numbers['goals_as_local_avg'] < $visitor_numbers['goals_against_as_visitor_avg']) ? $local_numbers['goals_as_local_avg'] : $visitor_numbers['goals_against_as_visitor_avg']) + (($visitor_numbers['goals_as_visitor_avg'] < $local_numbers['goals_against_as_local_avg']) ? $visitor_numbers['goals_as_visitor_avg'] : $local_numbers['goals_against_as_local_avg']);
				
			}
		}
		$block = -1;
		if(count($proms) > 0)
		{
			$abv_2_5 = 0;
			foreach($proms as $prom) if($prom > $pct) $abv_2_5 += 1;
			
			$block = ( $abv_2_5 / count($proms) ) * 100 ;
			
		}
		cache_set('under_2_5__1__view' . $nid .'_' . $pctstr . '_' . $arr_days_str, $block, 'cache');
	}	
	return $block;
}


function commentaries_inplaytips_getodds($pct, $pcte, $pcti, $winning, $competition)
{
	$today = new DateTime();
	$today = $today->modify( '-1 day' );
	
	$from = new DateTime();
	$from = $from->modify( '-60 day' );
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_match_date', 'value', $today->format('Y-m-d H:i:s'), '<')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')
		  ->fieldCondition('field_competition', 'target_id', $competition, '=')
		  ->fieldOrderBy('field_match_date', 'value', 'DESC');
	
	$result = $query->execute();
	$times = array();
	$ok = 0;
	if (isset($result['node'])) 
	{
		$match = array_keys($result['node']);
		$matches = entity_load('node', $match);
		foreach($matches as $objmatch)
		{
			$over2_5_pct = floatval(commentaries_inplaytips_over_under_2_5__1__view($objmatch->nid,$pct));		
			if($over2_5_pct >= $pcti)
			{
				$ok += ( (intval($objmatch->field_local_team_score['und'][0]['value']) 
							+ intval($objmatch->field_visitor_team_score['und'][0]['value'])) > $pcte ? 1 : 0);
				$times[] = ( (intval($objmatch->field_local_team_score['und'][0]['value']) 
							+ intval($objmatch->field_visitor_team_score['und'][0]['value'])) > $pcte ? 1 : 0);	
			}
		}
	}
	
	$winning = 1 + ($winning / 100);
	$odds = ( $winning / ( $ok / count($times) ) );
	
	return array( $ok / count($times) , count($times), $odds);
}





	
function tips_poisson_distribution_simulation_under($under_poisson_probability,$competitionid,$daystotest)
{
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
		
	$from = new DateTime();
	$from = $from->modify( '-' . $daystotest . ' days');
	
	$to = new DateTime();
	$to = $to->modify('-1 day');
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
	//	  ->fieldCondition('field_competition', 'target_id', $competitionid, '=')
		  ->fieldCondition('field_match_date', 'value', $to->format('Y-m-d H:i:s'), '<')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')		
		  ->fieldOrderBy('field_match_date', 'value', 'ASC');
	
	$result = $query->execute();
	
	$return_array = array();
	$wins_local = 0;
	$num_local = 0;
	
	$wins_visitor = 0;
	$num_visitor = 0;
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$match_items = entity_load('node', $matchids);
		foreach($match_items as $match)
		{
			$posson_stats = tips_poisson_distribution($match);
			
			$under_1_5_local_probability = $posson_stats['proximity_local'][0] + $posson_stats['proximity_local'][1];
			$under_1_5_visitor_probability = $posson_stats['proximity_visitor'][0] + $posson_stats['proximity_visitor'][1];
	
			if(is_array($posson_stats))
			{
				if($under_1_5_local_probability >= $under_poisson_probability)
				{
					if(intval($match->field_local_team_score['und'][0]['value']) < 1.5) 
						$wins_local += 1;
					$num_local += 1;	
				}
				if($under_1_5_visitor_probability >= $under_poisson_probability)
				{
					if(intval($match->field_visitor_team_score['und'][0]['value']) < 1.5) 
						$wins_visitor += 1;
					$num_visitor += 1;	
				}
			}
		}
	}
	
	$return_array['local_under_1_5_prob'] = ($wins_local/$num_local);
	$return_array['local_under_1_5'] = $wins_local;
	$return_array['local_num'] = $num_local;
	
	$return_array['visitor_under_1_5_prob'] = ($wins_visitor/$num_visitor);
	$return_array['visitor_under_1_5'] = $wins_visitor;
	$return_array['visitor_num'] = $num_visitor;
	
	return $return_array;
}


function tips_poisson_distribution_simulation_draw($draw_poisson_probability,$competitionid,$daystotest)
{
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
		
	$from = new DateTime();
	$from = $from->modify( '-' . $daystotest . ' days');
	
	$to = new DateTime();
	$to = $to->modify('-1 day');
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_competition', 'target_id', $competitionid, '=')
		  ->fieldCondition('field_match_date', 'value', $to->format('Y-m-d H:i:s'), '<')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')		
		  ->fieldOrderBy('field_match_date', 'value', 'ASC');
	
	$result = $query->execute();
	
	$return_array = array();
	$draws = 0;
	$num = 0;
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$match_items = entity_load('node', $matchids);
		foreach($match_items as $match)
		{
			$posson_stats = tips_poisson_distribution($match);
			
			$draw_poisson_probability = 0;
	
			foreach($proximity_local as $id => $value) $draw_poisson_probability += ($value * $proximity_visitor[$id]);
			
			if(is_array($posson_stats))
				if($posson_stats['draw_poisson_probability']>=$draw_poisson_probability)
				{
					if(intval($match->field_local_team_score['und'][0]['value']) == intval($match->field_visitor_team_score['und'][0]['value'])) 
						$draws += 1;
					$num += 1;	
				}
		}
	}
	
	$return_array['draws_prob'] = ($draws/$num);
	$return_array['draws'] = $draws;
	$return_array['nums'] = $num;
	
	return $return_array;
}


function tips_prediction_bases_in_posessiontime_simulation($local_possesiontime, $visitor_posessiontime, $daystotest)
{
	if ($cache = cache_get('tips_prediction_bases_in_posessiontime_simulation_' . $local_possesiontime . '_' . $visitor_posessiontime . '_' . $daystotest )) {
	  	$return_array =  $cache->data;
	}
	else 
	{	
			
		module_load_include('inc', 'commentaries', 'commentaries_strategies');
		
		$from = new DateTime();
		$from = $from->modify( '-' . $daystotest . ' days');
		
		$to = new DateTime();
		$to = $to->modify('-1 day');
		
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node')
			  ->entityCondition('bundle', 'match')
			  ->addMetaData('account', user_load(1))
			  ->fieldCondition('field_match_date', 'value', $to->format('Y-m-d H:i:s'), '<')
			  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')		
			  ->fieldOrderBy('field_match_date', 'value', 'ASC');
		
		$result = $query->execute();
		
		$return_array = array();
		$local_overs = array('0.5' => 0, '1.5' => 0, '2.5' => 0, '3.5' => 0, );
		$total_overs = array('0.5' => 0, '1.5' => 0, '2.5' => 0, '3.5' => 0, );
		$local_victory = 0;
		$real_num = 0;
		
		
		$big_local_overs = array('0.5' => 0, '1.5' => 0, '2.5' => 0, '3.5' => 0, );
		$big_total_overs = array('0.5' => 0, '1.5' => 0, '2.5' => 0, '3.5' => 0, );
		$big_local_victory = 0;
		$big_num = 0;
		
		if (isset($result['node'])) 
		{
			$matchids = array_keys($result['node']);
			$match_items = entity_load('node', $matchids);
			foreach($match_items as $match)
			{
				$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
				$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
				$matchdate = $match->field_match_date[LANGUAGE_NONE][0]['value'];
				
				$local_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($lid,$matchdate,MATCHES_TO_CHECK_IN_THE_PAST);
				$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($vid,$matchdate,MATCHES_TO_CHECK_IN_THE_PAST);
				
				$required = array('possestiontime', 'times_w_stats');
				if(count(array_intersect_key(array_flip($required), $local_numbers)) === count($required))
					if(count(array_intersect_key(array_flip($required), $visitor_numbers)) === count($required))
						if( $local_numbers['possestiontime'] >= $local_possesiontime 
							&&  $visitor_numbers['possestiontime'] <= $visitor_posessiontime
							&& $local_numbers['times_w_stats'] >= MATCHES_WITH_STATS_TO_CHECK && $visitor_numbers['times_w_stats'] >= MATCHES_WITH_STATS_TO_CHECK )
						{
							$local_victory += (intval($match->field_local_team_score['und'][0]['value']) > intval($match->field_visitor_team_score['und'][0]['value'])) ? 1 : 0 ;
							foreach($local_overs as $key => &$over) 
								$over += intval($match->field_local_team_score['und'][0]['value']) > intval($key) ? 1 : 0;
							foreach($total_overs as $key => &$over) 
								$over += (intval($match->field_local_team_score['und'][0]['value']) + intval($match->field_visitor_team_score['und'][0]['value']))> intval($key) ? 1 : 0;
							$real_num += 1;	
						}
				
				$big_local_victory += 
					(intval($match->field_local_team_score['und'][0]['value']) > intval($match->field_visitor_team_score['und'][0]['value'])) ? 1 : 0 ;
				foreach($big_local_overs as $key => &$over) 
					$over += intval($match->field_local_team_score['und'][0]['value']) > intval($key) ? 1 : 0;
				foreach($big_total_overs as $key => &$over) 
					$over += (intval($match->field_local_team_score['und'][0]['value']) + intval($match->field_visitor_team_score['und'][0]['value']))> intval($key) ? 1 : 0;
				$big_num += 1;		
				
			}
		}
		foreach($local_overs as $key => &$over) $over = $over/$real_num;
		foreach($total_overs as $key => &$over) $over = $over/$real_num;
		foreach($big_local_overs as $key => &$over) $over = $over/$big_num;
		foreach($big_total_overs as $key => &$over) $over = $over/$big_num;
		
		$return_array['local_overs'] = $local_overs;
		$return_array['total_overs'] = $total_overs;
		$return_array['local_victory'] = ($local_victory/$real_num);
		$return_array['real_num'] = $real_num;
		
		$return_array['big_local_overs'] = $big_local_overs;
		$return_array['big_total_overs'] = $big_total_overs;
		$return_array['big_local_victory'] = ($big_local_victory/$big_num);
		$return_array['big_num'] = $big_num;
		
		cache_set('tips_prediction_bases_in_posessiontime_simulation_' . $local_possesiontime . '_' . $visitor_posessiontime . '_' . $daystotest , $return_array, 'cache');	
	}	
	return $return_array;
}

