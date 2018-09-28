<?php
//使用
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');

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

$pay = new gamePay($_GET);
$pay->userid=$pay->getUID();

if($pay->userid>0){
	$sql="select level from wb_user where account_id=".$pay->userid." limit 1";
	$rs=$wangpu_db->getOne($sql);
	$pay -> lv = isset($rs)?$rs:0;
	
}

$pay->dealPay();



//----------------------------------------------------------------------------------------------------------------------------------

class gamePay{
  	public $Mode;
  	public $PayToUser;
  	public $ServerName;//游戏服号
  	public $ticket;
  	public $userid;
  	private $key; //我方提供的key

 


  	function __construct($GET){
    	$this->Mode       = intval($GET['Mode']);
    	$this->PayToUser  = shtmlspecialchars( urldecode(trim($GET['PayToUser'])) );
    	$this->ServerName = shtmlspecialchars( urldecode(trim($GET['ServerName'])) );
    	$this->ticket     = shtmlspecialchars( urldecode(trim($GET['ticket'])) );
    	$this->userid     = 0;
    	$this->key        = 'taiwan::wJCz@pay::frhSQFDATgrfnnUBZ';//平台提供


    }

  	function ckTicket(){
  		$str = md5($this->key.$this->Mode.$this->ServerName);
  		if($this -> ticket == $str){
  			return true;
  		}else{
  			return false;
  		}    	
    }

  	//根据用户名获取UID
  	function getUID(){
  		$sql = "select `id` from ".TBL_ACCOUNT." where name = '".$this -> PayToUser."' ";
  		$rs = $GLOBALS['acc_db'] -> getOne($sql);
  		return $rs;
    }


  	//处理充值
  	function dealPay(){
   	    if($this->Mode != 1) exit('mode_error');
    	if(empty($this->ServerName)) exit('servername_error'); //服务器号错误
    	if(empty($this->PayToUser)) exit('username_error');
   	    if(empty($this->ticket)) exit('ticket_error');
   	    if(!$this->ckTicket()) exit('bad_request');
    	$this->userid = $this->getUID();
    	if(!$this->userid) exit('account_error'); //该账户不存在

        $sql = "select * from ".TBL_SK_GENERALIZE." where account_id = '".$this -> userid."'";
        $res = $GLOBALS['acc_db'] -> getRow($sql);
     	$state = intval($res['state']);

        if($state != 2){
        	$sql = "insert into ".TBL_SK_GENERALIZE."(account_id,state,type) values(".$this->userid.",2,10)";
            $GLOBALS['acc_db']->query($sql);
            exit("ok");
        }else{
        	exit("exit");
        }
    }
}
?>