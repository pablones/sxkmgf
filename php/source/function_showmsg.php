<?php
if(!defined("IN_ZBPHP")) die("Access die!");

if(empty($msg) && !empty($gourl))
{
  header("Location: $gourl");  exit();
  }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php if(isset($GLOBALS['_Cfg']['sitename'])) echo $GLOBALS['_Cfg']['sitename'];?> 消息 - powered by ZBPHP.COM</title>
<style type="text/css">
<!--
body {	margin: auto;	width: 680px;	padding-top: 60px;	font-family: Verdana, Arial, Helvetica, sans-serif;	font-size: 13px;}
h1 { font-size: 15px; padding-bottom: 8px; border-bottom: solid 2px #EEE; }
.copyright { text-align:right;}
-->
</style>
</head>

<body>
<div id="content">
  <h1>提示信息:</h1>
  <div class="msg">
    <p id="msg_cnt"><?php echo $msg;?></p>
    <p id="msg_link"><span id="sec">5</span>秒后自动跳转到<span id="link"></span>...</p>
    </div>
</div>
<script language="javascript" type="text/javascript" src="<?=$_Cfg['rooturi']?>source/js/jquery.min.js"></script>
<script language="javascript" type="text/javascript" src="<?=$_Cfg['rooturi']?>source/js/jquery.corner.js"></script>
<script language="javascript" type="text/javascript" src="<?=$_Cfg['rooturi']?>source/js/common.js"></script>
<script language="javascript" type="text/javascript">
//<![CDATA[
var onlymsg = <?php echo $onlymsg == 1 ? 1:0 ;?>;//只提示信息
var gourl = '<?php echo $gourl; ?>';
var sec = <?=$sec?>; //倒计时
if(gourl == ''){
  var a = document.createElement('a');
  a.href = document.referrer;
  gourl = a.host == window.location.host ? a.href:'http:\/\/'+window.location.host;
  }

//只提示内容
if(onlymsg == 1){
  $('#msg_link').empty();
}else{
  $('#link').html('<a href="'+gourl+'">'+gourl+'<\/a>');
  var tm = window.setInterval(function(){
    $('#sec').html(sec);
    sec--;
    if(sec == 0){
      window.location = gourl;
      window.clearInterval(tm);
      };
    },1000);
  }
//]]>
</script>
<?php
if(!defined('IN_ZBPHP_ADMCP')) echo $_Cfg['_tj'];
?>
</body>
</html>
