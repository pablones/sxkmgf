<?php
define('IN_ECS', true);
require('../includes/init.php');
if($_REQUEST['act'] == 'create'){
	$sql =  "create table `acc_linelog`(";
	$sql .= "`logid` int(4) unsigned not null auto_increment,";
	$sql .= "`time` int(4) unsigned not null default '0',";
	$sql .= "`count_online` int(4) unsigned not null,";
	$sql .= "`line` varchar(20) not null default '',";
	$sql .= "primary key (`logid`),";
	$sql .= "unique key `servertime` (`time`,`line`),";
	$sql .= "key `time`(`time`)";
	$sql .=")engine=myisam default charset = gbk ";
	$acc_db -> query($sql);
	print_r("success");
}
?>