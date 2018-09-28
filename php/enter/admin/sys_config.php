<?php

/**
 * 管理中心系统设置
 * @author
 */

define('IN_ECS', true);

/* 代码 */
require(dirname(__FILE__) . '/includes/init.php');

//require(dirname(__FILE__) . '/admin_class/sysClas.php');
//$sys_class = new sysClass();

if($GLOBALS['_CFG']['certificate_id']  == '')
{
    $certi_id='error';
}
else
{
    $certi_id=$GLOBALS['_CFG']['certificate_id'];
}

$sess_id = $GLOBALS['sess']->get_session_id();

$auth = mktime();
$ac = md5($certi_id.'SHOPEX_SMS'.$auth);
$url = 'http://service.shopex.cn/sms/index.php?certificate_id='.$certi_id.'&sess_id='.$sess_id.'&auth='.$auth.'&ac='.$ac;

/*------------------------------------------------------ */
//-- 列表编辑 ?act=list_edit
/*------------------------------------------------------ */
if ($_REQUEST['act'] == 'list_edit')
{
    /* 检查权限 */
   	admin_priv('set_config');

    /* 可选语言 */
    $dir = opendir('../languages');
    $lang_list = array();
    while (@$file = readdir($dir))
    {
        if ($file != '.' && $file != '..' &&  $file != '.svn' && $file != '_svn' && is_dir('../languages/' .$file))
        {
            $lang_list[] = $file;
        }
    }
    @closedir($dir);
 
    
    $smarty -> assign('here' , $_LANG['list_edit']);
    $smarty->assign('lang_list',    $lang_list);
    $smarty->assign('ur_here',      $_LANG['01_shop_config']);
    $ex = get_settings(null);
    $smarty->assign('group_list',$ex);
    
//    $smarty->assign('countries',    get_regions());

    if (strpos(strtolower($_SERVER['SERVER_SOFTWARE']), 'iis') !== false)
    {
        $rewrite_confirm = $_LANG['rewrite_confirm_iis'];
    }
    else
    {
        $rewrite_confirm = $_LANG['rewrite_confirm_apache'];
    }
    $smarty->assign('rewrite_confirm', $rewrite_confirm);

    $smarty->assign('cfg', $_CFG);

    assign_query_info();
    $smarty->display('sys_config.htm');
}
if($_REQUEST['act'] == 'admin_login'){
    require(ROOT_PATH . 'data/upload/serverData.php');  
    $server_url = $_SERVER['HTTP_HOST'];
    $server_id = $_CFG['fuwuqi_id'];
    $smarty -> assign('here' , $_LANG['login']);   
    $smarty -> assign('server_id' , $server_id);
    $smarty -> display('sys_adminlogin.htm');
}
if($_REQUEST['act'] == 'login_query'){	
    require(dirname(__FILE__) . '/admin_class/userLogin.php');
    $login = new userLogin($_GET);
    $rs = $login->ckUserExist();
    $login -> userid = $rs['id'];
    $login -> userid_4399 = $rs['password'];

    $login->dealLogin();
  
}
/*------------------------------------------------------ */
//-- 邮件服务器设置
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'mail_settings')
{
    /* 检查权限 */
    admin_priv('11_system');

    $arr = get_settings(array(5));

    assign_query_info();

    $smarty->assign('ur_here',      $_LANG['mail_settings']);
    $smarty->assign('cfg', $arr[5]['vars']);
    $smarty->display('shop_config_mail_settings.htm');
}

/*------------------------------------------------------ */
//-- 提交   ?act=post
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'post')
{
    $type = empty($_POST['type']) ? '' : $_POST['type'];

    /* 检查权限 */
    //admin_priv('11_system');

    /* 允许上传的文件类型 */
    $allow_file_types = '|GIF|JPG|PNG|BMP|SWF|DOC|XLS|PPT|MID|WAV|ZIP|RAR|PDF|CHM|RM|TXT|CERT|';

    /* 保存变量值 */
    $count = count($_POST['value']);

    $arr = array();
    $sql = 'SELECT id,code, value FROM ' .TBL_SYS_CONFIG;
    $res= $acc_db->query($sql);
    while($row = $acc_db->fetchRow($res))
    {
        $arr[$row['id']] = $row['value'];
        $mg[$row['id']] = $_LANG['cfg_name'][$row['code']];
    }
    $massage='';

    foreach ($_POST['value'] AS $key => $val)
    {
        if($arr[$key] != $val)
        {   
            $sql = "UPDATE " . TBL_SYS_CONFIG . " SET value = '" . trim($val) . "' WHERE id = '" . $key . "'";          
            $query=$acc_db->query($sql);
            if($query)
            {
            	$massage.=' '.$mg[$key].',';
            }
        }
    }
   
    /* 处理上传文件 */
    $file_var_list = array();
    $sql = "SELECT * FROM " . TBL_SYS_CONFIG . " WHERE parent_id > 0 AND type = 'file'";
    $res = $acc_db->query($sql);
    while ($row = $acc_db->fetchRow($res))
    {
        $file_var_list[$row['code']] = $row;
    }

    foreach ($_FILES AS $code => $file)
    {
        /* 判断用户是否选择了文件 */
        if ((isset($file['error']) && $file['error'] == 0) || (!isset($file['error']) && $file['tmp_name'] != 'none'))
        {
          
            /* 检查上传的文件类型是否合法 */
            if (!check_file_type($file['tmp_name'], $file['name'], $allow_file_types))
            {
                sys_msg(sprintf($_LANG['msg_invalid_file'], $file['name']));
            }
            else
            {
                $file_name = $file_var_list[$code]['store_dir'] . $file['name'];
                /* 判断是否上传成功 */
                if (move_upload_file($file['tmp_name'], $file_name))
                {
                    $sql = "UPDATE " . TBL_SYS_CONFIG . " SET value = '$file_name' WHERE code = '$code'";
                    $acc_db->query($sql);
                }
                else
                {
                    sys_msg(sprintf($_LANG['msg_upload_failed'], $file['name'], $file_var_list[$code]['store_dir']));
                }
            }
        }
    }
    /* 记录日志 */
    if($massage){
    	$cnt = $_SESSION['admin_name'].$_LANG['list_edit'].' → '.$_LANG['button_reset'].': '.$massage;
        $sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
        $acc_db -> query($sql);
    }
    
    //admin_log('', 'edit', 'sys_config');

    /* 清除缓存 */
    clear_all_files();
	if(extension_loaded('memcache') && class_exists('Memcache')){	
		$memcached -> delete("cfgcache");
	}
	
    $_CFG = load_config();
    

   $links[] = array('text' => $_LANG['back_sys_config'], 'href' => 'sys_config.php?act=list_edit');
   sys_msg($_LANG['save_success'], 0, $links);
    
}
if($_REQUEST['act'] == 'clear_ver'){
	admin_priv('clear_ver');
    //require("./includes/all_server_num.php");
	set_time_limit(0);	
	$geturl = $_CFG['apiurl']."getserverconfig.php"; 
    $serverconfig = gethttpcnt($geturl);
	$serverconfig = unserialize($serverconfig);
	
    $type=$_REQUEST['type'];
    foreach ($serverconfig['arr_s'] as $key => $value){
    	for($i=1;$i<=$value;$i++){
    		if($i<=$serverconfig['hefu'][$key]&&$i%2==0){
    			continue;
    		}
    		$info[$key][]=$i;
    	}
    }
    foreach ($serverconfig['arr_s'] as $key => $value){
    	if(!empty($_POST['s_'.$key])){
    		$server[$key]=$_POST['s_'.$key];
    	}
    }

 
    if(!empty($server)){
        foreach ($server as $key => $value){
		    foreach($value as $k => $v){
		        $str = "http://".$serverconfig['prevpages'][$key]['prev'].$v.$serverconfig['prevpages'][$key]['last']."/interface/unlink.php?type=$type";
		        if($key=='Wtest'){
		        	$str = "http://113.107.160.179:8099/interface/unlink.php?type=$type";
		        }
		        $clear_url[] = $str;
		    }
        }
    }


	if(!empty($clear_url)){
	    foreach($clear_url as $key => $value){
		    $result = gethttpcnt($value);
		    $data[$key] = $result;
		}
		foreach ($data as $k => $v){
	        if(!$v){
		        $err_data[] = $k; 
		    }
        }
	}
    if(!empty($data)&&empty($err_data)){
    	sys_msg($_LANG['success_notice_'.$type],0,$links);
    }
//	$links[] = array('text' => $_LANG['back_sys_config'],'href' => 'sys_config.php?act=list_edit');
//	$smarty -> assign('here' , $_LANG['clear_'.$type]);
	$smarty -> assign('info',$info);
	$smarty -> assign('type',$type);

    $smarty -> display('sys_config_clear.htm');
	
	//sys_msg($_LANG['success_notice'],0,$links);
}
/*------------------------------------------------------ */
//-- 发送测试邮件
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'send_test_email')
{
    /* 检查权限 */
    check_authz_json('11_system');

    /* 取得参数 */
    $email          = trim($_POST['email']);

    /* 更新配置 */
    $_CFG['mail_service'] = intval($_POST['mail_service']);
    $_CFG['smtp_host']    = trim($_POST['smtp_host']);
    $_CFG['smtp_port']    = trim($_POST['smtp_port']);
    $_CFG['smtp_user']    = json_str_iconv(trim($_POST['smtp_user']));
    $_CFG['smtp_pass']    = trim($_POST['smtp_pass']);
    $_CFG['smtp_mail']    = trim($_POST['reply_email']);
    $_CFG['mail_charset'] = trim($_POST['mail_charset']);

    if (send_mail('', $email, $_LANG['test_mail_title'], $_LANG['cfg_name']['email_content'], 0))
    {
        make_json_result('', $_LANG['sendemail_success'] . $email);
    }
    else
    {
        make_json_error(join("\n", $err->_message));
    }
}

/*------------------------------------------------------ */
//-- 删除上传文件
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'del')
{
    /* 检查权限 */
    check_authz_json('11_system');

    /* 取得参数 */
    $code          = trim($_GET['code']);

    $filename = $_CFG[$code];

    //删除文件
    @unlink($filename);

    //更新设置
    update_configure($code, '');

    /* 记录日志 */
    admin_log('', 'edit', 'shop_config');

    /* 清除缓存 */
    clear_all_files();

    sys_msg($_LANG['save_success'], 0);

}

/**
 * 设置系统设置
 *
 * @param   string  $key
 * @param   string  $val
 *
 * @return  boolean
 */
function update_configure($key, $val='')
{
    if (!empty($key))
    {
        $sql = "UPDATE " . TBL_SHOP_CONFIG . " SET value='$val' WHERE code='$key'";

        return $GLOBALS['acc_db']->query($sql);
    }

    return true;
}

/**
 * 获得设置信息
 *
 * @param   array   $groups     需要获得的设置组
 * @param   array   $excludes   不需要获得的设置组
 *
 * @return  array
 */
function get_settings($groups=null, $excludes=null)
{
    global $acc_db, $ecs, $_LANG;

    $config_groups = '';
    $excludes_groups = '';

    if (!empty($groups))
    {
        foreach ($groups AS $key=>$val)
        {
            $config_groups .= " AND (id='$val' OR parent_id='$val')";
        }
    }

    if (!empty($excludes))
    {
        foreach ($excludes AS $key=>$val)
        {
            $excludes_groups .= " AND (parent_id<>'$val' AND id<>'$val')";
        }
    }

    /* 取出全部数据：分组和变量 */
    $sql = "SELECT * FROM " . TBL_SYS_CONFIG .
            " WHERE type<>'hidden' $config_groups $excludes_groups ORDER BY parent_id, sort_order, id";
    $item_list = $acc_db->getAll($sql);
    
    /* 整理数据 */
    $group_list = array();
    foreach ($item_list AS $key => $item)
    {
        $pid = $item['parent_id'];
        $item['name'] = isset($_LANG['cfg_name'][$item['code']]) ? $_LANG['cfg_name'][$item['code']] : $item['code'];
        $item['desc'] = isset($_LANG['cfg_desc'][$item['code']]) ? $_LANG['cfg_desc'][$item['code']] : '';

        if ($item['code'] == 'sms_shop_mobile')
        {
            $item['url'] = 1;
        }
        if ($pid == 0)
        {
            /* 分组 */
            if ($item['type'] == 'group')
            {
                $group_list[$item['id']] = $item;
            }
        }
        else
        {
            /* 变量 */
            if (isset($group_list[$pid]))
            {
                if ($item['store_range'])
                {
                    $item['store_options'] = explode(',', $item['store_range']);

                    foreach ($item['store_options'] AS $k => $v)
                    {
                        $item['display_options'][$k] = isset($_LANG['cfg_range'][$item['code']][$v]) ?
                                $_LANG['cfg_range'][$item['code']][$v] : $v;
                    }
                }
                $group_list[$pid]['vars'][] = $item;
            }
        }

    }
//    print_r($group_list);
    return $group_list;
}

?>