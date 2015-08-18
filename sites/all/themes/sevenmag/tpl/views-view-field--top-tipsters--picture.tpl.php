<?php
if(strlen(trim($output))==0)
{
	print _inplay_getdefaultimage($row->uid,'team_thumbnail-copy');
}	
else
	print $output; 
	

?>
