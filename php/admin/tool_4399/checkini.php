<?php

define('VERUPDATE_PW','yjjhxf');

require_once dirname(dirname(dirname(__FILE__))).'./common.php';

$filename = 'Test.ini';
$filepath = S_ROOT.'./admin/'.$filename;
$url="http://yjjh-r.my4399.com/admin/tool_4399/show.php";
$initxt=gethttpcnt($url);
$site = $_POST['site'];
$tmpstr = "http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'];
if(submitcheck())
{
  $password = shtmlspecialchars( trim( $_POST['psw'] ) );
  if(empty($password)) jump('?'.$_SG['timestamp'],'密码不能为空！');
  if($password != VERUPDATE_PW) jump('?'.$_SG['timestamp'],'密码填写错误！');

	  if(swritefile($filepath,$initxt))
	  {
		  
	    $str = $site.$tmpstr."更新成功！<br />";
		$file_path = dirname(__FILE__)."/failed.php";
		file_put_contents($file_path,$str);
	  }
	  else{
	  	$file_path = dirname(__FILE__)."/failed.php";
		$str = $site.$tmpstr."服更新ini失败(如果该服已撤销，则无需理会)<br />";
		file_put_contents($file_path,$str);  
		  }
      



  jump('?'.$_SG['timestamp'],'版本更新成功！');
  }
else if(!empty( $_GET['psw']))
{

	$password = shtmlspecialchars( trim( $_GET['psw'] ) );
	if($password != VERUPDATE_PW) 
	{
		echo "密码错误";exit;
	}
	$newv=swritefile($filepath,$initxt);
	if(!$newv)
	{
		echo '2';
//		echo json_encode($_SERVER['HTTP_HOST']."服更新ini失败（如果该域名不存在，则无需理会）<br />");
	}
	else{
		echo '1';
	}
	exit;
}
$vercnt = sreadfile($filepath);
$verdate = date("Y-m-d H:i:s",filemtime($filepath));

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>更新版本</title>
<style type="text/css">
<!--
body {	font-family: Arial, Helvetica, sans-serif;	font-size: 17px;	margin: auto;	width: 1100px;}
h1 {	font-size: 19px;}
ul li {	margin: 5px;}
th,td {	font-size: 17px;	border: 1px solid #EEEEEE;	padding: 5px;}
th {	background-color: #EEE;	text-align: left;}
#form1 #psw {	font-size: 17px;}
-->
</style>
</head>

<body>
<h1>御剑江湖-ini配置 自动更新：</h1>
<table width="100%" border="1">
  <tr>
    <th nowrap="nowrap">手动更新版本</th>
    <td><form id="form1" name="form1" method="post" action="">
      <table width="100%" border="1">
	  <tr>
    <th nowrap="nowrap">上次版本更新时间</th>
    <td><?php echo $verdate;?></td>
  </tr>
        <tr>
          <td width="60">密码</td>
          <td><label>
            <input name="psw" type="text" id="psw" />
            <input type="submit" name="submit" value="提交" />
            <input name="formhash" type="hidden" id="formhash" value="<?php echo formhash()?>" />
          </label>
          <em>版本文件txt <a href="<?php echo $filepath?>" target="_blank"><?=$filename?></a></em>
          </td>
          </tr>
      </table>
      </form></td>
  </tr>
</table>
</body>
</html>
