<?php
	
	print $output;
	
	if(tips_match_have_tip_withoffers($row->entity)) 
	 	print '<i class="fa fa-bolt" title="' . t('Tip with booker\'s offers available') . '"></i>';
	 elseif(tips_match_have_tip($row->entity)) 
	 	print '<i class="fa fa-lightbulb-o" title="' . t('Tip available') . '"></i>';
	