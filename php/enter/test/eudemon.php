<?php
define('IN_ECS', true);
require('../includes/init.php');

$sql = "select * from ".TBL_ACCOUNT." where name = 'king'";
$user_info = $acc_db -> getRow($sql);
print_r($user_info); echo "<br/>";

$sql = "select * from ".TBL_WB_USER." where account_id = '".$user_info['id']."'";
$player_info = $wangpu_db -> getRow($sql);
print_r($player_info); echo "<br/>";

//$sql = "select * from ".TBL_WB_EUDEMONTYPE." where name = '蝎子'";
//$eudmntype_info = $wangpu_db -> getRow($sql);
//print_r($eudmntype_info);echo "<br/>";
print_r($player_info['id']);
$sql = "INSERT INTO `wb_eudemon` VALUES (3000580412,'".$player_info['id']."',100,0,'蝎子大王',5873,35,450,0,0,80,0,4,3,9998,0,0,6,0,411,34,158,878,34,580,748,34,544,748,34,378,741,34,563,0,0,57035,0,0,0,0,0,0,0,191,'0','0');";
$wangpu_db -> query($sql);
print_r("ffff");
//$sql = "insert into ".TBL_WB_EUDEMON." set(`owner_id`,`typeid`,`itemid`,`name`,)";
//$sql = "select * from ".TBL_WB_EUDEMON." where owner_id = '".$player_info['id']."'";
//$eudemon_info = $wangpu_db -> getRow($sql);
//print_r($eudemon_info); echo "<br/>";
?>