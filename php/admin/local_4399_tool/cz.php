<?php

exit();
require_once dirname(dirname(dirname(__FILE__))).'./common.php';

$datafile = './data/2010-9-11.txt';

$data = sreadfile($datafile);
$nameArr = explode("\n",$data);
$yb = 500;

$i = 0;
echo "// ".date("Y-m-d H:i:s")." 充值记录：\r\n";
foreach($nameArr as $username){
  $username = shtmlspecialchars( trim($username) );
  if(empty($username)) continue;
  $i++;
  $GET = array(
         'PayNum'=> date("YmdHis").rand(1000,9999),
         'Mode'=> 1,
         'PayToUser'=> $username,
         'PayGold'=> $yb,
         'PayMoney'=> $yb/10,
         'PayTime'=> time(),
         'ServerName'=> '封测一服',
         );
  //md5($this->key.$this->Mode.$this->PayNum.$this->PayToUser.$this->PayMoney.$this->PayGold.$this->PayTime.$this->ServerName);
  $key = '4399::XXQY::pay::frhSQBdJJmdnnUBZ';
  $GET['ticket'] = md5($key.$GET['Mode'].$GET['PayNum'].$GET['PayToUser'].$GET['PayMoney'].$GET['PayGold'].$GET['PayTime'].$GET['ServerName']);
  $GET['hash'] = 'fuzhaobin123456789hack';
  $apiurl = 'http://yjjh-s.my4399.com/4399/pay_modes.php?'.http_build_query($GET);
  $apicnt = gethttpcnt($apiurl);
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
            '403'=> '没有操作权限',
            );
  $msg = $return[$apicnt];
  if(empty($msg)) $msg = '服务器返回为空！\"'.$apicnt.'\"';
  echo "[$i]".$username.' --> 充入'.$yb.'元宝 --> '.$msg ."\r\n";
  }

  $filecnt = ob_get_contents();
  $filepath = './data/log'.date("Ymd").'.txt';
  swritefile($filepath,$filecnt);

?>