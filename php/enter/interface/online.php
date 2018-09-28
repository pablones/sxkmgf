<?php
define('IN_ECS', true);
require( '../includes/init.php');
require( '../languages/zh_cn/interface/online.php');

$arr_sites=array(1=>$_LANG['one'],2=>$_LANG['two'],3=>$_LANG['three'],4=>$_LANG['four'],5=>$_LANG['five'],6=>$_LANG['six'],);
	
foreach($arr_sites as $site){
    $sql="select count_online from ".TBL_ACC_LINELOG." where line='$site' order by time desc limit 1";
	$row=$GLOBALS['acc_db'] -> getRow($sql);
	$nums[]=$row['count_online'];
}

echo array_sum($nums);

?>