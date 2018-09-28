<?php


/** 用户在线SESSION处理
  *
  * []=> 进入瞬间                 <请求参数 =  1>
  * []=> 退出瞬间(刷新或关闭页面) <请求参数 = -1>
  * []=> 游戏中掉线               <请求参数 = -2>
  *
  * 无法防止某用户不停的刷新页面,每次刷新就总结一次[有请求间隔过滤]
  *
  *-----------------------------------------------------------------------
  * COOKIE各个值的含义
  * [ftime] ->firstactive第一次响应时间
  * [ltime] ->lastactive最后一次响应时间
  *
  *
  * [stat = <oln><lss>] <oln>表示在线,<lss>表示离线或者掉线
  * [fromlogin = <yes><not>] <yes>表示是登陆而不是刷新,<not>表示的是刷新
  *
  */


//限定请求类型
if($_SERVER['REQUEST_METHOD'] != 'POST' || !isset($_POST['evt']) || !isset($_POST['u'])) exit('403');

require_once './ptcommon.php';

error_reporting(E_ALL);
$ptuser = new pt_user;
$ptuser->ckUser();

dbconn_pt();

//<从官网进入><刷新页面>
if($_POST['evt'] == 1){

  //未掉线,故意设置为oln,防止别人猜出含义[oln = online]
  ssetcookie( $ptuser->cookieid.'[stat]','oln');

  //请求间隔 <PT_UPDATEMIN:预设3分钟>
  if($ptuser->timestamp - $ptuser->firstactive < PT_UPDATEMIN) sexit('400');

  $userdaylogid = $ptuser->ckUserdaylog();

  if($ptuser->fromlogin == 1 && $userdaylogid > 0){
    $sql = "update @#__userdaylog set count_login=count_login+1 where logid='$userdaylogid' limit 1 ";
    $db->query($sql);
	//是否是4399过来的
    if($db->affected_rows()) ssetcookie($ptuser->cookieid.'[fromlogin]','not');
    }

  $ptuser->writeSession();
  //第一次响应时间
  ssetcookie($ptuser->cookieid.'[ftime]',$ptuser->timestamp);

  }else{

    //标志为<离线><掉线>
    ssetcookie( $ptuser->cookieid.'[stat]','lss');

    //掉线,关闭网页,COOKIE设置为lss,标识掉线[lss = loss]
    if($ptuser->timestamp - $ptuser->lastactive < PT_UPDATEMIN) sexit('400');

    //清除自己的SESSION,并且标志为离线
    $ptuser->clearSession();
  }

$db->close();
ssetcookie($ptuser->cookieid.'[ltime]',$ptuser->timestamp);
sexit('200');
?>