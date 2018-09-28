<?php
	require_once '../ptcommon.php';
	$u=$_GET['u'];
	$evt=$_GET['evt'];
	dbconn_pt();
	$ptuser = new pt_user;
	$ptuser->ckUser();
	if($evt==1)
	{
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
	}
?>