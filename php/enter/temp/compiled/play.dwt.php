<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta name="Generator" content="ECSHOP v2.7.2" />
    <title>幻想物语 双线1区</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv=X-UA-Compatible content=IE=EmulateIE7>
    <link href="./favicon.ico" type="image/x-icon" rel="icon">
    <link href="./favicon.ico" type="image/x-icon" rel="shortcut icon">
    <script src="themes/game/js/swfobject.js" type="text/javascript"></script>
    <script src="themes/game/js/common.js" type="text/javascript"></script>
    <script src="themes/game/js/jquery.min.js" type="text/javascript"></script>
    <link href="themes/game/css/default2.css" rel="stylesheet" type="text/css" />
    <meta content="MSHTML 6.00.2900.5726" name=GENERATOR>
 </head>
 <style>
@charset "GB2312";
*{ margin: 0; padding: 0; font-size: 12px; line-height: 1.2; font-family: "\5b8b\4f53",Verdana,Tahoma, Arial, 

Helvetica, sans-serif; color:#666;}
html, body {height:100%;background-color: #000;}
body {color:#0FC;margin:0;padding:0;overflow:hidden;text-align:center; height: 100%; position: relative;background: 

url(themes/game//images/bg_wapper_qq.jpg) no-repeat center -36px;}
.container { padding: 0 10px 10px; margin: 0 auto; background: url(themes/game//images/bg_cont_qq.jpg) no-repeat center -

10px;}
#flashContent {margin: 0 auto 1px;width:1250px;height:710px;position:relative; z-index: 1;}
#flashContent2{height: 100%; width: 100%;}
#flashContent2 .flaTips { margin: 0 auto; width: 1002px; height: 600px;display: block; text-indent: -999em; 

overflow: hidden; white-space: nowrap; background: url(themes/game//images/flatips.jpg) no-repeat left top;}
#flashContent object,#flashContent embed {outline: none; display: block; height: 100%; width: 100%;}
.wrapper {  width: 100%; overflow: hidden; height: 35px; position: fixed; _position: absolute; left: 0; bottom: 0; 

_bottom: -1px;z-index: 0; text-align: left;background: #212121 url(bg_png_all.png) repeat-x left -132px; 

color:#999;}
.wrapper .inner {margin: 0 auto;width: 1000px; height: 35px;}
.wrapper .fl {width: 618px;float: left; overflow: hidden;}
.wrapper .nav a { width: 87px; height: 34px; line-height: 34px; white-space: nowrap; display: block; float: left; 

background: url(bg_png_all.png) no-repeat left -88px; text-indent: -99em; overflow: hidden; vertical-align: middle;}
.wrapper .nav .logo {width: 91px; background-position:-1px -88px;}
.wrapper .nav .a_py {background-position:left top;}
.wrapper .nav .a_wb {background-position:-87px top;}
.wrapper .nav .a_sq {background-position:-174px top;}
.wrapper .nav .a_kf {background-position:-261px top;}
.wrapper .nav .a_skylt {background-position:-348px top;}
.wrapper .nav .a_lwj {background-position:-437px top;}
.wrapper .nav .a_jc {width: 88px;background-position:-523px top;}
.wrapper .nav .logo:hover{background-position:-1px -88px;}
.wrapper .nav .a_py:hover {background-position:left -44px;}
.wrapper .nav .a_wb:hover {background-position:-87px -44px;}
.wrapper .nav .a_sq:hover {background-position:-174px -44px;}
.wrapper .nav .a_kf:hover {background-position:-261px -44px;}
.wrapper .nav .a_skylt:hover {background-position:-348px -44px;}
.wrapper .nav .a_lwj:hover {background-position:-437px -44px;}
.wrapper .nav .a_jc:hover {background-position:-523px -44px;}
.wrapper .fr { padding: 10px 5px 0 0; height: 24px;width: 375px;float: right; overflow: hidden; background: url

(bg_png_all.png) no-repeat right -186px;}
.wrapper .fr p {text-align:right; color:#ccc;}

</style>
<body onResize="resize();">
<div>
<?php echo $this->fetch('library/play_header.lbi'); ?>
<div id="flashobjdiv">
<h1><?php echo $this->_var['lang']['play_msg1']; ?></h1><br/>
<p><a href="http://www.adobe.com/go/getflashplayer"><?php echo $this->_var['lang']['play_msg2']; ?></a></p>
</div>
<script type=text/javascript>
	var site = <?php echo $this->_var['json']; ?>;
	var swfurl = '<?php echo $this->_var['swfurl']; ?>';
	var so = new SWFObject(swfurl, "yjjh", document.body.clientWidth, document.body.clientHeight-35, "9", "#000000");
	
	so.useExpressInstall('expressinstall.swf');
	so.addParam('allowScriptAccess','always');
	for(var i in site){
		so.addVariable(i,site[i]);
	}
	so.write("flashobjdiv");
	var isfullscreen = false;

	function intofullscreen(){
    		document.getElementById('contentwrap').style.display = "none";
		document.getElementById('yjjh').width="100%";
		document.getElementById('yjjh').height="100%";
		isfullscreen = true;
	}	
	function exitfullscreen(){			
   		document.getElementById('contentwrap').style.display = "block";
		document.getElementById('yjjh').width=1000;
		document.getElementById('yjjh').height=580;
		isfullscreen = false;
	}
	
	function addfavor(url,title){
  		if(!url) url = "<?php echo $this->_var['serverwebpage']; ?>";
  		if(!title) title = "<?php echo $this->_var['lang']['page_title']; ?><?php echo $this->_var['serverid']; ?><?php echo $this->_var['lang']['play_msg6']; ?> -- <?php echo $this->_var['servername']; ?>";
		 try{
			window.external.AddFavorite(url, title);
		}catch (e){
			try{
				window.sidebar.addPanel(title, url, "");
			}catch (e){
				alert("在游戏页面按下CTRL+D即可收藏游戏");
			}
		}
		
	} 


	
	function createrolenum(){		
		$.ajax({
			type:'post',
			data:'username='+site.username+'&userid='+site.userid,
			url:site.rooturi+'enter/yjjhpt/createrolenum.php'
		});				
	}
	

	function joingamenum(){  	
		$.ajax({
			type:'post',
			data:'username='+site.username+'&userid='+site.userid,
			url:site.rooturi+'enter/yjjhpt/joingamenum.php'
		});
	}
	
	function loadgame(){
		document.body.style.backgroundImage="url(images/playbg.png)";	
		
		
	}

	function loadGameTitle(){
		$('#contentwrap').css("visibility","visible"); 
	}
	
	
	//防沉迷
	function fcmfuck(username){
		$.ajax({
			type:'post',
			data:'username=' + username,
			url:site.rooturi+'enter/yjjhpt/fcm.php'
		});		
	}
	
	function resize(){
		if(isfullscreen==true){
			document.getElementById('flashobj').height = document.body.clientHeight - 20;
		}
	}
</script>
<script type=text/javascript>
var googleDivIsShow = -1;
var lastZkey = 0;
var trueTitle = document.title;
var keyIdArr = [112,113,114,115,116,117,118,119,9,77,27,13,49,50,56,192];
function isFB(k){
	for(var i in keyIdArr){
		if(keyIdArr[i] == k){
			return true;
		}
	}
	return false;
}
//双击Z键(keycode == 90)就执行
function showGoogle(currkeycode){
	if(currkeycode != 90) return;
  	//判断双击
  	var timestamp = (new Date()).valueOf();
  	if(timestamp - lastZkey > 450){lastZkey = timestamp;  return;}
  	lastZkey = timestamp;
  	//处理老板键
	
	
  	if(googleDivIsShow == 1){
    		$('#contentwrap').hide(1,function(){			
 	    		document.body.style.background="url(images/baidu.gif)";
			document.body.style.backgroundAttachment="fixed";
			document.body.style.backgroundColor="#ffffff";
			document.body.style.backgroundRepeat="no-repeat";
			document.body.style.backgroundPosition="center top";
      			googleDivIsShow = -1;
      			document.title = '百度一下，你就知道';
    		});
		$('#flashobjdiv').css('padding-top',1000);

   	}else{
    		$('#contentwrap').show(1,function(){
			document.body.style.backgroundImage="url(images/playbg.png)";
			document.body.style.backgroundColor="#000000";
			document.body.style.backgroundRepeat="repeat";
      			googleDivIsShow = 1;
      			document.title = trueTitle;
			$('#flashobjdiv').css('padding-top',0);
    		});

    		//document.getElementById('contentwrap').style.display = "block";
		//document.getElementById('flashobjdiv').style.display = "block";
	}
	
}
//IE keydown
function keyDown(){
	var val = window.event.keyCode;
  	var key = String.fromCharCode(window.event.keyCode);
  	showGoogle(val);
  	if(isFB(val)){
		//window.document.flashobj.SetVariable("currkeycode", val);
	  	window.event.keyCode = 0;
	  	window.event.cancelBubble = true;
	  	return false;
  	}
}
if(navigator.userAgent.indexOf('MSIE') != -1){
	document.onkeydown = keyDown;
  	window.onbeforeunload = function(){
    		var n = window.event.screenX - window.screenLeft;
    		var b = n > document.documentElement.scrollWidth-20;
    		if(b && window.event.clientY < 0 || window.event.altKey){
    			addfavor("<?php echo $this->_var['serverwebpage']; ?>",site.sitename);
    		}else{
			//是刷新而非关闭
    		}
	}
}else if(navigator.userAgent.indexOf('Mozilla') != -1){
	$(window).keydown(function(event){
  		var val = event.keyCode;
    		var key = String.fromCharCode(event.keyCode);
    		showGoogle(val);
    		if(isFB(val)){
  	  		//window.document.flashobj.SetVariable("currkeycode", val);
  	  		event.keyCode = 0;
  	  		event.cancelBubble = true;
  	  		event.preventDefault();
  	  		event.returnValue = false;
  	  		return false;
    		}
  	});
  	$(window).bind("beforeunload",function(){
    		if($.cookie('addfavor') != 1){
      			addfavor("<?php echo $this->_var['serverwebpage']; ?>",site.sitename);
      			$.cookie('addfavor',1);
    		}
  	});
}

function wresizeSwf(){
	var paddingtop = document.documentElement.clientHeight - $('#flashobjdiv').height() - $('#footerdiv').height();
	paddingtop = paddingtop < 1 ? 0 : paddingtop/2;
	document.getElementById('flashobjdiv').style.display = "block";
	$('#flashobjdiv').css('padding-top',paddingtop);
}

</script>

</div >
<div class="wrapper">
	<div class="inner" id="wrap_inner">
		<div class="fl">
			<p class="nav"><a class="logo">天空之舞</a><a class="a_py" onclick="dialog_invite('赶紧来玩天空之舞吧', 'http://app100662496.imgcache.qzoneapp.com/app100662496/web/game/logo_75x75.png');_hmt.push(['_trackEvent', 'nav', 'click', 'dialog_invite']);return false;" href="javascript:void(0);">邀请好友</a><a class="a_sq" onclick="addBookmark('http://rc.qzone.qq.com/100662496');_hmt.push(['_trackEvent', 'nav', 'click', 'addBookmark']);return false;" href="javascript:void(0);">添加收藏</a><a class="a_wb" target="_blank" onclick="_hmt.push(['_trackEvent', 'nav', 'click', 'weibo'])" href="http://t.qq.com/woyao_sky5">关注微博</a><a class="a_kf" onclick="javascript:window.open('http://b.qq.com/webc.htm?new=0&amp;sid=800036151&amp;eid=2188z8p8p8p8P8x8q8R8q&amp;o=&amp;q=7&amp;ref='+document.location, '_blank', 'height=544, width=644,toolbar=no,scrollbars=no,menubar=no,status=no');_hmt.push(['_trackEvent', 'nav', 'click', 'kefu']);return false;" href="javascript:void(0);">在线客服</a><a class="a_skylt" target="_blank" onclick="_hmt.push(['_trackEvent', 'nav', 'click', 'bbs'])" href="http://bbs.open.qq.com/forum.php?mod=forumdisplay&action=list&fid=677">进入论坛</a><a class="a_jc" href="/showFlashVersion.php" onclick="_hmt.push(['_trackEvent', 'nav', 'click', 'login_jc'])" target="_blank">登录教程</a></p>
		</div>
		<div class="fr">
			<p> 本应用由“游之蚁”提供   您的游戏ID：D768B217CDC5E70E780385</p>
		</div>
	</div>
</div>
</body>
</html>
