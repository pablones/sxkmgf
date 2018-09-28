<?php


//用户登录类
class userLogin{
    public $userid_4399;  //4399那边的userid
    public $userid;       //本站的userid
    public $username;
    public $time;
    public $flag;
    public $fcm;          //防沉迷值
    public $yjjhuid;
    public $servernum; 	//服务器人数
    public $serverid;     //服务器的服号
    private $seccode;     //我方的
    private $apiurl;


    function __construct($GET){
        $this->yjjhuid  = intval($GET['game_src']); //yjjhuid
        $this->username = shtmlspecialchars( urldecode( trim($GET['username']) ) );
        $this->serverid = shtmlspecialchars( trim( $GET['serverid'] ) );
        $this->seccode  = $_CFG['seccode'];//'4399*:yjjh*:aX4L*:41@-504';//我方提供
        $apiurl = $_CFG['serverapiurl']."passport.php?userid=".$this->userid_4399."&username=".$this->username."&time=".$this->time."&flag=".$this->flag;
        $this->apiurl   = $apiurl;
    }

    //check username
    function isUserName(){
        return preg_match('/^(.+){2,21}/',$this->username);
    }

    //check flag
    function ckFlag(){
        return $this->flag == md5($this->userid_4399.urlencode($this->username).$this->time.$this->seccode);
    }


    function ckUserExist(){
        $sql = "select `id`,`fcm`,`password` from `".TBL_ACCOUNT."` where `name` = '".$this -> username."'";
        $result = $GLOBALS['acc_db'] -> getRow($sql);
		return $result;
    }

    //跳转到游戏开始页面
    //允许同一个浏览器开多个用户
    function gotogame(){

        $pm  = authcode($this->userid_4399,'ENCODE');
        
        $arr = array(
            'userid'   => $this->userid,
            'username' => $this->username,
            'pm'       => $pm,
            'fromlogin'=> 'yes',
        );
       
        setcookie("timestamp",$GLOBALS['_SG']['timestamp']);
        setcookie("userid",$this->userid);
        setcookie("fromlogin",'yes'); //是登陆而不是刷新
        setcookie("4399uid",$this->userid_4399); //加密
       
        setcookie("playername",$this->username);
        setcookie("serverid",$this->serverid);
        foreach($arr as $key => $val){
            setcookie('u'.$this->userid."[$key]",$val);
        }
        jump("play.php");
        exit();
    }

    //显示提示内容
    function showmsg($msg,$cnt = 0){
        if($cnt != '0'){
            $logpath = ROOT_PATH . 'data/logfile/failed_'.date('Ymd').'.txt';
            $logcnt = date("Y-m-d H:i:s")." (4399[$cnt] --> IP:".$GLOBALS['_SG']['ip'].") \r\n";
            $logcnt .= "    4399-User:  ".$this->username."\r\n";
            $logcnt .= "    4399-UID:   ".$this->userid_4399."\r\n";
            $logcnt .= '    CURR-URI:  '.$_SERVER['REQUEST_URI']."\r\n";
            $logcnt .= '    HTTP_HOST:  '.$_SERVER['HTTP_HOST']."\r\n";
            $logcnt .= "    4399-URL: ".$this->apiurl;
            $logcnt .= "\r\n---------------------------------------------------\r\n\r\n";
            swritefile($logpath,$logcnt,'a+');
            $msg .= $_LANG['php_failelogin'];
        }else{
            $msg = '';
        }
        $jump_url = $_CFG['serverwebpage']."?login_faile&cnt=".$cnt;
        jump($jump_url,$msg);
        exit();
    }

    //处理登录
    function dealLogin(){       
        if(empty($this->serverid)) $this->showmsg('serverid_value_error',-1);
	    $rs = $this->ckUserExist();
	    $this->userid = $rs['id'];
        $this -> userid = intval($this -> userid);      
	    if($this->userid){
		    $this->gotogame();
	    }else{
		    //先注册后进入游戏
		    $setarr = array(
		        'name'      => $this->username,
				'password'  => $this->userid_4399,
				'netbar_ip' => $_SERVER['SERVER_ADDR'],
				'regdate'   => time(),
				'fcm'       => $this->fcm,
		    );
            $this -> userid = $GLOBALS['acc_db'] -> autoExecute(TBL_ACCOUNT,$setarr);
           
		    if($this->userid > 0){
		        //注册成功，直接进入游戏,跳转到游戏界面,相应的COOKIE SESSION处理
		        $this->gotogame();
		    }else{
			    //注册失败！
			    $this->showmsg('reg_failed');
		    }
	    }
	}
}



?>