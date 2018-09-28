<?php

/**
 * 角色管理信息以及权限管理程序
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
$exc = new exchange(TBL_ADMIN_ROLE, $acc_db, 'role_id', 'role_name');

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
//-- 角色列表页面
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'list')
{
    /* 模板赋值 */
    $smarty->assign('ur_here',     $_LANG['admin_role']);
    $smarty->assign('action_link', array('href'=>'role.php?act=add', 'text' => $_LANG['admin_add_role']));
    $smarty->assign('full_page',   1);
    $smarty->assign('here',   $_LANG['role']);
    $smarty->assign('admin_list',  get_role_list());

    /* 显示页面 */
    assign_query_info();
    $smarty->display('role_list.htm');
}

/*------------------------------------------------------ */
//-- 查询
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'query')
{
    $smarty->assign('admin_list',  get_role_list());

    make_json_result($smarty->fetch('role_list.htm'));
}

/*------------------------------------------------------ */
//-- 添加角色页面
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'add')
{
    /* 检查权限 */
    admin_priv('admin_manage');
    include_once(ROOT_PATH . 'languages/' .$_CFG['lang']. '/admin/priv_action.php');

    $priv_str = '';

    /* 获取权限的分组数据 */
    $sql_query = "SELECT action_id, parent_id, action_code, relevance FROM " .TBL_ADMIN_ACTION.
                 " WHERE parent_id = 0";
    $res = $acc_db->query($sql_query);
    while ($rows = $acc_db->FetchRow($res))
    {
        $priv_arr[$rows['action_id']] = $rows;
    }


    /* 按权限组查询底级的权限名称 */
    $sql = "SELECT action_id, parent_id, action_code, relevance FROM " .TBL_ADMIN_ACTION.
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

     /* 模板赋值 */
    $smarty->assign('ur_here',     $_LANG['admin_add_role']);
    $smarty->assign('action_link', array('href'=>'role.php?act=list', 'text' => $_LANG['admin_list_role']));
    $smarty->assign('form_act',    'insert');
    $smarty->assign('action',      'add');
    $smarty->assign('lang',        $_LANG);
    $smarty->assign('priv_arr',    $priv_arr);

    /* 显示页面 */
    assign_query_info();
    $smarty->display('role_info.htm');




}

/*------------------------------------------------------ */
//-- 添加角色的处理
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'insert')
{
    admin_priv('admin_manage');
    $act_list = @join(",", $_POST['action_code']);
    $sql = "INSERT INTO ".TBL_ADMIN_ROLE." (role_name, action_list, role_describe) ".
           "VALUES ('".trim($_POST['user_name'])."','$act_list','".trim($_POST['role_describe'])."')";

    $acc_db->query($sql);
    /* 转入权限分配列表 */
    $new_id = $acc_db->Insert_ID();

    /*添加链接*/

    $link[0]['text'] = $_LANG['admin_list_role'];
    $link[0]['href'] = 'role.php?act=list';

    sys_msg($_LANG['add'] . "&nbsp;" .$_POST['user_name'] . "&nbsp;" . $_LANG['action_succeed'],0, $link);

    /* 记录管理员操作 */
    admin_log($_POST['user_name'], 'add', 'role');
 }

/*------------------------------------------------------ */
//-- 编辑角色信息
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'edit')
{
     include_once(ROOT_PATH . 'languages/' .$_CFG['lang']. '/admin/priv_action.php');
    $_REQUEST['id'] = !empty($_REQUEST['id']) ? intval($_REQUEST['id']) : 0;
        /* 获得该管理员的权限 */
    $priv_str = $acc_db->getOne("SELECT action_list FROM " .TBL_ADMIN_ROLE. " WHERE role_id = '$_GET[id]'");

    /* 查看是否有权限编辑其他管理员的信息 */
    if ($_SESSION['admin_id'] != $_REQUEST['id'])
    {
        admin_priv('admin_manage');
    }

    /* 获取角色信息 */
    $sql = "SELECT role_id, role_name, role_describe FROM " .TBL_ADMIN_ROLE.
           " WHERE role_id = '".$_REQUEST['id']."'";
    $user_info = $acc_db->getRow($sql);

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


    /* 模板赋值 */

    $smarty->assign('user',        $user_info);
    $smarty->assign('form_act',    'update');
    $smarty->assign('action',      'edit');
    $smarty->assign('ur_here',     $_LANG['admin_edit_role']);
    $smarty->assign('action_link', array('href'=>'role.php?act=list', 'text' => $_LANG['admin_list_role']));
    $smarty->assign('lang',        $_LANG);
    $smarty->assign('priv_arr',    $priv_arr);
    $smarty->assign('user_id',     $_GET['id']);

    assign_query_info();
    $smarty->display('role_info.htm');
}

/*------------------------------------------------------ */
//-- 更新角色信息
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'update')
{
    /* 更新管理员的权限 */
    $act_list = @join(",", $_POST['action_code']);
    $sql = "UPDATE " .TBL_ADMIN_ROLE. " SET action_list = '$act_list', role_name = '".$_POST['user_name']."', role_describe = '".$_POST['role_describe']." ' ".
           "WHERE role_id = '$_POST[id]'";
    $acc_db->query($sql);
    $user_sql = "UPDATE " .TBL_ADMIN_USER. " SET action_list = '$act_list' ".
           "WHERE role_id = '$_POST[id]'";
    $acc_db->query($user_sql);
    /* 提示信息 */
    $link[] = array('text' => $_LANG['back_admin_list'], 'href'=>'role.php?act=list');
    sys_msg($_LANG['edit'] . "&nbsp;" . $_POST['user_name'] . "&nbsp;" . $_LANG['action_succeed'], 0, $link);
}

/*------------------------------------------------------ */
//-- 删除一个角色
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'remove')
{
    admin_priv("admin_drop");
	$links[] = array('text' => $_LANG['back_admin_list'], 'href' => 'role.php?act=list');   
    $id = intval($_GET['id']);
    $num_sql = "SELECT count(*) FROM " .TBL_ADMIN_USER. " WHERE role_id = '$_GET[id]'";
    $remove_num = $acc_db->getOne($num_sql);
    if($remove_num > 0)
    {
    	sys_msg($_LANG['remove_cannot_user'],1,$links);
    }
    else
    {
        $exc->drop($id);
//        $url = 'role.php?act=query&' . str_replace('act=remove', '', $_SERVER['QUERY_STRING']);
    }
	
	/* 模板赋值 */
    $smarty->assign('ur_here',     $_LANG['admin_role']);
    $smarty->assign('action_link', array('href'=>'role.php?act=add', 'text' => $_LANG['admin_add_role']));
    $smarty->assign('full_page',   1);
    $smarty->assign('admin_list',  get_role_list());

    /* 显示页面 */
    assign_query_info();
    $smarty->display('role_list.htm');

}

/* 获取角色列表 */
function get_role_list()
{
    $list = array();
    $sql  = 'SELECT role_id, role_name, action_list, role_describe '.
            'FROM ' .TBL_ADMIN_ROLE.' ORDER BY role_id DESC';
    $list = $GLOBALS['acc_db']->getAll($sql);

    return $list;
}

?>
