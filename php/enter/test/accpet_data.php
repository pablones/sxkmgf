<?php
/**
 * 接受由push_data push过来的数据
 * @author King.L
 */
define('IN_ECS', true);
require('../includes/init.php');
require (ROOT_PATH . ADMIN_PATH ."/includes/all_server_num.php");

if($_REQUEST['act'] == accept){
	$admin_action = $_REQUEST['admin_action'];
	$sys_config = $_REQUEST['sys_config'];
	$num = $_REQUEST['num']; //服务器数量
	$pushid = $_REQUEST['pushid'];//推送过来的服务器id
	
	$sql = "TRUNCATE TABLE `".TBL_ADMIN_ACTION."`;";
	$sql .="TRUNCATE TABLE `".TBL_SYS_CONFIG."`";
	$acc_db -> query($sql);

	$sql = "insert into ".TBL_ADMIN_ACTION."(`action_id`,`parent_id`,`action_code`,`relevance`)values";
	if(isset($admin_action)){
		$admin_action = unserialize($admin_action);
		foreach($admin_action as $key => $value){
			$sql .= "('".$value['action']."','".$value['parent_id']."','".$value['action_code']."','".$value['relevance']."')";			
		}
	}
	$acc_db -> query($sql);
	
	$sql = "insert into ".TBL_SYS_CONFIG." (`id`,`parent_id`,`code`,`type`,`store_range`,`store_dir`,`value`,`sort_order`) values";
	if(isset($sys_config)){
		$sys_config = unserialize($sys_config);
		foreach($sys_config as $k => $v){
			if($v['code'] == 'fuwuqi_id'){
				$v['value'] = 'S'+$pushid+2;
				$sql .= "('".$v['id']."','".$v['parent_id']."','".$v['code']."','".$v['type']."','".$v['store_range']."','".$v['value']."','".$v['sort_order']."')";
			}else{
				$sql .= "('".$v['id']."','".$v['parent_id']."','".$v['code']."','".$v['type']."','".$v['store_range']."','".$v['value']."','".$v['sort_order']."')";
			}
						
		}
	}
	$acc_db -> query($sql);
	$hefu_id = isset($_REQUEST['hefu_id'])?$_REQUEST['hefu_id']:$hefu[$_CFG['s_name']];//合服数量
	
	/* 计算下个服务器id */
	if($pushid <= $hefu_id){
		$sid = $pushid + 2;
	}else{
		$sid = $pushid + 1;
	}
	if($sid >= $num){
		//递归推送
		$url = "http://yjjh".$sid."-s.my4399.com/4399/test/accpet_data.php?act=accept&admin_action=".$admin_action."&sys_config=".$sys_config."&num=".$num."&pushid=".$sid."&hefu_id=".$hefu_id;
		$result = gethttpcnt($url);
	}
	if($result){
		print_r($result);
	}else{
		echo "error";
	}
	
	
	
	exit;
}


?>