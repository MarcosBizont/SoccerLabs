<?php 
if(stripos($output, '/default_images/'))
{
	print '<a href="' . url('node/' . $row->field_field_local_team[0]['raw']['target_id']) . '" target="_blank">' . preg_replace("/<img[^>]+\>/i", _inplay_getdefaultimage($row->field_field_local_team[0]['raw']['target_id'],'team_thumbnail-copy'), $output) . '</a>';
}	
else
	print '<a href="' . url('node/' . $row->field_field_local_team[0]['raw']['target_id']) . '" target="_blank">' . $output . '</a>';



?>