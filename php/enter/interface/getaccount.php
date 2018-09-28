<?php
define('IN_ECS', true);
require( '../includes/init.php');


$sql="select count(id) as count from ".TBL_ACCOUNT."";
$rs=$db->getRow($sql);
echo $rs['count'];
unset($rs);
echo ",";

$sql="select count(id) as count from ".TBL_WB_USER."";

$rs=$wangpu_db->getRow($sql);

echo $rs['count'];


?>