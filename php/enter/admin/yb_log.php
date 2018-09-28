<?php

/**
 * 元宝及元宝票记录
 * @author DXJ
 */
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');


if($_REQUEST['act'] == 'consumelog'){
    if($_POST[query]){
		$username=trim($_POST['username']);
        $rolename=trim($_POST['rolename']);
        $startdate=trim($_POST['startdate']);
        $enddate=trim($_POST['enddate']);
        $info=$_POST;
	}else{
	    $username=trim($_REQUEST['username']);
        $rolename=trim($_REQUEST['rolename']);
        $startdate=trim($_REQUEST['startdate']);
        $enddate=trim($_REQUEST['enddate']);
        $info=$_REQUEST;
	}
	
	if(!empty($username)){
        $where.= " and `Account_Name` LIKE '%".$username."%'"; 
    }    
    if(!empty($startdate)){
    	$startdate1=strtotime($startdate);
    	$startdate1=date('Y-m-d H:i:s',$startdate1);
    	$where .= " and writeTime >= '$startdate1' ";
    }   
    if(!empty($enddate)){
    	$enddate1=strtotime($enddate);
    	$enddate1=date('Y-m-d H:i:s',$enddate1);
    	$where .= " and writeTime <= '$enddate1' ";
    }  
	
	if($rolename){
		$sql=" select id from ".TBL_WB_USER." where name like '%$rolename%' ";
		$id=$wangpu_db ->getOne($sql);
        $where .= "and `Gift_User_ID` ='$id' "; 
    } 

	if(!empty($where)){
		$where=" where 1 ".$where;
	    $sql=" select * from ".TBL_SER_CONSUMELOG_TABLE." $where";
	    $rs=$acc_db -> getAll($sql);
	}


	
	if(!empty($rs)){
		foreach ($rs as $key => $value){
			if(!$rolename){
				$sql=" select name from ".TBL_WB_USER." where id ='$value[Gift_User_ID]' ";
				$role=$wangpu_db ->getOne($sql);
				$rs[$key]['role']=$role;
			}else{
				$rs[$key]['role']=$rolename;
			}
			$rs[$key][Spend_VAS_Point]=intval($value[Spend_VAS_Point]);
			$rs[$key][ItemType_Amount]=intval($value[ItemType_Amount]);
			$sql=" select name from ".TBL_WB_ITEMTYPE." where id ='$value[ItemType_ID]' ";
			$rs[$key]['item']=$wangpu_db ->getOne($sql);
		}
	}
	
	$smarty -> assign('rs',$rs);
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['consumelog']);
	$smarty -> display('consumelog.htm');
}

if($_REQUEST['act'] == 'ticketlog'){
	
    if($_POST[query]){
		$username=trim($_POST['username']);
        $rolename=trim($_POST['rolename']);
        $startdate=trim($_POST['startdate']);
        $enddate=trim($_POST['enddate']);
        $info=$_POST;
	}else{
	    $username=trim($_REQUEST['username']);
        $rolename=trim($_REQUEST['rolename']);
        $startdate=trim($_REQUEST['startdate']);
        $enddate=trim($_REQUEST['enddate']);
        $info=$_REQUEST;
	}
	
	if(!empty($username)){
        $where.= " and `Account_Name` LIKE '%".$username."%'"; 
    }    
    if(!empty($startdate)){
    	$startdate1=strtotime($startdate);
    	$startdate1=date('Y-m-d H:i:s',$startdate1);
    	$where .= " and OccurDate >= '$startdate1' ";
    }   
    if(!empty($enddate)){
    	$enddate1=strtotime($enddate);
    	$enddate1=date('Y-m-d H:i:s',$enddate1);
    	$where .= " and OccurDate <= '$enddate1' ";
    }  
	
	if($rolename){
		$sql=" select id from ".TBL_WB_USER." where name like '$rolename' ";
		$id=$wangpu_db ->getOne($sql);
        $where .= "and `Gift_User_ID` ='$id' "; 
    } 
	
	if(!empty($where)){
		$where=" where 1 ".$where;
	    $sql=" select * from ser_vasticketlog $where";
	    $rs=$acc_db -> getAll($sql);
	}
    
	if(!empty($rs)){
		foreach ($rs as $key => $value){
			if(!$rolename){
				$sql=" select name from ".TBL_WB_USER." where id ='$value[Gift_User_ID]' ";
				$role=$wangpu_db ->getOne($sql);
				$rs[$key]['role']=$role;
			}else{
				$rs[$key]['role']=$rolename;
			}
			if($value[Oper_Type]==1){
				$rs[$key]['type']=$_LANG['type1'];
			}
		    if($value[Oper_Type]==2){
				$rs[$key]['type']=$_LANG['type2'];
			}
			$rs[$key]['VAS_Point']=intval($value['VAS_Point']);
			$sql=" select name from ".TBL_WB_ITEMTYPE." where id ='$value[ItemType_ID]' ";
			$rs[$key]['item']=$wangpu_db ->getOne($sql);
		}
	}
	
	$smarty -> assign('rs',$rs);
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['ticketlog']);
	$smarty -> display('ticketlog.htm');
}

?>