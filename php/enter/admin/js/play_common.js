//

//检测是否是网址

function isURL(str_url){
  var strRegex = "^(https|http|ftp|rtsp|mms)://"
  + "(([0-9a-z_!~*'().&=+$%-]+: )?[0-9a-z_!~*'().&=+$%-]+@)?"
        + "(([0-9]{1,3}\.){3}[0-9]{1,3}"
        + "|"
        + "([0-9a-z_!~*'()-]+\.)*"
        + "([0-9a-z][0-9a-z-]{0,61})?[0-9a-z]\."
        + "[a-z]{2,6})"
        + "(:[0-9]{1,4})?"
        + "(/[0-9a-zA-Z_!~*'/().;?:@&=+$,%#-]+)?$";
        return new RegExp(strRegex).test(str_url) ? true:false;
    }

function AutoScroll(obj){
    $(obj).find("ul:first").animate({
            marginTop:"-25px"
    },500,function(){
            $(this).css({marginTop:"0px"}).find("li:first").appendTo(this);
    });
  }


// index bottom search

function bottomSearch(){
  var k = $('#formOutSearch :text').eq(0);
  var s = $('#formOutSearch :checked');
  var u = searchURL[s.val()] + $.trim(k.val());
  window.open(u,'','');
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

function getLocation(){
  return encodeURIComponent(window.location.href);
  }



function getformhash(formid){
  $.get(site.siteurl+'/api/formhash.php',{'time':(new Date()).valueOf()},function(data){$('#'+formid+' input[name="formhash"]').val(data);});
  }


function isEmail(email){
  return /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(email);
}

//中文或英文或数字 无空字符
function isUsername(str){
  return /^[\u4E00-\u9FA5\w]{3,20}$/.test(str);
  }

function isPw(str){
  return /^[\S]{6,20}$/.test(str);
  }

function isSeccode(str){
  return /^\d{4}$/.test(str);
  }

function cfm(msg){
  msg = msg ? msg:'您是否真的要执行此动作?';
  return confirm(msg);
  }


//---------------------------------------------------------------------
function selectNull(startid,prefix,rangesize){
  var s = [];
  var maxnum = startid + rangesize;
  var nowid  = startid;
  while(nowid < maxnum){
    s = document.getElementById(prefix + nowid);
    s.options.length = 1;
    nullTxtId = s.name;
    s.options[0] = new Option('    ',0);
    nowid++;
    }
  }

function selectChange(subid,arr,prefix,TxtKeyName,IdKeyName){
  var s = document.getElementById(prefix+subid);
  s.options.length = 0;
  s.options[0] = new Option('    ',0);
  var j = 1;
  for(var i in arr){
    s.options[j] = new Option(arr[i][TxtKeyName],arr[i][IdKeyName]);
    j++;
    }
  }
//-----------------------------------------------------------

function updatehit(tb,fn,id){
  var apiurl = site.siteurl+'api/updatehit.php';
  var tip = {'ParamError':'参数错误','BadRequest':'非法请求','DoNotRepeat':'请勿重复操作'};
  var cookieId = tb + fn + id;
  if(fn != 'hit' && $.cookie(cookieId) == 1){
    alert('您已经投过票了，请勿重复投票！');
    return ;
    }
  var val = parseInt( $('#'+cookieId).text() );
  $.post(apiurl,{'tb':tb,'fn':fn,'id':id},
        function(data){
          if(data > 0){
            $('#'+cookieId).text(val+1);
            $.cookie(cookieId,1);
            }else{
              var msg = tip[data];
              if(!msg) msg = '系统忙，请稍后再试！';
              alert(msg);
            }
          }
        );
  }

//输入框背后的提示
function iptApiTip(imgId,isShow,filename,txt,txtId){
  if(!isShow){
    $('#'+imgId).hide();
    if(txtId) $('#'+txtId).hide();
    return;
    }
  if(filename) $('#'+imgId).attr('src',site.rooturi + 'images/icons/'+filename+'.gif');
  if(txt){
    $('#'+imgId).attr('title',txt);
    if(txtId){
      $('#'+txtId).text(txt);
      $('#'+txtId).show();
      }
    }else{
      if(txtId) $('#'+txtId).hide();
    }
    $('#'+imgId).show();
  }


