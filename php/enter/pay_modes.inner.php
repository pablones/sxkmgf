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

class gamePay{
  public $PayNum;
  public $Mode;
  public $PayToUser;
  public $PayGold;
  public $PayMoney;
  public $PayTime;
  public $ServerName;//游戏服号
  public $ticket;
  public $userid;
  private $key; //我方提供的key


  function __construct($GET){
    $this->PayNum     = $GET['PayNum'];
    $this->Mode       = intval($GET['Mode']);
    $this->PayToUser  = shtmlspecialchars( urldecode(trim($GET['PayToUser'])) );
    $this->PayGold    = $GET['PayGold']*1;
    $this->PayTime    = $GET['PayTime']*1;
    $this->PayMoney   = $GET['PayMoney']*1;
    $this->ServerName = shtmlspecialchars( urldecode(trim($GET['ServerName'])) );
    $this->ticket     = shtmlspecialchars( urldecode(trim($GET['ticket'])) );
    $this->userid     = 0;
    $this->key        = '4399::XXQY::pay::frhSQBdJJmdnnUBZ';//我方提供
    }

  function ckTicket(){
    return $this->ticket == md5($this->key.$this->Mode.$this->PayNum.$this->PayToUser.$this->PayMoney.$this->PayGold.$this->PayTime.$this->ServerName);
    }

  //根据用户名获取UID
  function getUID(){
    return $GLOBALS['db']->result_first("select `id` from `account` where `name`='".$this->PayToUser."' limit 1 ");
    }

    /*
     * 检测流水号是否过期了，10小时之内充值有效，超过10小时充值无效
     * 4399 充值流水号 格式是18位
     */
  function ckPayNum(){
    if(!preg_match('/^[0-9]{18}$/',$this->PayNum)) exit('paynum_error');
    $_paydate = substr($this->PayNum,0,14);
    $_paydate = strtotime($_paydate);
    if(abs($GLOBALS['_SG']['timestamp'] - $_paydate) > 36000) exit('paynum_error');//过期了，不得超过10个小时

    //验证是否已经记录过了,只验证paynum,不验证userid
    $sql = "select 1 from sk_paylog where `paynum`='".$this->PayNum."' limit 1 ";
    if($GLOBALS['db']->result_first($sql) == 1) exit('paynum_exist');//已经充值了，立即停止
    }

  //处理充值
  function dealPay(){
    global $db,$_SG;
    $this->ckPayNum();//检测PayNum
    if($this->Mode != 1) eixt('mode_error');
    if(empty($this->ServerName)) exit('servername_error'); //服务器号错误
    if(empty($this->PayToUser)) exit('username_error');
    if($this->PayGold <= 0) exit('paygold_error');
    if($this->PayTime <= 0) exit('paytime_error');
    if(empty($this->ticket)) exit('ticket_error');
    if(!$this->ckTicket()) exit('bad_request');

    $this->userid = $this->getUID();
    if(!$this->userid) exit('account_error'); //该账户不存在
    //从表 sk_vas_table 里面更新其字段 Vas_Pointer 的值(直接相加) 和最后更新的日期值,如果没有的话，插入新记录
    $sql = "select ID,Vas_Point from sk_vas_table where `Account_ID`='".$this->userid."' order by ID ASC limit 1 ";
    $rs = $db->fetch_first($sql);
    $setarr = array(
              'Vas_Point'=> ($rs['Vas_Point']+$this->PayGold),
              'LastConsumeDate'=> date("Y-m-d H:i:s"),
              );

    if($rs['ID'] <= 0){
      $setarr['FirstConsumeDate'] = date("Y-m-d H:i:s");//创建的日期
      $setarr['Account_ID'] = $this->userid; //用户账号
      $setarr['Server_Name'] = $this->ServerName; //服号
      }

    //先记录本次充值记录，必须记录本次充值记录
    $setarr2 = array(
              'paynum'=> $this->PayNum,
              'logdate'=> $_SG['timestamp'],
              'userid'=> $this->userid,
              'ip'=> $_SG['ip'],
              'serverid'=> $this->ServerName,
              'paygold'=> $this->PayGold,
              'paymoney'=> $this->PayMoney,
              );
    $data2 = affectrow('sk_paylog',$setarr2);
    if(!$data2)  exit('failed');


//冲过值的插入sk_generalize表
	$sql="select type from sk_generalize where type=2 and account_id=".$this->userid;
//				echo $sql."<br />";
	$query=$db->query($sql);
	if($db->num_rows($query)>0)
	{
		$row=$db->fetch_array($query);
		$type=$row['type'];
		$db->free_result($query);
	}
	else
	{
		$type=0;
	}
//				echo $type."<br />";
	if($type!=2)
	{
		$sql="insert into sk_generalize(account_id,type) values(".$this->userid.",2)";
//					echo $sql."<br />";
		$query=$db->query($sql);
	}

    //更新账号表中的充值总数
    $data = affectrow('sk_vas_table',$setarr,'ID',$rs['ID']);
    if($data){

        //如果定义了送点卷而且送点卷
        if(defined('GIVE_GOLD3') && GIVE_GOLD3){
          //游戏数据库加点，每加一点
          $gold3 = intval($this->PayGold/50);
          if($gold3 > 0){
  //          dbconn_game(); //connect the game database server
  //          $sql = "update wb_user set Gold3=Gold3+$gold3 where account_id='".$this->userid."' limit 1 ";
  			 $sql = "update sk_vas_table set VAS_Point_juan=VAS_Point_juan+$gold3 where Account_ID='".$this->userid."' limit 1 ";
            $db->query($sql);
            }
          }

        exit('ok');
      }else{
        //充值失败！
        exit('failed');
      }
    }
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

  //使用
  require_once '../common.php';
  //error_reporting(E_ALL);

  //访问的IP限定
  //是否送点卷
  if(in_array($_SG['ip'],$allowIpArrIn)){
    define('GIVE_GOLD3',TRUE); //送
    }else{
      define('GIVE_GOLD3',FALSE); //不送
    }
//echo $_SG['ip'];
  if(!in_array($_SG['ip'],$allowIpArrIn)) exit('403');//无权限操作

  dbconnect();
  $pay = new gamePay($_GET);
  $pay->dealPay();
?>