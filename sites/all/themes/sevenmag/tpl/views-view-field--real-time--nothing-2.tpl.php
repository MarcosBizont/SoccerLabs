<?php
  $arroutput = explode('||',$output);

  $toprint = $arroutput[0];

  if(trim($arroutput[1])=='FT') 
  	print '<i class="fa fa-clock-o" title="' . t('Game finished') . '"></i>';
  elseif(trim($arroutput[1])=='HT') 
  	print '<i class="fa fa-play" title="' . t('Game in progress') . '"></i>';
  elseif(is_numeric($arroutput[1])) 
  	print '<i class="fa fa-play" title="' . t('Game in progress') . '"></i>';
  //if($arroutput[1]=='HT') $toprint = $arroutput[1];
  //if(is_numeric($arroutput[1])) $toprint = $arroutput[1];
  
  //print '<i class="fa fa-play" title="' . t('Game in progress') . '"></i>';
  //print $output;
?>