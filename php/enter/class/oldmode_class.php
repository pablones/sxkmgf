<?php
/**
 * 充值通用类
 */
class gamePay{
    public $PayNum;
    public $Mode;
    public $PayToUser;
    public $PayGold;
    public $PayMoney;
    public $PayTime;
    public $ServerName;
    public $ticket;
    public $lv;
    public $mkey;
    public $allowIpArr;
    public $power;
    private $key; //我方提供的key

	function __construct($data){
	    $this -> PayNum     = $data['PayNum'];
	    $this -> Mode       = isset($data['Mode'])?intval($data['Mode']):1;
	    $this -> PayToUser  = shtmlspecialchars( urldecode(trim($data['PayToUser'])) );
	    $this -> PayGold    = $data['PayGold']*1;
	    $this -> PayTime    = $data['PayTime']*1;
	    $this -> PayMoney   = $data['PayMoney']*1;
	    $this -> ServerName = shtmlspecialchars( urldecode(trim($data['ServerName'])) );
	    $this -> ticket     = shtmlspecialchars( urldecode(trim($data['ticket'])) );
//	    $this -> userid     = 0;
	    $this -> power		= $data['power'];
	    $this -> key        = $data['modes_key'];
	    $this -> allowIpArr = $data['allowIpArr'];
	    $this -> mkey = USERINFO.$this -> PayToUser;
	    $this -> actcode    = $data['actcode']*1; //手机活动代码
	}

	function ckIp(){
		//访问的IP限定
		$allowIpArr = explode("|",$this -> allowIpArr);
		//是否送点卷
		if(in_array($_SERVER['REMOTE_ADDR'],$allowIpArr)){
		    define('GIVE_GOLD3',TRUE); //送
		}else{
		    define('GIVE_GOLD3',FALSE); //不送
		}
		if(!$this -> power){
    		if(!in_array($_SERVER['REMOTE_ADDR'],$allowIpArr)) exit('403');//无权限操作	
		}
	}
	
	function setmodekey($key){
		$this->key = $key;
	} 
    
	function ckTicket(){		
	    $md5_str = $this->key.$this->Mode.$this->PayNum.$this->PayToUser.$this->PayMoney.$this->PayGold.$this->PayTime.$this->ServerName;
	    if($this->ticket == md5($md5_str)){
	        $result = true;
	    }else{
	        $result = false;
	    }
	    return $result;//$this->ticket == md5($md5_str);
	}

	//根据用户名获取UID
	function getUID(){
		$sql = "select * from ".TBL_ACCOUNT." where `name` = '".$this->PayToUser."' limit 1";
		$userinfo = $GLOBALS['acc_db'] -> getRow($sql);
		if(!$userinfo['id']){
			exit("account_error");
		}	
	    return $userinfo;
	}
	
	function setPlayerlv($userinfo){
		$sql = "select `level` from ".TBL_WB_USER." where `account_id` = '".$userinfo['id']."' limit 1";
		$rs = $GLOBALS['wangpu_db'] -> getRow($sql);
		if($rs){
			$this -> lv =$rs['level'];
		}else{
			$this -> lv = 0;
		}		
	}

	function ckPayNum(){
	    //验证是否已经记录过了,只验证paynum,不验证userid	    	    
		$sql = "select count(logid) as num from ".TBL_SK_PAYLOG." where `paynum` = '".$this->PayNum."'";
	    $result = $GLOBALS['acc_db']->getOne($sql);	    	
	    if($result['num'] > 0) exit('paynum_exist'); //已经充值了，立即停止
	    
	}

	function YuanBaoPay(){
		$this -> ckIp();
		$userinfo = $this -> getUID();
		$this -> setPlayerlv($userinfo);
		$this->ckPayNum();
		if(!$this -> power){
	        if($this->Mode != 1) exit('mode_error');
	        if(empty($this->ServerName)) exit('servername_error'); //服务器号错误
	        if(empty($this->PayToUser)) exit('username_error');
	        if($this->PayGold <= 0) exit('paygold_error');
	        if($this->PayTime <= 0) exit('paytime_error');
	        if(empty($this->ticket)) exit('ticket_error');
	        if(!$this->ckTicket()) exit('bad_request');
	        if($userinfo['id']<=0) exit('account_error'); //该账户不存在
	    }
	    /* 返回珠宝 */
	    if($this->PayGold >= 100){
	    	$sql = "select logid from ".TBL_SK_PAYLOG." where `userid` = '".$userinfo['id']."' and `paygold` >=100 and logdate>='1308326400'";
	        $query = $GLOBALS['acc_db']->getRow($sql);
			if($query['logid'] <= 0){
	        	$sql_gen = "insert into `".TBL_SK_GENERALIZE."` (`account_id`,`generalize_id`,`state`,`type`) values('".$userinfo['id']."',4006,1000,1000)";
	           	$GLOBALS['acc_db'] -> query($sql_gen);
	        }    	 	
	    }
	    //先记录本次充值记录，必须记录本次充值记录
	    $setarr2 = array(
	        'paynum'=> $this->PayNum,
	        'logdate'=> time(),
	        'userid'=> $userinfo['id'],
	        'ip'=> $_SERVER['REMOTE_ADDR'],
	        'serverid'=> $this->ServerName,
	        'paygold'=> $this->PayGold,
	        'paymoney'=> $this->PayMoney,
	    );
	    $data2 = $GLOBALS['acc_db'] -> autoExecute(TBL_SK_PAYLOG,$setarr2);
	    if(!$data2)  exit('failed');
	
	    //冲过值的插入sk_generalize表
		$sql="select type from ".TBL_SK_GENERALIZE." where type=2 and account_id=".$userinfo['id'];
	    $result = $GLOBALS['acc_db']->getOne($sql);
	    $type = isset($result) ? $result : 0;
	
	
		if($type!=2){
			$sql="insert into ".TBL_SK_GENERALIZE."(`account_id`,`type`,`generalize_id`) values(".$userinfo['id'].",2,".$this->lv.")";
			$query = $GLOBALS['acc_db']->query($sql);
		}
	
	
		//从表 sk_vas_table 里面更新其字段 Vas_Pointer 的值(直接相加) 和最后更新的日期值,如果没有的话，插入新记录
	    $sql = "select ID,Vas_Point,`vastotal`,`vascount` from ".TBL_SK_VAS_TABLE." where `Account_ID`='".$userinfo['id']."' order by ID ASC limit 1 ";
	    $rs = $GLOBALS['acc_db']->getRow($sql);
	    $setarr = array(
	        'Vas_Point'=> ($rs['Vas_Point'] + $this->PayGold),
	        'LastConsumeDate'=> date("Y-m-d H:i:s"),
	        'vastotal' => ($rs['vastotal'] + $this->PayGold) ,
	        'vascount' => ($rs['vascount'] + $this->PayGold),
	    );
	
	    if($rs['ID'] <= 0){
	        $setarr['FirstConsumeDate'] = date("Y-m-d H:i:s");//创建的日期
	        $setarr['Account_ID'] = $userinfo['id']; //用户账号
	        $setarr['Server_Name'] = $this->ServerName; //服号
	    }
	    //更新账号表中的充值总数
	    if($rs['ID']>0){
	    	$where = "ID = ".$rs['ID'];
	    	$sql = "update ".TBL_SK_VAS_TABLE." set ";
	    	$setarr_num = count($setarr) -1;
	    	$i = 0;
	    	foreach($setarr as $key => $value){
	        	if($i < $setarr_num){
	            	$sql .= "".$key." = '".$value."' ,";
	        	}else{
	            	$sql .= "".$key." = '".$value."' ";
	        	}
	        	$i++;
	    	}
	    	$sql .= "where ID = ".$rs['ID']."";
	    	$data = $GLOBALS['acc_db'] -> query($sql);
	    }else{
	    	$sql = "insert into ".TBL_SK_VAS_TABLE." (`Account_ID`,`Server_Name`,`VAS_Point`,`vastotal`,`vascount`)VALUES('".$userinfo['id']."','".$this->ServerName."','".$setarr['Vas_Point']."','".$setarr['vastotal']."','".$setarr['vascount']."')";
	    	$data = $GLOBALS['acc_db'] -> query($sql);	    	
	    }
	    if($data){
	        //TODO 如果定义了送点卷而且送点卷
	        if(defined('GIVE_GOLD3') && GIVE_GOLD3){
	            //游戏数据库加点，每加一点
	            $gold3 = intval($this->PayGold/50);
	            if($gold3 > 0){
	                $sql = "update `".TBL_SK_VAS_TABLE."` set `VAS_Point_juan` = `VAS_Point_juan` + ".$gold3." where `Account_ID` = '".$userinfo['id']."'";
	                $GLOBALS['acc_db']->query($sql);
	            }
	        }
	        $str = 'ok';
	        $str = trim($str);
	        exit($str);
	    }else{
	        //充值失败！
	        exit('failed');
	    }
	}
	
	/**
	 * 手机充值验证
	 */
	function mckTicket(){
		$md5_str = $this->key.$this->Mode.$this->PayNum.$this->PayToUser.$this->PayMoney.$this->PayTime.$this->ServerName;
	    if($this->ticket == md5($md5_str)){
	        $result = true;
	    }else{
	        $result = false;
	    }
	    return $result;//$this->ticket == md5($md5_str);
	}
	
	/**
	 * 手机礼包接口
	 */
	function mob_act(){
		$userinfo = $this -> getUID();
        $this->ckPayNum();//检测PayNum
        //$this -> userid = $userinfo['id']; 
        if(!$this -> power){
       		if($this->Mode != 1) exit('mode_error');
        	if(empty($this->ServerName)) exit('servername_error'); //服务器号错误
        	if(empty($this->PayToUser)) exit('username_error');
        	if($this->actcode <= 0) exit('bad_request');
        	if($this->PayTime <= 0) exit('paytime_error');
        	if(empty($this->ticket)) exit('ticket_error');
        	if(!$this->mckTicket()) exit('bad_request');
        	if(!$userinfo['id']) exit('account_error'); //该账户不存在
        }

        //先记录本次充值记录，必须记录本次充值记录
        $setarr2 = array(
            'paynum' => $this->PayNum,
            'logdate' => time(),//$_SG['timestamp'],
            'userid' => $userinfo['id'],
            'ip' => $_SERVER['REMOTE_ADDR'],
            'serverid' => $this->ServerName,
            'paymoney' => $this->PayMoney,
        );
        $data2 = $GLOBALS['acc_db'] -> autoExecute(TBL_SK_PAYLOG,$setarr2);
        if(!$data2)  exit('failed');
        switch($this -> actcode){
            case "4001":
                $result = $this -> _mob_act(4,$userinfo);
                break;
            case "4002":
            	$result = $this -> _mob_act(5,$userinfo);
                break;
            case "4003":
            	$result = $this -> _mob_act(6,$userinfo);
                break;
        }

        if($result){
            exit('ok');
        }else{
            exit("failed");
        }		
	}
	
	private function _mob_act($type,$userinfo){
		$sql ="select count(id) as amount from ".TBL_SK_GENERALIZE." where `account_id` = '".$userinfo['id']."' and type = '".$type."' ";
       	$result = $GLOBALS['acc_db'] -> getRow($sql);
        if($result['amount'] <=0){
        	$sql = "insert into ".TBL_SK_GENERALIZE." (`account_id`,`type`,`generalize_id`,`state`)values('".$userinfo['id']."','".$type."','".$this->actcode."',1)";
            $result =  $GLOBALS['acc_db'] -> query($sql);
        }else{
            $sql = "UPDATE ".TBL_SK_GENERALIZE." SET `state` = `state` + '1' WHERE `account_id` = '".$userinfo['id']."'";
            $result =  $GLOBALS['acc_db'] -> query($sql);
       	}
       	return $result;
	}
}
?>