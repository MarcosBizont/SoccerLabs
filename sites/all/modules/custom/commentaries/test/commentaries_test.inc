<?PHP
	
function commentaries_inplaytips_test3()
{
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
	// Get 100 maches before today
	$today = new DateTime();
	$today = $today->modify( '-3 day' );
	//$today = $today->modify( '+10 days' );
	
	$from = new DateTime();
	$from = $from->modify( '-93 days');
    //$from = $from->modify( '+1 hours');
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_football_api_', 'value', '', '<>')
		  ->fieldCondition('field_match_date', 'value', $today->format('Y-m-d H:i:s'), '<')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')		
		  ->fieldOrderBy('field_match_date', 'value', 'ASC');
	
	$result = $query->execute();
	$testreturn = array();
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$goals = 0;
		$nummatchs = 0;
		$over_0_5 = 0;
		$over_1_5 = 0;
		$over_2_5 = 0;
		$over_3_5 = 0;
		$both_to_score = 0;
		$local_shots_pct = 0;
		$visitor_shots_pct = 0;
		$local_corners = 0;
		$visitor_corners = 0;
		$total_corners = 0;
		$rows = array();
		$promscore = 0;
		
		$prom_goals_as_local_avg = 0;
		$prom_goals_against_as_local_avg = 0;
		$prom_goals_as_visitor_avg = 0;
		$prom_goals_against_as_visitor_avg = 0;
		
		$prom_local_possestiontime = 0;
		$prom_visitor_possestiontime = 0;
		
		$local_victory = 0;
		$visitor_victory = 0;
		$draw = 0;

		foreach($matchids as $idmatch)
		{
			$match = node_load($idmatch);
			
			$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
			$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
			$matchdate = $match->field_match_date[LANGUAGE_NONE][0]['value'];
			$competition = node_load($match->field_competition['und'][0]['target_id']);
			
			$local_numbers = commentaries_inplaytips_getlastgamesgoals_team($lid,$matchdate,3);
			$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team($vid,$matchdate,3);
			
			$both_to_score_a = ( intval($match->field_local_team_score['und'][0]['value']) > 0  
										&& intval($match->field_visitor_team_score['und'][0]['value']) > 0 );

			if(1==1)
			{
				if(isset($local_numbers['goals_as_local_avg'])  && isset($local_numbers['goals_against_as_local_avg'])  &&
					isset($local_numbers['score_as_local'])  && isset($local_numbers['score_against_as_local'])  &&
					isset($local_numbers['local_shots'])  && isset($local_numbers['local_corners'])  && 																isset($local_numbers['local_possestiontime'])  && isset($local_numbers['visitor_possestiontime'])  && 
					
					isset($visitor_numbers['goals_as_visitor_avg'])  && isset($visitor_numbers['goals_against_as_visitor_avg'])  &&
					isset($visitor_numbers['score_as_visitor'])  && isset($visitor_numbers['score_against_as_visitor'])  &&
					isset($visitor_numbers['visitor_shots'])  && isset($visitor_numbers['visitor_corners'])  && 																isset($visitor_numbers['visitor_possestiontime']) && isset($visitor_numbers['local_possestiontime'])  
					 )
				{
					$act_promscore = (($local_numbers['score_as_local'] + $local_numbers['score_against_as_local'] +
							$local_numbers['score_as_visitor'] + $local_numbers['score_against_as_visitor'] +
							$visitor_numbers['score_as_local'] + $visitor_numbers['score_against_as_local'] +
							$visitor_numbers['score_as_visitor'] + $visitor_numbers['score_against_as_visitor']) / 8);
					//$local_numbers['local_possestiontime'] >=  50 &&
					//		$visitor_numbers['visitor_possestiontime'] >= 50 &&
							
					//if($act_promscore >= 80)
					if(( (($local_numbers['local_possestiontime']+$local_numbers['visitor_possestiontime'])/2)  >= 50.8
								&& (($visitor_numbers['local_possestiontime']+$visitor_numbers['visitor_possestiontime'])/2) <= 49.2 ) 
					 && ((( $local_numbers['goals_as_local_avg'] >= 1 && $visitor_numbers['goals_against_as_visitor_avg'] >= 1 ))) )
					
					{
						$rows[] = array();
						$testreturn[$match->nid]['obj'] = $match->title;
						$testreturn[$match->nid]['competition'] = $competition->title;
								
						$local_shots_pct += $local_numbers['local_shots'] ;
						$visitor_shots_pct += $visitor_numbers['visitor_shots'] ;
						
						$promscore += (($local_numbers['score_as_local'] + $local_numbers['score_against_as_local'] +
							$visitor_numbers['score_as_visitor'] + $visitor_numbers['score_against_as_visitor']) / 4);
						
						$prom_goals_as_local_avg += $local_numbers['goals_as_local_avg'];
						$prom_goals_against_as_local_avg += $local_numbers['goals_against_as_local_avg'];
						$prom_goals_as_visitor_avg += $visitor_numbers['goals_as_visitor_avg'];
						$prom_goals_against_as_visitor_avg += $visitor_numbers['goals_against_as_visitor_avg'];
						
						$prom_local_possestiontime += $local_numbers['local_possestiontime'];
						$prom_visitor_possestiontime += $visitor_numbers['visitor_possestiontime'];
								
						$gt =
							(intval($match->field_local_team_score['und'][0]['value']) 
									+ intval($match->field_visitor_team_score['und'][0]['value']));
						$testreturn[$match->nid]['goals'] = $gt;
						$goals += $gt;
						$nummatchs ++;
						$local_victory += (intval($match->field_local_team_score['und'][0]['value']) > intval($match->field_visitor_team_score['und'][0]['value'])) ? 1 : 0 ;
						$visitor_victory += (intval($match->field_local_team_score['und'][0]['value']) < intval($match->field_visitor_team_score['und'][0]['value'])) ? 1 : 0 ;
						$draw += (intval($match->field_local_team_score['und'][0]['value']) == intval($match->field_visitor_team_score['und'][0]['value'])) ? 1 : 0 ;
						/*
						$over_0_5 += $gt > 0.5 ? 1 : 0;
						$over_1_5 += $gt > 1.5 ? 1 : 0;
						$over_2_5 += $gt > 2.5 ? 1 : 0;
						$over_3_5 += $gt > 3.5 ? 1 : 0;
						*/
						
						$over_0_5 += intval($match->field_local_team_score['und'][0]['value']) > 0.5 ? 1 : 0;
						$over_1_5 += intval($match->field_local_team_score['und'][0]['value']) > 1.5 ? 1 : 0;
						$over_2_5 += intval($match->field_local_team_score['und'][0]['value']) > 2.5 ? 1 : 0;
						$over_3_5 += intval($match->field_local_team_score['und'][0]['value']) > 3.5 ? 1 : 0;
						
						$testreturn[$match->nid]['statsobj'] = commentaries_inplaytips_getStats($match);
						$local_corners += $testreturn[$match->nid]['statsobj']['local_corners'];   
						$visitor_corners += $testreturn[$match->nid]['statsobj']['visitor_corners'];
						$total_corners += $testreturn[$match->nid]['statsobj']['local_corners'] + $testreturn[$match->nid]['statsobj']['visitor_corners'];
						$testreturn[$match->nid]['statsobj'] = '';
						
						$both_to_score += ( intval($match->field_local_team_score['und'][0]['value']) > 0  
											&& intval($match->field_visitor_team_score['und'][0]['value']) > 0 );
						
						//$testreturn[$match->nid]['local'] = $local_numbers;
						//$testreturn[$match->nid]['visitor'] = $visitor_numbers;
						
						$rows[] = array($both_to_score_a, $local_numbers['goals_as_local_avg'], $local_numbers['goals_against_as_local_avg'], $visitor_numbers['goals_as_visitor_avg'], $visitor_numbers['goals_as_visitor_avg'], $local_numbers['local_possestiontime'], $visitor_numbers['visitor_possestiontime']);
					}
				}
			}
			
			
		}
		dpm('local_victory ' . $local_victory/$nummatchs);
		dpm('visitor_victory ' . $visitor_victory/$nummatchs);
		dpm('draw ' . $draw/$nummatchs);
		
		dpm('local_corners ' . $local_corners/$nummatchs);
		dpm('visitor_corners ' . $visitor_corners/$nummatchs);
		dpm('total_corners ' . $total_corners/$nummatchs);
		

		dpm('local_shots_pct ' . $local_shots_pct/$nummatchs);
		dpm('visitor_shots_pct ' . $visitor_shots_pct/$nummatchs);
		
		dpm('promscore ' . $promscore/$nummatchs);
		
		dpm('prom_goals_as_local_avg ' . $prom_goals_as_local_avg/$nummatchs);
		dpm('prom_goals_against_as_local_avg ' . $prom_goals_against_as_local_avg/$nummatchs);
		dpm('prom_goals_as_visitor_avg ' . $prom_goals_as_visitor_avg/$nummatchs);
		dpm('prom_goals_against_as_visitor_avg ' . $prom_goals_against_as_visitor_avg/$nummatchs);

		dpm('prom_local_possestiontime ' . $prom_local_possestiontime/$nummatchs);
		dpm('prom_visitor_possestiontime ' . $prom_visitor_possestiontime/$nummatchs);
		
		
		dpm('0.5 ' . $over_0_5/$nummatchs);
		dpm('1.5 ' . $over_1_5/$nummatchs);
		dpm('2.5 ' . $over_2_5/$nummatchs);
		dpm('3.5 ' . $over_3_5/$nummatchs);
		dpm('both_to_score ' . $both_to_score/$nummatchs);
		
		dpm('Pro G ' . $goals/$nummatchs);
		
		$headers = array();
		//foreach($rows as $row) echo $row[0] . '/n';
		//echo theme('table', array('header' => $headers, 'rows'=> $rows));
		
	}
	
	dpm($matchids);
	dpm($testreturn);	
}


function commentaries_inplaytips_test2()
{
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
	// Get 100 maches before today
	$today = new DateTime();
	$today = $today->modify( '-1 day' );
	//$today = $today->modify( '+24 hours' );
	
	$from = new DateTime();
	$from = $from->modify( '-30 days');
	//$from = $from->modify( '+16 hours');
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_football_api_', 'value', '', '<>')
		  ->fieldCondition('field_match_date', 'value', $today->format('Y-m-d H:i:s'), '<')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')		
		  ->fieldOrderBy('field_match_date', 'value', 'DESC');
	
	$result = $query->execute();
	$testreturn = array();
	
	if (isset($result['node'])) 
	{
		$matchids = array_keys($result['node']);
		$goals = 0;
		$nummatchs = 0;
		$over_0_5 = 0;
		$over_1_5 = 0;
		$over_2_5 = 0;
		$over_3_5 = 0;
		$both_to_score = 0;
		$local_shots_pct = 0;
		$visitor_shots_pct = 0;
		$local_corners = 0;
		$visitor_corners = 0;
		$total_corners = 0;
		$rows = array();
		$promscore = 0;
		
		$prom_goals_as_local_avg = 0;
		$prom_goals_against_as_local_avg = 0;
		$prom_goals_as_visitor_avg = 0;
		$prom_goals_against_as_visitor_avg = 0;
		
		$prom_local_possestiontime = 0;
		$prom_visitor_possestiontime = 0;

		foreach($matchids as $idmatch)
		{
			$match = node_load($idmatch);
			
			$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
			$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
			$matchdate = $match->field_match_date[LANGUAGE_NONE][0]['value'];
			
			$local_numbers = commentaries_inplaytips_getlastgamesgoals_team($lid,$matchdate,5);
			$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team($vid,$matchdate,5);
			
			///////////////////////////
			///// If both score what happen in stats? 
			//////////////////////////
			
			$both_to_score_a = ( intval($match->field_local_team_score['und'][0]['value']) > 0  
										&& intval($match->field_visitor_team_score['und'][0]['value']) > 0 );

										
			/*
			goals_as_local_avg
			goals_against_as_local_avg
			
			goals_as_visitor_avg
			goals_against_as_visitor_avg
			
			score_as_local
			score_against_as_local
			
			score_as_visitor
			score_against_as_visitor
			
			local_shots
			local_corners
			local_possestiontime
			
			visitor_shots
			visitor_corners
			visitor_possestiontime
			
								
								*/
										
			//if(isset($local_numbers['score_as_local']) && isset($local_numbers['score_against_as_local'])
			//		&& isset($visitor_numbers['score_as_visitor']) && isset($visitor_numbers['score_against_as_visitor']))
			if($both_to_score_a)
			{
				//if($promscores>=90)
				//if(($local_numbers['score_as_local'] >=60 && $local_numbers['score_against_as_local'] >= 50 &&
				//		$visitor_numbers['score_as_visitor'] >= 100 && $visitor_numbers['score_against_as_visitor'] >= 0))
				//if(1==1)
				if(isset($local_numbers['goals_as_local_avg'])  && isset($local_numbers['goals_against_as_local_avg'])  &&
					isset($local_numbers['score_as_local'])  && isset($local_numbers['score_against_as_local'])  &&
					isset($local_numbers['local_shots'])  && isset($local_numbers['local_corners'])  && 																isset($local_numbers['local_possestiontime'])  && 
					
					isset($visitor_numbers['goals_as_visitor_avg'])  && isset($visitor_numbers['goals_against_as_visitor_avg'])  &&
					isset($visitor_numbers['score_as_visitor'])  && isset($visitor_numbers['score_against_as_visitor'])  &&
					isset($visitor_numbers['visitor_shots'])  && isset($visitor_numbers['visitor_corners'])  && 																isset($visitor_numbers['visitor_possestiontime'])  
					 )
				{
					$rows[] = array();
					$testreturn[$match->nid]['obj'] = $match->title;
							
					$local_shots_pct += $local_numbers['local_shots'] ;
					$visitor_shots_pct += $visitor_numbers['visitor_shots'] ;
					
					$promscore += (($local_numbers['score_as_local'] + $local_numbers['score_against_as_local'] +
						$visitor_numbers['score_as_visitor'] + $visitor_numbers['score_against_as_visitor']) / 4);
					
					$prom_goals_as_local_avg += $local_numbers['goals_as_local_avg'];
					$prom_goals_against_as_local_avg += $local_numbers['goals_against_as_local_avg'];
					$prom_goals_as_visitor_avg += $visitor_numbers['goals_as_visitor_avg'];
					$prom_goals_against_as_visitor_avg += $visitor_numbers['goals_against_as_visitor_avg'];
					
					$prom_local_possestiontime += $local_numbers['local_possestiontime'];
					$prom_visitor_possestiontime += $visitor_numbers['visitor_possestiontime'];
							
					$gt =
						(intval($match->field_local_team_score['und'][0]['value']) 
								+ intval($match->field_visitor_team_score['und'][0]['value']));
					$testreturn[$match->nid]['goals'] = $gt;
					$goals += $gt;
					$nummatchs ++;
					
					$over_0_5 += $gt > 0.5 ? 1 : 0;
					$over_1_5 += $gt > 1.5 ? 1 : 0;
					$over_2_5 += $gt > 2.5 ? 1 : 0;
					$over_3_5 += $gt > 3.5 ? 1 : 0;
					
					$testreturn[$match->nid]['statsobj'] = commentaries_inplaytips_getStats($match);
					$local_corners += $testreturn[$match->nid]['statsobj']['local_corners'];   
					$visitor_corners += $testreturn[$match->nid]['statsobj']['visitor_corners'];
					$total_corners += $testreturn[$match->nid]['statsobj']['local_corners'] + $testreturn[$match->nid]['statsobj']['visitor_corners'];
					
					
					$both_to_score += ( intval($match->field_local_team_score['und'][0]['value']) > 0  
										&& intval($match->field_visitor_team_score['und'][0]['value']) > 0 );
					
					$testreturn[$match->nid]['local'] = $local_numbers;
					$testreturn[$match->nid]['visitor'] = $visitor_numbers;
					$rows[] = array(( intval($match->field_local_team_score['und'][0]['value']) > 0  
										&& intval($match->field_visitor_team_score['und'][0]['value']) > 0 ));
				}
			}
			
			
		}
		dpm('local_corners ' . $local_corners/$nummatchs);
		dpm('visitor_corners ' . $visitor_corners/$nummatchs);
		dpm('total_corners ' . $total_corners/$nummatchs);
		

		dpm('local_shots_pct ' . $local_shots_pct/$nummatchs);
		dpm('visitor_shots_pct ' . $visitor_shots_pct/$nummatchs);
		
		dpm('promscore ' . $promscore/$nummatchs);
		
		dpm('prom_goals_as_local_avg ' . $prom_goals_as_local_avg/$nummatchs);
		dpm('prom_goals_against_as_local_avg ' . $prom_goals_against_as_local_avg/$nummatchs);
		dpm('prom_goals_as_visitor_avg ' . $prom_goals_as_visitor_avg/$nummatchs);
		dpm('prom_goals_against_as_visitor_avg ' . $prom_goals_against_as_visitor_avg/$nummatchs);

		dpm('prom_local_possestiontime ' . $prom_local_possestiontime/$nummatchs);
		dpm('prom_visitor_possestiontime ' . $prom_visitor_possestiontime/$nummatchs);
		
		
		dpm('0.5 ' . $over_0_5/$nummatchs);
		dpm('1.5 ' . $over_1_5/$nummatchs);
		dpm('2.5 ' . $over_2_5/$nummatchs);
		dpm('3.5 ' . $over_3_5/$nummatchs);
		dpm('both_to_score ' . $both_to_score/$nummatchs);
		
		dpm('Pro G ' . $goals/$nummatchs);
		
		$headers = array();
		//foreach($rows as $row) echo $row[0] . '/n';
		//echo theme('table', array('header' => $headers, 'rows'=> $rows));
		
	}
	
	dpm($matchids);
	dpm($testreturn);	
}


function commentaries_inplaytips_test1()
{
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
	// Get 100 maches before today
	$today = new DateTime();
	$today = $today->modify( '-1 day' );
	
	$from = new DateTime();
	$from = $from->modify( '-30 day' );
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'match')
		  ->addMetaData('account', user_load(1))
		  ->fieldCondition('field_match_date', 'value', $today->format('Y-m-d H:i:s'), '<')
		  ->fieldCondition('field_match_date', 'value', $from->format('Y-m-d H:i:s'), '>=')
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
			$over2_5_pct = floatval(commentaries_inplaytips_over_under_2_5__1__view($objmatch->nid,3.5));		
			if($over2_5_pct >= 100)
			{
				$ok += ( (intval($objmatch->field_local_team_score['und'][0]['value']) 
							+ intval($objmatch->field_visitor_team_score['und'][0]['value'])) > 2.5 ? 1 : 0);
				$times[] = ( (intval($objmatch->field_local_team_score['und'][0]['value']) 
							+ intval($objmatch->field_visitor_team_score['und'][0]['value'])) > 2.5 ? 1 : 0);	
			}
		}
	}
	
	dpm($ok/ count($times) );
	dpm($times);
	
}
