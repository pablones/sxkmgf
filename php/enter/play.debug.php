<?php

require_once '../common.php';
//error_reporting(E_ALL);
$_SCOOKIE['4399uid_val'] = authcode($_SCOOKIE['4399uid'],'DECODE');


if(empty($_SCOOKIE['username']) || $_SCOOKIE['userid'] < 1 || empty($_SCOOKIE['4399uid_val'])){
  if(!empty($_SERVER['HTTP_COOKIE'])){
    $logfile = S_ROOT.'./data/logfile/jump_'.date("Ymd").'.txt';
    $logcnt = date("Y-m-d H:i:s")."[".$_SG['ip']."]\r\n";
    $logcnt .= "    username: ".$_SCOOKIE['username']."\r\n";
    $logcnt .= "    userid  : ".$_SCOOKIE['userid']."\r\n";
    $logcnt .= "    4399uid : ".$_SCOOKIE['4399uid_val']."\r\n";
    $logcnt .= "    HTTP_COOKIE     :  ".$_SERVER['HTTP_COOKIE']."\r\n";
    $logcnt .= "    HTTP_HOST       :  ".$_SERVER['HTTP_HOST']."\r\n";
    $logcnt .= "    HTTP_USER_AGENT :  ".$_SERVER['HTTP_USER_AGENT']."\r\n";
    $logcnt .= "--------------------------------------------\r\n\r\n";
    swritefile($logfile,$logcnt,'a');
  }

  $jumpurl = 'http://web.4399.com/yjjh/?msg=please_login_first';
  jump($jumpurl,'对不起，您尚未登录，请先到官网登录！\r\n如果您已经登录了，却出现此提示，请到官方交流群 117431165 / 48336440 反应此问题！');
  exit();
  }

$serverid = $_SCOOKIE['serverid'];

if(!array_key_exists($serverid,$serverArr)) jump('http://web.4399.com/yjjh/?msg=serverid_error','游戏服务器'.$serverid.'服不存在!');

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>4399 《御剑江湖》<?php echo $serverArr[$serverid]['id'];?>服 -- <?php echo $serverArr[$serverid]['servername']?></title>
<link href="default.css" rel="stylesheet" type="text/css" />
<script language="JavaScript" type="text/javascript" src="../source/js/common.js"></script>
<script language="JavaScript" type="text/javascript" src="../source/js/swfobject.js"></script>
<script language="JavaScript" type="text/javascript" src="../source/js/jquery.min.js"></script>
<script language="JavaScript" type="text/javascript" src="../source/js/jquery.wresize.js"></script>
<script language="JavaScript" type="text/javascript" src="../source/js/jquery.cookie.js"></script>
<script language="javascript" type="text/javascript">
<!--
<?php

$verfile = S_ROOT.'./data/ver.txt';
if(!file_exists($verfile)) swritefile($verfile,date("Ymd").rand(1000,9999));
$ver = shtmlspecialchars( trim( sreadfile($verfile) ) );

$json = array(
        '4399uid'   => $_SCOOKIE['4399uid_val'],
        'userid'    => $_SCOOKIE['userid'],
        'username'  => $_SCOOKIE['username'],
        'currurl'   => '',
        'timestamp' => $_SCOOKIE['timestamp'],
        'serverid'  => $_SCOOKIE['serverid'],
        'swfver'    => $ver,                  //SWF版本
        'rooturi'   => $_Cfg['rooturi'],
        );
?>
var site = <?=json_encode($json)?>;
site.sitename = document.title;
site.siteurl = 'http:\/\/'+window.location.host;
site.currurl = encodeURIComponent(window.location.href); //经过encodeURIComponent编码
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
  if(timestamp - lastZkey > 450){ lastZkey = timestamp;  return; }
  lastZkey = timestamp;

  //处理老板键
  if(googleDivIsShow == 1){
    $('#contentwrap').hide(1,function(){
      $('body').removeClass('bodyshow');
      $('body').addClass('bodyhide');
      googleDivIsShow = -1;
      document.title = '百度一下，你就知道';
    });
   }else{
    $('#contentwrap').show(1,function(){
      $('body').removeClass('bodyhide');
      $('body').addClass('bodyshow');
      googleDivIsShow = 1;
      document.title = trueTitle;
    });
    wresizeSwf();
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
      addfavor('http:\/\/web.4399.com\/yjjh\/',site.sitename);
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
      addfavor('http:\/\/web.4399.com\/yjjh\/',site.sitename);
      $.cookie('addfavor',1);
    }
  });
}


function wresizeSwf(){
  var paddingtop = document.documentElement.clientHeight - $('#flashcontent').height();
  paddingtop = paddingtop < 1 ? 0 : paddingtop/2;
  $('#flashcontent').css('padding-top',paddingtop);
  }


//send ajax
function ajUser(evt){
  $.post(site.rooturi + 'yjjhpt\/_ajUser.php?evt='+evt+'&'+(new Date()).valueOf(),{'u':site.userid,'evt':evt},function(data){ $('#apirtn').html( $('#apirtn').html() + ' ajUser ==> ' + data + '<br />' );});
  }

/*
//send ajax TMP func.
function _ajUser(evt){
  ajUser(evt);
  }
*/
function cuser(u){
  $.post(site.rooturi + 'yjjhpt\/_cuser.php?u='+u,{'u':u},function(data){ $('#apirtn').html( $('#apirtn').html() + 'cuser ==> ' + data + '<br />' );});
  }

//summary
function ajSummary(){
  $.post(site.rooturi + 'yjjhpt\/_ajSummary.php?'+(new Date()).valueOf(),function(data){ $('#apirtn').html( $('#apirtn').html() + 'ajSummary ==> ' + data + '<br />' );});
  }

//send online
function ajOnline(){
  $.post(site.rooturi + 'yjjhpt\/_online.php?'+(new Date()).valueOf(),{'u':site.userid},function(data){ $('#apirtn').html( $('#apirtn').html() + 'ajOnline ==> ' + data + '<br />' );});
  }

//change line
function changeLine(lineid){
  $.post(site.rooturi + 'yjjhpt\/_changeLine.php?lineid=' + lineid + '&'+(new Date()).valueOf(),{'u':site.userid,'lineid':lineid},function(data){ $('#apirtn').html( $('#apirtn').html() + ' changeLine ==>' + data + '<br />' );});
  }

var tm1 = window.setInterval('ajOnline()',50*1000); //per 50 sec
var tm2 = window.setInterval('ajSummary()',3*60*1000); //per 3 min

$(function(){
	wresizeSwf();
  //window resize
  $(window).wresize(function(){ wresizeSwf(); });
  });
//-->
</script>
</head>

<!-- <body onbeforeunload="addFav();return '正在游戏中，您是否真的要离开本页?';" class="bodyshow"> -->
<body class="bodyshow" onunload="ajUser(-1);" onload="ajUser(1);">
<div id="contentwrap">
	<div id="flashcontent">
  <div id="guardlink" class="rt">
      <!--<a href="http://pay.4399.com/?gamename=yjjh&amp;gameserver=<?php echo $_SCOOKIE['serverid']?>&amp;username=<?php echo $_SCOOKIE['username']?>" target="_blank"><span style="color:#00ff00">充值</span></a>-->
      <a href="http://web.4399.com/yjjh/201010/15-33073.html" target="_blank"><span style="color:#FF0000"><img src="images/ico_qqbi.png" alt="Q货" border="0" />升级送Q币</span></a>
      <a href="http://web.4399.com/yjjh/" target="_blank"><span style="color:green"><img src="images/ico_guanwang.png" alt="官网" border="0" />官网</span></a>
      <a href="http://bbs.youjia.cn/forum-293-1.html" target="_blank"><span><img src="images/ico_bbs.png" alt="论坛" border="0" />论坛</span></a>
      <a href="javascript:;" onclick="addfavor();"><span style="color:#ff66ff"><img src="images/ico_soucang.png" alt="收藏" border="0" />收藏</span></a>
      <a href="http://bbs.youjia.cn/forum-296-1.html" target="_blank"><span style="color:#FFFFFF"><img src="images/ico_help.png" alt="帮助" border="0" />帮助</span></a>
    </div>
	  <div id="flashobjdiv">
		<a href="http://www.adobe.com/go/getflash" target="_blank"><strong>您需要升级您的浏览器的Flash 播放器！</strong>
		This is replaced by the Flash content.<br />
		此区域是会被替换为FLASH，如果此区域未替换，请升级您的Flash 播放器。<br />
		<strong>点击此处下载最新版本的Flash Player http://www.adobe.com/go/getflash</strong></a><br />
		</div>
	</div>
	<div id="playerdl" style="display:none"></div>

	<script type="text/javascript">
		// <![CDATA[
	  var swfurl = '<?php echo $serverArr[$serverid]['swfurl'];?>';
		var so = new SWFObject(swfurl, "flashobj", "1000", "580", "9", "#000000");
		so.useExpressInstall('expressinstall.swf');
		for(var i in site){
		  so.addVariable(i,site[i]);
		  }
		so.addParam('allowScriptAccess','always');
		so.write("flashobjdiv");

    var version = deconcept.SWFObjectUtil.getPlayerVersion();
    if(version['major'] < 10)
    {
      var txt = '您的浏览器的当前Flash Player版本不是最新的，可能会影响游戏的运行速度或无法进入游戏，<a href="http://www.adobe.com/go/getflash" target="_blank">请点击此处下载最新版本的Flash Player<\/a> &nbsp;&nbsp;&nbsp;'+
                '<a href="javascript:;" onclick="$(\'#playerdl\').fadeOut()" class="meta">关闭提示<\/a>';
      $('#playerdl').html(txt);
      $('#playerdl').fadeIn(600);
     }
		// ]]>
	</script>
</div>
<div id="apirtn"></div>
</body>
</html>
