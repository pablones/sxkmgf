<?php
/**
 * 
 * 防沉迷应急工具
 * @author King.L
 */
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

if($_REQUEST['act'] == 'list'){
	$smarty -> display('fcm_tool.htm');
}
if($_REQUEST['act'] == 'fcm_query'){
	$account = $_POST['account'];
	$links[] = array('text' => $_LANG['back_list'], 'href'=>'fcm_tool.php?act=list');
	foreach($account as $key => $value){
		$sql = "select `fcm` from ".TBL_ACCOUNT." where `name` = '".$value."'";
		$fcm_info = $acc_db -> getOne($sql);
		if(!$fcm_info){
			$sql = "update ".TBL_ACCOUNT." set `fcm` = 1 where `name`='".$value."'";
			$acc_db -> query($sql);
		}
	}
	sys_msg($_LANG['success'],0,$links);
}
?>