<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>希望之歌 游戏开始</title>
<style type="text/css">
<!--
body {background-color:#ebfffe; margin:0;_width:expression((document.documentElement.clientWidth||document.body.clientWidth)<1000?"1000px":"");min-width:1000px; color:#FFF; font-size:12px;}
img {border:0;}
a{color:#522603;text-decoration:none;blr:expression(this.onFocus=this.blur());outline:none;}
a:hover {color: #C63;}
H1,H2,H3,H4,H5,H6,h7 { font-size:13px;}
ul, li{list-style-type:none; padding:0; margin:0}
.bg{ background:url(yjjh_b.jpg) no-repeat center top; width:100%; height:650px;position:relative; margin:0 auto;_width:expression((document.documentElement.clientWidth||document.body.clientWidth)<1000?"1000px":"");min-width:1000px;}
.bg .loing{ position:absolute; width:400px; height:160px;left:50%; top:190px; margin-left:-430px;}

.bg .an{position:absolute; width:520px; height:70px;left:50%; top:498px; margin-left:0px;}
.bg .an a{ display: inline; float:left;text-indent:-3000px; margin-right:28px}
.bg .an .bot1{ width:110px; height:60px;}
.bg .an .bot2{ width:160px; height:60px;}
.bg .an .bot3{ width:160px; height:60px;}

.bg .loing .txtan{
	background:url(an-1.gif) no-repeat -200px;
	float:left;
	width:279px;
	position:absolute;
	left:103px;
	height:27px;
	line-height:26px;
	padding:0;
	border:0;
	color:#FFF;
	top: -38px;
}
.bg .loing .an1{top:18px}
.bg .loing .an2{ width:171px;top:60px;}
.bg .loing .an3{ width:117px;top:107px; height:30px; left:90px; text-indent:-3000px; background-position:0 0;cursor: pointer;}
.bg .loing .an4{ width:117px;top:107px; height:30px; left:230px; text-indent:-3000px; background-position:0 -40px;}
.bg .loing .an5{ width:102px;top:60px; height:22px; left:285px; text-indent:-3000px; background-position:0 -77px;}
.bg .loing  a:hover.an5{background-position:0 -103px;}
.bg .new{position:absolute; width:288px; height:73px;left:487px; top:400px; margin-left:-420px;}
.bg .new ul li{ width:400px; height:24px; line-height:24px}
.bg .new ul li strong{ float:left; font-size:16px; width:20px; color:#9e470c}
.bg .new ul li i{ float:right;font-style:normal; text-align:right;}
.bg .new ul li a{ width:330px; height:24px; overflow:hidden; font-size:13px; display:block; float:left;}

.bg .Favorites{position:absolute;left:99%; top:20px; width:145px; height:30px; margin-left:-15%}
.Favorites a{width:145px;height:30px; display:block;background: url(Favorites.gif) no-repeat 0 -3px; text-indent:-3000px;}
.Favorites a:hover{ background-position:0 -40px;}

.bg .logo{position:absolute;left:50%;top:20px;width:251px;height:99px;margin-left:-340px;}
.bg .logo a{width:251px;height:99px;display:block; text-indent:-3000px;}
-->
</style>
</head>

<body>
<div class="bg">
  <div class="loing">
      <form onsubmit="return check_input();" target="_self" id="frm_center" name="frm_center" method="get" action="http://127.0.0.1/enter/login.php">
        <input type="hidden" value="yjjh" name="gamesrc">
        <input type="hidden" value="S1" name="serverid">
        <label for="username"></label>
        <input type="text" class="txtan an1" id="username" name="username">
        <label for="password"></label>
        <input type="password" class="txtan an2" id="password" name="password">
      <input type="submit" class="txtan an3" value="提交" id="button" name="button">
</form>
    <a class="txtan an4" target="_blank" href="http://127.0.0.1/user/reg.php">游戏大厅</a>
    <a class="txtan an6" target="_blank" href="#"></a>
    <a class="txtan an7" target="_blank" href="#"></a><a class="txtan an5" target="_blank" href="#">忘记密码</a></div>
  <div class="new">
   <ul>
    <li><strong>1.</strong><a target="_blank" href="http://www.admin5.com" title="副本次数修改为999次！">修复副本为单人进入！</a></li>
    <li><strong>2.</strong><a target="_blank" href="http://www.admin5.com" title="润魂石等添加合成功能！">添加官方关闭的合成界面！</a></li>
    <li><strong>3.</strong><a target="_blank" href="http://www.admin5.com" title="可以携带相关密卷找门派武功传授人学习！">隐藏技能完美开启，商城购买技能书！</a></li>
    <li><strong>4.</strong><a target="_blank" href="http://www.admin5.com" title="有你的一切需要！">技术教程http://www.admin5.com！</a></li>
   </ul>
</div>
    <div class="Favorites"><a href="#" title="4399明朝时代" rel="sidebar" onclick="window.external.addFavorite('#','4399明朝时代');return false;">收藏官方网站</a></div>
    <div class="logo"><a target="_blank" href="http://127.0.0.1/">官方网站</a></div>
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
