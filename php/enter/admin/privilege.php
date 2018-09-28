<?php

/**
 * 管理员信息以及权限管理程序
 * @author King.L
*/


define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

/* act操作项的初始化 */
if (empty($_REQUEST['act']))
{
    $_REQUEST['act'] = 'login';
}
else
{
    $_REQUEST['act'] = trim($_REQUEST['act']);
}

/* 初始化 $exc 对象 */
$exc = new exchange(TBL_ADMIN_USER, $acc_db, 'user_id', 'user_name');

/*------------------------------------------------------ */
//-- 退出登录
/*------------------------------------------------------ */
if ($_REQUEST['act'] == 'logout')
{
    /* 清除cookie */
    setcookie('ECSCP[admin_id]',   '', 1);
    setcookie('ECSCP[admin_pass]', '', 1);

    $sess->destroy_session();

    $_REQUEST['act'] = 'login';
}

/*------------------------------------------------------ */
//-- 登陆界面
/*------------------------------------------------------ */
if ($_REQUEST['act'] == 'login')
{
    
	header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
    header("Cache-Control: no-cache, must-revalidate");
    header("Pragma: no-cache");

    if ((intval($_CFG['captcha']) & CAPTCHA_ADMIN) && gd_version() > 0)
    {
        $smarty->assign('gd_version', gd_version());
        $smarty->assign('random',     mt_rand());
    }

    $smarty->display('login.htm');
}

/*------------------------------------------------------ */
//-- 验证登陆信息
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'signin')
{
	if (!empty($_SESSION['captcha_word']) && (intval($_CFG['captcha']) & CAPTCHA_ADMIN))
    {
        include_once(ROOT_PATH . 'includes/cls_captcha.php');

        /* 检查验证码是否正确 */
        $validator = new captcha();
        if (!empty($_POST['captcha']) && !$validator->check_word($_POST['captcha']))
        {
            sys_msg($_LANG['captcha_error'], 1);
        }
    }
	//访问的IP限定
	$allowIpArr = explode("|",$_CFG['ADIP']);
	
	if(!in_array($_SERVER['REMOTE_ADDR'],$allowIpArr)){	
		//exit("403");
	}
	
	if(!isset($_REQUEST['username'])){
		if(isset($_POST['username'])){
			$username = trim($_POST['username']);
		}else{
			$username = '';
		}
	}else{
		$username = $_REQUEST['username'];
	}
	
	if(!isset($_REQUEST['password'])){
		if(isset($_POST['password'])){
			$password = trim($_POST['[password']);
		}else{
			$password = '';
		}
	}else{
		$password = $_REQUEST['password'];
	}
	
  	$action = isset($_REQUEST['actions'])?trim($_REQUEST['actions']):'';
	
    $url = $_CFG['apiurl']."getadminuser.php?username=".$username."&password=".$password."&action=".$action; 
	
	$row = gethttpcnt($url);
  	$row = unserialize($row);
	if(!$row){
		$sql = "select * from admin_user where user_name = '".$_REQUEST['username']."' and password = '".md5($password)."'";
		$row = $acc_db -> getRow($sql);
	}
    if ($row)
    {
        // 登录成功
        set_admin_session($row['user_id'], $row['user_name'], $row['action_list'], $row['last_login']);
        $_SESSION['suppliers_id'] = $row['suppliers_id'];

        if($row['action_list'] == 'all' && empty($row['last_login']))
        {
            $_SESSION['shop_guide'] = true;
        }

        // 更新最后登录时间和IP
        $acc_db->query("UPDATE " .TBL_ADMIN_USER.
                 " SET last_login='" . time() . "', last_ip='" . real_ip() . "'".
                 " WHERE user_id='$_SESSION[admin_id]'");
        
        $cnt = $row['user_name']."登陆后台";
		$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION[admin_id]."','".$cnt."','".real_ip()."')";
		$acc_db -> query($sql);
        
		
		if (isset($_POST['remember']))
        {
            $time = gmtime() + 3600 * 24 * 365;
            setcookie('ECSCP[admin_id]',   $row['user_id'],                            $time);
            setcookie('ECSCP[admin_pass]', md5($row['password'] . $_CFG['hash_code']), $time);
        }

		
		$_REQUEST['act'] = '';
		jump("index.php");
        //ecs_header("Location: ./index.php\n");

        exit;
    }
    else
    {
        sys_msg($_LANG['login_faild'], 1);
    }
}

/*------------------------------------------------------ */
//-- 管理员列表页面
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'list')
{
	$result = get_admin_userlist();	
    /* 模板赋值 */
    $smarty->assign('ur_here',     $_LANG['admin_list']);
    $smarty->assign('action_link', array('href'=>'privilege.php?act=add', 'text' => $_LANG['admin_add']));
    $smarty->assign('full_page',   1);
    $smarty->assign('admin_list',  $result);
    $smarty->assign('admin_list',  $result);
    $smarty->assign('here',  $_LANG['list']);
    /* 显示页面 */
    assign_query_info();
    $smarty->display('privilege_list.htm');
}

/*------------------------------------------------------ */
//-- 查询
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'query')
{
    $smarty->assign('admin_list',  get_admin_userlist());

    make_json_result($smarty->fetch('privilege_list.htm'));
}

/*------------------------------------------------------ */
//-- 添加管理员页面
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'add')
{
    /* 检查权限 */
    admin_priv('admin_manage');

     /* 模板赋值 */
    $smarty->assign('ur_here',     $_LANG['admin_add']);
    $smarty->assign('action_link', array('href'=>'privilege.php?act=list', 'text' => $_LANG['admin_list']));
    $smarty->assign('form_act',    'insert');
    $smarty->assign('action',      'add');
    $smarty->assign('select_role',  get_role_list());

    /* 显示页面 */
    assign_query_info();
    $smarty->display('privilege_info.htm');
}

/*------------------------------------------------------ */
//-- 添加管理员的处理
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'insert')
{
    admin_priv('admin_manage');

    /* 判断管理员是否已经存在 */
    if (!empty($_POST['user_name']))
    {
        $is_only = $exc->is_only('user_name', stripslashes($_POST['user_name']));

        if (!$is_only)
        {
            sys_msg(sprintf($_LANG['user_name_exist'], stripslashes($_POST['user_name'])), 1);
        }
    }

    /* Email地址是否有重复 */
    if (!empty($_POST['email']))
    {
        $is_only = $exc->is_only('email', stripslashes($_POST['email']));

        if (!$is_only)
        {
            sys_msg(sprintf($_LANG['email_exist'], stripslashes($_POST['email'])), 1);
        }
    }

    /* 获取添加日期及密码 */
    $add_time = gmtime();
    
    $password  = md5($_POST['password']);
    $role_id = '';
    $action_list = '';
    if (!empty($_POST['select_role']))
    {
        $sql = "SELECT action_list FROM " . TBL_ADMIN_ROLE . " WHERE role_id = '".$_POST['select_role']."'";
        $row = $acc_db->getRow($sql);
        $action_list = $row['action_list'];
        $role_id = $_POST['select_role'];
    }

        $sql = "SELECT nav_list FROM " . TBL_ADMIN_USER . " WHERE action_list = 'all'";
        $row = $acc_db->getRow($sql);


    $sql = "INSERT INTO ".TBL_ADMIN_USER." (user_name, email, password, add_time, nav_list, action_list, role_id) ".
           "VALUES ('".trim($_POST['user_name'])."', '".trim($_POST['email'])."', '$password', '$add_time', '$row[nav_list]', '$action_list', '$role_id')";

    $acc_db->query($sql);
    /* 转入权限分配列表 */
    $new_id = $acc_db->Insert_ID();

    /*添加链接*/
    $link[0]['text'] = $_LANG['go_allot_priv'];
    $link[0]['href'] = 'privilege.php?act=allot&id='.$new_id.'&user='.$_POST['user_name'].'';

    $link[1]['text'] = $_LANG['continue_add'];
    $link[1]['href'] = 'privilege.php?act=add';

    sys_msg($_LANG['add'] . "&nbsp;" .$_POST['user_name'] . "&nbsp;" . $_LANG['action_succeed'],0, $link);

    /* 记录管理员操作 */
    admin_log($_POST['user_name'], 'add', 'privilege');
 }

/*------------------------------------------------------ */
//-- 编辑管理员信息
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'edit')
{
    /* 不能编辑demo这个管理员 */
    if ($_SESSION['admin_name'] == 'admin')
    {
       $link[] = array('text' => $_LANG['back_list'], 'href'=>'privilege.php?act=list');
       sys_msg($_LANG['edit_admininfo_cannot'], 0, $link);
    }

    $_REQUEST['id'] = !empty($_REQUEST['id']) ? intval($_REQUEST['id']) : 0;

    /* 查看是否有权限编辑其他管理员的信息 */
    if ($_SESSION['admin_id'] != $_REQUEST['id'])
    {
        admin_priv('admin_manage');
    }

    /* 获取管理员信息 */
    $sql = "SELECT user_id, user_name, email, password, agency_id, role_id FROM " .TBL_ADMIN_USER.
           " WHERE user_id = '".$_REQUEST['id']."'";
    $user_info = $acc_db->getRow($sql);


    /* 取得该管理员负责的办事处名称 */
//    if ($user_info['agency_id'] > 0)
//    {
//        $sql = "SELECT agency_name FROM " . $ecs->table('agency') . " WHERE agency_id = '$user_info[agency_id]'";
//        $user_info['agency_name'] = $db->getOne($sql);
//    }

    /* 模板赋值 */
    $smarty->assign('ur_here',     $_LANG['admin_edit']);
    $smarty->assign('action_link', array('text' => $_LANG['admin_list'], 'href'=>'privilege.php?act=list'));
    $smarty->assign('user',        $user_info);

    /* 获得该管理员的权限 */
    $priv_str = $acc_db->getOne("SELECT action_list FROM " .TBL_ADMIN_USER. " WHERE user_id = '$_GET[id]'");

    /* 如果被编辑的管理员拥有了all这个权限，将不能编辑 */
    if ($priv_str != 'all')
    {
       $smarty->assign('select_role',  get_role_list());
    }
    $smarty->assign('form_act',    'update');
    $smarty->assign('action',      'edit');

    assign_query_info();
    $smarty->display('privilege_info.htm');
}

/*------------------------------------------------------ */
//-- 更新管理员信息
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'update' || $_REQUEST['act'] == 'update_self')
{
	require(dirname(__FILE__) . '/includes/all_server_num.php');
    /* 变量初始化 */
    $admin_id    = !empty($_REQUEST['id'])        ? intval($_REQUEST['id'])      : 0;
    $admin_name  = !empty($_REQUEST['user_name']) ? trim($_REQUEST['user_name']) : '';
    $password = !empty($_POST['new_password']) ? ", password = '".md5($_POST['new_password'])."'"    : '';
    if ($_REQUEST['act'] == 'update')
    {
        /* 查看是否有权限编辑其他管理员的信息 */
    	if ($_SESSION['admin_id'] != $_REQUEST['id'])
        {
            admin_priv('admin_manage');
        }
        $g_link = 'privilege.php?act=list';
        $nav_list = '';
    }
    else
    {
        $nav_list = !empty($_POST['nav_list'])     ? ", nav_list = '".@join(",", $_POST['nav_list'])."'" : '';
        $admin_id = $_SESSION['admin_id'];
        $g_link = 'privilege.php?act=modif';
    }
    /* 判断管理员是否已经存在 */
    if (!empty($admin_name))
    {
        $is_only = $exc->num('user_name', stripslashes($admin_name), $admin_id);
        if ($is_only == 1)
        {
            sys_msg(sprintf($_LANG['user_name_exist'], stripslashes($admin_name)), 1);
        }
    }

    //如果要修改密码
    $pwd_modified = false;

    if (!empty($_POST['new_password']))
    {
        /* 查询旧密码并与输入的旧密码比较是否相同 */
        $sql = "SELECT password FROM ".TBL_ADMIN_USER." WHERE user_id = '$admin_id'";
        $old_password = $acc_db->getOne($sql);
        if ($old_password <> (md5($_POST['old_password'])))
        {
           $link[] = array('text' => $_LANG['go_back'], 'href'=>'javascript:history.back(-1)');
           sys_msg($_LANG['pwd_error'], 0, $link);
        }

        /* 比较新密码和确认密码是否相同 */
        if ($_POST['new_password'] <> $_POST['pwd_confirm'])
        {
           $link[] = array('text' => $_LANG['go_back'], 'href'=>'javascript:history.back(-1)');
           sys_msg($_LANG['js_languages']['password_error'], 0, $link);
        }
        else
        {
            $pwd_modified = true;
        }
    }

    $role_id = '';
    $action_list = '';
    if (!empty($_POST['select_role']))
    {
        $sql = "SELECT action_list FROM " . TBL_ADMIN_ROLE . " WHERE role_id = '".$_POST['select_role']."'";
        $row = $acc_db->getRow($sql);
        $action_list = ', action_list = \''.$row['action_list'].'\'';
        $role_id = ', role_id = '.$_POST['select_role'].' ';
    }
    //更新管理员信息
    $sql = "UPDATE " .TBL_ADMIN_USER. " SET ".
           "user_name = '$admin_name'".
           $action_list.
           $role_id.
           $password.
           $nav_list.
           "WHERE user_id = '$admin_id'";
	 
   	$acc_db->query($sql);
   	//组装url
//   	$url_info = $_SERVER['HTTP_HOST'];
//   	$sid = strpos($url_info,"-") - 1;
//   	$url_info = $url_info[$sid];
//   	$num = $arr_s[$_CFG['servername']];
//   	/*后台程序更新的服务器数量 */
//   	$update_num = $_CFG['update_num'];
//   	while($num > 0 && $update_num > 0){
//   		if($num != $url_info){
//   			$str = "http://".$pages[$_CFG['servername']]['prev'].$num."-".$pages[$_CFG['servername']]['last'];
//   			$str .= "/edit_pwd.php?password=".$_POST['new_password']."&role_id=".$_POST['select_role']."&action_list=".$row['action_list']."&user_name=".$admin_name."&user_id=".$admin_id;
//   			
//   			$url[] = $str;
//   		}	
//   		$num --;
//   	}

//   	$result = edit_all_pwd($url);
   
//   	if(count($result > 0)){  		
//   		$links[] = array('text' => $_LANG['back_pri'], 'href' => 'privilege.php?act=edit');
//   		sys_msg($_LANG['err_pwd'],1,$links);
//   	}
//   	exit;
   	/* 记录管理员操作 */
   	admin_log($_POST['user_name'], 'edit', 'privilege');
   

   	/* 如果修改了密码，则需要将session中该管理员的数据清空 */
   	if ($pwd_modified && $_REQUEST['act'] == 'update_self')
   	{
    	$sess->delete_spec_admin_session($_SESSION['admin_id']);
       	$msg = $_LANG['edit_password_succeed'];
   	}
   	else
   	{
       	$msg = $_LANG['edit_profile_succeed'];
   	}

   	/* 提示信息 */
   	$link[] = array('text' => strpos($g_link, 'list') ? $_LANG['back_admin_list'] : $_LANG['modif_info'], 'href'=>$g_link);
   	sys_msg("$msg<script>parent.document.getElementById('header-frame').contentWindow.document.location.reload();</script>", 0, $link);

}

/*------------------------------------------------------ */
//-- 编辑个人资料
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'modif')
{
    /* 不能编辑demo这个管理员 */
    if ($_SESSION['admin_name'] == 'demo')
    {
       $link[] = array('text' => $_LANG['back_admin_list'], 'href'=>'privilege.php?act=list');
       sys_msg($_LANG['edit_admininfo_cannot'], 0, $link);
    }

    include_once('includes/inc_menu.php');

    /* 包含插件菜单语言项 */
    $sql = "SELECT code FROM ".TBL_PLUGINS;
    $rs = $acc_db->query($sql);
    while ($row = $acc_db->FetchRow($rs))
    {
        /* 取得语言项 */
        if (file_exists(ROOT_PATH.'plugins/'.$row['code'].'/languages/common_'.$_CFG['lang'].'.php'))
        {
            include_once(ROOT_PATH.'plugins/'.$row['code'].'/languages/common_'.$_CFG['lang'].'.php');
        }

        /* 插件的菜单项 */
        if (file_exists(ROOT_PATH.'plugins/'.$row['code'].'/languages/inc_menu.php'))
        {
            include_once(ROOT_PATH.'plugins/'.$row['code'].'/languages/inc_menu.php');
        }
    }

    foreach ($modules AS $key => $value)
    {
        ksort($modules[$key]);
    }
    ksort($modules);

    foreach ($modules AS $key => $val)
    {
        $menus[$key]['label'] = $_LANG[$key];
        if (is_array($val))
        {
            foreach ($val AS $k => $v)
            {
                $menus[$key]['children'][$k]['label']  = $_LANG[$k];
                $menus[$key]['children'][$k]['action'] = $v;
            }
        }
        else
        {
            $menus[$key]['action'] = $val;
        }
    }

    /* 获得当前管理员数据信息 */
    $sql = "SELECT user_id, user_name, email, nav_list ".
           "FROM " .TBL_ADMIN_USER. " WHERE user_id = '".$_SESSION['admin_id']."'";
    $user_info = $acc_db->getRow($sql);

    /* 获取导航条 */
    $nav_arr = (trim($user_info['nav_list']) == '') ? array() : explode(",", $user_info['nav_list']);
    $nav_lst = array();
    foreach ($nav_arr AS $val)
    {
        $arr              = explode('|', $val);
        $nav_lst[$arr[1]] = $arr[0];
    }

    /* 模板赋值 */
    $smarty->assign('lang',        $_LANG);
    $smarty->assign('ur_here',     $_LANG['modif_info']);
    $smarty->assign('action_link', array('text' => $_LANG['admin_list'], 'href'=>'privilege.php?act=list'));
    $smarty->assign('user',        $user_info);
    $smarty->assign('menus',       $modules);
    $smarty->assign('nav_arr',     $nav_lst);

    $smarty->assign('form_act',    'update_self');
    $smarty->assign('action',      'modif');

    /* 显示页面 */
    assign_query_info();
    $smarty->display('privilege_info.htm');
}

/*------------------------------------------------------ */
//-- 为管理员分配权限
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'allot')
{
    include_once(ROOT_PATH . 'languages/' .$_CFG['lang']. '/admin/priv_action.php');

    admin_priv('allot_priv');
    if ($_SESSION['admin_id'] == $_GET['id'])
    {
        admin_priv('all');
    }

    /* 获得该管理员的权限 */
    $priv_str = $acc_db->getOne("SELECT action_list FROM " .TBL_ADMIN_USER. " WHERE user_id = '$_GET[id]'");

    /* 如果被编辑的管理员拥有了all这个权限，将不能编辑 */
    if ($priv_str == 'all')
    {
       $link[] = array('text' => $_LANG['back_admin_list'], 'href'=>'privilege.php?act=list');
       sys_msg($_LANG['edit_admininfo_cannot'], 0, $link);
    }

    /* 获取权限的分组数据 */
    $sql_query = "SELECT action_id, parent_id, action_code,relevance FROM " .TBL_ADMIN_ACTION.
                 " WHERE parent_id = 0";
    $res = $acc_db->query($sql_query);
    while ($rows = $acc_db->FetchRow($res))
    {
        $priv_arr[$rows['action_id']] = $rows;
    }

    /* 按权限组查询底级的权限名称 */
    $sql = "SELECT action_id, parent_id, action_code,relevance FROM " .TBL_ADMIN_ACTION.
           " WHERE parent_id " .db_create_in(array_keys($priv_arr));
    $result = $acc_db->query($sql);
    while ($priv = $acc_db->FetchRow($result))
    {
        $priv_arr[$priv["parent_id"]]["priv"][$priv["action_code"]] = $priv;
    }
    
    // 将同一组的权限使用 "," 连接起来，供JS全选
    foreach ($priv_arr AS $action_id => $action_group)
    {
        $priv_arr[$action_id]['priv_list'] = join(',', @array_keys($action_group['priv']));

        foreach ($action_group['priv'] AS $key => $val)
        {
            $priv_arr[$action_id]['priv'][$key]['cando'] = (strpos($priv_str, $val['action_code']) !== false || $priv_str == 'all') ? 1 : 0;
        }
    }

    /* 赋值 */
    $smarty->assign('lang',        $_LANG);
    $smarty->assign('ur_here',     $_LANG['allot_priv'] . ' [ '. $_GET['user'] . ' ] ');
    $smarty->assign('action_link', array('href'=>'privilege.php?act=list', 'text' => $_LANG['admin_list']));
    $smarty->assign('priv_arr',    $priv_arr);
    $smarty->assign('form_act',    'update_allot');
    $smarty->assign('user_id',     $_GET['id']);

    /* 显示页面 */
    assign_query_info();
    $smarty->display('privilege_allot.htm');
}

/*------------------------------------------------------ */
//-- 更新管理员的权限
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'update_allot')
{
    admin_priv('admin_manage');

    /* 取得当前管理员用户名 */
    $admin_name = $acc_db->getOne("SELECT user_name FROM " .TBL_ADMIN_USER. " WHERE user_id = '$_POST[id]'");

    /* 更新管理员的权限 */
    $act_list = @join(",", $_POST['action_code']);
    $sql = "UPDATE " .TBL_ADMIN_USER. " SET action_list = '$act_list', role_id = '' ".
           "WHERE user_id = '$_POST[id]'";

    $acc_db->query($sql);
    /* 动态更新管理员的SESSION */
    if ($_SESSION["admin_id"] == $_POST['id'])
    {
        $_SESSION["action_list"] = $act_list;
    }

    /* 记录管理员操作 */
    admin_log(addslashes($admin_name), 'edit', 'privilege');

    /* 提示信息 */
    $link[] = array('text' => $_LANG['back_admin_list'], 'href'=>'privilege.php?act=list');
    sys_msg($_LANG['edit'] . "&nbsp;" . $admin_name . "&nbsp;" . $_LANG['action_succeed'], 0, $link);

}

/*------------------------------------------------------ */
//-- 删除一个管理员
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'remove')
{
	admin_priv('admin_drop');
    $id = intval($_GET['id']);
    /* 获得管理员用户名 */
    $admin_name = $acc_db->getOne('SELECT user_name FROM '.TBL_ADMIN_USER." WHERE user_id='$id'");
	$links[] = array('text' => $_LANG['back_list'], 'href' => 'privilege.php?act=list');   
    /* demo这个管理员不允许删除 */
    if ($admin_name == 'admin')
    {
        sys_msg($_LANG['edit_remove_cannot'],1,$links);
    }

//    /* ID为1的不允许删除 */
//    if ($id == 1)
//    {
//        sys_msg($_LANG['remove_cannot'],1,$links);
//    }

    /* 管理员不能删除自己 */
    if ($id == $_SESSION['admin_id'])
    {
        sys_msg($_LANG['remove_self_cannot'],1,$links);
    }

    if ($exc->drop($id))
    {
        $sess->delete_spec_admin_session($id); // 删除session中该管理员的记录

        admin_log(addslashes($admin_name), 'remove', 'privilege');
        clear_cache_files();
    }
	
   	$result = get_admin_userlist();	
    /* 模板赋值 */
    $smarty->assign('ur_here',     $_LANG['admin_list']);
    $smarty->assign('action_link', array('href'=>'privilege.php?act=add', 'text' => $_LANG['admin_add']));
    $smarty->assign('full_page',   1);
    $smarty->assign('admin_list',  $result);

    /* 显示页面 */
    assign_query_info();
    $smarty->display('privilege_list.htm');
}

/* 获取管理员列表 */
function get_admin_userlist()
{
    $list = array();
    $sql  = 'SELECT user_id, user_name, email, add_time, last_login '.
            'FROM ' .TBL_ADMIN_USER.' ORDER BY user_id DESC';
    $list = $GLOBALS['acc_db']->getAll($sql);

    foreach ($list AS $key=>$val)
    {
        $list[$key]['add_time']     = local_date($GLOBALS['_CFG']['time_format'], $val['add_time']);
        $list[$key]['last_login']   = local_date($GLOBALS['_CFG']['time_format'], $val['last_login']);
    }
//    print_r($list);exit;
    return $list;
}

/* 清除购物车中过期的数据 */
function clear_cart()
{
//    /* 取得有效的session */
//    $sql = "SELECT DISTINCT session_id " .
//            "FROM " . $GLOBALS['ecs']->table('cart') . " AS c, " .
//                $GLOBALS['ecs']->table('sessions') . " AS s " .
//            "WHERE c.session_id = s.sesskey ";
//    $valid_sess = $GLOBALS['db']->getCol($sql);
//
//    // 删除cart中无效的数据
//    $sql = "DELETE FROM " . $GLOBALS['ecs']->table('cart') .
//            " WHERE session_id NOT " . db_create_in($valid_sess);
//    $GLOBALS['db']->query($sql);
}

/* 获取角色列表 */
function get_role_list()
{
    $list = array();
    $sql  = 'SELECT role_id, role_name, action_list '.
            'FROM ' .TBL_ADMIN_ROLE;
    $list = $GLOBALS['acc_db']->getAll($sql);
    return $list;
}

///**
// * 修改全平台密码
// */
//function edit_all_pwd($url){
//	foreach($url as $key => $value){
//		$res = gethttpcnt($url);
//		if(!$res){
//			$result[] = $value;
//		}
//	}	
//	return $result;
//}

?>
