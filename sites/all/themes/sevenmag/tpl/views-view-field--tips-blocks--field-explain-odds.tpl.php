<?php
	if(!inplay_is_anonymus())
	{
		$termcurrent = $row->_field_data['taxonomy_term_data_field_data_field_tip_type_tid']['entity'];
		echo check_markup(token_replace(t($output), array('term' => $termcurrent)), 'full_html',FALSE);
	}
	else
	{
		echo t('Accuracy: <span class="stand">??? %</span><br> Odds suggested: <span class="stand">???</span>')
			. '<br/>' . t('<span class="views-label socialmatchlinks"><a href="/user/simple-fb-connect">Connect with facebook</a> or <a href="/twitter/redirect">connect with twitter</a> to get the full information</span>');
	}
?>