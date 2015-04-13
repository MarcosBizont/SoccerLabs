<?PHP


function _football_api_feed_process_comments($matchid, $matchremoteid, $isinplay = false)
{
	//watchdog('debug','_football_api_feed_process_comments ' . $matchid . ' ' . $matchremoteid);
	
	module_load_include('inc', 'entity', 'includes/entity.controller');
	libraries_load('football-api');
	
	$foot = new FootballApi(variable_get('football_api_feed_apikey', ''));
	$commentaries = $foot->getMatchCommentaries($matchremoteid);
	
	$mt = node_load($matchid);
	if(isset($commentaries[0]))
	{
		if($isinplay)
			cache_set('comm_commentaries_' . $matchid, $commentaries[0]->comm_commentaries, 'cache', time() + 60*60*72);
		
		$localteam = node_load($mt->field_local_team[LANGUAGE_NONE][0]['target_id']);
	  	$visitorteam = node_load($mt->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	  	
	  	if(isset($commentaries[0]->comm_match_stats->localteam) && isset($commentaries[0]->comm_match_stats->visitorteam))
	  	{
		  	unset($mt->field_stats);
		  	node_save($mt);
	  	
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
		
	}
}
