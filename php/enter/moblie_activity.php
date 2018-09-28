<?php
	define('IN_ECS', true);

	//返回值说明
	$return = array(
	    'payum_error'=> 'PayNum 错误',
	    'mode_error'=> 'Mode 错误',
	    'paygold_error'=> 'PayGold 错误',
	    'paytime_error'=> 'PayTime 错误',
	    'paynum_exist'=> 'PayNum 已经存在',
	    'username_error'=> 'UserName 错误',
	    'ticket_error'=> 'Ticket 错误',
	    'bad_request'=> '非法请求',
	    'servername_error'=> '服务器号错误',
	    'account_error'=> '该账户不存在',
	    'ok'=> '充值成功',
	    'failed'=> '充值失败',
	);

	//使用
	require(dirname(__FILE__) . '/includes/init.php');
	if(extension_loaded('memcache')){
		require(dirname(__FILE__) . '/class/mode_class.php');
	}else{
		require(dirname(__FILE__) . '/class/oldmode_class.php');
	}
	require(ROOT_PATH . '/class/mode_class.php');

	//访问的IP限定
	$allowIpArr = explode("|",$_CFG['allowIpArr']);
	//是否送点卷
	if(in_array($_SERVER['REMOTE_ADDR'],$allowIpArr)){
	    define('GIVE_GOLD3',TRUE); //送
	}else{
	    define('GIVE_GOLD3',FALSE); //不送
	}

	//$allowIpArr[] = gethostbyname($_SERVER['HTTP_HOST']);//本服务器的程序的IP地址
	if(!in_array($_SERVER['REMOTE_ADDR'],$allowIpArr)) exit('403');//无权限操作
	$data = $_GET;
	$data['modes_key'] = $_CFG['modes_key'];
	$data['power'] = $_CFG['power'];
	$pay = new gamePay($data);
	$pay->mob_act();

?>