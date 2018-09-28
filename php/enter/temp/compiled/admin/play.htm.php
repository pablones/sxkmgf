<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo $this->_var['lang']['server']; ?><?php echo $this->_var['serverid']; ?><?php echo $this->_var['lang']['fu']; ?> -- <?php echo $this->_var['servername']; ?></title>
<link rel="icon" href="favicon.ico" type="image/x-icon" />
<link rel="shortcut icon" href="favicon.ico" type="image/x-icon" />
<link rel="Bookmark" href="favicon.ico" />
<link href="default.css" rel="stylesheet" type="text/css" />
<?php echo $this->smarty_insert_scripts(array('files'=>'../admin/js/jquery.min.js,jquery.cookie.js,jquery.wresize.js,swfobject.js,play_common.js,inc.js')); ?>
<script language="javascript" type="text/javascript">
var site = <?php echo $this->_var['json']; ?>;
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
    if(timestamp - lastZkey > 450){
        lastZkey = timestamp;
        return;
    }
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

var add_favourite = false;

function setbookmark(){
    add_favourite=true;
}

function bookmarkit(){
    if(add_favourite) {
        showFavourite();
    }
}

$(function(){
    wresizeSwf();
    //window resize
    $(window).wresize(function(){wresizeSwf();});
});
</script>
</head>

<body class="bodyshow">
<div id="contentwrap">
    <div id="flashcontent">
        <div id="guardlink" class="rt"></div>
	    <div id="flashobjdiv">
		    <a href="http://www.adobe.com/go/getflash" target="_blank">
                <strong><?php echo $this->_var['lang']['msg_1']; ?></strong>
		        <?php echo $this->_var['lang']['msg_2']; ?><br /><?php echo $this->_var['lang']['msg_3']; ?><br/>
		        <strong><?php echo $this->_var['lang']['msg_4']; ?></strong>
            </a><br />
		</div>
	</div>
	<div id="playerdl" style="display:none"></div>
	<div id="playerloss" style="display:none"></div>

	<script type="text/javascript">
        var swfurl ='<?php echo $this->_var['swfurl']; ?>';
		var so = new SWFObject(swfurl, "flashobj", "1000", "580", "9", "#000000");
		so.useExpressInstall('expressinstall.swf');
		for(var i in site){
		    so.addVariable(i,site[i]);
		}
		so.addParam('allowScriptAccess','always');
		so.write("flashobjdiv");

        var version = deconcept.SWFObjectUtil.getPlayerVersion();
        if(version['major'] < 10){
             var txt = '您的浏览器的当前Flash Player版本不是最新的，可能会影响游戏的运行速度或无法进入游戏，<a href="http://www.adobe.com/go/getflash" target="_blank">请点击此处下载最新版本的Flash Player<\/a> &nbsp;&nbsp;&nbsp;'+
                '<a href="javascript:;" onclick="$(\'#playerdl\').fadeOut()" class="meta">关闭提示<\/a>';
            $('#playerdl').html(txt);
            $('#playerdl').fadeIn(600);
        }
	</script>
</div>
</body>
</html>

