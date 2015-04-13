<?php
if($view->args[1] == $row->field_field_visitor_team[0]['raw']['target_id'])
	print '<strong>' . $output . '</strong>';
else
	print $output;
?>