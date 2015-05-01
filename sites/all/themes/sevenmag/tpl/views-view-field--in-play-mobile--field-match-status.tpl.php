<?php
	$arroutput = explode('||',$output);
	
	$toprint = $arroutput[0];
	if(trim($arroutput[1])=='HT') 
	{
		$toprint = $arroutput[1] . '<i class="fa fa-play" title="' . t('Game in progress') . '"></i>';
	}
	if(is_numeric($arroutput[1])) 
	{
		$toprint = $arroutput[1]."'" . '<i class="fa fa-play" title="' . t('Game in progress') . '"></i>';
	}

	print $toprint;
	
	
	
	 module_load_include('inc', 'commentaries', 'commentaries_strategies');
	 $nodegame = node_load($row->entity);
	/*if(commentaries_inplaytips_have_stats($nodegame))  
		print '<i class="fa fa-list" title="' . t('In play stats available') . '"></i>';
	*/
	
	if(tips_match_have_tip_withoffers($row->entity)) 
	 	print '<i class="fa fa-bolt" title="' . t('Tip with booker\'s offers available') . '"></i>';
	 elseif(tips_match_have_tip($row->entity)) 
	 	print '<i class="fa fa-bell" title="' . t('Tip available') . '"></i>';