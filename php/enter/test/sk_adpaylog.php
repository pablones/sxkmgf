<?php
/**
 * 对admin_action表的操作
 */
define('IN_ECS', true);
require('../includes/init.php');


$str  = "CREATE TABLE if not exists `sk_adpaylog` (";
$str .= "`logid` int(10) unsigned NOT NULL auto_increment,";
$str .= "`paynum` varchar(20) NOT NULL default '0' COMMENT '流水號，18位正整數',";
$str .= "`logdate` int(10) unsigned NOT NULL default '0' COMMENT '日志寫入時間戳',";
$str .= "`userid` int(10) NOT NULL COMMENT '玩家ID',";
$str .= "`ip` varchar(20) NOT NULL,";
$str .= "`serverid` varchar(20) NOT NULL COMMENT '游戲服標識，比如S1',";
$str .= "`paygold` int(10) unsigned NOT NULL default '0',";
$str .= "`paytype` int(5) default '1' COMMENT '后台充值类型1为元宝2为珠宝',";
$str .= "PRIMARY KEY  (`logid`),";
$str .= "UNIQUE KEY `paynum` (`paynum`)";
$str .= ") ENGINE=MyISAM  DEFAULT CHARSET=big5 COMMENT='后台充值記錄' AUTO_INCREMENT=10";
$acc_db -> query($str);
print_r("success");

?>