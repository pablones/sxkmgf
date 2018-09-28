<?php

define('VERUPDATE_PW','yjjhxf');

require_once dirname(dirname(dirname(__FILE__))).'./common.php';

$filename = 'data/ver.txt';
$filepath = S_ROOT.'./'.$filename;
if(!file_exists($filepath)) swritefile($filepath,date("Ymd.").rand(1000,9999));

if(submitcheck()){
  $password = shtmlspecialchars( trim( $_POST['psw'] ) );
  if(empty($password)) jump('?'.$_SG['timestamp'],'密码不能为空！');
  if($password != VERUPDATE_PW) jump('?'.$_SG['timestamp'],'密码填写错误！');
  $vercnt = date("Ymd").rand(1000,9999);
  $oldv=sreadfile($filepath);
  swritefile($filepath,$vercnt);
  $newv=sreadfile($filepath);
  jump('?'.$_SG['timestamp'],'版本更新成功！版本已从'.$oldv.'更新为'.$newv);
  }
else if(!empty( $_GET['psw']))
{
	$password = shtmlspecialchars( trim( $_GET['psw'] ) );
	if($password != VERUPDATE_PW) 
	{
		echo "密码错误";exit;
	}
	$vercnt = date("Ymd").rand(1000,9999);
	$oldv=sreadfile($filepath);
	swritefile($filepath,$vercnt);
	$newv=sreadfile($filepath);
	if($oldv==$newv)
	{
		$http=trim($_SERVER['HTTP_HOST']);
		if(eregi("4399",$http))
		{
			$http=explode("-",$http);
			$s="4399";
			$sid=substr($http[0],4);
		}
		else if(eregi("91wan",$http))
		{
			$http=explode("-",$http);
			$s="91wan";
			$sid=substr($http[0],4);
		}
		else if(eregi("kuaiwan",$http))
		{
			$http=explode("-",$http);
			$s="kuaiwan";
			$sid=substr($http[0],4);
		}
		else if(eregi("37wan",$http))
		{
			$http=explode(".",$http);
			$s="37wan";
			$sid=substr($http[0],1);
		}
		else if(eregi("5awan",$http))
		{
			$http=explode("-",$http);
			$s="5awan";
			$sid=substr($http[0],4);
		}
		else if(eregi("360",$http))
		{
			$http=explode(".",$http);
			$s="360";
			$sid=substr($http[0],1);
		}
		else if(eregi("aosheng",$http))
		{
			$http=explode("-",$http);
			$s="aosheng";
			$sid=substr($http[0],4);
		}
	    else if(eregi("kuwo",$http))
		{
			$http=explode("-",$http);
			$s="kuwo";
			$sid=substr($http[0],1);
		}
		 else if(eregi("is",$http))
		{
			$http=explode("-",$http);
			$s="is";
			$sid=substr($http[0],1);
		}
		 else if(eregi("woyo",$http))
		{
			$http=explode("-",$http);
			$s="woyo";
			$sid=substr($http[0],4);
		}
		else if(eregi("fengxing",$http))
		{
			$http=explode("-",$http);
			$s="fengxing";
			$sid=substr($http[0],1);
		}
		else if(eregi("TW",$http))
		{
			$http=explode("-",$http);
			$s="TW";
			$sid=substr($http[0],4);
		}
		else if(eregi("PPTV",$http))
		{
			$http=explode("-",$http);
			$s="PPTV";
			$sid=substr($http[0],1);
		}
		else if(eregi("05wan",$http))
		{
			$http=explode("-",$http);
			$s="05wan";
			$sid=substr($http[0],1);
		}
		else if(eregi("FL",$http))
		{
			$http=explode("-",$http);
			$s="FL";
			$sid=substr($http[0],1);
		}
		echo $s.$sid."服更新版本失败，版本号：".$oldv."<br />";
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
<h1>御剑江湖-AS资源 版本自动更新：</h1>
<table width="100%" border="1">
  <tr>
    <th width="120" nowrap="nowrap">当前版本</th>
    <td><?php echo $vercnt;?></td>
  </tr>
  <tr>
    <th nowrap="nowrap">上次版本更新时间</th>
    <td><?php echo $verdate;?></td>
  </tr>
  <tr>
    <th nowrap="nowrap">手动更新版本</th>
    <td><form id="form1" name="form1" method="post" action="">
      <table width="100%" border="1">
        <tr>
          <td width="60">密码</td>
          <td><label>
            <input name="psw" type="text" id="psw" />
            <input type="submit" name="submit" value="提交" />
            <input name="formhash" type="hidden" id="formhash" value="<?php echo formhash()?>" />
          </label>
          <em>版本文件txt <a href="<?php echo $_Cfg['rooturi'].$filename?>" target="_blank">ver.txt</a></em>
          </td>
          </tr>
      </table>
      </form></td>
  </tr>
</table>
</body>
</html>
