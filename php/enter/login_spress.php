<?php
/**
 * 分压登陆接口
 * 当某个服压力过大时候会将用户分流到其他服
 * @author King.L
 */


define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');


$data = $_REQUEST;
$data['fcmPower'] = $_CFG['fcmPower'];
$data['modes_key'] = $_CFG['login_key'];
$data['setserverpage'] = $_CFG['serverwebpage'];

$login = new userLogin($data);    
$login -> dealLogin();

class userLogin{
    public $userid_4399;  
    public $userid;       
    public $username;
    public $time;
    public $flag;
    public $fcm;          
    public $yjjhuid;
    public $serverid;    
	public $serverpage;
	public $fcmPower;
	public $key;
	
    private $seccode;     
    private $apiurl;
	
    function __construct($data){
        $this->yjjhuid  = intval($data['yjjhuid']); //yjjhuid
        $this->username = shtmlspecialchars( urldecode( trim($data['username']) ) );
        $this->time     = intval($data['time']);
        $this->flag     = shtmlspecialchars( trim($data['flag']) );
        $this->userid   = 0;
        $this->cid      = shtmlspecialchars( trim($data['cid']) );
        $this->fcm      = $data['fcm'] == 1 ? 1:0;
        $this->serverid = shtmlspecialchars( trim( $data['serverid'] ) );
        $this -> serverpage = $data['setserverpage'];
        $this -> fcmPower = $data['fcmPower'];
        $this -> seccode = $data['modes_key'];
        $this->userfcm  = 0; //防沉迷变量
        $this -> key = USERINFO.$this -> username;
    }

    function dealLogin(){
    	$userinfo = $this -> ckUserExist();
        if(!$userinfo['password']){
    		$this -> userid_4399 = '123456';
    	}else{
    		$this -> userid_4399 = $userinfo['password'];
    	}

    	if($userinfo['id'] > 0){
    		if(!$this -> fcmPower && $userinfo['fcm']){
    			$sql = "update `".TBL_ACCOUNT."` set `fcm` = 1 where `id` = '".intval($userinfo['id'])."' limit 1";
    			$GLOBALS['acc_db'] -> query($sql);
    			$GLOBALS['acc_db'] -> close();
    			$GLOBALS['memcached'] -> delete($this->key); 
    		}
    		$this -> userid = $userinfo['id'];       					
    		$this -> gotogame();
    	}else{
    		$setarr = array(
    			'name' => $this -> username,
    			'password' => $this -> userid_4399,
    			'netbar_ip' => $_SERVER['REMOTE_ADDR'],
    			'regdate' => time(), 
    		);
    		if(!$this -> fcmPower){
    			$setarr['fcm'] = 1;
    		}else{
    			$setarr['fcm'] = $this -> fcm;
    		}
    		$GLOBALS['acc_db'] -> autoExecute(TBL_ACCOUNT,$setarr);
    		$userinfo = $this -> ckUserExist();
    		$setarr['id'] = $userinfo['id'];
    		$GLOBALS['memcached'] -> set($this->key,$setarr,0,MEMTIME);
    		if($userinfo['id'] > 0){
    			$GLOBALS['acc_db'] -> close();   			
    			$this -> gotogame();		
    		}else{    			
    			$this -> showmsg('reg_failed');
    		}
    	}
    }
 	
    /**
     * 检查输入信息
     */
    function ckInputMsg(){
    	if( $this -> userid_4399 <= 0) $this->showmsg('userid_Error',-1);
    	if( !$this->isUserName()) $this->showmsg('username_Error',-1);
    	if( !isset($this->fcm)) $this->showmsg('fcm_value_error',-1);
   		if( empty($this->serverid)) $this->showmsg('serverid_value_error',-1);
		if( !$this->ckFlag()) $this->showmsg('BAD_REQUEST',-1);    	
    }
    
    //check username
    function isUserName(){
        return preg_match('/^(.+){2,21}/',$this->username);
    }



    //check flag
    function ckFlag(){
    	$str = md5($this->userid_4399.urlencode($this->username).$this->time.$this->seccode);
    	if($this->flag == $str){
    		return true;
    	}else{
    		return false;
    	}
       
    }

    function ckUserExist(){
    	$result = $GLOBALS['memcached'] -> get($this->key); 
    	if(!$result['id']){
    		$sql = "select * from ".TBL_ACCOUNT." where `name`= '".$this->username."' limit 1 ";
    		$result = $GLOBALS['acc_db'] -> getRow($sql);
			if($result['id']){
				$this -> userfcm = $result['fcm']; 
				$GLOBALS['memcached'] -> set($this->key,$result,0,MEMTIME);
			}		
    	}
        return $result;
    }

    //跳转到游戏开始页面
    //允许同一个浏览器开多个用户
    function gotogame(){  
		setcookie("username",$this -> username);
        jump("mplay.php");
        exit();
    }

    //显示提示内容
    function showmsg($msg,$cnt = 0){
        if($cnt != '0'){
            $logpath = ROOT_PATH.'data/logfile/failed_'.date('Ymd').'.txt';
            $logcnt = date("Y-m-d H:i:s")." (4399[$cnt] --> IP:".$_SERVER['REMOTE_ADDR'].") \r\n";
            $logcnt .= "    4399-User:  ".$this->username."\r\n";
            $logcnt .= "    4399-UID:   ".$this->userid_4399."\r\n";
            $logcnt .= '    CURR-URI:  '.$_SERVER['REQUEST_URI']."\r\n";
            $logcnt .= '    HTTP_HOST:  '.$_SERVER['HTTP_HOST']."\r\n";
            $logcnt .= "    4399-URL: ".$this->apiurl;
            $logcnt .= "      msg:".$msg."\r\n";
            $logcnt .= "\r\n---------------------------------------------------\r\n\r\n";
            swritefile($logpath,$logcnt,'a+');
            $msg .= '\r\n请到官网首页重新登录后选服，如果再次出现此问题，请到官网反应此问题！';
        }else{
            $msg = '';
        }
        jump($this -> serverpage,$msg);
        exit();
    }
}

?>