<?php

//平台通用文件 ptcommon.php
require_once dirname( dirname(__FILE__) ).'./common.php';
require_once S_ROOT.'./yjjhpt/ptconfig.php';

require_once S_ROOT.'./yjjhpt/class.pt_summary.php';
require_once S_ROOT.'./yjjhpt/class.pt_user.php';

//链接数据库
dbconnect();


//安全输出信息,同时关闭数据库
function sexit($msg = ''){
  $GLOBALS['db']->close();
  exit($msg);
  }

?>