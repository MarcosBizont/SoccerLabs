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
 // commentaries_inplaytips_have_stats($node)
// Social Changes : Todos pueden apostar mientras esten registrados
/* if(tips_match_have_tip_withoffers($row->entity,$accuracy))
 { 
	$class = $accuracy >= .5 ? 'green' : '';
 	print '<i class="fa fa-bolt ' . $class . '" title="' . t('Tip with booker\'s offers available') . '"></i>';
 }
 else*/
 if(tips_match_have_tip($row->entity,$accuracy))
 { 
 	$class = $accuracy >= .5 ? 'green' : '';
 	print '<i class="fa fa-lightbulb-o ' . $class . '" title="' . t('Tip available') .  '"></i>';
 }
 else
 	print '<div class="clearfix">&nbsp;</div>';
?>
