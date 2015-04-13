<?php
	
	echo t($output) . (isset($row->field_field_team[0]['rendered']['#markup']) ? ' - ' . $row->field_field_team[0]['rendered']['#markup'] : '');	
    echo l('<i title="' . t('How this tip is generated') . ' " class="fa fa-question-circle"></i>', 'how-tip-generated', 
    			array('html' => TRUE, 'attributes' => array('target'=>'_blank', 'class' => array('tipsisgenerated-link'))));
?>