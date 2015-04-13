<?PHP

function _tipgin_api_feed_process_matchdata($matchid, $matchdata)
{
	module_load_include('inc', 'entity', 'includes/entity.controller');
	
	$mt = node_load($matchid);
	if($mt->type != 'match') return false;
	if(isset($matchdata['events']['event']))
	{
		$localteam = node_load($mt->field_local_team[LANGUAGE_NONE][0]['target_id']);
	  	$visitorteam = node_load($mt->field_visitor_team[LANGUAGE_NONE][0]['target_id']);
	  	
		unset($mt->field_goals);
	  	unset($mt->field_cards);
	  	//unset($mt->field_stats);
	  	//unset($mt->field_lineup);
	  	//unset($mt->field_bench);
	  	//unset($mt->field_substitutions);
	  	node_save($mt);
	  	
	  	if(isset($matchdata['events']['event']['@attributes'])) $matchdata['events']['event'] = array($matchdata['events']['event']);
	  	
	  	foreach($matchdata['events']['event'] as $event)
	  	{
		  	$teamnid =  ($event['@attributes']['team']=='home' ? $localteam->nid : $visitorteam->nid );
			switch($event['@attributes']['type'])
			{
				case 'yellowcard':
				case 'redcard':
					$values = array(
					    'field_name' => 'field_cards',
					    'field_minute_str' => array(
					      LANGUAGE_NONE => array(array('value' => $event['@attributes']['minute'])),
					    ),
					    'field_color' => array(
					      LANGUAGE_NONE => array(array('value' => $event['@attributes']['type'])),
					    ),
					    'field_team' => array(
					      LANGUAGE_NONE => array(array('target_id' => $teamnid)),
					    ),
					    'field_player_name' => array(
					      LANGUAGE_NONE => array(array('value' => $event['@attributes']['player'])),
					    ),
					);
					$entity = entity_create('field_collection_item', $values);
					$entity->setHostEntity('node', $mt);
					$entity->save();
				break;
				case 'goal':
					$values = array(
					    'field_name' => 'field_goals',
					    'field_minute_str' => array(
					      LANGUAGE_NONE => array(array('value' => $event['@attributes']['minute'])),
					    ),
					    'field_team' => array(
					      LANGUAGE_NONE => array(array('target_id' => $teamnid)),
					    ),
					    'field_player_name' => array(
					      LANGUAGE_NONE => array(array('value' => $event['@attributes']['player'])),
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
