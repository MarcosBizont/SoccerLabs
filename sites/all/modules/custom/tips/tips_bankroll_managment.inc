<?php
	
	
function tips_bankroll_managment_checkbankrolls()
{
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'bankroll')
		  ->addMetaData('account', user_load(1));
	
	$result = $query->execute();
	
	if (isset($result['node'])) 
	{
		$bankids = array_keys($result['node']);
		foreach($bankids as $idbank)
		{
			$bank = node_load($idbank);
			tips_bankroll_managment_bankroll_close_bets($bank);
			tips_bankroll_managment_bankroll_makenew_bets($bank);
			
			//$match = node_load($tip->field_match[LANGUAGE_NONE][0]['target_id']);
		}
	}
	
}


function tips_bankroll_managment_save_betstake($idcollection,$field,$data)
{
	$obj = entity_load_single('field_collection_item', intval($idcollection));
	$obj = entity_metadata_wrapper('field_collection_item', $obj);
	$obj->{$field}->set($data);
    $obj->save(true);
}

function tips_bankroll_managment_bankroll_close_bets($bank)
{
	if(!isset($bank->field_bets_with_stake[LANGUAGE_NONE])) return false;
	foreach($bank->field_bets_with_stake[LANGUAGE_NONE] as $betstake)
	{
		$obj = reset(entity_load('field_collection_item', array($betstake['value'])));
		
		if($obj->field_balance[LANGUAGE_NONE][0]['value']==0)
		{
			$bet = node_load($obj->field_bet[LANGUAGE_NONE][0]['target_id']);
			$tip = node_load($bet->field_tip[LANGUAGE_NONE][0]['target_id']);
			if($tip->field_win[LANGUAGE_NONE][0]['value']==1)
			{
				$stake = doubleval($obj->field_stake[LANGUAGE_NONE][0]['value']);
				$betodds = $bet->field_bet_odds[LANGUAGE_NONE][0]['value'];
				if($obj->field_bet_odds[LANGUAGE_NONE][0]['value'] > 0) $betodds = $obj->field_bet_odds[LANGUAGE_NONE][0]['value'];
				$balance = ($stake * $betodds) - $stake;
				
				tips_bankroll_managment_save_betstake($betstake['value'],'field_balance',$balance);
				
				$bank->field_bank[LANGUAGE_NONE][0]['value'] = doubleval($bank->field_bank[LANGUAGE_NONE][0]['value']) + $balance;
				node_save($bank);
			}
			elseif($tip->field_win[LANGUAGE_NONE][0]['value']==2)
			{
				$stake = doubleval($obj->field_stake[LANGUAGE_NONE][0]['value']);
				$balance = -$stake;
				
				tips_bankroll_managment_save_betstake($betstake['value'],'field_balance',$balance);
				
				$bank->field_bank[LANGUAGE_NONE][0]['value'] = doubleval($bank->field_bank[LANGUAGE_NONE][0]['value']) + $balance;
				
				if($bank->field_static_stake[LANGUAGE_NONE][0]['value'] != 1)
					$bank->field_deficit[LANGUAGE_NONE][] = array('value' => $balance);
				node_save($bank);	
			}
		}
	}	
	
}

function tips_bankroll_managment_bankroll_isbetinbank($betnid, $bank)
{
	if(!isset($bank->field_bets_with_stake[LANGUAGE_NONE])) return false;
	foreach($bank->field_bets_with_stake[LANGUAGE_NONE] as $betstake)
	{
		$obj = reset(entity_load('field_collection_item', array($betstake['value'])));
		if($obj->field_bet[LANGUAGE_NONE][0]['target_id']==$betnid) return true;
	}	
	return false;
}

function tips_bankroll_managment_bankroll_makenew_bets($bank)
{
	if(isset($bank->field_manual_bankroll[LANGUAGE_NONE][0]['value']))
		if($bank->field_manual_bankroll[LANGUAGE_NONE][0]['value'] == 1) return false;	
	
	$betids = array();
	foreach($bank->field_tip_type[LANGUAGE_NONE] as $value) $betids[] = $value['target_id'];
	
	$from = new DateTime('now',new DateTimeZone('UTC'));
	
	$to = new DateTime('now',new DateTimeZone('UTC'));
	$to = $to->modify('+2 hours');
	
	$query = db_select('node', 'n');
	$query->join('field_data_field_win', 'a', 'n.nid = a.entity_id'); 
	$query->join('field_data_field_tip_type', 'b', 'n.nid = b.entity_id');
	$query->join('field_data_field_tip', 'c', 'n.nid = c.field_tip_target_id');
	$query->join('field_data_field_stake', 'd', 'c.entity_id = d.entity_id');
	$query->join('field_data_field_match_date', 'e', 'a.entity_id = e.entity_id');
	
	
	$query->fields('d',array('entity_id'))
		->condition('a.bundle', 'tip')->condition('b.bundle', 'tip')->condition('c.bundle', 'bet')->condition('d.bundle', 'bet')->condition('e.bundle', 'tip')
		->condition('a.field_win_value', 0)->condition('b.field_tip_type_target_id', $betids, 'IN')
		->condition('e.field_match_date_value', $to->format('Y-m-d H:i:s'), '<')->condition('e.field_match_date_value', $from->format('Y-m-d H:i:s'), '>=');
	
	
	$result = $query->execute();
	while($record = $result->fetchAssoc())
	{
		$bet = node_load($record['entity_id']);
		if(!tips_bankroll_managment_bankroll_isbetinbank($bet->nid, $bank))
		{
			$stake = doubleval($bank->field_bet_percentage[LANGUAGE_NONE][0]['value']) * doubleval($bank->field_bank[LANGUAGE_NONE][0]['value']);
			if(isset($bank->field_deficit[LANGUAGE_NONE][0]['value']))
			{
				$lastloss = abs($bank->field_deficit[LANGUAGE_NONE][0]['value']) * $bet->field_bet_odds[LANGUAGE_NONE][0]['value'];
				$stake = ( $lastloss / $bet->field_bet_odds[LANGUAGE_NONE][0]['value'] ) / 
						( ( $bet->field_bet_odds[LANGUAGE_NONE][0]['value'] - 1 ) / $bet->field_bet_odds[LANGUAGE_NONE][0]['value'] );	
				unset($bank->field_deficit[LANGUAGE_NONE][0]);
				node_save($bank);
				$bank = node_load($bank->nid);
			}
			$values = array(
			    'field_name' => 'field_bets_with_stake',
			    'field_bet' => array(
			      LANGUAGE_NONE => array(array('target_id' => $bet->nid)),
			    ),
			    'field_bookmaker' => array(
			      LANGUAGE_NONE => array(array('target_id' => $bet->field_bookmaker['und'][0]['target_id'])),
			    ),
			    'field_stake' => array(
			      LANGUAGE_NONE => array(array('value' => $stake)),
			    ),
			    'field_bet_odds' => array(
			      LANGUAGE_NONE => array(array('value' => $bet->field_bet_odds['und'][0]['value'])),
			    ),
			    'field_balance' => array(
			      LANGUAGE_NONE => array(array('value' => 0)),
			    ),
			);
			$entity = entity_create('field_collection_item', $values);
			$entity->setHostEntity('node', $bank);
			$entity->save();
		}
	}
	
	//dpm($bank);
	
	//$tip->field_match[LANGUAGE_NONE][0]['target_id']	
}
/*
	
	module_load_include('inc', 'tips', 'tips_bankroll_managment');
	tips_bankroll_managment_checkbankrolls();

*/