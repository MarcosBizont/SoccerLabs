<?php
	//dpm($row);
	echo t($output) . (isset($row->field_field_team[0]['rendered']['#markup']) ? ' - ' . $row->field_field_team[0]['rendered']['#markup'] : '');	
    echo l('<i title="' . t('How this tip is generated') . ' " class="fa fa-question-circle"></i>', 'how-tip-generated', 
    			array('html' => TRUE, 'attributes' => array('target'=>'_blank', 'class' => array('tipsisgenerated-link'))));
    			
    			
    if(strtoupper($row->field_field_win[0]['rendered']['#markup']) == 'WIN' )
    	echo '<i title="' . t('Tip succesful ') . ' " class="fa fa-check-circle "></i>';
    
    if(strtoupper($row->field_field_win[0]['rendered']['#markup']) == 'LOST' )
    	echo '<i title="' . t('Tip unsuccessful ') . ' " class="fa fa-times-circle "></i>';
    
?>