<?php
/**
 * 珠宝充值接口入口
 * @author King.L
 */

define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');
/*载入支付类*/
require_once(ROOT_PATH . 'class/mode_class.php');
/* 载入语言文件 */
require_once(ROOT_PATH . 'languages/' .$_CFG['lang']. '/user.php');

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



//访问的IP限定
//是否送点卷
//TODO $allowIpArrIn 有问题
if(in_array($_SG['ip'],$allowIpArrIn)){
    define('GIVE_GOLD3',TRUE); //送
}else{
    define('GIVE_GOLD3',FALSE); //不送
}
if(!in_array($_SG['ip'],$allowIpArrIn)) exit('403');//无权限操作

$path='../data/ptlog/zblog.php';


$pay = new gamePay($_GET);
$pay->ckPayNum();//检测PayNum
if($pay->Mode != 1) eixt('mode_error');
if(empty($pay->ServerName)) exit('servername_error'); //服务器号错误
if(empty($pay->PayToUser)) exit('username_error');
if($pay->PayGold <= 0) exit('paygold_error');
if($pay->PayTime <= 0) exit('paytime_error');
if(empty($pay->ticket)) exit('ticket_error');
if(!$pay->ckTicket()) exit('bad_request');
$pay->ZhuBaoPay();

?>