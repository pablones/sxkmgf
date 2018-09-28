
/*
  JS For yjjhpt
  Anypage which want to be summary must include this file!

  Before Inlcude This Js File ,Must Have a Js Object -> [site]
    site.userid   => 123456,
    site.username => myname,
    site.rooturi  => /,
*/


//create rolename ok!
function cuser(u){
  $.post(site.rooturi + 'yjjhpt\/_cuser.php?u='+u+'&cid='+site.cid+'&'+(new Date()).valueOf(),{'u':u,'cid':site.cid});
  }

function _cuser(u){
  cuser(u);
  }


//send ajax
function ajUser(evt){
  $.post(site.rooturi + 'yjjhpt\/_ajUser.php?evt='+evt+'&'+(new Date()).valueOf(),{'u':site.userid,'evt':evt});
  /*
  if(evt == -2){
    var txt = '您已断开连接，请刷新本页重试！ <a href="javascript:window.location.reload()">刷新本页<\/a>';
    $('#playerloss').html(txt);
    $('#playerloss').fadeIn(600);
    }
    */
  }
  
 function _ajUser(evt){
	 $.post(site.rooturi + 'yjjhpt\/accross.php?evt='+evt+'&'+(new Date()).valueOf(),{'u':site.userid,'evt':evt});
 }

//show message when loss the connection to web
function showbreak(){
  var txt = '您已断开连接，请刷新本页重试！ <a href="javascript:window.location.reload()">刷新本页<\/a>';
  $('#playerloss').html(txt);
  $('#playerloss').fadeIn(600);
  }

//send ajax TMP func.For Chen Min.
function _ajUser(evt){
  ajUser(evt);
  }

//summary
function ajSummary(){
  $.post(site.rooturi + 'yjjhpt\/_ajSummary.php?'+(new Date()).valueOf());
  }

//send online
function ajOnline(){
  $.post(site.rooturi + 'yjjhpt\/_online.php?'+(new Date()).valueOf(),{'u':site.userid});
  }

//change line
function changeLine(lineid){
  $.post(site.rooturi + 'yjjhpt\/_changeLine.php?lineid=' + lineid + '&'+(new Date()).valueOf(),{'u':site.userid,'lineid':lineid});
  }

function updatefirst(userid){
	$.post(site.rooturi + 'yjjhpt\/_updatefirst.php?&'+(new Date()).valueOf(),{'userid':userid});
}

function createrolenum(){
	$.post(site.rooturi + 'yjjhpt\/_createrolenum.php?&'+(new Date()).valueOf());
}

function joingamenum(){
	$.post(site.rooturi + 'yjjhpt\/_joingamenum.php?&'+(new Date()).valueOf());
}

function loadgame()
{
	document.body.style.backgroundImage="url(images/playbg.png)";
}

function loadGameTitle()
{
	$('#guardlink').css("visibility","visible"); 
}