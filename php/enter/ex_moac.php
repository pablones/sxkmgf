<?php
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');
	
if(extension_loaded('memcache')){
	require(dirname(__FILE__) . '/class/mode_class.php');
}else{
	require(dirname(__FILE__) . '/class/oldmode_class.php');
}
if($_REQUEST['act'] =='' ){
	$smarty -> display('ex_moac.dwt');
}
if($_REQUEST['act'] == 'query'){
	
	$data['PayNum'] = $_POST['PayNum'];
	$data['Mode'] = $_POST['Mode'];
	$data['PayToUser'] = $_POST['PayToUser'];
	$data['PayMoney'] = $_POST['PayMoney'];
	$data['PayTime'] = $_POST['PayTime'];
	$data['ServerName'] = $_POST['ServerName'];
	$data['modes_key'] = $_CFG['modes_key']; 	
	$data['allowIpArr'] = $_CFG['allowIpArr'];
	$data['actcode'] = $_POST['actcode'];
	//print_r($data); echo "<br/>";
	$md5_str = $data['modes_key'].$data['mode'].$data['PayNum'].$data['PayToUser'].$data['PayMoney'].$data['PayTime'].$data['ServerName'];
	$data['ticket'] = md5($md5_str);

	//$md5_str = $this->key.$this->Mode.$this->PayNum.$this->PayToUser.$this->PayMoney.$this->PayTime.$this->ServerName;
	$pay = new gamePay($data);
	$pay -> mob_act();
	$smarty -> assign("success" , 1);
	$smarty -> display('ex_moac.dwt');
}

?>