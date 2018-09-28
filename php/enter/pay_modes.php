<?php

/*
  ◆ 充值接口 ◆
  请求方式:HTTP GET 请求
  数据库: [acc_db] [wangpudb]
  涉及表: acc_db.ser_supplyvaslog_table,
          acc_db.sk_vas_table
          acc_db.sk_paylog,
          wangpudb.wb_user
*/

define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

if(extension_loaded('memcache')){
	require(dirname(__FILE__) . '/class/mode_class.php');
}else{
	require(dirname(__FILE__) . '/class/oldmode_class.php');
}

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


$data = $_GET;
$data['allowIpArr'] = $_CFG['allowIpArr'];
$data['modes_key'] = $_CFG['modes_key'];
$data['power'] = $_CFG['power'];
$pay = new gamePay($data);
$pay -> YuanBaoPay();

?>