<?php

/**
 * 常用工具
 * @author DXJ
 */
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');


if($_REQUEST['act'] == 'newuser'){
	
    $fileinfo = $_FILES['input_rolname'];
    $tool_type = $_POST['tool_type']; //获取设置类型
    $role_name = $_POST['rolename']; //获取角色名
    if($fileinfo['name']){
    	
	
    }else{
    	/* 单个导入 */
        switch($tool_type){ 
        	case "1":
        		$i=0;
        		foreach($role_name as $key => $value){
        			if($value=='') continue;
        			$sql="update " .TBL_WB_USER." set identity=$tool_type where name='$value' ";
        			$wangpu_db -> query($sql);
        			$row=$wangpu_db ->affected_rows();
        			if($row){
        				$i++;
        			}else{
        				//$str.=$_LANG['rolename'].$value.$_LANG['wrong']." <br />";
        				$str.=$_LANG['rolename'].$value.$_LANG['wrong'];
        			}
        		}
        		//$str=$_LANG['set'].$i.$_LANG['count'].$_LANG['zdy']."<br />".$str;
        		$str=$_LANG['set'].$i.$_LANG['count'].$_LANG['zdy'];
           	 	break;
        	case "2":
        		foreach($role_name as $key => $value){
        			if($value=='') continue;
        			$role=$value.'[PM]';
        			$sql="update " .TBL_WB_USER." set identity=$tool_type,name='$role' where name='$value' ";
        			$wangpu_db -> query($sql);
        			$row=$wangpu_db ->affected_rows();
        			if($row){
        				$i++;
        			}else{
        				//$str.=$_LANG['rolename'].$value.$_LANG['wrong']." <br />";
        				$str.=$_LANG['rolename'].$value.$_LANG['wrong'];
        			}
        		}
        		//$str=$_LANG['set'].$i.$_LANG['count'].$_LANG['zdy']."<br />".$str;
        		$str=$_LANG['set'].$i.$_LANG['count'].$_LANG['GM'];
           	 	break;
    	}
    }
	$sql="select id,name,identity from`wb_user` where identity=1 or identity=2 order by identity desc";
	$rs=$wangpu_db -> getAll($sql);

    $smarty -> assign('str',$str);
	$smarty -> assign('rs',$rs);
	$smarty -> assign('here',$_LANG['newuser']);
	$smarty -> display('tool_newuser.htm');
	
    
}

if($_REQUEST['act'] == 'del'){
	$role=$_REQUEST['role'];
	$sql="update " .TBL_WB_USER." set identity=0 where name='$role' ";
    $wangpu_db -> query($sql);
    $row=$wangpu_db ->affected_rows();
	if($row){
		$str=$_LANG['del_success']."[".$role."]";
	}
	$sql="select id,name,identity from`wb_user` where identity=1 or identity=2 order by identity desc";
	$rs=$wangpu_db -> getAll($sql);

    $smarty -> assign('str',$str);
	$smarty -> assign('rs',$rs);
	$smarty -> assign('here',$_LANG['newuser']);
	$smarty -> display('tool_newuser.htm');
}

if($_REQUEST['act'] == 'GManswer'){
	
   $arr_type=array(
		0 => $_LANG['up_bug'],
		1 => $_LANG['complain'],
		2 => $_LANG['suggest'],
		3 => $_LANG['other'],
	);
	
	$arr_state=array(
		0 => $_LANG['no_deal'],
		1 => $_LANG['deal'],
	);
	
	$state=$_REQUEST['state'];
	
	if(!$state){

	    $sql="select * from ".TBL_YJ_USERASK." where state=0 order by time desc";
	}
	if($state==1){
		$sql="select * from ".TBL_YJ_USERASK." where state=1 order by time desc";
	}
	if($state==2){
		$sql="select * from  ".TBL_YJ_USERASK." order by time desc";
	}
	
    $res=$yjjhtp_db ->getAll($sql);;
    
    
	
    $smarty -> assign('type',$arr_type);
    $smarty -> assign('state',$arr_state);
	$smarty -> assign('res',$res);
	$smarty -> assign('here',$_LANG['answer']);
	$smarty -> assign('full_page',1);
	$smarty -> display('tool_answer.htm');
}

if($_REQUEST['act'] == 'remove'){
      $arr_type=array(
		0 => $_LANG['up_bug'],
		1 => $_LANG['complain'],
		2 => $_LANG['suggest'],
		3 => $_LANG['other'],
	);
	
	$arr_state=array(
		0 => $_LANG['no_deal'],
		1 => $_LANG['deal'],
	);
	
	$id=$_REQUEST['id'];
	$sql="delete from ".TBL_YJ_USERASK." where id =$id ";
	$query=$yjjhtp_db -> query($sql);
	
    $sql="select * from ".TBL_YJ_USERASK." order by time desc";	
    $res=$yjjhtp_db ->getAll($sql);;

    $smarty -> assign('type',$arr_type);
    $smarty -> assign('state',$arr_state);
	$smarty -> assign('res',$res);
	$smarty -> assign('here',$_LANG['answer']);
	$smarty -> assign('full_page',1);
	$smarty -> display('tool_answer.htm');
	
}

if($_REQUEST['act'] == 'showask'){
	
	$arr_type=array(
		0 => $_LANG['up_bug'],
		1 => $_LANG['complain'],
		2 => $_LANG['suggest'],
		3 => $_LANG['other'],
	);
	
	$arr_state=array(
		0 => $_LANG['no_deal'],
		1 => $_LANG['deal'],
	);
	
	$id=$_REQUEST['id'];
	$sql="select * from ".TBL_YJ_USERASK." where id =$id ";
	$info=$yjjhtp_db ->getRow($sql);

	$smarty->assign('action_link', array('href'=>'tool.php?act=GManswer', 'text' => $_LANG['asklist'])); 
	$smarty -> assign('type',$arr_type);
    $smarty -> assign('state',$arr_state);
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['answer']);
	$smarty -> assign('full_page',1);
	$smarty -> display('tool_showask.htm');
}

if($_REQUEST['act'] == 'review'){
	
   $arr_type=array(
		0 => $_LANG['up_bug'],
		1 => $_LANG['complain'],
		2 => $_LANG['suggest'],
		3 => $_LANG['other'],
	);
	
	$arr_state=array(
		0 => $_LANG['no_deal'],
		1 => $_LANG['deal'],
	);	
	$id=$_REQUEST['id'];
	$username=$_REQUEST['username'];
	$content=$_REQUEST['content'];
	
	$sql="select * from ".TBL_YJ_USERASK." where id =$id ";
	$info=$yjjhtp_db ->getRow($sql);
	
	$sql="select id from ".TBL_ACCOUNT."  where name='yjjhadm01' limit 1";
	$userid=$acc_db ->getOne($sql);
	if($userid){
	    $sql="select name from ".TBL_WB_USER." where account_id=$userid limit 1";
		$name=$wangpu_db -> getOne($sql);
	}
	
	if(!empty($content)){
	    $time=date('YmdHis');
	    $sql="insert into ".TBL_WB_LEAVEWORD."(user_name,send_name,time,words,color) values('$username','$name','$time','$content','ffffff')";
	    $rs=$wangpu_db -> query($sql);
	    if($rs){
	        $ok=1;
        }
	    if($ok){
		    $sql="update ".TBL_YJ_USERASK." set state=1 where id=$id";
            $rs=$yjjhtp_db -> query($sql);
	   }
	   if(!rs){
	   	    $ok=0;
	   }
	}
	
    $smarty -> assign('ok',$ok);
	$smarty -> assign('action_link', array('href'=>'tool.php?act=GManswer', 'text' => $_LANG['asklist'])); 
	$smarty -> assign('type',$arr_type);
    $smarty -> assign('state',$arr_state);
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['answer']);
	$smarty -> assign('full_page',1);
	$smarty -> display('tool_review.htm');
}

if($_REQUEST['act'] == 'export_detail'){
	
    //require("./includes/all_server_num.php");
    set_time_limit(0);	
	$geturl = $_CFG['apiurl']."getserverconfig.php"; 
    $serverconfig = gethttpcnt($geturl);
	$serverconfig = unserialize($serverconfig);
	
    date_default_timezone_set('PRC');
	$stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
        $stime=date('Y-m-01', strtotime(now)); 
	    $stime=strtotime($stime);
		$etime=strtotime('now');
	}else{
	    $stime=strtotime($stime);
		$etime=strtotime($etime)+ONE_DAY;
	}

    foreach ($serverconfig['arr_s'] as $key => $value){
        for($i=1;$i<=$value;$i++){
    		if($i<=$serverconfig['hefu'][$key]&&$i%2==0){
    			continue;
    		}
        	if(empty($serverconfig['prevpages'][$key])){
				continue;
			}
    		$info[$key][]=$i;
    	}
    }
    //print_r($info);
    foreach ($serverconfig['arr_s'] as $key => $value){
    	if(!empty($_POST['s_'.$key])){
    		$server[$key]=$_POST['s_'.$key];
    	}
    }
 
    if(!empty($server)){
        foreach ($server as $key => $value){
		    foreach($value as $k => $v){
		        $str = "http://".$serverconfig['prevpages'][$key]['prev'].$v.$serverconfig['prevpages'][$key]['last']."/interface/export_detail.php?bdate=$stime&edate=$etime ";
		        $url[$key][$v] = $str;
		    }
        }
    }
    //print_r($url);

    if(!empty($url)){
        foreach ($url as $pt => $s){
    	    foreach ($s as $k =>$v){
    		    $result = gethttpcnt($v);
		        $data[$pt][$k] = unserialize($result);
    	    }
        }
    }
    $export="<table>";
    $export.="<tr align='center'>";
    $export.="<td>".$_LANG['time']."</td>";
    $export.="<td>".$_LANG['order_num']."</td>";
    $export.="<td>IP</td>";
    $export.="<td>".$_LANG['timestamp']."</td>";
    $export.="<td>".$_LANG['username']."</td>";
    $export.="<td>".$_LANG['rmb']."</td>";
    $export.="<td>".$_LANG['yb']."</td>";
    $export.="<td>".$_LANG['ptname']."</td>";
    $export.="<td>".$_LANG['sid']."</td>";
    $export.="</tr>";
    if(!empty($data)){
        foreach ($data as $pt => $s){
    	    foreach ($s as $k =>$v){
    		    if(!empty($v)){
                    $export1.=$v;
    		    }else{
    		    	$err_data[$pt][]=$k;
    		    }
    	    }
        }
    }
    if(!empty($export1)){
    	if(!empty($err_data)){
    		$msg="<tr>".$_LANG['nodata']."</tr>";
    		foreach ($err_data as $k =>$v){
    			$msg.="<tr><td>$k:</td></tr><tr>";
    			foreach ($v as $key => $rs){
    			    $msg.="<td>$rs,</td>";	
    			}
    			$msg.="</tr>";
    		}
    		echo $msg;
    	}
        $export.=$export1."</table>";
        header("Content-Type:application/vnd.ms-excel");
        header("Content-Disposition:filename=detail".date('Ymd'));
        print_r($export);
        exit;
    }

    
    
	$smarty -> assign('date',$date);
	$smarty -> assign('here',$_LANG['tool_export_detail']);
	$smarty -> assign('err' , $err_data);
	$smarty -> assign('info' ,$info);
	$smarty -> display('tool_export_detail.htm');
}


?>