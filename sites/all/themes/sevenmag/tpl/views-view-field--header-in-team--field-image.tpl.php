<?php 
	
if(stripos($output, '/default_images/'))
{
	print _inplay_getdefaultimage($row->nid,'medium');
}	
else
	print $output; ?>