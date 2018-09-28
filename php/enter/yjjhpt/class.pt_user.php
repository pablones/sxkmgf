<?php

if(!defined('IN_ZBPHP'))
{
	exit('Access Denied');
}

//数据库 yjjhpt ,对象user
class pt_user {

  public $userid;
  public $username;
  //public $rolename;
  public $password;

  public $fromlogin;

  public $ip;
  public $ippoz;

  public $lastuid; //上一次的登录UID

  public $userdaylogid;
  public $curdate;

  public $timestamp;
  public $firstactive;
  public $lastactive;

  public $lineid;     //所在线路的ID
  public $none_act_time;
  public $cookieid;


  function __construct(){
    global $_SCOOKIE;

    //注意,有的ID前面有补0
    $this->userid       = trim($GLOBALS['_POST']['u']);
    if(!preg_match("/\d+/",$this->userid)) sexit('403');

    $this->cookieid     = 'u'.$this->userid;
    $this->username     = shtmlspecialchars( $_SCOOKIE[$this->cookieid]['username'] );
    //$this->rolename   = shtmlspecialchars( $_SCOOKIE[$this->cookieid]['rolename'] );
    $this->fromlogin    = trim(  $_SCOOKIE[$this->cookieid]['fromlogin']) == 'yes' ? 1:0;
    $this->password     = isset( $_SCOOKIE[$this->cookieid]['pm'] ) ? authcode( $_SCOOKIE[$this->cookieid]['pm'] ,'DECODE'):'';
    $this->ip           = $GLOBALS['_SG']['ip'];
    $this->ippoz        = '';
    $this->timestamp    = $GLOBALS['_SG']['timestamp'];
    $this->firstactive  = isset($_SCOOKIE[$this->cookieid]['ftime'] )  ? intval($_SCOOKIE[$this->cookieid]['ftime']):0;
    $this->lastactive   = isset($_SCOOKIE[$this->cookieid]['ltime'] )  ? intval($_SCOOKIE[$this->cookieid]['ltime']):0;
    $this->lastuid      = isset($_SCOOKIE[$this->cookieid]['lastuid']) ? intval($_SCOOKIE[$this->cookieid]['lastuid']):0;
    $this->none_act_time= abs($this->timestamp - $this->lastactive);
    $this->lineid       = isset($_SCOOKIE[$this->cookieid]['lineid'])  ? intval($_SCOOKIE[$this->cookieid]['lineid']):0;
    $this->curdate      = date("Y-m-d");
    //尚未登陆
    if($this->userid == 0 || empty($this->username) || empty($this->password)) sexit('301');
    }

  //get ip poz
  function getIppoz(){
    if(empty($this->ippoz)) $this->ippoz = convertip($this->ip);
    return $this->ippoz;
    }

  //DB:acc_db
  //TB:account
  function ckUser(){

    //错误的COOKIE
    $sql = "select 1 from account where `id`='".$this->userid."' and `password`='".$this->password."' and `name`='".$this->username."' limit 1 ";
    if($GLOBALS['db']->result_first($sql) == 1) return 1;
    //设置为掉线,未登录
    ssetcookie($this->cookieid.'[stat]','lss');
    sexit('bad_cookie');
    }



  function writeUserdaylog($date){
    $setarr = array(
              'userid'      => $this->userid,
              'username'    => $this->username,
              'date'        => $date,
              //'rolename'  => $this->getRolename(),
              'logtime1'    => $this->timestamp,
              'logtime2'    => $this->timestamp,
              'count_login' => ($date == date("Y-m-d") ? 0:1),
              );
    return affectrow('userdaylog',$setarr);
    }

  //TB:@#__session
  function writeSession(){
    $setarr = array(
              'userid'      => $this->userid,
              'ip'          => $this->ip,
              'ippoz'       => $this->getIppoz(),
              'firstactive' => $this->timestamp,
              'lastactive'  => $this->timestamp,
              'lineid'      => $this->lineid,//默认为0,表示[加载中]...
              );
    return affectrow('session',$setarr,'','',1);
    }

  //更新最后响应时间
  function updateLastactive(){
    $sql = "update @#__session set `lastactive`=unix_timestamp() where userid='".$this->userid."' limit 1 ";
    $GLOBALS['db']->query($sql);
    }

  //检查SESSION
  function ckSession(){
    $sql = "select 1 from @#__session where userid='".$this->userid."' limit 1 ";
    if($GLOBALS['db']->result_first($sql) == 1) return TRUE;
    return $this->writeSession();
    }

  function ckUserdaylog($date = ''){
    if(empty($date)) $date = date("Y-m-d");
    $sql = "select logid from @#__userdaylog where userid='".$this->userid."' and `date`='$date' limit 1 ";
    $logid = $GLOBALS['db']->result_first($sql);
    if($logid > 0) return $logid;

    //写入userdaylog表
    return $this->writeUserdaylog($date);
    }

  //TB:@#__session
  //特例情况:某玩家在线数天后才关闭浏览器
  function clearSession(){
    global $db;
    $sql = "select firstactive from @#__session where userid='".$this->userid."' limit 1 ";
    $rs = array();
    $rs['firstactive'] = $db->result_first($sql);
    if(!$rs['firstactive']) return;

    //最后响应时间为当前时间
    $rs['lastactive'] = $this->timestamp;
    while(1){
      $date1   = date("Y-m-d",$rs['firstactive']);
      $date2   = date("Y-m-d",$rs['lastactive']);

      if($date1 == $date2){
        $oltime = $rs['lastactive'] - $rs['firstactive'];
      }else{
        $oltime = $rs['lastactive'] - strtotime($date2);
        $rs['lastactive'] = strtotime($date2) - 1;//减去1秒,到达前一天
      }

      $logid = $this->ckUserdaylog($date2);
      if($logid > 0 && $oltime >= PT_USER_OLTIME){
        $sql = "update @#__userdaylog set sum_oltime=sum_oltime+'$oltime',logtime2=unix_timestamp() where logid='$logid' limit 1 ";
        $db->query($sql);
        }

      if($date2 <= $date1 || $rs['lastactive'] <= $rs['firstactive']) break;
      }

      //删除这个SESSION
      $sql = "delete from @#__session where userid='".$this->userid."' limit 1 ";
      $db->query($sql);
    }


    //换线
    function changeLine($lineid){
      if($this->lineid > 0 && $this->lineid == $lineid) return;
      global $db;
      $this->lineid = $lineid;
      $sql = "update @#__session set `lineid`='".$this->lineid."',`lastactive`=unix_timestamp() where userid='".$this->userid."' limit 1 ";
      $db->query($sql);
      if($db->affected_rows() > 0) ssetcookie($this->cookieid.'[lineid]',$this->lineid);
      }
  }
?>