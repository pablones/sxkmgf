<?php

require_once dirname(dirname(dirname(__FILE__))).'./common.php';
//error_reporting(E_ALL);
$_SCOOKIE['4399uid'] = authcode($_SCOOKIE['4399uid'],'DECODE');


if(empty($_SCOOKIE['username']) || $_SCOOKIE['userid'] < 1 || $_SCOOKIE['4399uid'] < 1){
  if(!empty($_SERVER['HTTP_COOKIE'])){
    $logfile = './logfile/jump_'.date("Ymd").'.txt';
    $logcnt = date("Y-m-d H:i:s")."[".$_SG['ip']."]\r\n";
    $logcnt .= "    username: ".$_SCOOKIE['username']."\r\n";
    $logcnt .= "    userid  : ".$_SCOOKIE['userid']."\r\n";
    $logcnt .= "    4399uid : ".$_SCOOKIE['4399uid']."\r\n";
    $logcnt .= "    HTTP_COOKIE     :  ".$_SERVER['HTTP_COOKIE']."\r\n";
    $logcnt .= "    HTTP_HOST       :  ".$_SERVER['HTTP_HOST']."\r\n";
    $logcnt .= "    HTTP_USER_AGENT :  ".$_SERVER['HTTP_USER_AGENT']."\r\n";
    $logcnt .= "--------------------------------------------\r\n\r\n";
    swritefile($logfile,$logcnt,'a+');
  }

  $jumpurl = 'http://web.4399.com/yjjh/?msg=please_login_first';
  echo jump($jumpurl,'对不起，您尚未登录，请先到官网登录！\r\n如果您已经登录了，却出现此提示，请到官方交流群 117431165 / 48336440 反应此问题！');
  exit();
  }


$serverid = $_SCOOKIE['serverid'];
$serverArr = array(
             'S1'=> array(
                    'id' => 1,
                    'servername' => '江山如画',
                    'swfurl'=> 'http://yjjh1-as.my4399.com/Gameloader.swf',
                    'swfurl'=> 'http://yjjh1-s.my4399.com:8076/Gameloader.swf',
                    ),
             );

if(!array_key_exists($serverid,$serverArr)) echo jump('http://web.4399.com/yjjh/?msg=serverid_error','游戏服务器'.$serverid.'服不存在!');

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>4399 《御剑江湖》<?php echo $serverArr[$serverid]['id'];?>服 -- <?php echo $serverArr[$serverid]['servername']?></title>
<link href="default.css" rel="stylesheet" type="text/css" />
<script language="JavaScript" type="text/javascript" src="../source/js/jquery.min.js"></script>
<script language="JavaScript" type="text/javascript" src="../source/js/wresize.js"></script>
<script language="JavaScript" type="text/javascript" src="../source/js/swfobject.js"></script>
<script language="JavaScript" type="text/javascript" src="../source/js/jquery.cookie.js"></script>
<script language="javascript" type="text/javascript">
<!--
<?php

$verfile = './ver.txt';
if(!file_exists($verfile)) swritefile($verfile,date("Ymd").rand(1000,9999));
$ver = shtmlspecialchars( trim( sreadfile($verfile) ) );


$json = array(
        '4399uid'=> $_SCOOKIE['4399uid'],
        'userid'=> $_SCOOKIE['userid'],
        'username'=> $_SCOOKIE['username'],
        'currurl'=> '',
        'timestamp'=> time(),
        'serverid'=> $_SCOOKIE['serverid'],
        'swfver'=> $ver, //SWF版本
        'idhash'=> $_SCOOKIE['idhash'],
        'uri'=> '/',
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


function setHomepg(){
 if (document.all){
  document.body.style.behavior='url(#default#homepage)';
  document.body.setHomePage(site['siteurl']);
  }else if (window.sidebar){
    if(window.netscape){
         try{
            netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
         }catch (e){
          alert( "您的浏览器不支持,请在地址栏内输入about:config,找到signed.applets.codebase_principal_support,双击将其值切换为true后重试" );
         }
    }
    var prefs = Components.classes['@mozilla.org/preferences-service;1'].getService(Components. interfaces.nsIPrefBranch);
    prefs.setCharPref('browser.startup.homepage',site['siteurl']);
 }
}


function addfavor(url,title){
  if(!url) url = 'http:\/\/web.4399.com\/yjjh\/';
  if(!title) title = site.sitename;
  var ua = navigator.userAgent.toLowerCase();
  if(ua.indexOf("msie 8")>-1){
      external.AddToFavoritesBar(url,title,title);//IE8
  }else{
      try {
          window.external.addFavorite(url, title);
      } catch(e) {
          try {
              window.sidebar.addPanel(title, url, "");//firefox
          } catch(e) {
              alert("加入收藏失败，请手动进行添加");
          }
      }
  }
  return false;
}

function addFav(){
  site.siteurl = 'http:\/\/web.4399.com\/yjjh\/';
  if(document.all){
      window.external.addFavorite(site['siteurl'],site['sitename']);
    }else if(window.sidebar){
      window.sidebar.addPanel(site['sitename'], site['siteurl'],'');
    }
}



function wresizeSwf(){
  var paddingtop = document.documentElement.clientHeight - $('#flashcontent').height();
  paddingtop = paddingtop < 1 ? 0 : paddingtop/2;
  $('#flashcontent').css('padding-top',paddingtop);
  }

//window resize
$(window).wresize(function(){ wresizeSwf(); });

function online(){
  $.post('../api/online.php?'+(new Date()).valueOf(),{'idhash':site.idhash,'timestamp':(new Date()).valueOf()});
  }
//-->
</script>
</head>

<!-- <body onbeforeunload="addFav();return '正在游戏中，您是否真的要离开本页?';" class="bodyshow"> -->
<body class="bodyshow">
<div id="contentwrap">
	<div id="flashcontent">
		<a href="http://www.adobe.com/go/getflash" target="_blank"><strong>您需要升级您的浏览器的Flash 播放器！</strong>
		This is replaced by the Flash content.<br />
		此区域是会被替换为FLASH，如果此区域未替换，请升级您的Flash 播放器。<br />
		<strong>点击此处下载最新版本的Flash Player http://www.adobe.com/go/getflash</strong></a><br />
	</div>
	<div id="playerdl" style="display:none"></div>

	<script type="text/javascript">
		// <![CDATA[
	  var swfurl = '<?php echo $serverArr[$serverid]['swfurl'];?>';
		var so = new SWFObject(swfurl, "flashobj", "1000", "580", "9", "#000000");
		for(var i in site){
		  so.addVariable(i,site[i]);
		  }
		so.addParam('allowScriptAccess','always');
		so.write("flashcontent");
		wresizeSwf();

		online();
    var tmOnline = window.setInterval('online()',60*5000); //per 5 min
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
<div style="display:none">
<script language="javascript" type="text/javascript" src="http://js.users.51.la/4124011.js"></script>
<noscript><a href="http://www.51.la/?4124011" target="_blank"><img alt="&#x6211;&#x8981;&#x5566;&#x514D;&#x8D39;&#x7EDF;&#x8BA1;" src="http://img.users.51.la/4124011.asp" style="border:none" /></a></noscript>
</div>
</body>
</html>
