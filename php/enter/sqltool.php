<?php

define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');
if(!isset($_REQUEST['act'])){
	$_REQUEST['act'] = '';
	
}

if($_REQUEST['act'] == ''){
	$smarty -> display('sqltool.dwt');	
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
	$smarty -> assign('view' ,$view);
	$smarty -> assign('result',$result);
	$smarty -> display('sqltool.dwt');
	
}

if($_REQUEST['act'] == 'show_status'){
	$sql = "show variables like 'max_connections'";
	$max_connections = $acc_db -> getRow($sql); //最大连接数
	$smarty -> assign('max_connections', $max_connections);
	
	$sql = "show global status like 'max_used_connections'";
	$max_used_connections = $acc_db -> getRow($sql); //响应连接数
	$smarty -> assign('max_used_connections' , $max_used_connections);
	
	$connection_rate = $max_used_connections['Value'] / $max_connections['Value'];
	$smarty -> assign('connection_rate',$connection_rate);
	
	$sql = "show variables like 'key_buffer_size'";
	$key_buffer_size = $acc_db -> getRow($sql); 
	$smarty -> assign('key_buffer_size' , $key_buffer_size);
		
	$sql = "show global status like 'key_read%'";
	$key_read = $acc_db -> getAll($sql); //索引读取请求
	$key_read_rate = $key_read[1]['Value'] / $key_read[0]['Value'];
	$smarty -> assign('key_read_rate' , $key_read_rate);
	$smarty -> assign('key_read' , $key_read);
		
	$sql = "show global status like 'key_blocks_u%'";
	$key_blocks_u = $acc_db -> getAll($sql); //缓存簇族数
	$key_blocks_rate = $key_blocks_u[1]['Value'] / ($key_blocks_u[0]['Value'] + $key_blocks_u[1]['Value']);
	$smarty -> assign('key_blocks_rate' ,$key_blocks_rate);
	$smarty -> assign('key_blocks_u' , $key_blocks_u);
		
	$sql = "show global status like 'open%tables%'";
	$opentables = $acc_db -> getAll($sql); 
	$smarty -> assign('opentables' , $opentables);
		
	$sql = "show variables like 'table_cache'";
	$table_cache = $acc_db -> getRow($sql);	
	$smarty -> assign('table_cache' , $table_cache);
	
	$open_table_rate = $opentables[0]['Value'] / $opentables[1]['Value'];
	if(!empty($table_cache)){
		$table_rate = $opentables[0]['Value'] / $table_cache['Value'];
		
	}
	$smarty -> assign('open_table_rate' , $open_table_rate);
	$smarty -> assign('table_rate' , $table_rate);
	
	$sql = "show global status like 'Thread%'"; //进程使用情况
	$thread = $acc_db -> getAll($sql);

	$smarty -> assign('thread' , $thread);

	$sql = "show variables like 'thread_cache_size'"; 
	$thread_cache_size = $acc_db -> getRow($sql);

	$smarty -> assign('thread_cache_size' , $thread_cache_size);
	
	$sql = "show global status like 'table_locks%'";
	$table_locks = $acc_db -> getAll($sql);	
	$smarty -> assign('table_locks' , $table_locks);
	

	$smarty -> display('MySQLstatus.dwt');
	
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