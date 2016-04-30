<?php

require 'autoload.php';

$ml = new MonkeyLearn\Client('6f783f4985a739f54c028b1e2bb975f8322ad38a');

$text_list = ["oigan saben a que hora juegan las rayas?"];

echo("Texto usado: ".$text_list[0]."<br/>");

//Este módulo clasifica el texto en pregunta, saludo o agradecimiento
$module_id = 'cl_hqNxVmTU';
$res = $ml->classifiers->classify($module_id, $text_list, true);


$tipo = $res->result[0][0]['label'];

echo("Tipo: ".$tipo."(P = ".$res->result[0][0]['probability'].")<br/>");

if ($tipo == "previous" || $tipo == "upcoming")
{
  //Este otro lo clasifica en el equipo del cual está hablando
  $module_id = 'cl_sbv6FQvb';
  $res = $ml->classifiers->classify($module_id, $text_list, true);

  echo("Equipo: ".$res->result[0][0]['label']."(P = ".$res->result[0][0]['probability'].")<br/>");
}
?>
