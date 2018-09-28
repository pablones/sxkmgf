<?php
define('IN_ECS', true);
require('../includes/init.php');

//删除客户端缓存
if($_REQUEST['type']=='ver'){
	$file_path = ROOT_PATH."data/upload/ver.txt";
    @unlink($file_path);
    $verfile = ROOT_PATH.'data/upload/ver.txt';
    swritefile($verfile,date("Ymd").rand(1000,9999));
    print_r("success");
    exit();
}

//删除服务端缓存
if($_REQUEST['type']=='server'){
    clear_all_files();
    print_r("success");
    exit();
}


?>