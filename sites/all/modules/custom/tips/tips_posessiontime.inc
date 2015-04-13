<?PHP
function tips_check_posessiontime_tips_creation($daystocheck)
{
	module_load_include('inc', 'commentaries', 'commentaries_strategies');
		
	$from = new DateTime();
	
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
			$lid = $match->field_local_team[LANGUAGE_NONE][0]['target_id'];
			$vid = $match->field_visitor_team[LANGUAGE_NONE][0]['target_id'];
			$matchdate = $match->field_match_date[LANGUAGE_NONE][0]['value'];
			
			$local_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($lid,$matchdate,MATCHES_TO_CHECK_IN_THE_PAST);
			$visitor_numbers = commentaries_inplaytips_getlastgamesgoals_team_both($vid,$matchdate,MATCHES_TO_CHECK_IN_THE_PAST);
			
			$required = array('possestiontime', 'times_w_stats');
			if(count(array_intersect_key(array_flip($required), $local_numbers)) === count($required)) 
			if(count(array_intersect_key(array_flip($required), $visitor_numbers)) === count($required))
			{
				if($local_numbers['possestiontime'] >= LOCAL_POSSESION_TIME &&  
					$visitor_numbers['possestiontime'] <= VISITOR_POSSESION_TIME && 
					$local_numbers['times_w_stats'] >= MATCHES_WITH_STATS_TO_CHECK && 
					$visitor_numbers['times_w_stats'] >= MATCHES_WITH_STATS_TO_CHECK )
				{
					$team = node_load($lid);
					$title = 'Winner by possession time : Local - ' . $team->title . ' : ' . intval($local_numbers['possestiontime']) . '% vs ' 
						. intval($visitor_numbers['possestiontime']) . '%';
					tips_create_tip($match, '', $title, $lid, 0, 583);
				}
			}
					
		}
	}	
}	

function tips_close_tips_posessiontime($tip)
{
	$match = node_load($tip->field_match[LANGUAGE_NONE][0]['target_id']);
	switch($tip->field_tip_type[LANGUAGE_NONE][0]['target_id'])
	{
		case 583:
			if(($match->field_local_team_score[LANGUAGE_NONE][0]['value'] > $match->field_visitor_team_score[LANGUAGE_NONE][0]['value'])) 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 1;
			else 
				$tip->field_win[LANGUAGE_NONE][0]['value'] = 2;
			node_save($tip);
		break;	
	}
				
		
}
