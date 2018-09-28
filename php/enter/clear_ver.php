<?php
/**
 * 
 * 清除缓存应急工具
 * 当全服清缓存失效时候用
 * @author King.L
 */
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');
require (dirname(__FILE__) . '/class/FileUtil.php');

if($_REQUEST['type']=='ver'){
	$file_path = ROOT_PATH."data/upload/ver.txt";
    $rs=swritefile($file_path,date("Ymd").rand(1000,9999));
    if($rs){
    	print_r("success");
    }else{
    	print_r("false");
    }
    exit();
}


?>