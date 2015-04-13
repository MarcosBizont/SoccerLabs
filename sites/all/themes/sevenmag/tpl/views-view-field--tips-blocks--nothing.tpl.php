<?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */
module_load_include('inc', 'tips', 'tips_tipfin_odds_library');
$termcurrent = $row->_field_data['taxonomy_term_data_field_data_field_tip_type_tid']['entity'];
$tip = node_load($row->nid);
$obj = tips_tipfin_odds_library_check_odds_for_tip($tip);

//

foreach($obj as $value) 
{
	if($value['odd'] >= $termcurrent->field_odds_suggested['und'][0]['value'])
		echo '<span class="stand">' . $value['bookmakername'] . " : " . $value['odd'] . '</span> | ';
	else
		echo $value['bookmakername'] . " : " . $value['odd'] . ' | ';
}

?>