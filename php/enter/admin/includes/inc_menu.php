<?php

/**
 * 管理中心菜单数组
 * @author King.L
*/

if (!defined('IN_ECS'))
{
    die('Hacking attempt');
}

/* 充值消费管理 */
$modules['01_charge_consume_manage']['py_statics'] = 'pay_consume.php?act=py_statics';
//$modules['01_charge_consume_manage']['py_list'] = 'pay_consume.php?act=py_list';
$modules['01_charge_consume_manage']['py_yb'] = 'pay_consume.php?act=py_yb';
$modules['01_charge_consume_manage']['report_sell_day'] = 'report.php?act=daysell';
$modules['01_charge_consume_manage']['report_sell_all'] = 'report.php?act=allsell';
$modules['01_charge_consume_manage']['report_all_ptcz'] = 'report.php?act=all_ptcz';
$modules['01_charge_consume_manage']['report_czall'] = 'report.php?act=czall';
$modules['01_charge_consume_manage']['report_czday'] = 'report.php?act=czday';
$modules['01_charge_consume_manage']['report_czmonth'] = 'report.php?act=czmonth';
$modules['01_charge_consume_manage']['report_all_ptxf'] = 'report.php?act=all_ptxf';
$modules['01_charge_consume_manage']['report_allconsume'] = 'report.php?act=allconsume';
$modules['01_charge_consume_manage']['report_dayconsume'] = 'report.php?act=dayconsume';
$modules['01_charge_consume_manage']['report_monthconsume'] = 'report.php?act=monthconsume';
//$modules['01_charge_consume_manage']['all_statics'] = 'all_statices.php?act=list';

/* 资源管理工具 */

$modules['02_resource']['yb_res'] = 'resource.php?act=yuanbao';
$modules['02_resource']['zb_res'] = 'resource.php?act=zhubao';
$modules['02_resource']['card_res'] = 'resource.php?act=card';
$modules['02_resource']['goods_res'] = 'resource.php?act=goods';
$modules['02_resource']['act_manage'] = 'resource.php?act=activity';
//$modules['02_resource']['mkdk_card'] = 'resource.php?act=mkdkcard';
$modules['02_resource']['dianjuan'] = 'resource.php?act=dianjuan';
$modules['02_resource']['ex_paymodes'] = 'ex_paymodes.php';

/* 玩家管理 */
$modules['03_player_manage']['player_list'] = 'player.php?act=list';
$modules['03_player_manage']['player_equire'] = 'player.php?act=equire';
$modules['03_player_manage']['player_skill'] = 'player.php?act=skill';
$modules['03_player_manage']['player_pet'] = 'player.php?act=pet';
$modules['03_player_manage']['player_goods'] = 'player.php?act=goods';
$modules['03_player_manage']['player_ranking'] = 'player.php?act=ranking';
$modules['03_player_manage']['player_searchpwd'] = 'player.php?act=searchpwd';
$modules['03_player_manage']['admin_login'] = 'sys_config.php?act=admin_login';
$modules['03_player_manage']['player_setitempwd'] = 'player.php?act=setitempwd';

$modules['03_player_manage']['search_mob'] = 'mobsearch.php?act=moblist';

$modules['03_player_manage']['player_first_pay'] = 'player.php?act=first_pay';

$modules['03_player_manage']['player_game_log'] = 'player.php?act=game_log';
$modules['03_player_manage']['player_consumelog'] = 'yb_log.php?act=consumelog';
$modules['03_player_manage']['player_ticketlog'] = 'yb_log.php?act=ticketlog';
$modules['03_player_manage']['big_yb'] = 'big_yb.php?act=list';

/* 报表分析 */
$modules['04_report_manage']['report_online_num'] = 'report.php?act=online';
$modules['04_report_manage']['report_rate_num'] = 'report.php?act=ratenum';
$modules['04_report_manage']['report_player_new'] = 'report.php?act=new';
$modules['04_report_manage']['report_player_create'] = 'report.php?act=create';
$modules['04_report_manage']['report_player_lost'] = 'report.php?act=lost';



/*常用工具*/
$modules['05_tool_manage']['tool_newuser'] = 'tool.php?act=newuser';
$modules['05_tool_manage']['edit_pwd'] = 'edit_pwd.php?act=list';
$modules['05_tool_manage']['fcm_tool'] = 'fcm_tool.php?act=list';
$modules['05_tool_manage']['export_detail'] = 'tool.php?act=export_detail';
//$modules['05_tool_manage']['search_card'] = 'search_card.php?act=list';




/*补偿仓库*/

$modules['06_compensation_manage']['compensation_pet'] = 'compensation.php?act=pet';
$modules['06_compensation_manage']['compensation_item'] = 'compensation.php?act=item';
$modules['06_compensation_manage']['compensation_other'] = 'compensation.php?act=other';
$modules['06_compensation_manage']['item_edit'] = 'item.php?act=list';
$modules['06_compensation_manage']['pet_edit'] = 'player.php?act=pet';

//$modules['10_priv_admin']['admin_logs']             = 'admin_logs.php?act=list';
//$modules['10_priv_admin']['admin_list']             = 'privilege.php?act=list';
//$modules['10_priv_admin']['admin_role']             = 'role.php?act=list';
//$modules['10_priv_admin']['agency_list']            = 'agency.php?act=list';


$modules['11_system']['set_config']             = 'sys_config.php?act=list_edit';
//$modules['11_system']['push_all']				   = 'push_data.php?act=push';
$modules['11_system']['clear_ver']  			   = 'sys_config.php?act=clear_ver&type=ver'; 
$modules['11_system']['clear_server']  			   = 'sys_config.php?act=clear_ver&type=server'; 
$modules['11_system']['logs_manage']  			   = 'log_search.php?act=list'; 
$modules['11_system']['sqltool']  			   = 'sqltool.php'; 
//$modules['11_system']['memcache']				   = 'memcache.php?act=select';
//$modules['11_system']['clear_server']  			   = 'sys_config.php?act=clear_ver&type=server'; 



?>
