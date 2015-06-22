<?php 
if(stripos($output, '/default_images/'))
{
	print _inplay_getdefaultimage($row->_entity_properties['field_visitor_team:entity object']->nid);
}	
else
	print $output; ?>