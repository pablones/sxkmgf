<?php

/**
 +----------------------------------------------------------------------------
 * 充值总检测该4399账号是否创建了用户
 +----------------------------------------------------------------------------
 *  @game       yjjh
 *  @serverid   S1
 *  @username   mysql3306
 *  @sign       $sign=md5($username.$game.$serverid.'Irosg34#$9utgfds')
 +----------------------------------------------------------------------------
 *  加密的密匙  Irosg34#$9utgfds ( 配合 Loong  www@lianglong.org[83527511] )
 +----------------------------------------------------------------------------
 */




define('API_CK_KEY','Irosg34#$9utgfds');


require_once '../common.php';
$getkeyArr = array('game','serverid','username','sign');
foreach($getkeyArr as $key){
  if(!isset($_GET[$key]) || empty($_GET[$key])) exit("bad_param[$key]");
  $$key = shtmlspecialchars( urldecode( trim($_GET[$key]) ) );
  }


if(!array_key_exists($serverid,$serverArr)) exit("serveridErr");
$signMD5 = md5($username.$game.$serverid.API_CK_KEY);

if($signMD5 != $sign) exit("signErr");

dbconnect();
$sql    = "SELECT 1 FROM account WHERE `name`='$username' LIMIT 1 ";
$result = $db->result_first($sql);
echo $result ? 1:0;
$db->close();
?>