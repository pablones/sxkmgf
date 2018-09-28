<?php
/**
 * 补偿仓库
 * @author DXJ
 */

define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');
if($_REQUEST['act'] == 'other'){
	admin_priv('compensation_other');
	$links[] = array('text' => $_LANG['back'], 'href' => 'compensation.php?act=other');
	$username=$_POST['username'];
	$type=$_POST['type'];
	$amount=$_POST['amount'];
	switch ($type){
		case 1:
			break;
		case 2:
			break;
		default:
			$sql="select id from ".TBL_WB_USER." where name like '$username' ";
			$userid=$wangpu_db ->getOne($sql);
			if($userid&&$username){
			    $sql= "insert into ".TBL_WB_COMPENSATIONDATA." (`owner_id` ,`type` ,`status` ,`typeid` ,`amount` ,`writetime` ,`param` )
                values('$userid', '$type', '0', '0', '$amount',now() , '1')";
			    $rs=$wangpu_db -> query($sql);
			    if($rs){
				    $ok=1;
			    }
			}
	}
	if($ok==1){
		//记录管理员操作日志
		$sql = "select `id`,level,account_id,Exit_Time from ".TBL_WB_USER." where `name` LIKE ('".$username."')";
		$rs = $wangpu_db->getRow($sql);
        $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$rs[account_id]' ";
        $rs1=$acc_db ->getRow($sql);
        $itemtype=$_LANG[compensate].' '.$_LANG['other'.$type];
		$name=$_LANG[username].$rs1['name'];
		$regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		$lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		$lastlogin=$_LANG[lastlogin].$lastlogin;
		$level=$_LANG[level].$rs['level'];
    
		$cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[role].$username.'|'.$level.'|';
		$cnt.=$itemtype.'|'.$_LANG[count].$amount.'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		$cnt.=$_POST[add_type];
		if($_POST[add_type]==$_LANG['staff']){
		    $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		}
		//$content=$_SESSION[admin_name]."$_LANG[ur_last]→".$_LANG['compensate'].$_LANG['other'.$type]." : $amount ".$_LANG['is_username'].": $username";
	    $sql = 'INSERT INTO ' .TBL_ADMIN_LOG . ' (log_time, user_id, log_info, ip_address) ' .
            " VALUES ('" . gmtime() . "', $_SESSION[admin_id], '".$cnt."', '" . real_ip() . "')";
        $GLOBALS['acc_db']->query($sql);
	    sys_msg($_LANG['success'], 1, $links); 
	}
    if(!$ok&&$_POST[query]){
    	sys_msg($_LANG['fail'], 1, $links);
    }
    $msg=$_LANG['compensate'].$username.$_LANG['other'.$type].$amount;
    
	$smarty -> assign('here',$_LANG['compensation_other']);
	$smarty -> assign('ok',$ok);
	$smarty -> assign('msg',$msg);
	$smarty -> display('compensation_other.htm');
}

if($_REQUEST['act'] == 'item'){
	admin_priv("compensation_item");
	$links[] = array('text' => $_LANG['back'], 'href' => 'compensation.php?act=item');
	$username=$_POST['username'];
	$type=1;
	$amount=$_POST['amount']?$_POST['amount']:1;
	$typeid=$_POST['typeid'];
	$param='';
    foreach($_POST as $key => $value){
    	if(in_array($key,array('username', 'typeid','amount','query','add_type','request','action')))
    	continue;
    	if($value){
    		$param.=' '.$value;
    	}else{
    		$param.=' '.'0';
    	}
    	
    }
	
	$sql="select id from ".TBL_WB_USER." where name like '$username' ";
	$userid=$wangpu_db ->getOne($sql);
	if($userid&&$username){
	    $sql= "insert into ".TBL_WB_COMPENSATIONDATA." (`owner_id` ,`type` ,`status` ,`typeid` ,`amount` ,`writetime` ,`param` )
        values('$userid', '$type', '0', '$typeid', '$amount',now() , '$param')";
		$rs=$wangpu_db -> query($sql);
		if($rs){
		    $ok=1;
		}
	}
	//添加管理员操作日志
	if($ok==1){
		$sql="select name from ".TBL_WB_ITEMTYPE." where id ='$typeid' ";
		$itemname=$wangpu_db -> getOne($sql);
		
		$sql = "select `id`,level,account_id,Exit_Time from ".TBL_WB_USER." where `name` LIKE ('".$username."')";
		$rs = $wangpu_db->getRow($sql);
        $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$rs[account_id]' ";
        $rs1=$acc_db ->getRow($sql);
        $sql="select name from ".TBL_WB_ITEMTYPE." where id ='$typeid' ";
        $itemtype=$_LANG[compensate].' '.$wangpu_db -> getOne($sql);
		$name=$_LANG[username].$rs1['name'];
		$regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		$lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		$lastlogin=$_LANG[lastlogin].$lastlogin;
		$level=$_LANG[level].$rs['level'];
		
    
	    $cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[role].$username.'|'.$level.'|';
		$cnt.=$itemtype.'|'.$_LANG[count].$amount.'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		$cnt.=$_POST[add_type];
		if($_POST[add_type]==$_LANG['staff']){
		    $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		}
		//$content=$_SESSION[admin_name]."$_LANG[ur_last]→".$_LANG['compensate'].$_LANG['item']." :$itemname ".$_LANG['is_username'].": $username";
	    $sql = 'INSERT INTO ' .TBL_ADMIN_LOG . ' (log_time, user_id, log_info, ip_address) ' .
            " VALUES ('" . gmtime() . "', $_SESSION[admin_id], '" . $cnt. "', '" . real_ip() . "')";
        $GLOBALS['acc_db']->query($sql);
	    sys_msg($_LANG['success'], 1, $links); 
	}
    if(!$ok&&$_POST[query]){
    	sys_msg($_LANG['fail'], 1, $links);
    }
	
	$smarty -> assign('here',$_LANG['compensation_item']);
	$smarty -> assign('ok',$ok);
	$smarty -> display('compensation_item.htm');
}

if($_REQUEST['act'] == 'pet'){
	admin_priv("compensation_pet");
	$links[] = array('text' => $_LANG['back'], 'href' => 'compensation.php?act=pet');
	$username=$_POST['username'];
	$type=2;
	$amount=$_POST['amount'];
	$typeid=$_POST['typeid'];
	$param='';
    foreach($_POST as $key => $value){
    	if(in_array($key,array('username', 'typeid','amount','query','add_type','request','action')))
    	continue;
    	if($value){
    		$param.=' '.$value;
    	}else{
    		$param.=' '.'0';
    	}
    	
    }
	$sql="select id from ".TBL_WB_USER." where name like '$username' ";
	$userid=$wangpu_db ->getOne($sql);
	if($userid&&$username){
	    $sql= "insert into ".TBL_WB_COMPENSATIONDATA." (`owner_id` ,`type` ,`status` ,`typeid` ,`amount` ,`writetime` ,`param` )
        values('$userid', '$type', '0', '$typeid', '$amount',now() , '$param')";
		$rs=$wangpu_db -> query($sql);
		if($rs){
		    $ok=1;
		}
	}  
	if($ok==1){
		$sql="select name from ".TBL_WB_EUDEMONTYPE." where id ='$typeid' ";
		$itemname=$wangpu_db -> getOne($sql);
		
		$sql = "select `id`,level,account_id,Exit_Time from ".TBL_WB_USER." where `name` LIKE ('".$username."')";
		$rs = $wangpu_db->getRow($sql);
        $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$rs[account_id]' ";
        $rs1=$acc_db ->getRow($sql);
        $sql="select name from ".TBL_WB_EUDEMONTYPE." where id ='$typeid' ";
        $itemtype=$_LANG[compensate].' '.$wangpu_db -> getOne($sql);
		$name=$_LANG[username].$rs1['name'];
		$regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		$lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		$lastlogin=$_LANG[lastlogin].$lastlogin;
		$level=$_LANG[level].$rs['level'];
		
    
	    $cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[role].$username.'|'.$level.'|';
		$cnt.=$itemtype.'|'.$_LANG[count].$amount.'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		$cnt.=$_POST[add_type];
		if($_POST[add_type]==$_LANG['staff']){
		    $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		}
		//$content=$_SESSION[admin_name]."$_LANG[ur_last]→".$_LANG['compensate'].$_LANG['pet']." :$itemname ".$_LANG['is_username'].": $username";
	    $sql = 'INSERT INTO ' .TBL_ADMIN_LOG . ' (log_time, user_id, log_info, ip_address) ' .
            " VALUES ('" . gmtime() . "', $_SESSION[admin_id], '" . $cnt . "', '" . real_ip() . "')";
        $GLOBALS['acc_db']->query($sql);
	    sys_msg($_LANG['success'], 1, $links); 
	}
    if(!$ok&&$_POST[query]){
    	sys_msg($_LANG['fail'], 1, $links);
    }
	
	
	$smarty -> assign('here',$_LANG['compensation_pet']);
	$smarty -> assign('ok',$ok);
	$smarty -> display('compensation_pet.htm');
}

?>