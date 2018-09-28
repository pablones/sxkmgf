<?php

require_once '../common.php';
$_SCOOKIE['4399uid'] = authcode($_SCOOKIE['4399uid'],'DECODE');


if(empty($_SCOOKIE['username']) || $_SCOOKIE['userid'] < 1 || $_SCOOKIE['4399uid'] < 1){
  //$jumpurl = 'http://web.4399.com/stat/togame_yjjh.php?target=yjjh&server_id=S1&a=good4399';
  $jumpurl = './?msg=please_login_first';
  header("HTTP/1.1 301 Moved Permanently");
  header("Location: $jumpurl");
  exit();
  }

foreach($_SCOOKIE as $key => $val){
  ssetcookie($key, '', -86400 * 365);
  }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>退出成功</title>
</head>

<body>
  <h1>账号退出成功，<a href="./">点击这里进行登录！</a></h1>
</body>
</html>