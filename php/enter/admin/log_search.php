<?php
/**
 * 管理员日志查询
 * @author sam.D
 */
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

date_default_timezone_set('PRC');

if($_REQUEST['act'] == 'list'){
	admin_priv('logs_manage');
	$admin=trim($_POST[admin]);
	$ip=trim($_POST[ip]);
	$sdate=trim($_POST[sdate]);
	$edate=trim($_POST[edate]);
    $url=$_CFG['apiurl']."get_admininfo.php?";
	
	if(!empty($admin)){
		$url1=$url.'username='.$admin;
//		echo $url;
//		echo "<p>";
		$info=gethttpcnt($url1);
		$info=unserialize($info);
        $where .= " and user_id= '$info[user_id]' "; 
//        echo $where;
//        exit;
    } 
    
    
    
	if(!empty($ip)){
        $where .= " and ip_address= '$ip' "; 
    } 
    if(!empty($sdate)){
    	$startdate1=strtotime($sdate);
    	$where .= " and log_time >= '$startdate1' ";
    }   
    if(!empty($edate)){
       $enddate1=strtotime($edate)+ONE_DAY;
	   $where .= " and log_time <= '$enddate1' ";
    }  

	if(!empty($where)){
		$where=" where 1 ".$where;
		$sql=" select * from ".TBL_ADMIN_LOG.$where;
        $rs=$acc_db -> getAll($sql);
	}
    
	if(!empty($rs)){
		foreach ($rs as $key => $value){
			$rs[$key][log_time]=date('Y-m-d H:i:s',$value[log_time]);
			if(!$admin){
			    $url1=$url.'userid='.$value[user_id];
		        $info=gethttpcnt($url1);
		        $info=unserialize($info);
                $rs[$key][user_name]=$info[user_name]; 
			}else{
				$rs[$key][user_name]=$admin;
			}
			
		}
	}
	if(!empty($rs)){
		$rs=array_reverse($rs);
	}
	$smarty -> assign('here',$_LANG['list']);
	$smarty -> assign('info',$_POST);
	$smarty -> assign('rs',$rs);
	$smarty -> display('log_list.htm');
}


?>