<?php
if($view->args[0] == $row->field_field_local_team[0]['raw']['target_id'])
	print '<strong>' . $output . '</strong>';
else
	print $output;
?>