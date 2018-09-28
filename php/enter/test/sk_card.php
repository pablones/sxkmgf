<?php
/**
 * 卡片表生成
 */

define('IN_ECS', true);
require('../includes/init.php');

if($_REQUEST['act'] == 'create'){
	
	$sql  = "CREATE TABLE `sk_tpcard` (";
	$sql .= "`Id` int(11) NOT NULL auto_increment,";
	$sql .= "`newcard` char(64) NOT NULL default '0',";
	$sql .= "`picked` int(4) unsigned zerofill NOT NULL default '0000',";
	$sql .= "PRIMARY KEY  (`Id`),";
	$sql .= "UNIQUE KEY `newcard` (`newcard`)";
	$sql .= ") ENGINE=MyISAM  DEFAULT CHARSET=big5;";
	
	$acc_db -> query($sql);
	print_r("success");
}

?>