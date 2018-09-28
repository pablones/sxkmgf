<?php


require_once dirname(dirname(dirname(__FILE__))).'./common.php';

$_SCOOKIE['4399uid'] = authcode($_SCOOKIE['4399uid'],'DECODE');

if(!empty($_SCOOKIE['username']) && $_SCOOKIE['userid'] > 0 && $_SCOOKIE['4399uid'] > 0){
  $jumpurl = '../4399/play.php';
  header("HTTP/1.1 301 Moved Permanently");
  header("Location: $jumpurl");
  exit();
  }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>御剑江湖 游戏开始</title>
<style type="text/css">
<!--
body {background-color:#000; margin:0;_width:expression((document.documentElement.clientWidth||document.body.clientWidth)<1000?"1000px":"");min-width:1000px; color:#FFF; font-size:12px;}
img {border:0;}
a{color:#fff;text-decoration:none;blr:expression(this.onFocus=this.blur());outline:none;}
a:hover {color: #F90;}
H1,H2,H3,H4,H5,H6,h7 { font-size:13px;}
ul, li{list-style-type:none; padding:0; margin:0}
.bg{ background:url(mcsd_b.jpg) no-repeat center top; width:100%; height:650px;position:relative; margin:0 auto; _width:expression((document.documentElement.clientWidth||document.body.clientWidth)<1000?"1000px":"");min-width:1000px;}
.bg .loing{ position:absolute; width:380px; height:190px;left:50%; top:247px; margin-left:-290px;}
.bg .an{position:absolute; width:500px; height:60px;left:50%; top:510px; margin-left:-480px;}
.bg .loing .txtan{ background: url(an.gif) no-repeat -200px; float:left; width:150px; position:absolute; left:123px; height:22px; line-height:22px; padding:0; border:0;}
.bg .loing .an1{top:16px}
.bg .loing .an2{ width:150px;top:60px;}
.bg .loing .an3{width:127px;top:101px;height:34px;left:125px;text-indent:-3000px;background-position:0 0;cursor: pointer;}
.bg .loing .an4{width:80px;top:156px;height:20px;left:50px;text-indent:-3000px;background-position:5px -55px;}
.bg .loing a:hover.an4 {background-position:-89px -55px;}
.bg .loing .an6{width:80px;top:156px;height:20px;left:145px;text-indent:-3000px;background-position:5px -94px;}
.bg .loing a:hover.an6 {background-position:-89px -94px;}
.bg .loing .an7{width:80px;top:156px;height:20px;left:240px;text-indent:-3000px;background-position:5px -131px;}
.bg .loing a:hover.an7{background-position:-89px -131px;}
.bg .loing .an5{width:60px;top:60px;height:20px;left:293px;}
.bg .new{position:absolute; width:260px; height:80px;left:50%; top:486px; margin-left:-450px;}
.bg .new ul li{ width:300px; height:24px; line-height:24px}
.bg .new ul li strong{ float:left; font-size:16px; width:20px; color:#F90}
.bg .new ul li i{ float:right;font-style:normal; text-align:right;}
.bg .new ul li a{ width:240px; height:24px; overflow:hidden; display:block; float:left;}

.bg .Favorites{position:absolute;left:99%; top:20px; width:145px; height:30px; margin-left:-15%}
.Favorites a{width:145px;height:30px; display:block;background: url(Favorites.gif) no-repeat 0 -3px; text-indent:-3000px;}
.Favorites a:hover{ background-position:0 -40px;}

.bg .logo{position:absolute;left:50%;top:54px;width:201px;height:99px;margin-left:-470px;}
.bg .logo a{width:201px;height:99px;display:block; text-indent:-3000px;}
-->
</style>
</head>

<body>
<div class="bg">
    <div class="loing">
      <form onsubmit="return check_input();" target="_self" id="frm_center" name="frm_center" method="post" action="http://web.4399.com/user/login_actions.php">
        <input type="hidden" value="yjjh" name="game_src">
        <input type="hidden" value="S1" name="server_id">
        <input type="hidden" value="http://web.4399.com/stat/togame_yjjh.php?target=yjjh&server_id=S1&a=good4399" name="topage">
        <label for="username"></label>
        <input type="text" class="txtan an1" id="username" name="username">
        <label for="password"></label>
        <input type="password" class="txtan an2" id="password" name="password">
      <input type="submit" class="txtan an3" value="提交" id="button" name="button">
</form>
    <a class="txtan an4" target="_blank" href="http://web.4399.com/yjjh/">游戏大厅</a>
    <a class="txtan an6" target="_blank" href="http://web.4399.com/yjjh/">进入官网</a>
    <a class="txtan an7" target="_blank" href="http://web.4399.com/user/reg.php">注册账号</a>
    <a class="txtan an5" target="_blank" href="#">忘记密码</a>
    </div>
  <div class="new">
   <ul>
    <li><strong>1.</strong><a target="_blank" href="#" title="充值1+1，无限量领天外陨石">充值1+1，无限量领天外陨石</a></li>
    <li><strong>2.</strong><a target="_blank" href="#" title="88服常胜诸葛17日震撼开启">88服常胜诸葛17日震撼开启</a></li>
    <li><strong>3.</strong><a target="_blank" href="#" title="激情八月、杀破狼惊现世！">激情八月、杀破狼惊现世！</a></li>
   </ul>
</div>
    <div class="Favorites"><a href="#" title="4399明朝时代" rel="sidebar" onclick="window.external.addFavorite('#','4399明朝时代');return false;">收藏官方网站</a></div>
    <div class="logo"><a target="_blank" href="http://web.4399.com/mcsd/">官方网站</a></div>
</div>
<script language="javascript" type="text/javascript">
function check_input(){
    if(document.forms['frm_center'].username.value != '' && document.forms['frm_center'].password.value != ''){
        return true;
    }else{
        alert("请检查您的用户名和密码！");
        return false;
    }
}
</script>
</body>
</html>
