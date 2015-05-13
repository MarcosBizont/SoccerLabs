<?php
	module_load_include('inc', 'tips', 'tips_buffer');
	global $language;
	global $user;
	
	//$query = array('utm_source' => 'u_' . $user->uid, 'utm_medium' => 'twitter', 'utm_campaign' => 'share',  
	//				'tipid' => $row->nid );
	
	//$tweet = tips_get_tweet_tip($row->nid, $language->language, true,$urltoshare, $urltosharefull, false, '',$query);
	$tip = node_load($row->nid);
	$match = node_load($tip->field_match[LANGUAGE_NONE][0]['target_id']);
	
	$url = url('node/' . $match->nid, 
				array('base_url' => 'https://soccer-labs.com/' . $language->language, 
						'absolute' => true,
						'query' => array('utm_source' => 'u_' . $user->uid,
											'utm_medium' => 'twitter',
											'utm_campaign' => 'tips_share' ),
						'language' => (object)array(
							  'language' => $language->language,
							)
						));
						
						
	
	$oldtexts = array();
	if($language->language=='es')
		$tweet = getMsgTextES($match,$oldtexts,true);
	else
		$tweet = getMsgTextEN($match,$oldtexts,true);
    
	
	echo t($output) . (isset($row->field_field_team[0]['rendered']['#markup']) ? ' - ' . $row->field_field_team[0]['rendered']['#markup'] : '');	
    echo l('<i title="' . t('How this tip is generated') . ' " class="fa fa-question-circle"></i>', 'how-tip-generated', 
    			array('html' => TRUE, 'attributes' => array('target'=>'_blank', 'class' => array('tipsisgenerated-link'))));
    			
    			
    if(strtoupper($row->field_field_win[0]['rendered']['#markup']) == 'WIN' )
    	echo '<i title="' . t('Tip succesful ') . ' " class="fa fa-check-circle "></i>';
    
    if(strtoupper($row->field_field_win[0]['rendered']['#markup']) == 'LOST' )
    	echo '<i title="' . t('Tip unsuccessful ') . ' " class="fa fa-times-circle "></i>';
    
/*


*/

?>

<a href="https://twitter.com/share?via=<?php echo t('soccer_labsen'); ?>&amp;url=<?php echo twitter_shorten_url($url) ?>&amp;text=<?php echo $tweet ?>" rel="external" class="twitter" target="_blank"><i title="<?php echo t('Share via twitter ') ?>" class="fa fa-twitter "></i></a>


<a href="https://www.facebook.com/sharer/sharer.php?u=<?php echo $url ?>" rel="external" class="twitter" target="_blank"><i title="<?php echo t('Share via facebook ') ?>" class="fa fa-facebook "></i></a>