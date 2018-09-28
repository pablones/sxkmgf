<?php
/**
 * 对admin_action表的操作
 */
define('IN_ECS', true);
require('../includes/init.php');

//查询
if($_REQUEST['act'] == 'select'){
	$sql = "select * from ".TBL_ADMIN_ACTION."";
	$result = $acc_db -> getAll($sql);
	print_r($result);
}
if($_REQUEST['act'] == 'create'){
	$sql = "delete from ".TBL_ADMIN_ACTION." where `action_code` = '05_tool_manage'";
	$acc_db -> query($sql);
	$sql = "insert into ".TBL_ADMIN_ACTION." (";
	$sql .= "`parent_id`,`action_code`)";
	$sql .= "values";
	$sql .="('0','05_tool_manage')";
	$acc_db -> query($sql);
	$sql = "select * from ".TBL_ADMIN_ACTION." where `action_code` = '05_tool_manage'";
	$result = $acc_db -> getAll($sql);
	

		

	$str =  "insert into `admin_action`(";
	$str .= "`parent_id`,`action_code`)";
	$str .= " values";

	$sql_1 = "('".$result['action_id']."','fcm_tool')";
	$sql_2 = "('".$result['action_id']."','edit_pwd')";
	$sql_3 = "('".$result['action_id']."','tool_newuser')";
	
	$sql_1 = $str . $sql_1;
	$sql_2 = $str . $sql_2;
	$sql_3 = $str . $sql_3;
	$acc_db -> query($sql_1);
	$acc_db -> query($sql_2);
	$acc_db -> query($sql_3);
	
	$sql = "select * from ".TBL_ADMIN_ACTION." where action_code = '02_resource'";
	$res = $acc_db -> getRow($sql);
	print_r($res);
	$sql = "insert into ".TBL_ADMIN_ACTION." (";
	$sql .= "parent_id,action_code)";
	$sql .= "values";
	$sql .= "('".$res['action_id']."','mkdk_card')";
	print_r($sql);
	$acc_db -> query($sql);
	print_r("success");
}
if($_REQUEST['act'] == 'insert'){
	$sql = "select * from ".TBL_ADMIN_ACTION." where `action_code` = '05_tool_manage'";
	$p_id = $acc_db -> getRow($sql);
	$sql = "insert into ".TBL_ADMIN_ACTION."(`parent_id`,`action_code`)values('".$p_id['action_id']."','tool_newuser')";
	$acc_db->query($sql);
	$sql = "insert into ".TBL_ADMIN_ACTION."(`parent_id`,`action_code`)values('".$p_id['action_id']."','edit_pwd')";
	$acc_db->query($sql);
	$sql = "insert into ".TBL_ADMIN_ACTION."(`parent_id`,`action_code`)values('".$p_id['action_id']."','fcm_tool')";
	$acc_db -> query($sql);
	
	$sql = "select * from ".TBL_ADMIN_ACTION." where `action_code` = '02_resource'";
	$p_id = $acc_db -> getRow($sql);	
	$sql = "insert into ".TBL_ADMIN_ACTION."(`parent_id`,`action_code`)values('".$p_id['action_id']."','mkdk_card')";
	$acc_db -> query($sql);
	
	print_r("success");
}
if($_REQUEST['act'] == 'delete'){
	$sql ="delete from ".TBL_ADMIN_ACTION." where `action_code` = 'fcm_tool'";
	$acc_db -> query($sql);
	$sql ="delete from ".TBL_ADMIN_ACTION." where `action_code` = 'edit_pwd'";
	$acc_db -> query($sql);
	$sql ="delete from ".TBL_ADMIN_ACTION." where `action_code` = 'tool_newuser'";
	$acc_db -> query($sql);
	$sql = "DELETE FROM ".TBL_ADMIN_ACTION." WHERE `action_code` = 'ex'";
	$acc_db -> query($sql);
	$sql = "delete from ".TBL_ADMIN_ACTION." where `action_code` = 'exxx'";
	$acc_db -> query($sql);
	print_r("success");
}
?>