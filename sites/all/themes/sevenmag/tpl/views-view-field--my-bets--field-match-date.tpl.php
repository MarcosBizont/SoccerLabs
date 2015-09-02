<?php
$arroutput = explode('||',$output);

$toprint = t('Match date') . ' : ' . $arroutput[0];

if(trim($arroutput[1])=='FT') $toprint .= '<br/>' . t('Game finished');
if(trim($arroutput[1])=='HT') $toprint = t('Game in progress') . ' : ' . $arroutput[1];
if(is_numeric($arroutput[1])) $toprint = t('Game in progress') . ' : ' . $arroutput[1]."'";


echo $toprint;