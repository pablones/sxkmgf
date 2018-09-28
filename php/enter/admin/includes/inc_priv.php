<?php

/**
 * 权限对照表
 * @author King.L
*/

if (!defined('IN_ECS'))
{
    die('Hacking attempt');
}

    //充值消费管理权限
    $purview['01_charge_consume_manage'] = '01_charge_consume_manage';
    $purview['py_statics'] = 'py_statics';
    $purview['py_list'] = 'py_list';
    $purview['py_yb'] = 'py_yb';
    $purview['all_statics'] = 'all_statics';
    $purview['view_all_server'] = "view_all_server";
    $purview['report_czall'] = 'report_czall';
    $purview['report_allconsume'] = 'report_allconsume';
    $purview['report_all_ptcz'] = 'report_all_ptcz';
    $purview['report_all_ptxf'] = 'report_all_ptxf';
    $purview['report_sell_day'] = 'report_sell_day';
    $purview['report_sell_all'] = 'report_sell_all';
    $purview['report_dayconsume'] = 'report_dayconsume';
    $purview['report_monthconsume'] = 'report_monthconsume';
    $purview['report_czday'] = 'report_czday';
    $purview['report_czmonth'] = 'report_czmonth';
    
  

    //* 资源管理工具 */
    $purview['02_resource'] = '02_resource';
    $purview['yb_res'] = 'yb_res';
    $purview['zb_res'] = 'zb_res';
    $purview['card_res'] = 'card_res';
    $purview['goods_res'] = 'goods_res';
    $purview['act_manage'] = 'act_manage';
    $purview['mkdk_card'] = 'mkdk_card';
    $purview['dianjuan'] = 'dianjuan';
    $purview['ex_paymodes'] = 'ex_paymodes';

    //玩家管理
    $purview['03_player_manage'] = '03_player_manage';
    $purview['player_list'] = 'player_list';
    $purview['player_equire'] = 'player_equire';
    $purview['player_skill'] = 'player_skill';
    $purview['player_pet'] = 'player_pet';
    $purview['player_goods'] = 'player_goods';
    $purview['player_ranking'] = 'player_ranking';
    $purview['player_searchpwd'] = 'player_searchpwd';
    $purview['admin_login'] = 'admin_login';
    $purview['player_setitempwd'] = 'player_setitempwd';
    
    $purview['search_mob'] = 'search_mob';

    $purview['player_first_pay'] = 'player_first_pay';
    $purview['player_game_log'] = 'player_game_log';
    $purview['player_consumelog'] = 'player_consumelog';
    $purview['player_ticketlog'] = 'player_ticketlog';
    $purview['big_yb'] = 'big_yb';
   


    $purview['04_report_manage'] = '04_report_manage';
    $purview['report_online_num'] = 'report_online_num';
    $purview['report_rate_num'] = 'report_rate_num';
    $purview['report_player_new'] = 'report_player_new';
    $purview['report_player_create'] = 'report_player_create';
    $purview['report_player_lost'] = 'report_player_lost';

    
    
    $purview['05_tool_manage'] = '05_tool_manage';
    $purview['tool_newuser'] = 'tool_newuser';
    $purview['edit_pwd'] = 'edit_pwd';
    $purview['fcm_tool'] = 'fcm_tool';
    $purview['export_detail'] = 'export_detail';
    $purview['search_card'] = 'search_card';


    $purview['10_priv_admin'] = '10_priv_admin';
    $purview['admin_list'] = 'admin_list';
    $purview['admin_list_role'] = 'admin_list_role';
    $purview['admin_role'] = 'admin_role';
    $purview['admin_add'] = 'admin_add';
    $purview['admin_add_role'] = 'admin_add_role';
    $purview['admin_edit_role'] = 'admin_edit_role';
    $purview['admin_logs'] = 'admin_logs';


    $purview['11_system'] = '11_system';
    $purview['set_config'] = 'set_config';
    $purview['admin_logs']          = array('logs_manage', 'logs_drop');
    $purview['admin_list']          = array('admin_manage', 'admin_drop', 'allot_priv');
    $purview['agency_list']         = 'agency_manage';
    $purview['admin_role']          = 'role_manage';
    $purview['clear_ver']			= 'clear_ver';
    $purview['clear_server']		= 'clear_server';
    $purview['view_all']            = 'view_all';
    $purview['memcache']			= 'memcache';
    $purview['sqltool'] = 'sqltool';
    $purview['logs_manage'] = 'logs_manage';
    
    
    $purview['06_compensation_manage'] = '06_compensation_manage';
    $purview['compensation_pet'] = 'compensation_pet';
    $purview['compensation_item'] = 'compensation_item';
    $purview['compensation_other'] = 'compensation_other';
    $purview['item_edit'] = 'item_edit';
    $purview['pet_edit'] = 'pet_edit';




?>