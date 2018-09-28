<?php
/**
 * 道具修改
 * @var unknown_type
 */
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');
require_once (dirname(__FILE__) . '/includes/item_config.php');

if($_REQUEST['act'] == 'list'){
	$smarty -> display('item_list.htm');
}
if($_REQUEST['act'] == 'search'){
	$link[] = array('text' => $_LANG['back_list'], 'href' => 'item.php?act=list');
	$username = isset($_REQUEST['username']) ? $_REQUEST['username'] : '';
	$rolename = isset($_REQUEST['rolename']) ? $_REQUEST['rolename'] : '';
	if($username || $rolename){
		if($username){
			$sql = "select * from ".TBL_ACCOUNT." where name = '".$username."'";
			$userinfo = $acc_db -> getRow($sql);
			$sql = "select * from ".TBL_WB_USER." where account_id = '".$userinfo['id']."'";
			$playerinfo = $wangpu_db -> getRow($sql);
		}
		if(!$username && $rolename){
			$sql = "select * from ".TBL_WB_USER." where account_id = '".$rolenames."'";
			$playerinfo = $wangpu_db -> getRow($sql);
			$sql = "select * from ".TBL_ACCOUNT." where id = '".$playerinfo['account_id']."'";
			$userinfo =  $acc_db -> getRow($sql);
			$username = $userinfo['name'];
		}
		if($playerinfo){
			$sql = "select * from ".TBL_WB_ITEM." where owner_id = '".$playerinfo['id']."'";
			$result = $wangpu_db -> getAll($sql);
			foreach($result as $key => $value){
				$sql = "select * from ".TBL_WB_ITEMTYPE." where id = '".$value['TYPE']."'";
				$item = $wangpu_db -> getRow($sql);
				$result[$key]['account'] = $username;
				$result[$key]['playername'] = $playerinfo['name'];
				$result[$key]['level'] = $playerinfo['level'];
				$result[$key]['name'] = $item['name'];
			}
			
		}else{
			sys_msg($_LANG['error_player'], 1, $link);
		}
	}else{		                
        sys_msg($_LANG['error_input'], 1, $link);		
	}
	
	$info = array(
		'username' => $username,
		'rolename' => $rolename,
	);
	$smarty -> assign('info' , $info);
	$smarty -> assign('result' , $result);
	$smarty -> assign('view' , 1);
	$smarty -> display('item_list.htm');
}
if($_REQUEST['act'] == 'edit'){
	admin_priv("item_edit");
	$id = $_GET['id'];
	$username = isset($_GET['username']) ? $_GET['username'] : '';
	$rolename = isset($_GET['rolename']) ? $_GET['rolename'] : '';
	$sql = "select * from ".TBL_WB_ITEM." where id = '".$id."'";
	$item_info = $wangpu_db -> getRow($sql);
	
	$sql = "select * from ".TBL_WB_ITEMTYPE." where id='".$typeid."'";
	$item = $wangpu_db -> getRow($sql);
	$item_info['item_name'] = $item['name'];
	$item_info['kidnap'] = intval($item_info['kidnap']);
	$item_info['addlev'] = intval($item_info['addlev']);
	$gem = array(
		'itemGem1' => $item_info['itemGem1'],
		'itemGem2' => $item_info['itemGem2'],
		'itemGem3' => $item_info['itemGem3'],
		'itemGem4' => $item_info['itemGem4'],
	);
	

	
	$action_link = array(
		'href' => 'item.php?act=search&username='.$username."&rolename=".$rolename,
		'text' => $_LANG['back_list'],
	);
	
	
	$smarty -> assign('gem' , $gem);
	$smarty -> assign('action_link' , $action_link);
	$smarty -> assign('result' , $item_info);
	$smarty -> display('item_edit.htm');
}
if($_REQUEST['act'] == 'edit_query'){
	admin_priv("item_edit");
	$type = $_POST['typeid'];
	$owner_id = $_POST['owner_id'];
	$star = isset($_POST['star']) ? $_POST['star'] : $_POST['oldstar'];
	$strong = isset($_POST['strong']) ? $_POST['strong'] : $_POST['oldaddlev'];
	$id = isset($_POST['id']) ? $_POST['id']: '';
	$kidnap = isset($_POST['kidnap']) ? $_POST['kidnap'] : ''; //魂印状态
	$gem = isset($_POST['gem']) ? $_POST['gem'] : '';
	$baoshi = array(
		0 => isset($_POST['baoshi1'])?$_POST['baoshi1']:'',
		1 => isset($_POST['baoshi2'])?$_POST['baoshi2']:'',
		2 => isset($_POST['baoshi3'])?$_POST['baoshi3']:'',
		3 => isset($_POST['baoshi4'])?$_POST['baoshi4']:'',
	);
	
	//装备打孔
	foreach($gem as $key => $value){
		if($value == 'on'){
			$itemGem[$key] = '88888';
			if($baoshi[$key] != ''){
				$itemGem[$key] = $baoshi[$key];
			}
		}else{
			$itemGem[$key] = '99999';
		}
	}
	
	$soulatt = isset($_POST['soulatt']) ? $_POST['soulatt'] : '';
	$soullevel = isset($_POST['soullevel'])? $_POST['soullevel'] : '';
	$soulexp = isset($_POST['soulexp']) ? $_POST['soulexp'] : '';

	$SoulState = $soulatt * 10000000 + $soullevel * 100000 + $soulexp;
	
	$link[] = array('text' => $_LANG['back_list'], 'href' => 'item.php?act=edit&id='.$id);
	$sql = "select * from ".TBL_WB_ITEM." where id = '".$id."' ";
	$item_info = $wangpu_db -> getRow($sql);
	$item_info['addlev'] = intval($item_info['addlev']);	
	$start_array = array(
		'itemAddition1' => $item_info['itemAddition1'],
		'itemAddition2' => $item_info['itemAddition2'],
		'itemAddition3' => $item_info['itemAddition3'],
		'itemAddition4' => $item_info['itemAddition4'],
		'itemAddition5' => $item_info['itemAddition5'],
		'itemAddition6' => $item_info['itemAddition6'],
		'itemAddition7' => $item_info['itemAddition7'],
		'itemAddition8' => $item_info['itemAdditnon8'],
	
	);
	$base_array = array(
		'itemBase1' => $item_info['itemBase1'],
		'itemBase2' => $item_info['itemBase2'],
	);
	
	if($item_info){
		/* 计算星级 */
		$item_info['addlev'] = intval($item_info['addlev']);	
		foreach($start_array as $key => $value){
			if($value != 0){
				if($item_config['start_cfg'][$star] != 0){
					$start_array[$key] = intval($value / 10000) * 10000 + $value % 10000 / $item_config['start_cfg'][$item_info['star']] * $item_config['start_cfg'][$star]; 
					$start_array[$key] = intval($start_array[$key]);
				}else{
					sys_msg($_LANG['start_limit'],1,$link);
				}

			}			
		}
		/* 计算强化等级 */
		foreach($base_array as $key => $value){
			if($value != 0){	
				if($item_config['addlevel_cfg'][$strong] != 0){
					$base_array[$key] = intval($value / 10000) * 10000 + $value % 10000 / $item_config['addlevel_cfg'][$item_info['addlev']] * $item_config['addlevel_cfg'][$strong];
					$base_array[$key] = intval($base_array[$key]); 
				}else{
					sys_msg($_LANG['level_limit'],1,$link);
				}

			}
		}

		$sql = "update ".TBL_WB_ITEM." set `itemGem1`='".$itemGem[0]."',`itemGem2`='".$itemGem[1]."',`itemGem3`='".$itemGem[2]."',`itemGem4`='".$itemGem[3]."',`kidnap` = '".$kidnap."' ,`star` = '".$star."',`addlev` = '".$strong."',`itemBase1` = '".$base_array['itemBase1']."',`itemBase2` = '".$base_array['itemBase2']."',`itemAddition1` = '".$start_array['itemAddition1']."',`itemAddition2` = '".$start_array['itemAddition2']."',`itemAddition3` = '".$start_array['itemAddition3']."',`itemAddition4` = '".$start_array['itemAddition4']."',`itemAddition5` = '".$start_array['itemAddition5']."',`itemAddition6` = '".$start_array['itemAddition6']."',`itemAddition7` = '".$start_array['itemAddition7']."',`itemAddition8` = '".$start_array['itemAddition8']."' where `id` = '".$id."'";		
		$result = $wangpu_db -> query($sql);
		if($result){
			if($SoulState != 0){
				$sql = "update ".TBL_WB_ITEM." set `SoulState` = '".$SoulState."' where `id` = '".$id."'";
				$result = $wangpu_db -> query($sql);
			}
		}
		if($result){
			$cnt = $_SESSION['admin_name'].$_LANG['admin_log'].$owner_id;
			$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
			$acc_db -> query($sql);
			sys_msg($_LANG['success'],0,$link);
		}else{
			sys_msg($_LANG['error_edit'],1,$link);
		}
	}else{
		sys_msg($_LANG['error_player'], 1, $link);
	}
}

?>