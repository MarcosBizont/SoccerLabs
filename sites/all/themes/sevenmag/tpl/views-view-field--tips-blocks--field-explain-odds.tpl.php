<?php
	$termcurrent = $row->_field_data['taxonomy_term_data_field_data_field_tip_type_tid']['entity'];
	echo check_markup(token_replace(t($output), array('term' => $termcurrent)), 'full_html',FALSE);
?>