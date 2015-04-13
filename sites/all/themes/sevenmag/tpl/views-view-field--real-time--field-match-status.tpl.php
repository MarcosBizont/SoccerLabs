<?php
$arroutput = explode('||',$output);

$toprint = $arroutput[0];

if($arroutput[1]=='FT') $toprint = $arroutput[1];
if($arroutput[1]=='HT') $toprint = $arroutput[1];
if(is_numeric($arroutput[1])) $toprint = $arroutput[1]."'";


echo $toprint;