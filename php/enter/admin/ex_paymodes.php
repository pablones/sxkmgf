<?php
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');


if($_REQUEST['act'] == ''){	
	admin_priv('ex_paymodes');
	$smarty -> display('paymodes.htm');
}
if($_REQUEST['act'] == 'query'){
    $links[] = array('text' => $_LANG['back'], 'href' => 'javascript:history.go(-1)'); 
	 
	$username=$_POST['PayToUser'];
	$paynum=$_POST['PayNum']; 
	$paygold=$_POST['PayGold'];  
	$paymoney=$_POST['PayMoney']; 
	$ip=$_SERVER['REMOTE_ADDR'];
	$serverid=$_CFG['fuwuqi_id'];
	$logdate=time(); 
	$sql="select id from ".TBL_ACCOUNT." where name='$username' ";
	$userid=$acc_db -> getOne($sql); 
    if(!$userid){
     	sys_msg($_LANG['account_error'], 1, $links);
    }  
    $sql=" select paynum from ".TBL_SK_PAYLOG." where paynum='$paynum'";
	$rs=$acc_db -> getOne($sql);
	
	if($rs){
	 	sys_msg($_LANG['paynum_error'], 1, $links);
	}
	$sql = "update ".TBL_SK_VAS_TABLE." set `VAS_Point` = `VAS_Point` + ".$paygold." where `Account_ID` = ".$userid."";
	$res=$acc_db -> query($sql);	
	$sql=" INSERT INTO ".TBL_SK_PAYLOG." (`paynum` ,`logdate` ,`userid` ,`ip` ,`serverid` ,`paygold` ,`paymoney` )
	     VALUES ('$paynum', '$logdate', '$userid', '$ip', '$serverid', '$paygold', '$paymoney')";
	$res1=$acc_db ->query($sql);
	if($res&&$res1){
		$content=$_LANG['act'].",".$_LANG['username'].":$username".$_LANG['PayGold'].":$paygold".$_LANG['PayMoney'].":$paymoney";
	    $sql = "INSERT INTO " .TBL_ADMIN_LOG . " (log_time, user_id, log_info, ip_address) " .
	           " VALUES ('" . gmtime() . "', $_SESSION[admin_id], '" . $content . "', '" . real_ip() . "')";
        $GLOBALS['acc_db']->query($sql);
        sys_msg($_LANG['ok'], 1, $links);
	}

	
}


?>