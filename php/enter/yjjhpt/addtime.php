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

		  //δ����,��������Ϊoln,��ֹ���˲³�����[oln = online]
		  ssetcookie( $ptuser->cookieid.'[stat]','oln');
		
		  //������ <PT_UPDATEMIN:Ԥ��3����>
		  if($ptuser->timestamp - $ptuser->firstactive < PT_UPDATEMIN) sexit('400');
		
		  $userdaylogid = $ptuser->ckUserdaylog();
		
		  if($ptuser->fromlogin == 1 && $userdaylogid > 0){
			$sql = "update @#__userdaylog set count_login=count_login+1 where logid='$userdaylogid' limit 1 ";
			$db->query($sql);
			//�Ƿ���4399������
			if($db->affected_rows()) ssetcookie($ptuser->cookieid.'[fromlogin]','not');
			}
		
		  $ptuser->writeSession();
		  //��һ����Ӧʱ��
		  ssetcookie($ptuser->cookieid.'[ftime]',$ptuser->timestamp);
		
		  }else{
		
			//��־Ϊ<����><����>
			ssetcookie( $ptuser->cookieid.'[stat]','lss');
		
			//����,�ر���ҳ,COOKIE����Ϊlss,��ʶ����[lss = loss]
			if($ptuser->timestamp - $ptuser->lastactive < PT_UPDATEMIN) sexit('400');
		
			//����Լ���SESSION,���ұ�־Ϊ����
			$ptuser->clearSession();
		  }
		
		$db->close();
		ssetcookie($ptuser->cookieid.'[ltime]',$ptuser->timestamp);
		sexit('200');
	}
?>