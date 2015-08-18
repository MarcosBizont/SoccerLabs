<?php 
if(stripos($output, '/default_images/'))
{
	print _inplay_getdefaultimage($row->node_field_data_field_local_team_nid,'team_thumbnail-copy');
}	
else
	print $output; ?>