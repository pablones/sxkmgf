<?php
/**
 * @author King.L
 * 全平台充值统计查询
 */
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');
require("./includes/all_server_num.php");

if($_REQUEST['act'] == 'list'){
	//暂时只统计4399
	admin_priv('all_statics');
	//没有查看其他平台的权限
	if (strpos(',' . $_SESSION['action_list'] . ',', ',view_all_server,') === false){
		$server_num = $arr_s[$_CFG['s_name']];
		$i = 1;	
		while($i <= $server_num){
			$str = "http://".$prevpages[$_CFG['s_name']]['prev'].$i."-".$prevpages[$_CFG['s_name']]['last']."/interface/all_statices.php?sid=".$i;
			$url[] = $str;
			$i++;
		}
		foreach($url as $key => $value){
			$result = gethttpcnt($value);
			$data[] = unserialize($result);
		}
		foreach ($data as $k => $v){
			if(!$v){
				$err_data[] = $k+1;
			}
		}
		
	}else{
		
	}

	$smarty -> assign('err' , $err_data);
	$smarty -> assign('data' ,$data);
	$smarty -> display('as_list.htm');
}

?>