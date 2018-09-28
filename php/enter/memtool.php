<?php
/**
 * memcache 测试工具
 * @var unknown_type
 */
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');

$host='127.0.0.1';
$port=11211;
$mem=new Memcache();
$mem->connect($host,$port);
if($_REQUEST['act'] == ''){
	$items=$mem->getExtendedStats ('items');
	$items=$items["$host:$port"]['items'];
	if($items){
		foreach($items as $key=>$values){
			$number=$key;;
			$str=$mem->getExtendedStats ("cachedump",$number,0);
			$line=$str["$host:$port"];
			if( is_array($line) && count($line)>0){
				foreach($line as $key=>$value){
					$result[$key] = $mem -> get($key);
				}
			}
		}		
	}
	$smarty -> assign('result' , $result);
	$smarty -> display('memtool.dwt');
}
if($_REQUEST['act'] == 'delete'){
	$key = $_REQUEST['key'];
	$mem -> delete($key);
	$items=$mem->getExtendedStats ('items');
	$items=$items["$host:$port"]['items'];
	if($items){
		foreach($items as $key=>$values){
			$number=$key;;
			$str=$mem->getExtendedStats ("cachedump",$number,0);
			$line=$str["$host:$port"];
			if( is_array($line) && count($line)>0){
				foreach($line as $key=>$value){
					$result[$key] = $mem -> get($key);
				}
			}
		}		
	}
	$smarty -> assign('result' , $result);
	$smarty -> display('memtool.dwt');
}
if($_REQUEST['act'] == 'flush'){	
	$mem -> flush();
	$items=$mem->getExtendedStats ('items');
	$items=$items["$host:$port"]['items'];
	if($items){
		foreach($items as $key=>$values){
			$number=$key;;
			$str=$mem->getExtendedStats ("cachedump",$number,0);
			$line=$str["$host:$port"];
			if( is_array($line) && count($line)>0){
				foreach($line as $key=>$value){
					$result[$key] = $mem -> get($key);
				}
			}
		}		
	}
	$smarty -> assign('result' , $result);
	$smarty -> display('memtool.dwt');
}
?>