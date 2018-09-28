<?php

if(!defined('IN_ZBPHP'))
{
	exit('Access Denied');
}

//分类表缓存控制
function cat_cache_check(){
  global $_CACHE;
  cat_table_cache_check('','key','val');
  foreach($_CACHE['cat'] as $key => $val){
    if($key == 'confirm'){
        cat_table_cache_check($key,'key','val');
      }elseif(in_array($key,array('industry','profession'))){
        //有二级分类的表
        cat2_table_cache_check($key);
      }else{
        cat_table_cache_check($key);
      }
    }
  }


//分类表缓存生成,只弄一级分类的表
function cat_table_cache_check($tbname = '',$idFn = 'catid',$vlFn = 'catname',$orderFn = 'order',$orderType = 'ASC'){
  global $_CACHE,$db;
  $tbname = 'cat'.(empty($tbname) ? '':'_').$tbname;
  $cachefile_php = S_ROOT.'./data/cache_'.$tbname.'.php';
  $cachefile_js  = S_ROOT.'./data/cache_'.$tbname.'.js';

  if(file_exists($cachefile_php)){
    include_once $cachefile_php;
    }else{
      $sql = "select * from @#__{$tbname} order by `$orderFn` $orderType  ";
      $query = $db->query($sql);
      $_CACHE[$tbname] = array();
      while($rs = $db->fetch_array($query)){
        $_CACHE[$tbname][$rs[$idFn]] = $rs[$vlFn];
        }
      cache_write($tbname,'_CACHE[\''.$tbname.'\']',$_CACHE[$tbname]);
    }

   //生成JAVASCRIPT缓存文件
   if(!file_exists($cachefile_js)){
    jscache_write($tbname,$_CACHE[$tbname],$cachefile_js);
    }
  }


//分类表缓存生成,只弄有二级分类的表
function cat2_table_cache_check($tbname = '',$idFn = 'catid',$vlFn = 'catname',$orderFn = 'order',$fidFn = 'fid',$orderType = 'ASC'){
  global $_CACHE,$db;
  $tbname = 'cat'.(empty($tbname) ? '':'_').$tbname;
  $cachefile_php = S_ROOT.'./data/cache_'.$tbname.'.php';
  $cachefile_js  = S_ROOT.'./data/cache_'.$tbname.'.js';

  if(file_exists($cachefile_php)){
    include_once $cachefile_php;
    }else{
      $sql = "select * from @#__{$tbname} where `$fidFn`=0 order by `$orderFn`  $orderType  ";
      $query = $db->query($sql);
      $_CACHE[$tbname] = array();
      while($rs = $db->fetch_array($query)){
        $_CACHE[$tbname][$rs[$idFn]][$idFn] = $rs[$idFn];
        $_CACHE[$tbname][$rs[$idFn]][$vlFn] = $rs[$vlFn];
        $_CACHE[$tbname][$rs[$idFn]]['sub'] = array();
        $sql2 = "select * from @#__{$tbname} where `$fidFn`='$rs[$idFn]' order by `$orderFn` $orderType ";
        $query2 = $db->query($sql2);
        while($rs2 = $db->fetch_array($query2)){
          $_CACHE[$tbname][$rs[$idFn]]['sub'][$rs2[$idFn]][$idFn] = $rs2[$idFn];
          $_CACHE[$tbname][$rs[$idFn]]['sub'][$rs2[$idFn]][$vlFn] = $rs2[$vlFn];
          }
        }
      cache_write($tbname,'_CACHE[\''.$tbname.'\']',$_CACHE[$tbname]);
    }

   //生成JAVASCRIPT缓存文件
   if(!file_exists($cachefile_js)){
    jscache_write($tbname,$_CACHE[$tbname],$cachefile_js);
    }
  }



//省市区JS PHP缓存生成函数
function region_cache_check(){
  global $_CACHE,$db;
  $cachefile_php = S_ROOT.'./data/cache_region.php';
  $cachefile_js  = S_ROOT.'./data/cache_region.js';
  if(file_exists($cachefile_php)){
    include_once $cachefile_php;
    }else{
      $cacheArray_php = region_cache_data_get(0,1,0);
      cache_write('region','_CACHE[\'region\']',$cacheArray_php);
    }

  if(!file_exists($cachefile_js)){
    $cacheArray_js = region_cache_data_get(0,1);
    jscache_write('region',$cacheArray_js,$cachefile_js);
    }
  }


//获取region数据
function region_cache_data_get($type = 0,$pid = 1,$autoContinue = 1){
  global $db;
  $type++;
  $REC = array();
  $sql = "select * from @#__region where type='$type' and pid='$pid' order by rid asc ";
  $query = $db->query($sql);
  while($rs = $db->fetch_array($query)){

    //如果小于3的类型并且是自动获取下属分类,那么获取他的下属城市信息
    $rs['sub'] = ($rs['type'] < 3 && $autoContinue) ? region_cache_data_get($rs['type'],$rs['rid']):array();

    $REC[$rs['rid']] = array(
                       'rid'=> $rs['rid'],
                       'name'=> $rs['name'],
                       'sub'=> $rs['sub'],
                       );
    }
    return $REC;
  }

//更新配置文件
function config_cache($updatedata = true) {
  global $db,$_Cfg;
	$_Cfg = $GLOBALS['_SC'];
	/*
	$sql = "select * from @#__config ";
	$query = $db->query($sql);
	while($rs = $db->fetch_array($query))
	{
	  $_Cfg[$rs['key']] = $rs['val'];
	  }
	  */
  if(empty($_Cfg['siteurl'])) $_Cfg['siteurl'] = getsiteurl();
  cache_write('config','_Cfg',$_Cfg);
}

//递归清空目录
function deltreedir($dir){
	$files = sreaddir($dir);
	foreach ($files as $file)	{
		if(is_dir("$dir/$file")){
			deltreedir("$dir/$file");
		}	else {
			@unlink("$dir/$file");
		}
	}
}

//数组转换成字串
function arrayeval($array, $level = 0){
	$space = '';
	for($i = 0; $i <= $level; $i++)	{
		$space .= "\t";
	}
	$evaluate = "Array\n$space(\n";
	$comma = $space;
	foreach($array as $key => $val)	{
		$key = is_string($key) ? '\''.addcslashes($key, '\'\\').'\'' : $key;
		$val = !is_array($val) && (!preg_match("/^\-?\d+$/", $val) || strlen($val) > 12 || substr($val, 0, 1)=='0') ? '\''.addcslashes($val, '\'\\').'\'' : $val;
		if(is_array($val)) {
			$evaluate .= "$comma$key => ".arrayeval($val, $level + 1);
		}	else {
			$evaluate .= "$comma$key => $val";
		}
		$comma = ",\n$space";
	}
	$evaluate .= "\n$space)";
	return $evaluate;
}

//写入
function cache_write($filename, $varName, $values) {
	$cachefile = S_ROOT.'./data/cache_'.$filename.'.php';
	$cachetext = "<?php\r\n".
		"if(!defined('IN_ZBPHP')) exit('Access Denied');\r\n".
		'$'.$varName.'='.arrayeval($values).";\r\n".
		"\r\n?>";
	if(!swritefile($cachefile, $cachetext))
	{
		exit("File: $cachefile write error.");
	}
}

//JS缓存文件创建
function jscache_write($objName,$data,$cachefile){

  $json = "/**\r\n";
  $json.= "\tCache Make Time:".date("Y-m-d H:i:s")."\r\n";
  $json.= "\tDo Not Modify Me!\r\n";
  $json.= "\tCopyright:\t ".$GLOBALS['_Cfg']['sitename']."\r\n";
  $json.= "\tURL:\t".$GLOBALS['_Cfg']['siteurl']."\r\n";
  $json.= "\tThis is not a free soft!\r\n";
  $json.= "\t\r\n**/\r\n\r\n";
  $json.= 'var '.$objName.' = '.json_encode($data).";\r\n";

  //生成JS缓存文件
	if(!swritefile($cachefile, $json))
	{
		exit("File: $cachefile write error.");
	}
}

?>