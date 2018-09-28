<?php
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');


if($_REQUEST['act'] == 'list'){
	$sql=" select * from ".TBL_SK_VAS_TABLE." where VAS_Point>10000 or VAS_Point_juan>10000";
	
	$row=$acc_db -> getAll($sql);
	if(!empty($row)){
		foreach ($row as $key => $value){
			$sql=" select name from ".TBL_ACCOUNT." where id='". $value['Account_ID']."'";
			$username=$acc_db ->getOne($sql);
			$sql=" select name,level from ".TBL_WB_USER." where account_id='". $value['Account_ID']."'";
			$role=$wangpu_db -> getRow($sql);
			$info[$key]=$value;
			$info[$key][username]=$username;
			$info[$key][role]=$role[name];
			$info[$key][level]=$role[level];
			$info[$key][VAS_Point]=intval($value[VAS_Point]);
			$info[$key][VAS_Point_juan]=intval($value[VAS_Point_juan]);
		}
	}
	//print_r($info);
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['ticketlog']);
	$smarty -> display('big_yb.htm');
}
if($_REQUEST['act'] == 'output'){
	$sql=" select * from ".TBL_SK_VAS_TABLE." where VAS_Point>10000 or VAS_Point_juan>10000";
	
	$row=$acc_db -> getAll($sql);
	if(!empty($row)){
		foreach ($row as $key => $value){
			$sql=" select name from ".TBL_ACCOUNT." where id='". $value['Account_ID']."'";
			$username=$acc_db ->getOne($sql);
			$sql=" select name,level from ".TBL_WB_USER." where account_id='". $value['Account_ID']."'";
			$role=$wangpu_db -> getRow($sql);
			$info[$key]=$value;
			$info[$key][username]=$username;
			$info[$key][role]=$role[name];
			$info[$key][level]=$role[level];
			$info[$key][VAS_Point]=intval($value[VAS_Point]);
			$info[$key][VAS_Point_juan]=intval($value[VAS_Point_juan]);
		}
	}
	
    header("Content-Type:application/vnd.ms-excel");
    header("Content-Disposition:filename=test.xls");
    echo iconv('utf-8','gb2312',$_LANG[username])."\t";
    echo iconv('utf-8','gb2312',$_LANG[rolename])."\t";
    echo iconv('utf-8','gb2312',$_LANG[level])."\t";
    echo iconv('utf-8','gb2312',$_LANG[sdate])."\t";
    echo iconv('utf-8','gb2312',$_LANG[edate])."\t";
    echo iconv('utf-8','gb2312',$_LANG[dianjuan])."\t";
    echo iconv('utf-8','gb2312',$_LANG[yb])."\t\n";
    foreach ($info as $k =>$v){
    	$v[role]=iconv('utf-8','gb2312',$v[role]);
        echo $v[username]."\t".$v[role]."\t".$v[level]."\t".$v[FirstConsumeDate]."\t".$v[LastConsumeDate]."\t".$v[VAS_Point_juan]."\t".$v[VAS_Point]."\t\n";
    }
	
}
?>