<?php
define('IN_ECS', true);
require( '../includes/init.php');

$key = $_CFG['login_key'];

$username=urldecode($_GET['username']);
$sign=$_GET['sign'];

if(md5($username.$key)!=$sign){
	exit("403");//私钥错误
}
if(!empty($username)){		
	$sql="select id from ".TBL_ACCOUNT." where name='".$username."'";	
	$result=$acc_db->getOne($sql);
	if(!$result){
		exit("501");//没有该账号	
	}else{
		$sql="select name from ".TBL_WB_USER." where account_id = '".$result."'";
		$rs = $wangpu_db -> getOne($sql);
		if($rs){
			echo $rs;
		}else{
			exit("503");
		}
	}
}else{
	exit("505");
}
		
?>