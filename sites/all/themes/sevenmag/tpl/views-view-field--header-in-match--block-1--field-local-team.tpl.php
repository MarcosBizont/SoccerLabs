<?php 
//$row->nid

if(stripos($output, '/default_images/'))
{
	print preg_replace("/<img[^>]+\>/i", _inplay_getdefaultimage($row->field_field_local_team[0]['raw']['target_id'],'medium_circle'), $output);
}	
else
	print $output;


if(simbet_bet_isvalid_pre($row->nid))
{
module_load_include('inc', 'tips', 'tips_tipfin_odds_library');
$odds = reset(tips_tipfin_odds_library_check_odds_for_match($row->nid, array(832))); 
if(isset($odds[1]))
{
$odds = reset($odds[1]);

?>
<br><a role="button" class='colorbox-inline betnowmatch symple-button green' href="?width=500&height=200&inline=true#block-block-27"><span class="symple-button-inner" style="border-radius:4px"><?php echo t('Win :money now', array(':money' => '$'  . ($odds*100) )); ?></span></a><br><span class="betnowmatchdescription"><?php echo t('If you bet $100 you could win :money<br>in case this team win', array(':money' => '$'  . ($odds*100) )); ?></span>
<?php }} ?>