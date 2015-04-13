<?php
$status = $row->field_field_match_date[0]['rendered']['#markup'];

?><span class="time"><?php print $status; ?></span>
<div class='clearfix'></div>
<span class="score"><?php print $row->field_field_local_team_score[0]['rendered']['#markup']; ?><i class="fa fa-times"></i><?php print $row->field_field_visitor_team_score[0]['rendered']['#markup']; ?></span>
