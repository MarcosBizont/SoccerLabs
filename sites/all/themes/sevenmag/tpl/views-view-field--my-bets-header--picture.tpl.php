<?php
if(strlen(trim($output))==0)
{
	print _inplay_getdefaultimage($row->uid,'medium_circle');
}	
else
	print $output; 
	

?>
