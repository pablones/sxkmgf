<?php

define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');

if($_REQUEST['act'] == 'edit'){
	admin_priv("pet_edit");
	$id = $_REQUEST['id'];
	$sql = "select * from ".TBL_WB_EUDEMON." where id = '".$id."'";
	$result = $wangpu_db -> getRow($sql);
	
	$sql = "select * from ".TBL_WB_EUDEMONTYPE." where Id= '".$result['typeid']."'";
	$pet_type = $wangpu_db -> getRow($sql);
	$result['pet_type'] = $pet_type['name'];
	$result['sex'] = intval($result['sex']);	
	
	$sql = "select * from ".TBL_WB_USER." where `id` = '".$result['owner_id']."'";
	$player_info = $wangpu_db -> getRow($sql);
	$sql = "select * from ".TBL_ACCOUNT." where `id` = '".$player_info['account_id']."'";
	$account_info = $acc_db -> getRow($sql);
	
	$action_link = array(
		'href' => 'player.php?act=pet&username='.$account_info['name'],
		'text' => $_LANG['back_list'],
	);
	
	$smarty -> assign('action_link' , $action_link);
	$smarty -> assign('res' , $result);
	$smarty -> display('pet_list.htm');
	
}
if($_REQUEST['act'] == 'edit_query'){
	admin_priv("pet_edit");
	$resid = $_POST['resid'];
	$data = array(
		'life' => isset($_POST['life']) ? $_POST['life'] : '',
		'level' => isset($_POST['level']) ? $_POST['level'] : '',
		'exp' => isset($_POST['exp']) ? $_POST['exp'] : '',
		'happy' => isset($_POST['happy']) ? $_POST['happy'] : '',
		'bone' => isset($_POST['bone']) ? $_POST['bone'] : '',
		'lifespan' => isset($_POST['lifespan']) ? $_POST['lifespan'] : '',
		'strchg' => isset($_POST['strchg']) ? $_POST['strchg'] : '',
		'intchg' => isset($_POST['intchg']) ? $_POST['intchg'] : '',
		'stachg' => isset($_POST['stachg']) ? $_POST['stachg'] : '',
		'spichg' => isset($_POST['spichg']) ? $_POST['spichg'] : '',
		'agichg' => isset($_POST['agichg']) ? $_POST['agichg'] : '',
		'gift' => isset($_POST['gift']) ? $_POST['gift'] : '',
		'grow' => isset($_POST['grow']) ? $_POST['grow'] : '',
		'addpoint' => isset($_POST['addaddtion']) ? $_POST['addaddtion'] : '',
		'skill_1' => isset($_POST['skill1']) ? $_POST['skill1'] : '',
		'skill_2' => isset($_POST['skill2']) ? $_POST['skill2'] : '',
		'skill_3' => isset($_POST['skill3']) ? $_POST['skill3'] : '',
		'skill_4' => isset($_POST['skill4']) ? $_POST['skill4'] : '',
		'skill_5' => isset($_POST['skill5']) ? $_POST['skill5'] : '',
		'skill_6' => isset($_POST['skill6']) ? $_POST['skill6'] : '',
		'skill_7' => isset($_POST['skill7']) ? $_POST['skill7'] : '',
		'skill_8' => isset($_POST['skill8']) ? $_POST['skill8'] : '',
		'skill_9' => isset($_POST['skill9']) ? $_POST['skill9'] : '',
		'skill_10' => isset($_POST['skill10']) ? $_POST['skill10'] : '',
	);
	if($_POST['huanshou'] == 2){
		$data['sex'] = $_POST['huanshou'];
		$data['understand'] = isset($_POST['understand']) ? $_POST['understand'] : '';
		$data['intelligence'] = isset($_POST['intelligence']) ? $_POST['intelligence'] : '';
		$data['fusion'] = isset($_POST['fusion']) ? $_POST['fusion'] : '';
	}else{
		$data['sex'] = $_POST['huanshou'];
		$data['understand'] = 0;
		$data['intelligence'] = 0;
		$data['fusion'] = 0;
	}
	
	$num = count($data);
	$sql = "update ".TBL_WB_EUDEMON." set ";
	foreach($data as $key => $value){
		if($num > 1){
			$sql .= $key."= '".$value."',";	
		}else{
			$sql .= $key."= '".$value."'";
		}
		$num--;
		
	}
	$sql .= " where id = '".$resid."'";	
	
	$result = $wangpu_db -> query($sql);
	if($result){
		$sql = "select `owner_id` from ".TBL_WB_EUDEMON." where `id` = '".$resid."'";
		$owner_id = $wangpu_db -> getOne($sql);
		$cnt = $_SESSION['admin_name'].$_LANG['admin_log']. $owner_id;
		$sql = "insert into ".TBL_ADMIN_LOG." (`log_time`,`user_id`,`log_info`,`ip_address`) values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
		//$sql = "insert into '".TBL_ADMIN_LOG."'(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";		
		$acc_db -> query($sql);
		$link[] = array('text' => $_LANG['back_list'], 'href' => 'pet.php?act=edit&id='.$resid);
		sys_msg($_LANG['edit_success'],0,$link);
	}else{
		$link[] = array('text' => $_LANG['back_list'], 'href' => 'pet.php?act=edit&id='.$resid);
		sys_msg($_LANG['edit_error'],1,$link);
	}
}
?>