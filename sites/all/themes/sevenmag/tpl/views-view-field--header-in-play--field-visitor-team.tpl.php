<?php 
if(stripos($output, '/default_images/'))
{
	print preg_replace("/<img[^>]+\>/i", _inplay_getdefaultimage($row->field_field_visitor_team[0]['raw']['target_id'],'team_thumbnail-copy'), $output);
}	
else
	print $output;



?>