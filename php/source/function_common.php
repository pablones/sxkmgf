<?php

if(!defined('IN_ZBPHP')) {
	exit('Access Denied');
}

//SQL ADDSLASHES
function saddslashes($string) {
	if(is_array($string))	{
		foreach($string as $key => $val) {
			$string[$key] = saddslashes($val);
		}
	}	else	{
		$string = addslashes($string);
	}
	return $string;
}

//取消HTML代码
function shtmlspecialchars($string) {
	if(is_array($string))	{
		foreach($string as $key => $val) {
			$string[$key] = shtmlspecialchars($val);
		}
	} else{
		$string = preg_replace('/&amp;((#(\d{3,5}|x[a-fA-F0-9]{4})|[a-zA-Z][a-z0-9]{2,5});)/', '&\\1',
			str_replace(array('&', '"', '<', '>','\''), array('&amp;', '&quot;', '&lt;', '&gt;','&#039;'), $string));
	}
	return $string;
}

//过滤intval
function sintval($string){
	if(is_array($string)){
		foreach($string as $key => $val){
			$string[$key] = sintval($val);
		}
	}else{
	  $string = intval($string);
	}
	return $string;
}

//过滤strim
function strim($string){
	if(is_array($string)){
		foreach($string as $key => $val) {
			$string[$key] = strim($val);
		}
	}else{
	  $string = trim($string);
	}
	return $string;
}


//字符串解密加密
function authcode($string, $operation = 'DECODE', $key = '', $expiry = 0) {

	$ckey_length = 4;	// 随机密钥长度 取值 0-32;
				// 加入随机密钥，可以令密文无任何规律，即便是原文和密钥完全相同，加密结果也会每次不同，增大破解难度。
				// 取值越大，密文变动规律越大，密文变化 = 16 的 $ckey_length 次方
				// 当此值为 0 时，则不产生随机密钥

	$key = md5($key ? $key : $GLOBALS['_SG']['authkey']);
	$keya = md5(substr($key, 0, 16));
	$keyb = md5(substr($key, 16, 16));
	$keyc = $ckey_length ? ($operation == 'DECODE' ? substr($string, 0, $ckey_length): substr(md5(microtime()), -$ckey_length)) : '';

	$cryptkey = $keya.md5($keya.$keyc);
	$key_length = strlen($cryptkey);

	$string = $operation == 'DECODE' ? base64_decode(substr($string, $ckey_length)) : sprintf('%010d', $expiry ? $expiry + time() : 0).substr(md5($string.$keyb), 0, 16).$string;
	$string_length = strlen($string);

	$result = '';
	$box = range(0, 255);

	$rndkey = array();
	for($i = 0; $i <= 255; $i++)
	{
		$rndkey[$i] = ord($cryptkey[$i % $key_length]);
	}

	for($j = $i = 0; $i < 256; $i++)
	{
		$j = ($j + $box[$i] + $rndkey[$i]) % 256;
		$tmp = $box[$i];
		$box[$i] = $box[$j];
		$box[$j] = $tmp;
	}

	for($a = $j = $i = 0; $i < $string_length; $i++)
	{
		$a = ($a + 1) % 256;
		$j = ($j + $box[$a]) % 256;
		$tmp = $box[$a];
		$box[$a] = $box[$j];
		$box[$j] = $tmp;
		$result .= chr(ord($string[$i]) ^ ($box[($box[$a] + $box[$j]) % 256]));
	}

	if($operation == 'DECODE')
	{
		if((substr($result, 0, 10) == 0 || substr($result, 0, 10) - time() > 0) && substr($result, 10, 16) == substr(md5(substr($result, 26).$keyb), 0, 16))
		{
			return substr($result, 26);
		}
		 else
		 {
			return '';
		}
	}
	 else
	 {
		return $keyc.str_replace('=', '', base64_encode($result));
	}
}

//清空cookie
function clearcookie() {
	global $_SG;

	obclean();
	$arr = array('userid','username','4399uid');
	foreach($arr as $key){
	  ssetcookie($key, '', -86400 * 365);
	  }
	$_SG['uid'] = 0;
	$_SG['username'] = '';
	$_SG['member'] = array();
}

//cookie设置
function ssetcookie($var, $value = '', $life = 0, $prefix = 1, $httponly = false) {
	global $_SG,$_SC,$_SERVER;
	$var = ($prefix ? $_SC['cookiepre'] : '').$var;
	if($value == '' || $life < 0) {
		$value = '';
		$life = -1;
	}
	$life = $life > 0 ? $_SG['timestamp'] + $life : ($life < 0 ? $_SG['timestamp'] - 31536000 : 0);
	$path = $httponly && PHP_VERSION < '5.2.0' ? $_SC['cookiepath']."; HttpOnly" : $_SC['cookiepath'];
	$secure = $_SERVER['SERVER_PORT'] == 443 ? 1 : 0;
	if(PHP_VERSION < '5.2.0') {
		setcookie($var, $value, $life, $_SC['cookiepath'], $_SC['cookiedomain'], $secure);
	} else {
		setcookie($var, $value, $life, $_SC['cookiepath'], $_SC['cookiedomain'], $secure, $httponly);
	}
}
//数据库连接
function dbconnect() {
	global $_SC,$db;

	include_once(S_ROOT.'./source/class_mysql.php');

	if(!is_object($db))
	{
		$db = new dbstuff;
		$db->charset = $_SC['dbcharset'];
		$db->connect($_SC['dbhost'], $_SC['dbuser'], $_SC['dbpw'], $_SC['dbname'], $_SC['pconnect']);
	}
}


//获取在线IP
function getip($format=0)
{
	global $_SG;

	if(empty($_SG['ip']))
	{
		if(getenv('HTTP_CLIENT_IP') && strcasecmp(getenv('HTTP_CLIENT_IP'), 'unknown'))
		{
			$ip = getenv('HTTP_CLIENT_IP');
		}
		 elseif(getenv('HTTP_X_FORWARDED_FOR') && strcasecmp(getenv('HTTP_X_FORWARDED_FOR'), 'unknown'))
		 {
			$ip = getenv('HTTP_X_FORWARDED_FOR');
		}
		 elseif(getenv('REMOTE_ADDR') && strcasecmp(getenv('REMOTE_ADDR'), 'unknown'))
		 {
			$ip = getenv('REMOTE_ADDR');
		}
		 elseif(isset($_SERVER['REMOTE_ADDR']) && $_SERVER['REMOTE_ADDR'] && strcasecmp($_SERVER['REMOTE_ADDR'], 'unknown'))
		 {
			$ip = $_SERVER['REMOTE_ADDR'];
		}
		preg_match("/[\d\.]{7,15}/", $ip, $onlineipmatches);
		$_SG['ip'] = $onlineipmatches[0] ? $onlineipmatches[0] : 'unknown';
	}
	if($format) $_SG['ip'] = ipformat($_SG['ip']);
	return $_SG['ip'];
}

//格式化IP
function ipformat($ip,$cutLength = 3){
  $ipArr = explode('.',$ip);
  if(count($ipArr) != 4) return '**火星IP**'; //如果IP不是4位数,那么返回错误提示
  $ipStr = $comma = '';
  for($i = 0; $i < 4;$i++){
    $ipStr.= $comma.( $i >= $cutLength ? '*':intval($ipArr[$i]) );
    $comma = '.';
    }
    return $ipStr;
  }

//判断当前用户登录状态
function checkauth()
{
	global $_SG, $_Cfg,$_SCOOKIE,$db;
	if(!empty($_SCOOKIE['auth']))
	{
		@list($uid,$password) = explode("\t",authcode($_SCOOKIE['auth'],'DECODE'));
		$_SG['uid'] = intval($uid);
		$usertb = authcode($_SCOOKIE['usertb'],'DECODE');

		if($password && $_SG['uid'] > 0 && in_array($usertb,array('user','co'))) {
		  $uidFn = $usertb == 'user' ? 'uid':'coid';
		  $sql = "select `$uidFn`,`md5`,`email`,`username`,`password` from @#__{$usertb} where `$uidFn`='".$_SG['uid']."' limit 1 ";
		  $member = $db->fetch_first($sql);
		  if($member && $member['password'] == $password){
		    $_SG['username'] = $member['username'];
		    $_SG['member'] = $member;
		    $_SG['usertb'] = $usertb;
		    }
		}
		if(empty($_SG['username']) || $_SG['uid'] <= 0 || $_SG['usertb'] == '') clearcookie();
	}
}

/**
 函数:  showmsg()
 用途:  显示提示信息，并且跳转,无返回值
 参数:
  @msgkey   提示内容,可以是msgkey,可以为空
  @gourl    要跳转到的网址,可以为空
  @onlymsg  如果值为1的话，不跳转，只显示信息内容
  @sec      隔多长时间进行跳转,默认值为5秒钟,单位秒

  假如,msgkey 和 gourl 都为空，那么提示内容后直接返回到首页

**/
//显示提示的内容并且结束程序运行
function showmsg($msgkey = '',$gourl = '',$onlymsg = 0,$sec = 5)
{
  global $_Cfg,$_LANG,$_SG;
  if(empty($msgkey))
  {
    //如果msgkey 和 gourl 都为空的 ,那么默认跳转到首页
    if($gourl == ''){
      $msg = '接下来跳转到首页';
      $gourl = $_Cfg['siteurl'];
    }else{
      $msg = '';
      }
  }
  else
  {
     if(preg_match('/^[\w]*$/',$msgkey))
     {
      $msgkey = strtoupper($msgkey);
      $msg = array_key_exists($msgkey,$_LANG['showmsg']) ? $_LANG['showmsg'][$msgkey]: $msgkey;
      }
      else
      {
        $msg = $msgkey;
      }
    }
  include_once dirname(__FILE__).'/function_showmsg.php';
  exit();
}


//判断提交是否正确
function submitcheck($var = 'submit')
{
	if(!empty($_POST[$var]) && $_SERVER['REQUEST_METHOD'] == 'POST')
	{
		if((empty($_SERVER['HTTP_REFERER']) || preg_replace("/https?:\/\/([^\:\/]+).*/i", "\\1", $_SERVER['HTTP_REFERER']) == preg_replace("/([^\:]+).*/", "\\1", $_SERVER['HTTP_HOST'])) && $_POST['formhash'] == formhash())
		{
			return true;
		}
		 else
		 {
			showmsg('非法请求!',$GLOBALS['_Cfg']['siteurl']);
		}
	}
	 else
	 {
		return false;
	}
}

//添加数据
function inserttable($tablename, $insertsqlarr, $returnid=0, $replace = false, $silent=0)
{
	global $db;

	$insertkeysql = $insertvaluesql = $comma = '';
	foreach ($insertsqlarr as $insert_key => $insert_value)
	{
		$insertkeysql .= $comma.'`'.$insert_key.'`';
		$insertvaluesql .= $comma.'\''.$insert_value.'\'';
		$comma = ', ';
	}
	$method = $replace?'REPLACE':'INSERT';
	$db->query($method.' INTO @#__'.$tablename.' ('.$insertkeysql.') VALUES ('.$insertvaluesql.')', $silent ? 'SILENT':'');
	if($returnid && !$replace)
	{
		return $db->insert_id();
	}
}

//更新数据
function updatetable($tablename, $setsqlarr, $wheresqlarr, $silent=0)
{
	global $db;

	$setsql = $comma = '';
	foreach ($setsqlarr as $set_key => $set_value)
	{
		$setsql .= $comma.'`'.$set_key.'`'.'=\''.$set_value.'\'';
		$comma = ', ';
	}
	$where = $comma = '';
	if(empty($wheresqlarr))
	{
		$where = '1';
	}
	 elseif(is_array($wheresqlarr))
	 {
		foreach ($wheresqlarr as $key => $value)
		{
			$where .= $comma.'`'.$key.'`'.'=\''.$value.'\'';
			$comma = ' AND ';
		}
	}
	 else
	 {
		$where = $wheresqlarr;
	}

	$db->query('UPDATE @#__'.$tablename.' SET '.$setsql.' WHERE '.$where, $silent ? 'SILENT':'');
}


//add or update row
function affectrow($table,$setarr,$idfn='',$idvl='',$replace=false)
{
  global $db;
  $sql = $comma = $keysql = $valsql = '';
  $table = '`'.$GLOBALS['_Cfg']['tablepre'].$table.'`';

  //修改
  if($idfn && $idvl)
  {
    foreach($setarr as $key => $val)
    {
      $sql .= $comma."`$key`='$val'";
      $comma = ',';
      }
      $sql = "update $table set $sql where `$idfn`='$idvl' limit 1 ;";
    }
    else
    {
      //添加
      foreach($setarr as $key => $val)
      {
        $keysql .= $comma."`$key`";
        $valsql .= $comma."'$val'";
        $comma = ',';
        }
        $sql = ($replace ? 'replace':'insert').' into '.$table.'('.$keysql.')values('.$valsql.') ;';
    }
    $db->query($sql);
    return ($idfn && $idvl)? $db->affected_rows():$db->insert_id();
  }


//写运行日志
function runlog($file, $log, $halt=0)
{
	global $_SG,$_SCOOKIE,$_SERVER;

	$nowurl = $_SERVER['REQUEST_URI']?$_SERVER['REQUEST_URI']:($_SERVER['PHP_SELF']?$_SERVER['PHP_SELF']:$_SERVER['SCRIPT_NAME']);
	$log = sgmdate('Y-m-d H:i:s', $_SG['timestamp'])."\t".$GLOBALS['_SG']['ip']."\t$_SCOOKIE[userid]\t".$_SERVER['REQUEST_URI']."\t".str_replace(array("\r", "\n"), array(' ', ' '), trim($log))."\n";
	$yearmonth = sgmdate('Ym', $_SG['timestamp']);
	$logdir = S_ROOT.'./data/runlog/';
	if(!is_dir($logdir)) mkdir($logdir, 0777);
	$logfile = $logdir.$yearmonth.'_'.$file.'.php';
	if(@filesize($logfile) > 2048000)
	{
		$dir = opendir($logdir);
		$length = strlen($file);
		$maxid = $id = 0;
		while($entry = readdir($dir))
		{
			if(strexists($entry, $yearmonth.'_'.$file))
			{
				$id = intval(substr($entry, $length + 8, -4));
				$id > $maxid && $maxid = $id;
			}
		}

		closedir($dir);
		$logfilebak = $logdir.$yearmonth.'_'.$file.'_'.($maxid + 1).'.php';
		@rename($logfile, $logfilebak);
	}

	if($fp = @fopen($logfile, 'a'))
	{
		@flock($fp, 2);
		fwrite($fp, "<?PHP exit;?>\t".str_replace(array('<?', '?>', "\r", "\n"), '', $log)."\n");
		fclose($fp);
	}
	if($halt) exit();
}


//获取字符串
function getstr($string, $length)
{
	global $_Cfg, $_SG;

	$string = trim($string);
	if($length && strlen($string) > $length)
	{
		//截断字符
		$wordscut = '';
		if(strtolower($_Cfg['charset']) == 'utf-8')
		{
			//utf8编码
			$n = 0;
			$tn = 0;
			$noc = 0;
			while ($n < strlen($string))
			{
				$t = ord($string[$n]);
				if($t == 9 || $t == 10 || (32 <= $t && $t <= 126))
				{
					$tn = 1;
					$n++;
					$noc++;
				}
				 elseif(194 <= $t && $t <= 223)
				 {
					$tn = 2;
					$n += 2;
					$noc += 2;
				}
				 elseif(224 <= $t && $t < 239)
				 {
					$tn = 3;
					$n += 3;
					$noc += 2;
				}
				 elseif(240 <= $t && $t <= 247)
				 {
					$tn = 4;
					$n += 4;
					$noc += 2;
				}
				 elseif(248 <= $t && $t <= 251)
				 {
					$tn = 5;
					$n += 5;
					$noc += 2;
				}
				 elseif($t == 252 || $t == 253)
				 {
					$tn = 6;
					$n += 6;
					$noc += 2;
				}
				 else
				 {
					$n++;
				}

				if ($noc >= $length)
				{
					break;
				}
			}

			if ($noc > $length)
			{
				$n -= $tn;
			}
			$wordscut = substr($string, 0, $n);
		}
		 else
		 {
			for($i = 0; $i < $length - 1; $i++)
			{
				if(ord($string[$i]) > 127)
				{
					$wordscut .= $string[$i].$string[$i + 1];
					$i++;
				}
				 else
				 {
					$wordscut .= $string[$i];
				}
			}
		}
		$string = $wordscut;
	}
	return trim($string);
}

//时间格式化
function sgmdate($dateformat, $timestamp='', $format=0)
{
	global $_SCONFIG, $_SGLOBAL;
	if(empty($timestamp))
	{
		$timestamp = $_SGLOBAL['timestamp'];
	}
	$timeoffset = strlen($_SGLOBAL['member']['timeoffset'])>0?intval($_SGLOBAL['member']['timeoffset']):intval($_SCONFIG['timeoffset']);
	$result = '';
	if($format)
	{
		$time = $_SGLOBAL['timestamp'] - $timestamp;
		if($time > 24*3600)
		{
			$result = gmdate($dateformat, $timestamp + $timeoffset * 3600);
		}
		 elseif ($time > 3600)
		 {
			$result = intval($time/3600).lang('hour').lang('before');
		}
		 elseif ($time > 60)
		 {
			$result = intval($time/60).lang('minute').lang('before');
		}
		 elseif ($time > 0)
		 {
			$result = $time.lang('second').lang('before');
		}
		 else
		 {
			$result = lang('now');
		}
	}
	 else
	 {
		$result = gmdate($dateformat, $timestamp + $timeoffset * 3600);
	}

	return $result;
}

//字符串时间化
function sstrtotime($string)
{
	global $_SG, $_Cfg;
	$time = '';
	if($string)
	{
		$time = strtotime($string);
		if(gmdate('H:i', $_SG['timestamp'] + $_Cfg['timeoffset'] * 3600) != date('H:i', $_SG['timestamp']))
		{
			$time = $time - $_Cfg['timeoffset'] * 3600;
		}
	}
	return $time;
}



//ob
function obclean()
{
	global $_Cfg;

	ob_end_clean();
	if ($_Cfg['gzipcompress'] && function_exists('ob_gzhandler'))
	{
		ob_start('ob_gzhandler');
	}
	 else
	 {
		ob_start();
	}
}

//模板调用
function template($name){
	global $_Cfg, $_SG;

	if(!empty($_SG['mobile'])){
		$objfile = S_ROOT.'./api/mobile/tpl_'.$name.'.php';
		if (!file_exists($objfile)) {
			showmsg('m_function_is_disable_on_wap');
		}
	}else{
		$objfile = S_ROOT.'./template/'.$_Cfg['template'].'/'.$name.'.php';
		if(!file_exists($objfile)){
		  showmsg('The Template Not Found!');
		}
	}
	return $objfile;
}


function xml_out($content)
{
	global $_SC;
	@header("Expires: -1");
	@header("Cache-Control: no-store, private, post-check=0, pre-check=0, max-age=0", FALSE);
	@header("Pragma: no-cache");
	@header("Content-type: application/xml; charset=$_SC[charset]");
	echo '<'."?xml version=\"1.0\" encoding=\"$_SC[charset]\"?>\n";
	echo "<root><![CDATA[".trim($content)."]]></root>";
	exit();
}

//rewrite链接
function rewrite_url($pre, $para)
{
	$para = str_replace(array('&','='), array('-', '-'), $para);
	return '<a href="'.$pre.$para.'.html"';
}

//外链
function iframe_url($url)
{
	$url = rawurlencode($url);
	return "<a href=\"link.php?url=http://$url\"";
}

//处理搜索关键字
function stripsearchkey($string)
{
	$string = trim($string);
	$string = str_replace('*', '%', addcslashes($string, '%_'));
	$string = str_replace('_', '\_', $string);
	return $string;
}

//是否屏蔽二级域名
function isholddomain($domain)
{
	global $_SCONFIG;

	$domain = strtolower($domain);

	if(preg_match("/^[^a-z]/i", $domain)) return true;
	$holdmainarr = empty($_SCONFIG['holddomain'])?array('www'):explode('|', $_SCONFIG['holddomain']);
	$ishold = false;
	foreach ($holdmainarr as $value)
	{
		if(strpos($value, '*') === false)
		{
			if(strtolower($value) == $domain)
			{
				$ishold = true;
				break;
			}
		}
		 else
		 {
			$value = str_replace('*', '', $value);
			if(@preg_match("/$value/i", $domain))
			{
				$ishold = true;
				break;
			}
		}
	}
	return $ishold;
}


//连接字符
function simplode($ids)
{
	return "'".implode("','", $ids)."'";
}

//显示进程处理时间
function debuginfo()
{
	global $_SG, $_Cfg,$db;

	if(empty($_Cfg['debuginfo'])){
		$mtime = explode(' ', microtime());
		$totaltime = number_format(($mtime[1] + $mtime[0] - $_SG['starttime']), 4);
		$_Cfg['debuginfo'] = 'Processed in '.$totaltime.' second(s), '.$db->querynum.' queries'.
				($_Cfg['gzipcompress'] ? ', Gzip enabled' : NULL);
	  }
	return $_Cfg['debuginfo'];
}

//格式化大小函数
function formatsize($size)
{
	$prec=3;
	$size = round(abs($size));
	$units = array(0=>" B ", 1=>" KB", 2=>" MB", 3=>" GB", 4=>" TB");
	if ($size==0) return str_repeat(" ", $prec)."0$units[0]";
	$unit = min(4, floor(log($size)/log(2)/10));
	$size = $size * pow(2, -10*$unit);
	$digi = $prec - 1 - floor(log($size)/log(10));
	$size = round($size * pow(10, $digi)) * pow(10, -$digi);
	return $size.$units[$unit];
}

//获取文件内容
function sreadfile($filename)
{
	$content = '';
	if(function_exists('file_get_contents'))
	{
		@$content = file_get_contents($filename);
	}
	 else
	 {
		if(@$fp = fopen($filename, 'r'))
		{
			@$content = fread($fp, filesize($filename));
			@fclose($fp);
		}
	}

	return $content;
}

//写入文件
function swritefile($filename, $writetext, $openmod='w')
{
	if(@$fp = fopen($filename, $openmod))
	{
		flock($fp, 2);
		fwrite($fp, $writetext);
		fclose($fp);
		return true;
	}
	 else
	 {
		runlog('error', "File: $filename write error.");
		return false;
	}
}

//产生随机字符
function random($length, $numeric = 0)
{
	PHP_VERSION < '4.2.0' ? mt_srand((double)microtime() * 1000000) : mt_srand();
	$seed = base_convert(md5(print_r($_SERVER, 1).microtime()), 16, $numeric ? 10 : 35);
	$seed = $numeric ? (str_replace('0', '', $seed).'012340567890') : ($seed.'zZ'.strtoupper($seed));
	$hash = '';
	$max = strlen($seed) - 1;
	for($i = 0; $i < $length; $i++)
	{
		$hash .= $seed[mt_rand(0, $max)];
	}
	return $hash;
}


//判断字符串是否存在
function strexists($haystack, $needle)
{
	return !(strpos($haystack, $needle) === FALSE);
}


//站点链接
function getsiteurl(){
	global $_Cfg;
	if(empty($_Cfg['siteurl'])){
	  if(empty($_SERVER['DOCUMENT_ROOT'])){
	    $_Cfg['rooturi'] = dirname($_SERVER['PHP_SELF']);
	    }else{
	    $a = ucfirst( str_replace('\\','/',$_SERVER['DOCUMENT_ROOT']) );
	    $b = ucfirst( str_replace('\\','/',dirname(__FILE__)) );
	    $_Cfg['rooturi'] = str_replace($a,'',$b);
	    }
	  $_Cfg['rooturi'] = strlen($_Cfg['rooturi']) < 6 ? '/':substr($_Cfg['rooturi'],0,-6);
	  $_Cfg['tpl'] = $_Cfg['rooturi'].'template/'.$_Cfg['template'].'/';
	  $_Cfg['siteurl'] = shtmlspecialchars('http://'.($_SERVER['HTTP_HOST'] ? $_SERVER['HTTP_HOST']:$_SERVER['SERVER_NAME'])).$_Cfg['rooturi'];
    }
	return $_Cfg['siteurl'];
}


//获取文件名后缀
function fileExt($filename)
{
	return strtolower(trim(substr(strrchr($filename, '.'), 1)));
}

//去掉slassh
function sstripslashes($string)
{
	if(is_array($string))
	{
		foreach($string as $key => $val)
		{
			$string[$key] = sstripslashes($val);
		}
	}
	 else
	 {
		$string = stripslashes($string);
	}
	return $string;
}



//编码转换
function siconv($str, $out_charset, $in_charset='')
{
	global $_SC;

	$in_charset = empty($in_charset)?strtoupper($_SC['charset']):strtoupper($in_charset);
	$out_charset = strtoupper($out_charset);
	if($in_charset != $out_charset)
	{
		if (function_exists('iconv') && (@$outstr = iconv("$in_charset//IGNORE", "$out_charset//IGNORE", $str)))
		{
			return $outstr;
		}
		 elseif (function_exists('mb_convert_encoding') && (@$outstr = mb_convert_encoding($str, $out_charset, $in_charset)))
		 {
			return $outstr;
		}
	}
	return $str;//转换失败
}

//处理上传图片连接
function pic_get($filepath, $thumb, $remote, $return_thumb=1)
{
	global $_SCONFIG, $_SC;

	if(empty($filepath))
	{
		$url = 'image/nopic.gif';
	}
	 else
	 {
		$url = $filepath;
		if($return_thumb && $thumb) $url .= '.thumb.jpg';
		if($remote)
		{
			$url = $_SCONFIG['ftpurl'].$url;
		}
		 else
		 {
			$url = $_SC['attachurl'].$url;
		}
	}

	return $url;
}

//获得封面图片链接
function pic_cover_get($pic, $picflag)
{
	global $_SCONFIG, $_SC;

	if(empty($pic))
	{
		$url = 'image/nopic.gif';
	}
	 else
	 {
		if($picflag == 1)
		{//本地
			$url = $_SC['attachurl'].$pic;
		}
		 elseif ($picflag == 2)
		 {//远程
			$url = $_SCONFIG['ftpurl'].$pic;
		}
		 else
		 {//网络
			$url = $pic;
		}
	}

	return $url;
}


//处理头像
function avatar($uid, $size='small', $returnsrc = FALSE)
{
	global $_SCONFIG, $_SN;

	$size = in_array($size, array('big', 'middle', 'small')) ? $size : 'small';
	$avatarfile = avatar_file($uid, $size);
	return $returnsrc ? UC_API.'/data/avatar/'.$avatarfile : '<img src="'.UC_API.'/data/avatar/'.$avatarfile.'" onerror="this.onerror=null;this.src=\''.UC_API.'/images/noavatar_'.$size.'.gif\'">';
}

//得到头像
function avatar_file($uid, $size)
{
	global $_SGLOBAL, $_SCONFIG;

	$type = empty($_SCONFIG['avatarreal'])?'virtual':'real';
	$var = "avatarfile_{$uid}_{$size}_{$type}";
	if(empty($_SGLOBAL[$var]))
	{
		$uid = abs(intval($uid));
		$uid = sprintf("%09d", $uid);
		$dir1 = substr($uid, 0, 3);
		$dir2 = substr($uid, 3, 2);
		$dir3 = substr($uid, 5, 2);
		$typeadd = $type == 'real' ? '_real' : '';
		$_SGLOBAL[$var] = $dir1.'/'.$dir2.'/'.$dir3.'/'.substr($uid, -2).$typeadd."_avatar_$size.jpg";
	}
	return $_SGLOBAL[$var];
}




//语言替换
function lang_replace($text, $vars)
{
	if($vars) {
		foreach ($vars as $k => $v) {
			$rk = $k + 1;
			$text = str_replace('\\'.$rk, $v, $text);
		}
	}
	return $text;
}


//截取链接
function sub_url($url, $length)
{
	if(strlen($url) > $length)
	{
		$url = str_replace(array('%3A', '%2F'), array(':', '/'), rawurlencode($url));
		$url = substr($url, 0, intval($length * 0.5)).' ... '.substr($url, - intval($length * 0.3));
	}
	return $url;
}



//取数组中的随机个
function sarray_rand($arr, $num=1)
{
	$r_values = array();
	if($arr && count($arr) > $num)
	{
		if($num > 1)
		{
			$r_keys = array_rand($arr, $num);
			foreach ($r_keys as $key)
			{
				$r_values[$key] = $arr[$key];
			}
		}
		 else
		 {
			$r_key = array_rand($arr, 1);
			$r_values[$r_key] = $arr[$r_key];
		}
	}
	 else
	 {
		$r_values = $arr;
	}
	return $r_values;
}


//产生form防伪码
function formhash(){
	global $_SG, $_Cfg;
	if(empty($_SG['formhash'])) {
	  $_SG['formhash'] = substr($_SG['authkey'],0,-7);
	}
	return $_SG['formhash'];
}

//UTF-8
function isUsername($username){
  //ANSI
  //return preg_match('/^[\u4E00-\u9FA5\w]{4,20}$/',$username);
  return strlen($username) > 4 && preg_match('/^[a-z0-9_\x80-\xff]+[^_]$/',$username);
  }


//检查邮箱是否有效
function isEmail($email) {
	return strlen($email) > 6 && preg_match("/^[\w\-\.]+@[\w\-\.]+(\.\w+)+$/", $email);
}

//检查密码格式是否正确
function isPw($pw){
  return preg_match('/^[\S]{6,}$/',$pw);
}

//验证提问
function question()
{
	global $_SGLOBAL;

	include_once(S_ROOT.'./data/data_spam.php');
	if($_SGLOBAL['spam']['question'])
	{
		$count = count($_SGLOBAL['spam']['question']);
		$key = $count>1?mt_rand(0, $count-1):0;
		ssetcookie('seccode', $key);
		echo $_SGLOBAL['spam']['question'][$key];
	}
}

//获取目录
function sreaddir($dir, $extarr=array())
{
	$dirs = array();
	if($dh = opendir($dir))
	{
		while (($file = readdir($dh)) !== false)
		{
			if(!empty($extarr) && is_array($extarr))
			{
				if(in_array(strtolower(fileext($file)), $extarr))
				{
					$dirs[] = $file;
				}
			}
			 else if($file != '.' && $file != '..')
			 {
				$dirs[] = $file;
			}
		}
		closedir($dh);
	}
	return $dirs;
}



//自定义分页
function mob_perpage($perpage)
{
	global $_SGLOBAL;

	$newperpage = isset($_GET['perpage'])?intval($_GET['perpage']):0;
	if($_SGLOBAL['mobile'] && $newperpage>0 && $newperpage < 500) {
		$perpage = $newperpage;
	}
	return $perpage;
}


//检查用户的特殊身份
function ckspacelog()
{
	global $_SGLOBAL;

	if(empty($_SGLOBAL['supe_uid'])) return false;
	$query = $_SGLOBAL['db']->query("SELECT * FROM ".tname('spacelog')." WHERE uid='$_SGLOBAL[supe_uid]'");
	if($value = $_SGLOBAL['db']->fetch_array($query))
	{
		if($value['expiration'] && $value['expiration'] <= $_SGLOBAL['timestamp'])
		{//到期
			$_SGLOBAL['db']->query("DELETE FROM ".tname('spacelog')." WHERE uid='$_SGLOBAL[supe_uid]'");
		}
		$expiration = sgmdate('Y-m-d H:i', $value['expiration']);
		showmessage('no_authority_expiration'.($value['expiration']?'_date':''), '', 1, array($expiration));
	}
}

//获取分页URL
function getfenyeUrl($param='page')
{
    unset($_GET[$param]);
    $_GET[$param] = $querystr = $comma = '';
    foreach($_GET as $key => $val)
    {
      $querystr .= $comma."$key=$val";
      $comma = '&amp;';
      }
    return '?'.$querystr;
  }

//跳转
function jump($url='',$msg='',$exit=1)
{
  $js = '<script language="javascript" type="text/javascript">';
  $js .= $msg ? 'alert("'.$msg.'");':'';
  $js .= $url ? 'window.location="'.$url.'";':'history.go(-1);';
  $js .= '</script>';
  if($exit){
    echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><meta http-equiv="Content-Type" content="text/html; charset=utf-8" /><title>&nbsp;&nbsp;</title></head><body>'.$js.'</body></html>';
    exit();
    }else{
    @header("contet-type:text/html;charset=utf-8");
    echo $js;
    }
  }

//新的分页函数
function fenye($fenyeurl,$page,$totalnum,$perpage,$rewrite=0){
  $page = max($page,1);
  $totalpage = ceil($totalnum/$perpage);
  $totalpage = $totalpage < 1 ? 1 :$totalpage;
  $rangepage = 6;

  if($page <= $rangepage){
    $startpage = 1;
    }else{
      if(($page + $rangepage) > $totalpage && ($totalpage - $rangepage*2) > 0){
        $startpage = $totalpage - $rangepage*2 + 1;
        }else{
          $startpage = $totalpage <= $rangepage*2 ? 1:($page - $rangepage);
        }
    }

  $fileext = $rewrite ? '.html':'';

  $html = '<ul>';
  $html .= '<li><a href="'.$fenyeurl.'1'.$fileext.'">&laquo;</a></li>';
  $html .= $page > 1 ? '<li><a href="'.$fenyeurl.($page-1).$fileext.'">[Prev]</a></li>':'';
  for($i = $startpage;$i < ($startpage + $rangepage*2);$i++){
    $html .= '<li><a href="'.$fenyeurl.$i.$fileext.'"'.($page == $i ? ' class="curr"':'').'>'.$i.'</a></li>';
    if($i == $totalpage) break;
    }
  $html .= $page < $totalpage ? '<li><a href="'.$fenyeurl.($page+1).$fileext.'">[Next]</a></li>':'';
  $html .= '<li><a href="'.$fenyeurl.$totalpage.$fileext.'">&raquo;</a></li>';
  $html .= '</ul>';

  return $html;
  }


function notfound()
{
  showmsg("很抱歉，该项没有找到!");
  }


function getData($sql)
{
  global $db;
  $query = $db->query($sql);
  $data = array();
  while($rs = $db->fetch_array($query)){
    $data[] = $rs;
    }
   return $data;
  }

//用户是否登录，公司和个人用户
function UserIsLogin($exit = 1 , $usertb = '') {
  global $_SG;
  if($_SG['uid'] <= 0 || empty($_SG['username'])){
      $qx = 0;
    }else{
      $qx = empty($usertb) ? 1 :($usertb == $_SG['usertb']);
    }
  if($exit && !$qx) showmsg("对不起，您没有这个权限!请先登录");
  return $qx;
  }


//get pic src from html code

function getpic($html)
{
  $regx = '/\<img|input[^src]*src="?([^"]+)"?/';
  if(preg_match($regx,$html,$match))
  {
    return $match[1];
    }
  return array();
  }

//strip pic html

function strippic($html)
{
  $regx = '/\<(img|input)[^\>]*\>/';
  return preg_replace($regx,'',$html);
  }


//获取图片的网址路径
function picurl($picurl,$thumb=0)
{
  global $_Cfg;
  return $_Cfg['siteurl'].$_Cfg['uploadurl'].$picurl.($thumb ? '.thumb.jpg':'');
  }


//获取图片的物理路径
function picpath($picurl,$thumb=0)
{
  global $_Cfg;
  return S_ROOT.$_Cfg['uploaddir'].$picurl.($thumb ? '.thumb.jpg':'');
  }


//站外搜索列链接表列表
function outso($kw,$totalnum=0)
{
  $html = '<ul class="outso">';
  $i = 0;
  $kwurl = urlencode( strip_tags( $kw ) );

  foreach($GLOBALS['_SG']['cache']['outso'] as $searchName => $searchUrl)
  {
    $html .= '<li><a href="'.$searchUrl.$kwurl.'" target="_blank"><span class="soname">'.$searchName.'</span>搜索“<span class="kw">'.$kw.'</span>”</a></li>';
    $i++;
    if($totalnum > 0 && $i == $totalnum) break;
  }
  $html.= '</ul>';
  return $html;
  }


//载入语言包！
function loadlanguage($lang='zh_CN'){
  global $_LANG;
  $fileArr = array('showmsg','source');
  foreach($fileArr as $f)
  {
    $file = S_ROOT.'./lang/'.$lang.'/'.$f.'.php';
    if(file_exists($file))
    {
      include_once $file;
    }
    else
    {
      if($lang == 'zh_CN') showmsg('Can\'t load language file '.$f.',beacuse not find! ');

      //如果不是zh_CN,载入zh_CN语言包
      loadlanguage();
    }
    }
  }

//返回提示文字 只source里面的有效
function __($langkey)
{
  global $_LANG;

  //$langkey 是小写的
  $langkey = strtolower($langkey);
  return array_key_exists($langkey,$_LANG['source']) ? $_LANG['source'][$langkey]:$langkey;
  }


//直接输出提示文字
function _e($langkey)
{
  echo __($langkey);
}


//详细页的链接 detail.php 包括评论链接的URL
function durl($id_md5)
{
  return $GLOBALS['_Cfg']['rooturi'].'public/'.$id_md5.'.html';
}

//对象转换为数组
function objectToArray(&$object)
{
 $object = (array)$object;
 foreach ($object as $key => $value) {
  if (is_object($value) || is_array($value)) {
   objectToArray($value);
   $object[$key] = $value;
  }
 }
 return $object;
}

//获取文件路径,指的是source目录中的
function sourcepath($name,$prefix = 'function',$exit = 0)
{
  $file = S_ROOT.'./source/'.$prefix.'_'.$name.'.php';
  if(file_exists($file)) {
    return $file;
  }else{
    if($exit) showmsg('PHP_INNER_ERROR! NOT FIND PHP SCRIPT ');
    return '';
  }
}


//--------------------------------------------------------------

//IP归属地查询
function convertip($ip) {

	$return = '';

	if(preg_match("/^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/", $ip)) {

		$iparray = explode('.', $ip);

		if($iparray[0] == 10 || $iparray[0] == 127 || ($iparray[0] == 192 && $iparray[1] == 168) || ($iparray[0] == 172 && ($iparray[1] >= 16 && $iparray[1] <= 31))) {
			$return = '- LAN';
		} elseif($iparray[0] > 255 || $iparray[1] > 255 || $iparray[2] > 255 || $iparray[3] > 255) {
			$return = '- Invalid IP Address';
		} else {

		  //IP dat 文件名结尾加了 _1983 防止他人直接下载
			$tinyipfile = S_ROOT.'./ipdata/tinyipdata_1983.dat';
			$fullipfile = S_ROOT.'./ipdata/qqwry_1983.dat';
			if(@file_exists($tinyipfile)) {
				$return = convertip_tiny($ip, $tinyipfile);
			} elseif(@file_exists($fullipfile)) {
				$return = convertip_full($ip, $fullipfile);
			}
		}
	}
	return $return;
}

function convertip_tiny($ip, $ipdatafile) {

	static $fp = NULL, $offset = array(), $index = NULL;

	$ipdot = explode('.', $ip);
	$ip    = pack('N', ip2long($ip));

	$ipdot[0] = (int)$ipdot[0];
	$ipdot[1] = (int)$ipdot[1];

	if($fp === NULL && $fp = @fopen($ipdatafile, 'rb')) {
		$offset = unpack('Nlen', fread($fp, 4));
		$index  = fread($fp, $offset['len'] - 4);
	} elseif($fp == FALSE) {
		return  '- Invalid IP data file';
	}

	$length = $offset['len'] - 1028;
	$start  = unpack('Vlen', $index[$ipdot[0] * 4] . $index[$ipdot[0] * 4 + 1] . $index[$ipdot[0] * 4 + 2] . $index[$ipdot[0] * 4 + 3]);

	for ($start = $start['len'] * 8 + 1024; $start < $length; $start += 8) {

		if ($index{$start} . $index{$start + 1} . $index{$start + 2} . $index{$start + 3} >= $ip) {
			$index_offset = unpack('Vlen', $index{$start + 4} . $index{$start + 5} . $index{$start + 6} . "\x0");
			$index_length = unpack('Clen', $index{$start + 7});
			break;
		}
	}

	fseek($fp, $offset['len'] + $index_offset['len'] - 1024);
	if($index_length['len']) {
		return fread($fp, $index_length['len']);
	} else {
		return 'Unknown';
	}
}

function convertip_full($ip, $ipdatafile) {

	if(!$fd = @fopen($ipdatafile, 'rb')) {
		return '- Invalid IP data file';
	}

	$ip = explode('.', $ip);
	$ipNum = $ip[0] * 16777216 + $ip[1] * 65536 + $ip[2] * 256 + $ip[3];

	if(!($DataBegin = fread($fd, 4)) || !($DataEnd = fread($fd, 4)) ) return;
	@$ipbegin = implode('', unpack('L', $DataBegin));
	if($ipbegin < 0) $ipbegin += pow(2, 32);
	@$ipend = implode('', unpack('L', $DataEnd));
	if($ipend < 0) $ipend += pow(2, 32);
	$ipAllNum = ($ipend - $ipbegin) / 7 + 1;

	$BeginNum = $ip2num = $ip1num = 0;
	$ipAddr1 = $ipAddr2 = '';
	$EndNum = $ipAllNum;

	while($ip1num > $ipNum || $ip2num < $ipNum) {
		$Middle= intval(($EndNum + $BeginNum) / 2);

		fseek($fd, $ipbegin + 7 * $Middle);
		$ipData1 = fread($fd, 4);
		if(strlen($ipData1) < 4) {
			fclose($fd);
			return '- System Error';
		}
		$ip1num = implode('', unpack('L', $ipData1));
		if($ip1num < 0) $ip1num += pow(2, 32);

		if($ip1num > $ipNum) {
			$EndNum = $Middle;
			continue;
		}

		$DataSeek = fread($fd, 3);
		if(strlen($DataSeek) < 3) {
			fclose($fd);
			return '- System Error';
		}
		$DataSeek = implode('', unpack('L', $DataSeek.chr(0)));
		fseek($fd, $DataSeek);
		$ipData2 = fread($fd, 4);
		if(strlen($ipData2) < 4) {
			fclose($fd);
			return '- System Error';
		}
		$ip2num = implode('', unpack('L', $ipData2));
		if($ip2num < 0) $ip2num += pow(2, 32);

		if($ip2num < $ipNum) {
			if($Middle == $BeginNum) {
				fclose($fd);
				return '- Unknown';
			}
			$BeginNum = $Middle;
		}
	}

	$ipFlag = fread($fd, 1);
	if($ipFlag == chr(1)) {
		$ipSeek = fread($fd, 3);
		if(strlen($ipSeek) < 3) {
			fclose($fd);
			return '- System Error';
		}
		$ipSeek = implode('', unpack('L', $ipSeek.chr(0)));
		fseek($fd, $ipSeek);
		$ipFlag = fread($fd, 1);
	}

	if($ipFlag == chr(2)) {
		$AddrSeek = fread($fd, 3);
		if(strlen($AddrSeek) < 3) {
			fclose($fd);
			return '- System Error';
		}
		$ipFlag = fread($fd, 1);
		if($ipFlag == chr(2)) {
			$AddrSeek2 = fread($fd, 3);
			if(strlen($AddrSeek2) < 3) {
				fclose($fd);
				return '- System Error';
			}
			$AddrSeek2 = implode('', unpack('L', $AddrSeek2.chr(0)));
			fseek($fd, $AddrSeek2);
		} else {
			fseek($fd, -1, SEEK_CUR);
		}

		while(($char = fread($fd, 1)) != chr(0))
		$ipAddr2 .= $char;

		$AddrSeek = implode('', unpack('L', $AddrSeek.chr(0)));
		fseek($fd, $AddrSeek);

		while(($char = fread($fd, 1)) != chr(0))
		$ipAddr1 .= $char;
	} else {
		fseek($fd, -1, SEEK_CUR);
		while(($char = fread($fd, 1)) != chr(0))
		$ipAddr1 .= $char;

		$ipFlag = fread($fd, 1);
		if($ipFlag == chr(2)) {
			$AddrSeek2 = fread($fd, 3);
			if(strlen($AddrSeek2) < 3) {
				fclose($fd);
				return '- System Error';
			}
			$AddrSeek2 = implode('', unpack('L', $AddrSeek2.chr(0)));
			fseek($fd, $AddrSeek2);
		} else {
			fseek($fd, -1, SEEK_CUR);
		}
		while(($char = fread($fd, 1)) != chr(0))
		$ipAddr2 .= $char;
	}
	fclose($fd);

	if(preg_match('/http/i', $ipAddr2)) {
		$ipAddr2 = '';
	}
	$ipaddr = "$ipAddr1 $ipAddr2";
	$ipaddr = preg_replace('/CZ88\.NET/is', '', $ipaddr);
	$ipaddr = preg_replace('/^\s*/is', '', $ipaddr);
	$ipaddr = preg_replace('/\s*$/is', '', $ipaddr);
	if(preg_match('/http/i', $ipaddr) || $ipaddr == '') {
		$ipaddr = '* Unknown *';
	}

	//return '- '.$ipaddr;
	return mb_convert_encoding($ipaddr,"utf-8","gb2312");

}


//--------------------------------------------------------------
//region 省市区 关系是否正确检查
function region_check($rid1 = 0,$rid2 = 0,$rid3 = 0){
  global $db;
  if($rid1 > 0){
    $rs1 = $db->fetch_first("select * from @#__region where rid = $rid1 and `type`=1 limit 1 ");
     if($rid2 > 0){
       $rs2 = $db->fetch_first("select * from @#__region where rid = $rid2 and `type`=2 and pid='$rid1' limit 1 ");
       if(!$rs2) return 0;
       if($rid3 > 0){
         $rs3 = $db->fetch_first("select * from @#__region where rid = $rid3 and `type`=3  and pid='$rid2' limit 1 ");
         return $rs3 ? 1:0;
       }
       return 1;
     }
    return 1;
  }
  return 1;
 }


//通过一个ID获取其完整的所属地
function getregion($rid,$type){
  global $db;
  $where = ($type < 4 && $type > 0) ? " where r".$type.".`rid`='$rid' and r".$type.".`type`='$type' ":'';
  $concat = $comma = '';
  for($i = 1;$i <= $type;$i++){
    $concat .= $comma.'r'.$i.'.name';
    $comma = ',';
    }
  if($concat == '') return '';
  $sql = "select concat($concat)
  from @#__region r1
  left join @#__region r2 on r2.pid=r1.rid and r2.type=2
  left join @#__region r3 on r3.pid=r2.rid and r3.type=3
  $where limit 1  ";
  return $db->result_first($sql);
  }

//--------------------------------------------------------------------------------
//检测是否是网址
function isUrl($url){
  return preg_match('/^https?:\/\/([\w-]+\.)*([\w-]+)\.([\w]+){2,5}(\/.+)?$/i',$url);
  }

//获取网址中的域名
function getHost($url){
  preg_match("/^(http:\/\/)?([^\/]+)/i",$url, $matches);
  return $matches[2];
  }

/*
  检测提交表单中的验证码是否填写正确!
  每次检查后将验证码立即清零,防止刷新页面反复提交数据
*/

function checkyzm($yzm,$exit = 1,$cookieId = 'seccode'){
  ssetcookie($cookieId,random(6));
  $result = strtolower($yzm) == authcode($GLOBALS['_SCOOKIE'][$cookieId]);
  if(!$result && $exit) showmsg('验证码填写错误！');
  return $result;
  }

/*
  比较看是否是最新的，添加日期为当前,用于记录添加的比较
*/
function isNewest($date1,$length = 2,$format = 'Ym'){
  return (date($format) - date($format,$date1) <= $length);
  }

//获取远程网址的内容,采用curl,一般用于外站的API
function gethttpcnt($url,$username = '',$password = '',$timeout = 5){
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $timeout);

  //在需要用户检测的网页里需要增加下面两行
  if($username && $password){
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_setopt($ch, CURLOPT_USERPWD, $username.":".$password);
    }
  $cnt = curl_exec($ch);
  curl_close($ch);
  return $cnt;
  }


//分钟转换为秒数,只能转换YY-mm-dd HH:ii:ss这样格式的时间
//sec_to_time 和 time_to_sec 其实这两个函数是mysql自带的
function time_to_sec($time){
  $time = trim($time);
  if(strpos($time,':') === FALSE) return intval($time);
  $array = explode(':',str_replace(array(' ','-'),':',$time));
  $array = array_reverse($array);
  $sec = 0;
  $arrayLength = count($array);
  for($i = 0;$i < $arrayLength; $i++){
    $sec += pow(60,$i)*$array[$i];
    }
    return $sec;
  }


//时间输入必须单位
function sec_to_time($sec,$isMillisecond = 0){
  /*
    但是土豆视频的单位是毫秒 1秒 = 1000毫秒
    如果isMillisecond == 1 的话，表示单位为毫秒，那么除以 1000
  */
	if($isMillisecond == 1) $sec /= 1000;

  //$arr = array(round($sec/3600),round($sec%3600/60),$sec%60);

  //如果超过1个小时只算分钟制
  $arr = array(round($sec/3600),round(($sec%3600)/60),$sec%60);
  if($arr[0] <= 0) unset($arr[0]);
  $time = $comma = '';
  foreach($arr as $val){
    $time .= $comma . sprintf('%02d',$val);
    $comma = ':';
    }
  return $time;
  }

//生成HTML select 选项
function optionMake($optionArray,$defaultValue = '',$nullTxt = '  '){
  $html = '';
  if(!empty($nullTxt)) $optionArray = array_merge(array($nullTxt),$optionArray);
  foreach($optionArray as $key => $val){
    $html .= '<option value="'.$key.'"';
    if($key == $defaultValue) $html .= ' selected="selected"';
    $html .= '>'.$val.'</option>';
    }
  return $html;
  }

/* year range */
function y2m2($y1,$m1,$y2 = 0,$m2 = 0,$split = '.'){
  $html = $y1.$split.$m1;
  $html .= ' -- ';
  $html .= ($y2 == 0 && $m2 == 0) ? '':$y2.$split.$m2;
  return $html;
  }

//检测用户是否存在? $password 是经过加密的
function userIsExist($username,$password){
  $sql = "select 1 from account where `name`='$username' limit 1 ";
  return $GLOBALS['db']->result_first($sql);
  }


?>