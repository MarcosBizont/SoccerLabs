<?php 
	//dpm($row);
	// $row->_entity_properties['entity object']->field_match['und'][0]['target_id']
	// print $output; 
	module_load_include('inc', 'tips', 'tips_tipfin_odds_library');
	$termcurrent = taxonomy_term_load($row->_entity_properties['entity object']->field_tip_type['und'][0]['target_id']);
	$tip = node_load($row->entity);
	$obj = tips_tipfin_odds_library_check_odds_for_tip($tip);
	
	simbet_format_list_offers($obj, $termcurrent->field_odds_suggested['und'][0]['value'], $tip);	
?>