<?php
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');


if(extension_loaded('memcache')){
	require(dirname(__FILE__) . '/class/mode_class.php');
}else{
	require(dirname(__FILE__) . '/class/oldmode_class.php');
}

if($_REQUEST['act'] == ''){	
	$smarty -> display('paymodes.dwt');
}
if($_REQUEST['act'] == 'query'){
	$data = $_POST;
	
	$data['allowIpArr'] = $_CFG['allowIpArr'];
	$data['modes_key'] = $_CFG['modes_key'];
	$data['power'] = $_CFG['power'];
	$data['PayNum'] = 'se'.$data['PayNum'];
	$data['ticket'] = $data['modes_key'].$data['PayNum'].$data['PayToUser'].$data['PayMoney'].$data['PayGold'].$data['PayTime'].$data['ServerName'];
	$data['ticket'] = md5($data['ticket']);
	$pay = new gamePay($data);
	$pay -> YuanBaoPay();	
	
}


?>