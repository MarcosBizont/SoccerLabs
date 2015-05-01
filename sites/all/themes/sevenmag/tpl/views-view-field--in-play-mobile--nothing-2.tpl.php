<?php
	$arroutput = explode('||',$output);
	
	$toprint = $arroutput[0];
	if(trim($arroutput[1])=='HT') 
	{
		print '<i class="fa fa-play" title="' . t('Game in progress') . '"></i><span class="tiplalbel">' . t('Game in progress') . '</span>';
	}
	if(is_numeric($arroutput[1])) 
	{
		print '<i class="fa fa-play" title="' . t('Game in progress') . '"></i><span class="tiplalbel">' . t('Game in progress') . '</span>';
	}
	
  
?>