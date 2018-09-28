<?php
	require_once '../ptcommon.php';
	$u=$_POST['u'];
	$evt=$_POST['evt'];
	if(!empty($uri) && !empty($u) && !empty($evt))
	{
		$ret=gethttpcnt("http://127.0.0.1/youyue/yjjhpt/addtime?evt=".$evt."&u=".$u);
	}
	if($ret=="200")
	{
		echo 200;
	}
?>