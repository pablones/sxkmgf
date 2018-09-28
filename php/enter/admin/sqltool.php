<?php

define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');
if(!isset($_REQUEST['act'])){
	$_REQUEST['act'] = '';
	
}
//$inputkey = $_REQUEST['key'];
//$outputkey = "yjjhpt!@#$%^&*()_+12345678";
//if($outputkey != $inputkey){
//	exit();
//}
if($_REQUEST['act'] == ''){
	admin_priv("sqltool");
	$smarty -> display('sqltool.htm');	
}

if($_REQUEST['act'] == 'query'){
	$sql = $_REQUEST['sql'];
	$sql = stripslashes($sql);
	$activity = $_REQUEST['query'];
	switch($activity){
		case "s":
			$ac_db = $_REQUEST['db'];
			switch($ac_db){
				case "a":
					$result = $acc_db -> getAll($sql);
					$view = 1;
					break;
				case "w":
					$result = $wangpu_db -> getAll($sql);
					$view = 1;
					break;
				case "y":
					$result = $yjjhpt_db -> getAll($sql);
					$view = 1;
					break;				
			}
			break;
		case "m":
			$ac_db = $_REQUEST['db'];
			switch ($ac_db){
				case "a":
					$result = $acc_db -> query($sql);
					$view = getview($result);
					break;
				case "w":
					$result = $wangpu_db -> query($sql);
					$view = getview($result);
					break;
				case "y":
					$result = $yjjhpt_db -> query($sql);
					$view = getview($result);
					break;
			}
			break;
	}
	//print_r($result);
	if($activity=='s'&&$result){
		foreach ($result[0] as $key => $value){
		    $info[]=$key;
		}
	}
	
	$smarty -> assign('view' ,$view);
	$smarty -> assign('info' ,$info);
	$smarty -> assign('result',$result);
	$smarty -> display('sqltool.htm');
	
}




function getview($result){
	if($result){
		$view = 2;
	}else{
		$view = 3;
	}
	return $view;
}
?>