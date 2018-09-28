<?php
define('IN_ECS', true);
require('../includes/init.php');

$sql = "select * from ".TBL_WB_EUDEMONTYPE." where name like '%火麒麟%'";
$result = $wangpu_db -> getRow($sql);

$sql = "select * from ".TBL_WB_EUDEMON." where typeid = '".$result['Id']."'";
$result = $wangpu_db -> getAll($sql);
print_r($result); echo "<br/><br/>";
print_r(count($result)); echo "<br/><br/>";

$sql = "select * from ".TBL_WB_USER." where id='".$result[0]['owner_id']."'";
$player = $wangpu_db -> getAll($sql);
print_r($player); echo "<br/><br/>";
$sql = "select * from ".TBL_YJJHPT_ADDITEMLOG." where admin = 'admin'";
$result = $yjjhpt_db -> getAll($sql);
print_r($result); echo "<br/>";



?>