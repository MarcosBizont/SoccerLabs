<?PHP

function _football_api_feed_process_comments_fromBK()
{
	$query = db_select('BK_field_data_field_football_api_', 'a');
	$query->fields('a',array('entity_id'));
		
	$result = $query->execute();
	$queue = DrupalQueue::get('processCommentariesQueue');
	while($record = $result->fetchAssoc())
		$queue->createItem(array($record['entity_id'],''));
	
	//_football_api_feed_process_comments($record['entity_id']);
}	

function _football_api_feed_process_comments($matchid, $commentaries = false)
{
	module_load_include('inc', 'entity', 'includes/entity.controller');
	//$commentaries = $foot->getMatchCommentaries($matchobj->match_id);
	if(!is_array($commentaries))
	{
		$query = db_select('BK_field_data_field_football_api_', 'a');
		$query->fields('a',array('field_football_api__value'))
			->condition('a.entity_id', $matchid);
		$result = $query->execute();
        $result = $result->fetchField();
        $commentaries = json_decode($result);	
	}
	
	$mt = node_load($matchid);
	if(isset($commentaries[0]))
	{
		$localteam = node_load($mt->field_local_team[LANGUAGE_NONE][0]['target_id']);
	  	$visitorteam = node_load($mt->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	  	
	  	createTermsFromString($commentaries[0]->comm_match_info->stadium->name,':','stadium', $mt->field_stadium);
		createTermsFromString($commentaries[0]->comm_match_info->referee->name,':','referee', $mt->field_referee);
		
		for($x=1; $x<=20; $x++)
		{
			if(isset($commentaries[0]->comm_match_teams->localteam->player->$x))
				if($player = $commentaries[0]->comm_match_teams->localteam->player->$x)
					_football_api_feed_create_player($player->id, $player->number, $player->name, $player->pos, $localteam->nid);
			if(isset($commentaries[0]->comm_match_subs->localteam->player->$x))
				if($player = $commentaries[0]->comm_match_subs->localteam->player->$x)
					_football_api_feed_create_player($player->id, $player->number, $player->name, $player->pos, $localteam->nid);
				
			if(isset($commentaries[0]->comm_match_teams->visitorteam->player->$x))
				if($player = $commentaries[0]->comm_match_teams->visitorteam->player->$x)
					_football_api_feed_create_player($player->id, $player->number, $player->name, $player->pos, $visitorteam->nid);
			if(isset($commentaries[0]->comm_match_subs->visitorteam->player->$x))
				if($player = $commentaries[0]->comm_match_subs->visitorteam->player->$x)
					_football_api_feed_create_player($player->id, $player->number, $player->name, $player->pos, $visitorteam->nid);
		}
		
		$mt->field_attendance[LANGUAGE_NONE][0]['value'] = intval($commentaries[0]->comm_match_info->attendance->name);
		
		unset($mt->field_goals);
	  	unset($mt->field_cards);
	  	unset($mt->field_stats);
	  	unset($mt->field_lineup);
	  	unset($mt->field_bench);
	  	unset($mt->field_substitutions);
	  	node_save($mt);
	  	
	  	if(isset($commentaries[0]->comm_match_subs->localteam->player))
	  	{
		  	$local_bench = commentaries_tablefromarray(
	  							$commentaries[0]->comm_match_subs->localteam->player, 
	  							array(), 
	  							array('number','name', 'pos', 'id'), false, true);
	  		foreach($local_bench as $bench)
		  	{
			  	$player = 
			  		_football_api_feed_create_player($bench[3], $bench[0], $bench[1], $bench[2], $localteam->nid);
			  	$playernid = $player->nid;
			  	$values = array(
				    'field_name' => 'field_bench',
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
				    ),
				    'field_player' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid)),
				    ),
				);
				$entity = entity_create('field_collection_item', $values);
				$entity->setHostEntity('node', $mt);
				$entity->save();
		  	}
	  	}
	  	if(isset($commentaries[0]->comm_match_subs->visitorteam->player))
	  	{
		  	$visitor_bench = commentaries_tablefromarray(
		  						$commentaries[0]->comm_match_subs->visitorteam->player, 
	  							array(), 
	  							array('number','name', 'pos', 'id'), false, true);
	  		foreach($visitor_bench as $bench)
		  	{
			  	$player = 
			  		_football_api_feed_create_player($bench[3], $bench[0], $bench[1], $bench[2], $visitorteam->nid);
			  	$playernid = $player->nid;
			  	$values = array(
				    'field_name' => 'field_bench',
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
				    ),
				    'field_player' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid)),
				    ),
				);
				$entity = entity_create('field_collection_item', $values);
				$entity->setHostEntity('node', $mt);
				$entity->save();
		  	}
	  	}
	  	
	  	
		$localgoals = commentaries_tablefromarray($commentaries[0]->comm_match_summary->localteam->goals, 
	  							array(), 
	  							array('minute','name','owngoal','penalty', 'id'), false, true); 
	  	
	  	
	  	foreach($localgoals as $goal)
	  	{
		  	$playernid = _football_api_feed_get_player($goal[4], $goal[1], $localteam->nid);
		  	$values = array(
			    'field_name' => 'field_goals',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[0])),
			    ),
			    'field_own_goal' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[2])),
			    ),
			    'field_penalty' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[3])),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
	  	
	  	$visitorgoals = commentaries_tablefromarray($commentaries[0]->comm_match_summary->visitorteam->goals, 
	  							array(), 
	  							array('minute','name','owngoal','penalty', 'id'), false, true);
	  							
	  	foreach($visitorgoals as $goal)
	  	{
		  	$playernid = _football_api_feed_get_player($goal[4], $goal[1], $visitorteam->nid);
		  	$values = array(
			    'field_name' => 'field_goals',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[0])),
			    ),
			    'field_own_goal' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[2])),
			    ),
			    'field_penalty' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[3])),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
	  	
	  	
	  	$localteam_yellowcards = commentaries_tablefromarray(
						$commentaries[0]->comm_match_summary->localteam->yellowcards, 
						array(), 
						array('minute','name', 'id'), false, true);
		
		foreach($localteam_yellowcards as $card)
	  	{
		  	$playernid = _football_api_feed_get_player($card[2], $card[1], $localteam->nid);
		  	$values = array(
			    'field_name' => 'field_cards',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $card[0])),
			    ),
			    'field_color' => array(
			      LANGUAGE_NONE => array(array('value' => 'Yellow')),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
						
		$localteam_redcards = commentaries_tablefromarray(
						$commentaries[0]->comm_match_summary->localteam->redcards, 
						array(), 
						array('minute','name', 'id'), false, true);
						 
		foreach($localteam_redcards as $card)
	  	{
		  	$playernid = _football_api_feed_get_player($card[2], $card[1], $localteam->nid);
		  	$values = array(
			    'field_name' => 'field_cards',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $card[0])),
			    ),
			    'field_color' => array(
			      LANGUAGE_NONE => array(array('value' => 'Red')),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
	  	
	  	$visitorteam_yellowcards = commentaries_tablefromarray(
						$commentaries[0]->comm_match_summary->visitorteam->yellowcards, 
						array(), 
						array('minute','name', 'id'), false, true);
		
		foreach($visitorteam_yellowcards as $card)
	  	{
		  	$playernid = _football_api_feed_get_player($card[2], $card[1], $visitorteam->nid);
		  	$values = array(
			    'field_name' => 'field_cards',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $card[0])),
			    ),
			    'field_color' => array(
			      LANGUAGE_NONE => array(array('value' => 'Yellow')),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
						
		$visitorteam_redcards = commentaries_tablefromarray(
						$commentaries[0]->comm_match_summary->visitorteam->redcards, 
						array(), 
						array('minute','name', 'id'), false, true);
		
		foreach($visitorteam_redcards as $card)
	  	{
		  	$playernid = _football_api_feed_get_player($card[2], $card[1], $visitorteam->nid);
		  	$values = array(
			    'field_name' => 'field_cards',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $card[0])),
			    ),
			    'field_color' => array(
			      LANGUAGE_NONE => array(array('value' => 'Red')),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
		
		if(isset($commentaries[0]->comm_match_stats->localteam) && isset($commentaries[0]->comm_match_stats->visitorteam))
	  	{
		  	$commentaries[0]->comm_match_stats->localteam->teamnid = $localteam->nid;
		  	$commentaries[0]->comm_match_stats->visitorteam->teamnid = $visitorteam->nid;
			$arrstats = array($commentaries[0]->comm_match_stats->localteam, $commentaries[0]->comm_match_stats->visitorteam);
		  	
		  	foreach($arrstats as $statobj)
		  	{
		  	  	$rows = array();
				foreach($statobj as $key => $stat)
			  	{
				  	switch($key)
				  	{
					  	case 'shots';
					  		$values = array(
							    'field_name' => 'field_stats',
							    'field_stat' => array(
							      LANGUAGE_NONE => array(array('value' => 'shots')),
							    ),
							    'field_value' => array(
							      LANGUAGE_NONE => array(array('value' => $stat->total)),
							    ),
							    'field_team' => array(
							      LANGUAGE_NONE => array(array('target_id' => $statobj->teamnid)),
							    ),
							);
							$entity = entity_create('field_collection_item', $values);
							$entity->setHostEntity('node', $mt);
							$entity->save();
							
							$values = array(
							    'field_name' => 'field_stats',
							    'field_stat' => array(
							      LANGUAGE_NONE => array(array('value' => 'shotsongoal')),
							    ),
							    'field_value' => array(
							      LANGUAGE_NONE => array(array('value' => $stat->ongoal)),
							    ),
							    'field_team' => array(
							      LANGUAGE_NONE => array(array('target_id' => $statobj->teamnid)),
							    ),
							);
							$entity = entity_create('field_collection_item', $values);
							$entity->setHostEntity('node', $mt);
							$entity->save();	
					  	break;
					  	case 'fouls';
					  	case 'corners';
					  	case 'offsides';
					  	case 'possestiontime';
					  	case 'yellowcards';
					  	case 'redcards';
					  	case 'saves';
					  		$values = array(
							    'field_name' => 'field_stats',
							    'field_stat' => array(
							      LANGUAGE_NONE => array(array('value' => $key)),
							    ),
							    'field_value' => array(
							      LANGUAGE_NONE => array(array('value' => $stat->total)),
							    ),
							    'field_team' => array(
							      LANGUAGE_NONE => array(array('target_id' => $statobj->teamnid)),
							    ),
							);
							$entity = entity_create('field_collection_item', $values);
							$entity->setHostEntity('node', $mt);
							$entity->save();
					  	break;
				  	}
			  	}
			}
		}
		
		if(isset($commentaries[0]->comm_match_teams->localteam->player))
	  	{
		  	$local_lineup = commentaries_tablefromarray(
	  							$commentaries[0]->comm_match_teams->localteam->player, 
	  							array(), 
	  							array('number','name', 'pos','id'), false, true);
	  	
	  		foreach($local_lineup as $lineup)
		  	{
			  	$playernid = _football_api_feed_get_player($lineup[3], $lineup[1], $localteam->nid);
			  	$values = array(
				    'field_name' => 'field_lineup',
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
				    ),
				    'field_player' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid)),
				    ),
				);
				$entity = entity_create('field_collection_item', $values);
				$entity->setHostEntity('node', $mt);
				$entity->save();
		  	}
	  	}
	  	
	  	
	  	
	  	if(isset($commentaries[0]->comm_match_teams->visitorteam->player))
	  	{
		  	$visitor_lineup = commentaries_tablefromarray(
		  						$commentaries[0]->comm_match_teams->visitorteam->player, 
	  							array(), 
	  							array('number','name', 'pos','id'), false, true);
	  		
	  		foreach($visitor_lineup as $lineup)
		  	{
			  	$playernid = _football_api_feed_get_player($lineup[3], $lineup[1], $visitorteam->nid);
			  	$values = array(
				    'field_name' => 'field_lineup',
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
				    ),
				    'field_player' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid)),
				    ),
				);
				$entity = entity_create('field_collection_item', $values);
				$entity->setHostEntity('node', $mt);
				$entity->save();
		  	}
	  	}
	  	
	  	if(isset($commentaries[0]->comm_match_substitutions->localteam->substitution))
	  	{
		  	$local_substitutions = commentaries_tablefromarray(
	  							$commentaries[0]->comm_match_substitutions->localteam->substitution, 
	  							array(), 
	  							array('minute','off', 'on','off_id','on_id'), false, true);
	  		
	  		foreach($local_substitutions as $sub)
		  	{
			  	$playernid_on = _football_api_feed_get_player($sub[4], $sub[2], $localteam->nid);
			  	$playernid_off = _football_api_feed_get_player($sub[3], $sub[1], $localteam->nid);
			  	$values = array(
				    'field_name' => 'field_substitutions',
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
				    ),
				    'field_in' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid_on)),
				    ),
				    'field_out' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid_off)),
				    ),
				    'field_minute_str' => array(
				      LANGUAGE_NONE => array(array('value' => $sub[0])),
				    ),
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
				    ),
				);
				$entity = entity_create('field_collection_item', $values);
				$entity->setHostEntity('node', $mt);
				$entity->save();
		  	}
	  	}
	  	if(isset($commentaries[0]->comm_match_substitutions->visitorteam->substitution))
	  	{
		  	$visitor_substitutions = commentaries_tablefromarray(
		  						$commentaries[0]->comm_match_substitutions->visitorteam->substitution, 
	  							array(),
	  							array('minute','off', 'on','on_id','off_id'), false, true);
	  		
	  		foreach($visitor_substitutions as $sub)
		  	{
			  	$playernid_on = _football_api_feed_get_player($sub[4], $sub[2], $visitorteam->nid);
			  	$playernid_off = _football_api_feed_get_player($sub[3], $sub[1], $visitorteam->nid);
			  	$values = array(
				    'field_name' => 'field_substitutions',
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
				    ),
				    'field_in' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid_on)),
				    ),
				    'field_out' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid_off)),
				    ),
				    'field_minute_str' => array(
				      LANGUAGE_NONE => array(array('value' => $sub[0])),
				    ),
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
				    ),
				);
				$entity = entity_create('field_collection_item', $values);
				$entity->setHostEntity('node', $mt);
				$entity->save();
		  	}
	  	}
	  	
	  	
	  	//$commentaries = $commentaries[0]->comm_commentaries->comment;
	  	//$mt->field_football_api_[LANGUAGE_NONE][0]['value'] = json_encode($commentaries);
	  	//if($matchobj->match_status=='HT') $mt->field_football_api_half[LANGUAGE_NONE][0]['value'] = json_encode($commentaries);
		node_save($mt);	
	}
}

function _football_api_feed_process_comments_inplay($matchid, $commentaries = false)
{
	module_load_include('inc', 'entity', 'includes/entity.controller');
	//$commentaries = $foot->getMatchCommentaries($matchobj->match_id);
	if(!is_array($commentaries))
	{
		$query = db_select('BK_field_data_field_football_api_', 'a');
		$query->fields('a',array('field_football_api__value'))
			->condition('a.entity_id', $matchid);
		$result = $query->execute();
        $result = $result->fetchField();
        $commentaries = json_decode($result);	
	}
	
	$mt = node_load($matchid);
	if(isset($commentaries[0]))
	{
		$localteam = node_load($mt->field_local_team[LANGUAGE_NONE][0]['target_id']);
	  	$visitorteam = node_load($mt->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	  	
	  	unset($mt->field_goals);
	  	unset($mt->field_cards);
	  	unset($mt->field_stats);
	  	unset($mt->field_substitutions);
	  	node_save($mt);
	  	
	  	$localgoals = commentaries_tablefromarray($commentaries[0]->comm_match_summary->localteam->goals, 
	  							array(), 
	  							array('minute','name','owngoal','penalty', 'id'), false, true); 
	  	
	  	
	  	foreach($localgoals as $goal)
	  	{
		  	$playernid = _football_api_feed_get_player($goal[4], $goal[1], $localteam->nid);
		  	$values = array(
			    'field_name' => 'field_goals',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[0])),
			    ),
			    'field_own_goal' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[2])),
			    ),
			    'field_penalty' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[3])),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
	  	
	  	$visitorgoals = commentaries_tablefromarray($commentaries[0]->comm_match_summary->visitorteam->goals, 
	  							array(), 
	  							array('minute','name','owngoal','penalty', 'id'), false, true);
	  							
	  	foreach($visitorgoals as $goal)
	  	{
		  	$playernid = _football_api_feed_get_player($goal[4], $goal[1], $visitorteam->nid);
		  	$values = array(
			    'field_name' => 'field_goals',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[0])),
			    ),
			    'field_own_goal' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[2])),
			    ),
			    'field_penalty' => array(
			      LANGUAGE_NONE => array(array('value' => $goal[3])),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
	  	
	  	
	  	$localteam_yellowcards = commentaries_tablefromarray(
						$commentaries[0]->comm_match_summary->localteam->yellowcards, 
						array(), 
						array('minute','name', 'id'), false, true);
		
		foreach($localteam_yellowcards as $card)
	  	{
		  	$playernid = _football_api_feed_get_player($card[2], $card[1], $localteam->nid);
		  	$values = array(
			    'field_name' => 'field_cards',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $card[0])),
			    ),
			    'field_color' => array(
			      LANGUAGE_NONE => array(array('value' => 'Yellow')),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
						
		$localteam_redcards = commentaries_tablefromarray(
						$commentaries[0]->comm_match_summary->localteam->redcards, 
						array(), 
						array('minute','name', 'id'), false, true);
						 
		foreach($localteam_redcards as $card)
	  	{
		  	$playernid = _football_api_feed_get_player($card[2], $card[1], $localteam->nid);
		  	$values = array(
			    'field_name' => 'field_cards',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $card[0])),
			    ),
			    'field_color' => array(
			      LANGUAGE_NONE => array(array('value' => 'Red')),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
	  	
	  	$visitorteam_yellowcards = commentaries_tablefromarray(
						$commentaries[0]->comm_match_summary->visitorteam->yellowcards, 
						array(), 
						array('minute','name', 'id'), false, true);
		
		foreach($visitorteam_yellowcards as $card)
	  	{
		  	$playernid = _football_api_feed_get_player($card[2], $card[1], $visitorteam->nid);
		  	$values = array(
			    'field_name' => 'field_cards',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $card[0])),
			    ),
			    'field_color' => array(
			      LANGUAGE_NONE => array(array('value' => 'Yellow')),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
						
		$visitorteam_redcards = commentaries_tablefromarray(
						$commentaries[0]->comm_match_summary->visitorteam->redcards, 
						array(), 
						array('minute','name', 'id'), false, true);
		
		foreach($visitorteam_redcards as $card)
	  	{
		  	$playernid = _football_api_feed_get_player($card[2], $card[1], $visitorteam->nid);
		  	$values = array(
			    'field_name' => 'field_cards',
			    'field_minute_str' => array(
			      LANGUAGE_NONE => array(array('value' => $card[0])),
			    ),
			    'field_color' => array(
			      LANGUAGE_NONE => array(array('value' => 'Red')),
			    ),
			    'field_team' => array(
			      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
			    ),
			    'field_player' => array(
			      LANGUAGE_NONE => array(array('target_id' => $playernid)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $mt);
			$entity->save();
	  	}
		
		if(isset($commentaries[0]->comm_match_stats->localteam) && isset($commentaries[0]->comm_match_stats->visitorteam))
	  	{
		  	$commentaries[0]->comm_match_stats->localteam->teamnid = $localteam->nid;
		  	$commentaries[0]->comm_match_stats->visitorteam->teamnid = $visitorteam->nid;
			$arrstats = array($commentaries[0]->comm_match_stats->localteam, $commentaries[0]->comm_match_stats->visitorteam);
		  	
		  	foreach($arrstats as $statobj)
		  	{
		  	  	$rows = array();
				foreach($statobj as $key => $stat)
			  	{
				  	switch($key)
				  	{
					  	case 'shots';
					  		$values = array(
							    'field_name' => 'field_stats',
							    'field_stat' => array(
							      LANGUAGE_NONE => array(array('value' => 'shots')),
							    ),
							    'field_value' => array(
							      LANGUAGE_NONE => array(array('value' => $stat->total)),
							    ),
							    'field_team' => array(
							      LANGUAGE_NONE => array(array('target_id' => $statobj->teamnid)),
							    ),
							);
							$entity = entity_create('field_collection_item', $values);
							$entity->setHostEntity('node', $mt);
							$entity->save();
							
							$values = array(
							    'field_name' => 'field_stats',
							    'field_stat' => array(
							      LANGUAGE_NONE => array(array('value' => 'shotsongoal')),
							    ),
							    'field_value' => array(
							      LANGUAGE_NONE => array(array('value' => $stat->ongoal)),
							    ),
							    'field_team' => array(
							      LANGUAGE_NONE => array(array('target_id' => $statobj->teamnid)),
							    ),
							);
							$entity = entity_create('field_collection_item', $values);
							$entity->setHostEntity('node', $mt);
							$entity->save();	
					  	break;
					  	case 'fouls';
					  	case 'corners';
					  	case 'offsides';
					  	case 'possestiontime';
					  	case 'yellowcards';
					  	case 'redcards';
					  	case 'saves';
					  		$values = array(
							    'field_name' => 'field_stats',
							    'field_stat' => array(
							      LANGUAGE_NONE => array(array('value' => $key)),
							    ),
							    'field_value' => array(
							      LANGUAGE_NONE => array(array('value' => $stat->total)),
							    ),
							    'field_team' => array(
							      LANGUAGE_NONE => array(array('target_id' => $statobj->teamnid)),
							    ),
							);
							$entity = entity_create('field_collection_item', $values);
							$entity->setHostEntity('node', $mt);
							$entity->save();
					  	break;
				  	}
			  	}
			}
		}
		
		if(isset($commentaries[0]->comm_match_substitutions->localteam->substitution))
	  	{
		  	$local_substitutions = commentaries_tablefromarray(
	  							$commentaries[0]->comm_match_substitutions->localteam->substitution, 
	  							array(), 
	  							array('minute','off', 'on','off_id','on_id'), false, true);
	  		
	  		foreach($local_substitutions as $sub)
		  	{
			  	$playernid_on = _football_api_feed_get_player($sub[4], $sub[2], $localteam->nid);
			  	$playernid_off = _football_api_feed_get_player($sub[3], $sub[1], $localteam->nid);
			  	$values = array(
				    'field_name' => 'field_substitutions',
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
				    ),
				    'field_in' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid_on)),
				    ),
				    'field_out' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid_off)),
				    ),
				    'field_minute_str' => array(
				      LANGUAGE_NONE => array(array('value' => $sub[0])),
				    ),
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $localteam->nid)),
				    ),
				);
				$entity = entity_create('field_collection_item', $values);
				$entity->setHostEntity('node', $mt);
				$entity->save();
		  	}
	  	}
	  	if(isset($commentaries[0]->comm_match_substitutions->visitorteam->substitution))
	  	{
		  	$visitor_substitutions = commentaries_tablefromarray(
		  						$commentaries[0]->comm_match_substitutions->visitorteam->substitution, 
	  							array(),
	  							array('minute','off', 'on','on_id','off_id'), false, true);
	  		
	  		foreach($visitor_substitutions as $sub)
		  	{
			  	$playernid_on = _football_api_feed_get_player($sub[4], $sub[2], $visitorteam->nid);
			  	$playernid_off = _football_api_feed_get_player($sub[3], $sub[1], $visitorteam->nid);
			  	$values = array(
				    'field_name' => 'field_substitutions',
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
				    ),
				    'field_in' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid_on)),
				    ),
				    'field_out' => array(
				      LANGUAGE_NONE => array(array('target_id' => $playernid_off)),
				    ),
				    'field_minute_str' => array(
				      LANGUAGE_NONE => array(array('value' => $sub[0])),
				    ),
				    'field_team' => array(
				      LANGUAGE_NONE => array(array('target_id' => $visitorteam->nid)),
				    ),
				);
				$entity = entity_create('field_collection_item', $values);
				$entity->setHostEntity('node', $mt);
				$entity->save();
		  	}
	  	}
	  	
	  	
	  	//$commentaries = $commentaries[0]->comm_commentaries->comment;
	  	//$mt->field_football_api_[LANGUAGE_NONE][0]['value'] = json_encode($commentaries);
	  	//if($matchobj->match_status=='HT') $mt->field_football_api_half[LANGUAGE_NONE][0]['value'] = json_encode($commentaries);
		node_save($mt);	
	}
}