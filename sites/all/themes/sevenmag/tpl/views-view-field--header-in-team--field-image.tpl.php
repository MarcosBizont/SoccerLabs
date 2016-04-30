<?php 
	
if(stripos($output, '/default_images/'))
{
	$output = _inplay_getdefaultimage($row->nid,'medium');
}
?>
	
	
<?php print str_replace('<img','<img itemprop="image"',$output); ?>