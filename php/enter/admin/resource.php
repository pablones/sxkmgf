<?php
/**
 * 资源管理工具
 */
define('IN_ECS', true);
  
require(dirname(__FILE__) . '/includes/init.php');

date_default_timezone_set('PRC');
/* 元宝充值*/
if($_REQUEST['act'] == 'yuanbao'){  
	admin_priv("yb_res");
	
	$smarty -> assign('here',$_LANG['yuanbao']);
    $smarty -> display('res_yb.htm');
}
/* 珠宝处理 */
if($_REQUEST['act'] == 'zhubao'){
	admin_priv("zb_res");
	
	$smarty -> assign('here',$_LANG['zhubao']);	
    $smarty -> display('res_zb.htm');
}
/* 卡片生成 */
if($_REQUEST['act'] == 'card'){
	admin_priv("card_res");

	$smarty -> assign('here',$_LANG['card']);
	$smarty -> display('res_card.htm');
}
/* 添加物品 */
if($_REQUEST['act'] == 'goods'){
	admin_priv("goods_res");

	$smarty -> assign('here',$_LANG['add_goods']);	
	$smarty -> display('res_good.htm');
}
/* 活动管理 */
if($_REQUEST['act'] == 'activity'){   
	admin_priv("act_manage");

	$smarty -> assign('here',$_LANG['activity']);	
	$smarty -> display('res_act.htm');
}

/* 元宝充值处理 */
if($_REQUEST['act'] == 'yb_query'){
	$fileinfo = $_FILES['input_account'];
    $links[] = array('text' => $_LANG['back_yb'], 'href' => 'resource.php?act=yuanbao');   
    if($fileinfo['name']){    	
   		$filepath = ROOT_PATH."data/upload/pay_yb/";
    	if(!is_dir($filepath)) mkdir($filepath,0777);
       	$filepath .= "yb_".time().".txt";
		if(file_exists($filepath)){
			unlink($filepath);
		}
        if(move_uploaded_file($fileinfo['tmp_name'],$filepath)){
			if(file_exists($filepath)){
				$handle = fopen($filepath,"r");
				while (!feof($handle)) {
					$str=iconv('UTF-8','GBK',trim(fgets($handle, 4096)));
					if(!empty($str)){
						$buffer[] = $str;
					}
					unset($str);
				}
				fclose($handle);
			}
		}else{
			sys_msg($_LANG['copy_error'], 1, $links);
		}
		if(empty($buffer)){
		    sys_msg($_LANG['file_empty'],1,$links);
		}	
		$paynum = time();	
		foreach($buffer as $key => $value){
			$temp = explode(",", $value);
			$sql = "select `id` from ".TBL_ACCOUNT." where `name` = '".$temp[0]."'";
			
			$account_id = $acc_db -> getOne($sql);
			
			if($account_id){
				$sql = "update ".TBL_SK_VAS_TABLE." set `VAS_Point` = `VAS_Point` + ".$temp[1]." where `Account_ID` = ".$account_id."";
				$acc_db -> query($sql);			
				$logdate = time();
				$ip = $_SERVER['REMOTE_ADDR'];
				$http_str = $_SERVER['HTTP_HOST'];
        		$serverid = stripos($http_str,'-');
        		$serverid = $http_str[$serverid - 1];
        		/* 后台充值，记录进后台元宝充值日志表 */
        		$sql = "insert into ".TBL_SK_ADPAYLOG."(`paynum`,`logdate`,`userid`,`ip`,`serverid`,`paygold`,`paytype`) values('".$paynum."','".$logdate."','".$account_id."','".$ip."','".$serverid."','".$temp[1]."',".PAY_TYPE_YB.")";
        		$acc_db -> query($sql);
        		//记录管理员操作日志
        		$sql = "select `id`,name,level,Exit_Time from ".TBL_WB_USER." where `account_id` ='$account_id' ";
		        $rs = $wangpu_db->getRow($sql);
                $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$account_id' ";
                $rs1=$acc_db ->getRow($sql);
                $itemtype=$_LANG[add_yb];
		        $name=$_LANG[is_account].' '.$rs1['name'];
		        $regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		        $lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		        $lastlogin=$_LANG[lastlogin].$lastlogin;
		        $level=$_LANG[level].$rs['level'];
				$role=$rs['name'];
				
		        $cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[is_username].' '.$role.'|'.$level.'|';
		        $cnt.=$itemtype.'|'.$_LANG[count].$temp[1].'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		        $cnt.=$_POST[add_type];
				if($_POST[add_type]==$_LANG['staff']){
			        $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		        }

        		//$cnt = $_SESSION['admin_name'].":添加元宝,".$temp[1];
        		$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
        		$acc_db -> query($sql);
        		$paynum++;
			}else{
				$error[] = $temp[0];
			}
		}  
		if(count($error) != 0){
			foreach($error as $key => $value){
				$msg_err .= $value ."\n";
			}
			$msg = $_LANG['error_account'] . $msg_err;
			sys_msg($msg,1,$links,FALSE);
		}
    }else{   	
    	$user_name = $_POST['platform_account'];
    	$yb_amount = $_POST['yb_amount'];
    	$paynum = time();
       	/* 增加玩家账号 */
    	if(empty($user_name[0])||empty($yb_amount[0])){
    	    sys_msg($_LANG['user_empty'],0,$links);
    	}
		foreach($user_name as $key => $value){
			$sql = "select `id` from ".TBL_ACCOUNT." where `name` = '".$value."'";
    		$account_id = $acc_db -> getOne($sql);
			$sql = "update ".TBL_SK_VAS_TABLE." SET `VAS_Point` = `VAS_Point`+ ".$yb_amount[$key]."  WHERE `Account_ID` =".$account_id." ";
			$acc_db -> query($sql);
        	$logdate = time();
        	$ip = $_SERVER['REMOTE_ADDR'];
        	/* 截取服务器编号 */
        	$http_str = $_SERVER['HTTP_HOST'];
        	$serverid = stripos($http_str,'-');
        	$serverid = $http_str[$serverid - 1];
        	/* 后台充值，记录进后台元宝充值日志表 */
        	$sql = "insert into ".TBL_SK_ADPAYLOG."(`paynum`,`logdate`,`userid`,`ip`,`serverid`,`paygold`,`paytype`) values('".$paynum."','".$logdate."','".$account_id."','".$ip."','".$serverid."','".$yb_amount[$key]."',".PAY_TYPE_YB.")";
        	$acc_db -> query($sql);
        	//添加管理员日志
		    $sql = "select `id`,name,level,Exit_Time from ".TBL_WB_USER." where `account_id` ='$account_id' ";
		    $rs = $wangpu_db->getRow($sql);
            $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$account_id' ";
            $rs1=$acc_db ->getRow($sql);
            $itemtype=$_LANG[add_yb];
		    $name=$_LANG[is_account].' '.$rs1['name'];
		    $regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		    $lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		    $lastlogin=$_LANG[lastlogin].$lastlogin;
		    $level=$_LANG[level].$rs['level'];
			$role=$rs['name'];
				
		    $cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[is_username].' '.$role.'|'.$level.'|';
		    $cnt.=$itemtype.'|'.$_LANG[count].$yb_amount[$key].'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		    $cnt.=$_POST[add_type];
			if($_POST[add_type]==$_LANG['staff']){
			    $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		    }
        	//$cnt = $_SESSION['admin_name'].":添加元宝,".$yb_amount;
        	$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
        	$acc_db -> query($sql);
        	$paynum++;
		}

    }
    sys_msg($_LANG['success'],0,$links);
    $smarty -> display('res_yb.htm');
}

/* 珠宝充值处理 */
if($_REQUEST['act'] == 'zb_query'){
	
	$fileinfo = $_FILES['input_account'];
    $links[] = array('text' => $_LANG['back_yb'], 'href' => 'resource.php?act=zhubao');   
    
    if($fileinfo['name']){   
     	
   		$filepath = ROOT_PATH."data/upload/pay_yb/";
    	if(!is_dir($filepath)) mkdir($filepath,0777);
       	$filepath .= "yb_".time().".txt";
		if(file_exists($filepath)){
			unlink($filepath);
		}
        if(move_uploaded_file($fileinfo['tmp_name'],$filepath)){
			if(file_exists($filepath)){
				$handle = fopen($filepath,"r");
				while (!feof($handle)) {
					$str=iconv('UTF-8','GBK',trim(fgets($handle, 4096)));
					if(!empty($str)){
						$buffer[] = $str;
					}
					unset($str);
				}
				fclose($handle);
			}
		}else{
			sys_msg($_LANG['copy_error'], 1, $links);
		}
		if(empty($buffer)){
		    sys_msg($_LANG['file_empty'],1,$links);
		}	
		$paynum = time();
		foreach($buffer as $key => $value){
			$temp = explode(",", $value);
			$sql = "select `id` from ".TBL_ACCOUNT." where `name` = '".$temp[0]."'";
			$account_id = $acc_db -> getAll($sql);			
			if($account_id){
				foreach($account_id as $k => $v){
					$sql = "select * from ".TBL_WB_USER." where account_id = '".$v['id']."'";
					$result = $wangpu_db -> getRow($sql);
					if($result){
						$sql = "update ".TBL_WB_USER." set Gold4 = Gold4 + '".$temp[1]."' where `account_id` = '".$v['id']."'";
						$result = $wangpu_db -> query($sql);
						//添加管理员日志
					    $sql = "select `id`,name,level,Exit_Time from ".TBL_WB_USER." where `account_id` ='$v[id]' ";
		                $rs = $wangpu_db->getRow($sql);
                        $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$v[id]' ";
                        $rs1=$acc_db ->getRow($sql);
                        $itemtype=$_LANG[add_zb];
		                $name=$_LANG[is_account].' '.$rs1['name'];
		                $regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		                $lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		                $lastlogin=$_LANG[lastlogin].$lastlogin;
		                $level=$_LANG[level].$rs['level'];
				        $role=$rs['name'];
				
		                $cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[is_username].' '.$role.'|'.$level.'|';
		                $cnt.=$itemtype.'|'.$_LANG[count].$temp[1].'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		                $cnt.=$_POST[add_type];
				        if($_POST[add_type]==$_LANG['staff']){
			                $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		                }
						//$cnt = $_SESSION['admin_name'].":添加珠宝,".$temp[1];
        				$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
        				$acc_db -> query($sql);
						if(!$result){
							$error = $temp[0];	
						}
					}
				}
			}else{
				$error[] = $temp[0];
			}
		}  
		/* 错误信息处理  */
		if(count($error) != 0){
			foreach($error as $key => $value){
				$msg_err .= $value ."\n";
			}
			$msg = $_LANG['error_account'] . $msg_err;
			sys_msg($msg,1,$links,FALSE);
		}else{
			$flag=1;
		}
    }else{   	
    	$user_name = $_POST['platform_account'];
    	$zb_amount = $_POST['zb_amount'];
    	$paynum = time();
       	/* 增加玩家账号 */
		foreach($user_name as $key => $value){
			$sql = "select `id` from ".TBL_ACCOUNT." where `name` = '".$value."'";
    		$account_id = $acc_db -> getAll($sql); 		
    		if($account_id){
    			foreach($account_id as $k => $v){   			
    				$sql  = "update ".TBL_WB_USER." set Gold4 = Gold4 +'".$zb_amount[$key]."' where `account_id` = '".$v['id']."'";
    				$result = $wangpu_db -> query($sql);  
    				$flag=1;				
    				$logdate = time();
        			$ip = $_SERVER['REMOTE_ADDR'];
        			/* 后台充值，记录进后台元宝充值日志表 */
        			if($result){
        				$sql = "insert into ".TBL_SK_ADPAYLOG."(`paynum`,`logdate`,`userid`,`ip`,`serverid`,`paygold`,`paytype`) values('".$paynum."','".$logdate."','".$account_id."','".$ip."','".$_CFG['fuwuqi_id']."','".$zb_amount[$key]."',".PAY_TYPE_ZB.")";
        				$acc_db -> query($sql);
        				//添加管理员日志
					    $sql = "select `id`,name,level,Exit_Time from ".TBL_WB_USER." where `account_id` ='$v[id]' ";
		                $rs = $wangpu_db->getRow($sql);
                        $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$v[id]' ";
                        $rs1=$acc_db ->getRow($sql);
                        $itemtype=$_LANG[add_zb];
		                $name=$_LANG[is_account].' '.$rs1['name'];
		                $regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		                $lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		                $lastlogin=$_LANG[lastlogin].$lastlogin;
		                $level=$_LANG[level].$rs['level'];
				        $role=$rs['name'];
				
		                $cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[is_username].' '.$role.'|'.$level.'|';
		                $cnt.=$itemtype.'|'.$_LANG[count].$zb_amount[$key].'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		                $cnt.=$_POST[add_type];
				        if($_POST[add_type]==$_LANG['staff']){
			                $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		                }
        				//$cnt = $_SESSION['admin_name'].":添加珠宝,".$zb_amount[$key];
        				$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
        				$acc_db -> query($sql);
        				$paynum++;
        			}
    			}
    		}
		}

    }
    if($flag){
    	sys_msg($_LANG['success'],0,$links);
    }else{
    	sys_msg($_LANG['err_acount'],0,$links);
    }
    $smarty -> display('res_yb.htm');
}
/* 添加物品处理 */
if($_REQUEST['act'] == 'goods_query'){
	$fileinfo = $_FILES['input_account'];
	$links[] = array('text' => $_LANG['back_goods'], 'href' => 'resource.php?act=goods');
	if($fileinfo['name']){
		/* 批量添加物品 */
    	$filepath = ROOT_PATH."data/upload/pay_goods/";
	    if(!is_dir($filepath)) mkdir($filepath,0777);
       	$filepath .= "reset_".time().".txt";
		if(file_exists($filepath)){
			unlink($filepath);
		}
        if(move_uploaded_file($fileinfo['tmp_name'],$filepath)){
        	if(file_exists($filepath)){
                $contents = file_get_contents($filepath);
                $encoding = mb_detect_encoding($contents, array('GB2312','GBK','UTF-16','UCS-2','UTF-8','BIG5','ASCII'));
			    $fp = fopen($filepath,"r");
        	    $num = 0;
                if(!(feof($fp))) {
                    $num++;
                    $str = trim(fgets($fp));
                    if ($encoding != false) {
                        $str = iconv($encoding, 'UTF-8', $str);
                        if ($str != "" and $str != NULL) {
                            $role_name[] = $str;
                        }

                    }else {
                        $str = mb_convert_encoding ( $str, 'UTF-8','Unicode');
                        if ($str != "" and $str != NULL) {
                            $role_name[] = $str;
                        }
                    }
                }
                while(!(feof($fp))) {
                    $str = '';
                    $str = trim(fgets($fp));
                    if ($encoding != false) {
                        $str = iconv($encoding, 'UTF-8', $str);
                        if ($str != "" and $str != NULL) {
                            $role_name[] = $str;
                        }
                     }else {
                         $str = mb_convert_encoding ( $str, 'UTF-8','Unicode');
                         if ($str != "" and $str != NULL) {
                             $role_name[]=$str;
                         }
                     }
                }
            }
        }else{
			sys_msg($_LANG['copy_error'], 1, $links);
	    }
	    if(empty($role_name)){
		    sys_msg($_LANG['file_empty'],1,$links);
	    }
	    //print_r($role_name);
		foreach($role_name as $key => $value){
			if($value=='') continue;
			$temp = explode(",", $value);
			$sql = "select `id` from ".TBL_WB_USER." where `name` like '$temp[0]' ";

			$rs = $wangpu_db -> getOne($sql);
			if($rs > LEGAL_USERID){
				$sql = "insert into ".TBL_WB_ITEM." (`TYPE`,`owner_id`,`player_id`,`amount`,`amount_limit`,`color`,`kidnap`,`POSITION`) values(".$temp[1].",".$rs.",".$rs.",".$temp[2].",".$temp[3].",".$temp[4].",".$temp[5].",".$temp[6].")";
				$rs = $wangpu_db -> query($sql);
				if($rs){
				    $ok=1;
				    $res[]=$_LANG['is_username']."[".$temp[0]."]".$_LANG['add_success'];
				}else{
					$res[]=$_LANG['is_username']."[".$temp[0]."]".$_LANG['add_false'];
				}  	
			}else{
				$error = $temp[0];
				$res[]=$_LANG['is_username']."[".$temp[0]."]".$_LANG['no_role'];
			}
			if($ok){
				//$cnt = $_SESSION['admin_name'].":添加物品 ".$temp[1]." 添加用户 ：".$temp[0];
			    $sql = "select `id`,level,account_id,Exit_Time from ".TBL_WB_USER." where `name` LIKE ('".$temp[0]."')";
		        $rs = $wangpu_db->getRow($sql);
                $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$rs[account_id]' ";
                $rs1=$acc_db ->getRow($sql);
                $sql="select name from ".TBL_WB_ITEMTYPE." where id ='$temp[1]' ";
                $itemtype=$_LANG[additem].$wangpu_db -> getOne($sql);
		        $name=$_LANG[is_account].' '.$rs1['name'];
		        $regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		        $lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		        $lastlogin=$_LANG[lastlogin].$lastlogin;
		        $level=$_LANG[level].$rs['level'];
		
		        $cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[is_username].' '.$temp[0].'|'.$level.'|';
		        $cnt.=$itemtype.'|'.$_LANG[count].$temp[2].'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		        $cnt.=$_POST[add_type];
		        if($_POST[add_type]==$_LANG['staff']){
			        $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		        }
        		$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)".
        		        "values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
        		$acc_db -> query($sql);
			}
		}
	}else{	
		/* 单个添加物品 */	
		$user_name = $_POST['username'];
	    $goods_id = $_POST['goods_id'];
	    $goods_amount = $_POST['goods_amount'];
	    $overlay_amount = $_POST['overlay_amount'];
	    $goods_color = $_POST['goods_color'];
	    $binding = $_POST['binding'];
	    $goods_position = $_POST['goods_position'];
		
		$sql = "select `id` from ".TBL_WB_USER." where `name` LIKE ('".$user_name."')";
		$id = $wangpu_db->getOne($sql);
		
	
		if($id > LEGAL_USERID){
		    $sql = "insert into ".TBL_WB_ITEM." (`TYPE`,`owner_id`,`player_id`,`amount`,`amount_limit`,`color`,`kidnap`,`POSITION`) values(".$goods_id.",".$id.",".$id.",".$goods_amount.",".$overlay_amount.",".$goods_color.",".$binding.",".$goods_position.")";
			$rs = $wangpu_db -> query($sql);
			if($rs)  $ok=1;	    
		}else{
		    sys_msg($_LANG['goods_error'],1,$links);
		}
		//如果添加成功则插入管理员日志
	    if($ok){	    
	    	$sql = "select `id`,level,account_id,Exit_Time from ".TBL_WB_USER." where `name` LIKE ('".$user_name."')";
		    $rs = $wangpu_db->getRow($sql);
            $sql=" select name,regdate  from ".TBL_ACCOUNT." where id ='$rs[account_id]' ";
            $rs1=$acc_db ->getRow($sql);
            $sql="select name from ".TBL_WB_ITEMTYPE." where id ='$goods_id' ";
            $itemtype=$_LANG[additem].$wangpu_db -> getOne($sql);
		    $name=$_LANG[is_account].' '.$rs1['name'];
		    $regdate=$_LANG[regdate].date('Y-m-d H:i:s',$rs1['regdate']);
		    $lastlogin="20".substr($rs['Exit_Time'],0,2)."-".substr($rs['Exit_Time'],2,2)."-".substr($rs['Exit_Time'],4,2)." ".substr($rs['Exit_Time'],6,2).":".substr($rs['Exit_Time'],8,2);
		    $lastlogin=$_LANG[lastlogin].$lastlogin;
		    $level=$_LANG[level].$rs['level'];
    
		    $cnt=$_LANG[s_id].$_CFG['fuwuqi_id']."|".$name.'|'.$_LANG[is_username].' '.$user_name.'|'.$level.'|';
		    $cnt.=$itemtype.'|'.$_LANG[count].$overlay_amount.'|'.$regdate.'|'.$lastlogin.'|'.$_LANG[admin_account].$_SESSION['admin_name'].'|'.$_LANG[add_type].' ';
		    $cnt.=$_POST[add_type];
		    if($_POST[add_type]==$_LANG['staff']){
			    $cnt.='|'.$_LANG[request].' '.$_POST[request].'|'.$_LANG[act_man].' '.$_POST[action];
		    }
        	$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)".
        		   "values('".time()."','".$_SESSION['admin_id']."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";
        	$rs2=$acc_db -> query($sql);
			if($rs2){
			   sys_msg($_LANG['success'],0,$links);
			}
		}    
	}

    $smarty -> assign('res',$res);
    $smarty -> display('res_good.htm');
}
/* 卡片生成器处理 */
if($_REQUEST['act'] == 'card_query'){
	$links[] = array('text' => $_LANG['back_card'], 'href' => 'resource.php?act=card');
    $cardtype = $_POST['cardtype'];
    $card_amount = intval($_POST['card_amount']);
    if($card_amount == 0) sys_msg($_LANG['card_empty'],1,$links);  
    if($card_amount > 5000) sys_msg($_LANG['card_full'],1,$links);   
    $j = 1;
    $k = 100;
    switch($cardtype){
        case "1":
            $sql = "INSERT INTO ".TBL_SK_NEWCARD."(`newcard`)VALUES";
            while($j <= $card_amount){
                $card   = 'YJJH-'.random(8,1).'-'.randEn(4);
                if($j != $k && $j < $card_amount){
                    $sql .= "('".$card."'),";
                }else{
                    $sql .= "('".$card."')";
                    $k = $k + 100;
                    $acc_db -> query($sql);
                    $sql = "INSERT INTO ".TBL_SK_NEWCARD."(`newcard`)VALUES";
                }
                $j++;
            }
            break;
        case "2":
            $sql = "INSERT INTO ".TBL_SK_TPCARD." (`newcard`)VALUES";
            while($j <= $card_amount){
                $card   = 'YJTP-'.random(8,1).'-'.randEn(4);
                if($j != $k && $j < $card_amount){
                    $sql .= "('".$card."'),";
                }else{
                    $sql .= "('".$card."')";
                    $k = $k + 100;
                    $acc_db -> query($sql);
                    $sql = "INSERT INTO ".TBL_SK_TPCARD." (`newcard`)VALUES";
                }
                $j++;
            }
            break;
        case "3":
        	sys_msg($_LANG['voice_notic'],1,$links);
            break;
    }
    if($j>$card_amount){
        sys_msg($_LANG['success'],0,$links);
    }

    $smarty -> display('res_card.htm');
}

/*导出卡片*/
if($_REQUEST['act'] == 'card_output'){
	$links[] = array('text' => $_LANG['back_card'], 'href' => 'resource.php?act=card');
	$cardtype = $_REQUEST['cardtype'];
	$count = $_REQUEST['card_amount'];

	switch($cardtype){
		case "1":
			$filepath = ROOT_PATH.'data/upload/newcard/lstDumpEnd'.$cardtype.'.log';
      		if(file_exists($filepath)){
        		$start = sreadfile($filepath);
        		//$start = ceil($start/100)*100;
      		}else{
        		$start = 0;
        	}
      		$end   = $start + $count;

      		$where  = " WHERE Id > $start   AND picked<1 ";
      		$sql    = "SELECT COUNT(*) FROM ".TBL_SK_NEWCARD." $where ";
      		$totalnum = $acc_db ->getOne($sql);
      		
      		if($totalnum < $count){
      			//sys_msg($msg_detail);
      			jump("",$_LANG['out_false']."\\n".$_LANG['because']."({$start}-{$end})".$_LANG['allcard']."{$totalnum}".$_LANG['tip1'].$_LANG['liter']."{$count}!\\n".$_LANG['createnew']);
        	}
      		$sql = "SELECT * FROM ".TBL_SK_NEWCARD." $where ORDER BY Id ASC LIMIT $count ; ";
      		$result = $acc_db->getAll($sql);
		    $cnt= date("Y-m-d H:i:s").">>>".$_SG['admname'].$_LANG['out']."{$count}".$_LANG['tip2']."{$totalnum}".$_LANG['tip3']."<{$start}-{$end}>. \r\n";
		    $cnt .= "ID\t".$_LANG['newcardid']."\r\n";
		    foreach($result as $key => $value){
		    	$cnt .= $value['Id']."\t";
		    	$cnt .= $value['picked']>0? $_LANG['card_used']:$value['newcard'];
		    	$cnt .= "\r\n";
		    }
      		//记录这次的最后一个区间
     		swritefile($filepath,$end);

      		//保存今日的导出日志
      		$logfile = ROOT_PATH.'data/upload/newcard/dumplog_'.date("Ymd").".php";
      		swritefile($logfile,"<?php exit()?>\n\n".$cnt."\n\n------------------------------------------\n\n",'a');

      		header("Content-type: text/plain"); //纯文本格式
      		header("Content-Disposition: attachment; filename=NewcardDump_".date("Ymd")."_[".($start+1)."-".$end."].txt");
      		echo $cnt;
      		exit;
		case "2":
			$filepath = ROOT_PATH.'data/upload/newcard/lstDumpEnd'.$cardtype.'.log';
      		if(file_exists($filepath)){
        		$start = sreadfile($filepath);
        		//$start = ceil($start/100)*100;
      		}else{
        		$start = 0;
        	}
      		$end   = $start + $count;

      		$where  = " WHERE Id > $start   AND picked<1 ";
      		$sql    = "SELECT COUNT(*) FROM ".TBL_SK_TPCARD." $where ";
      		$totalnum = $acc_db ->getOne($sql);
      		if($totalnum < $count){
      			//sys_msg($msg_detail);
        		jump("",$_LANG['out_false']."\\n".$_LANG['because']."({$start}-{$end})".$_LANG['allcard']."{$totalnum}".$_LANG['tip1'].$_LANG['liter']."{$count}!\\n".$_LANG['createnew']);
        	}
      		$sql    = "SELECT * FROM ".TBL_SK_TPCARD." $where ORDER BY Id ASC LIMIT $count ; ";
      		$result  = $acc_db->getAll($sql);
		    $cnt= date("Y-m-d H:i:s").">>>".$_SG['admname'].$_LANG['out']."{$count}".$_LANG['tip2']."{$count}".$_LANG['tip3']."<{$start}-{$end}>. \r\n";
		    $cnt   .= "ID\t".$_LANG['mtcardid']."\r\n";
		    foreach($result as $key => $value){
		    	$cnt .= $value['Id']."\t";
		    	$cnt .= $value['picked']>0? $_LANG['card_used']:$value['newcard'];
		    	$cnt .= "\r\n";
		    }
      		//记录这次的最后一个区间
     		swritefile($filepath,$end);

      		//保存今日的导出日志
      		$logfile = ROOT_PATH.'data/upload/newcard/dumplog_'.date("Ymd").".php";
      		swritefile($logfile,"<?php exit()?>\n\n".$cnt."\n\n------------------------------------------\n\n",'a');

      		header("Content-type: text/plain"); //纯文本格式
      		header("Content-Disposition: attachment; filename=NewcardDump_".date("Ymd")."_[".($start+1)."-".$end."].txt");
      		echo $cnt;
      		exit;
		case "3":
			break;
	}
	$smarty -> display('res_card.htm');
}

/* 活动管理处理 */
if($_REQUEST['act'] == 'activity_query'){
    $cardtype = $_POST['cardtype'];
    $fileinfo = $_FILES['act_account'];
    $filepath = ROOT_PATH."data/upload/act/";
    $links[] = array('text' => $_LANG['back_act'], 'href' => 'resource.php?act=activity');
    if(empty($fileinfo['name'])){   	
   		sys_msg($_LANG['file_act_empty'], 1, $links);
    }
    if(!is_dir($filepath)) mkdir($filepath,0777);
        $filepath .= time().".txt";
		if(file_exists($filepath)){
		    unlink($filepath);
		}
        if(move_uploaded_file($fileinfo['tmp_name'],$filepath)){
		    if(file_exists($filepath)){
			    $handle = fopen($filepath,"r");
				while (!feof($handle)) {
				    $str=iconv('UTF-8','GBK',trim(fgets($handle, 4096)));
					if(!empty($str)){
					    $buffer[] = $str;
					}
					unset($str);
				}
				fclose($handle);
			}
		}else{
			sys_msg($_LANG['copy_error'], 1, $links);
	}
    if(empty($buffer)){
    	sys_msg($_LANG['file_empty'],1,$links);
    }
    switch($cardtype){
        /* 签名活动 */
        case "1":
            foreach($buffer as $key =>$value){
                $sql = "select `id`,`name` from ".TBL_ACCOUNT." where `name` = '".$value."'";
                $account_info = $acc_db -> getRow($sql);
                if(!empty($account_info['id'])){
                    $sql = "select * from ".TBL_SK_GENERALIZE." where `account_id` = ".$account_info['id']." and `type` =".$cardtype."";
                    $generalize_info =$acc_db -> getRow($sql);
                }else{
                	$error[] = $value;
                	continue;
                }
                if(!$generalize_info['id']){
                    $sql = "insert into ".TBL_SK_GENERALIZE." (`account_id`,`type`) values(".$account_info['id'].",'".$cardtype."')";
                    $acc_db -> query($sql);
                }else{
					$sql = "update ".TBL_SK_GENERALIZE." set state = 0 where `account_id` = ".$account_info['id']."";
					$acc_db -> query($sql);
				}
            }
            if(empty($error)){
            	sys_msg($_LANG['input_success'],1,$links);
            }
            break;
        /* 万兽之王 */
        case "3":
            $state = 1 ;
            foreach($buffer as $key => $value){
                $sql = "select `id`,`name` from ".TBL_ACCOUNT." where `name` = '".$value."'";
                $account_info = $acc_db -> getRow($sql);
                if(!empty($account_info['id'])){
                    $sql = "select * from ".TBL_SK_GENERALIZE." where `account_id` = ".$account_info['id']." and `type` =".$cardtype." and `state` = ".$state."";
                    $generalize_info =$acc_db -> getRow($sql);
                }else{
                	$error[] = $value;
                	continue;
                }
                if(!$generalize_info['id']){
                    $sql="insert into ".TBL_SK_GENERALIZE." (account_id,state,type) values(".$account_info['id'].",".$state.",".$cardtype.")";
                    $acc_db -> query($sql);
                }
                $state++;
            }
            if(empty($error)){
            	sys_msg($_LANG['input_success'],1,$links);
            }
            break;
    }
    $smarty -> assign('error' ,$error);
    $smarty -> assign('display' ,1);
    $smarty -> display('res_act.htm');
}

/*台服版本生成卡片*/

if($_REQUEST['act'] == 'mkdkcard'){
    $arr_typer=array(
	1 => 'YJXS', //遊家新手卡
	2 => 'BHMT', //巴哈姆特禮卡
	3 => 'JDXS', //基地新手卡
	4 => 'YXXS', //遊戲庫新手卡
	5 => 'FSJM', //粉絲團見面卡
	6 => 'XSLL', //新手來禮卡
	7 => 'BHHK', //巴哈回馈卡
	8 => 'JDHK', //基地回饋卡
	9 => 'YJHK', //游家回饋卡
	10 => 'HXKP',//中信酷碰卡
	11 => 'VTYK',//VIP體驗卡
    );
    $arr_tname=array(
	1 => $_LANG['YJXS'],
	2 => $_LANG['BHMT'],
	3 => $_LANG['JDXS'], 
	4 => $_LANG['YXXS'],
	5 => $_LANG['FSJM'],
	6 => $_LANG['XSLL'], 
	7 => $_LANG['BHHK'],
	8 => $_LANG['JDHK'],
	9 => $_LANG['YJHK'],
	10 => $_LANG['HXKP'],
	11 => $_LANG['VTYK'],
    );
    $links[] = array('text' => $_LANG['back_mkdkcard'], 'href' => 'resource.php?act=mkdkcard');
    if($_POST[query]){
        $cardtype = $_POST['cardtype'];
        $card_amount = intval($_POST['card_amount']);
        $card_amount--;
        if($card_amount<0) sys_msg($_LANG['card_empty'],1,$links);  
        if($card_amount > 5000) sys_msg($_LANG['card_full'],1,$links);   
        $j = 0;
        $k = 100;
        switch($cardtype){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                $sql = "INSERT INTO ".TBL_SK_NEWCARD3."(`newcard`,`type`)VALUES";
                while($j <= $card_amount){
                    $card   = $arr_typer[$cardtype].'-'.random(8,1).'-'.randEn(4);
                    if($j != $k && $j < $card_amount){
                        $sql .="('$card',$cardtype),";
                    }else{
                        $sql .="('$card',$cardtype)";
                        $k = $k + 100;
                        $rs=$acc_db -> query($sql);
                        if(!$rs){
                    	    break;
                        }
                        $sql = "INSERT INTO ".TBL_SK_NEWCARD3."(`newcard`,`type`)VALUES";
                    }
                    $j++;
                }
                break;  
            case 10:
            case 11:
                $sql = "INSERT INTO ".TBL_SK_NEWCARD3."(`newcard`,`type`)VALUES";
                while($j <= $card_amount){
                    $card   = $arr_typer[$cardtype].'0'.random(8,1).'0'.randEn(4);
                    if($j != $k && $j < $card_amount){
                        $sql .="('$card',$cardtype),";
                    }else{
                        $sql .="('$card',$cardtype)";
                        $k = $k + 100;
                        $rs=$acc_db -> query($sql);
                        if(!$rs){
                    	    break;
                        }
                        $sql = "INSERT INTO ".TBL_SK_NEWCARD3."(`newcard`,`type`)VALUES";
                    }
                    $j++;
                }
                break;    	
        }
    	
    }

    if($j>$card_amount){
    	sys_msg($_LANG['success'],0,$links);
    }
//    $sql="SELECT * FROM ".TBL_SK_NEWCARD3." where picked<1 ORDER BY Id DESC";
//    $info= $acc_db -> getAll($sql);

    $smarty -> assign('info',$info);    
    $smarty -> assign('here',$_LANG['mkdk']);
    $smarty -> assign('cardname',$arr_tname);
    $smarty -> display('res_mkdkcard.htm');
}

/*台服版本导出卡片*/

if($_REQUEST['act'] == 'mkdkcard_output'){

	$links[] = array('text' => $_LANG['back_card'], 'href' => 'resource.php?act=card');
    $cardtype = $_REQUEST['cardtype'];
	$count = $_REQUEST['card_amount'];
    $filepath = ROOT_PATH.'data/upload/newcard/lstDumpEnd_TW'.$cardtype.'.log';
    if(file_exists($filepath)){
    $start = sreadfile($filepath);
    //$start = ceil($start/100)*100;
    }else{
        $start = 0;
    }
    $end = $start + $count;
    $where = "where type=$cardtype AND picked<1";
    $sql = "SELECT COUNT(*) FROM ".TBL_SK_NEWCARD3." $where ";
    $totalnum = $acc_db ->getOne($sql);
    $flg=$totalnum-$start;
    if($flg<=0||$totalnum<$end){
        jump("",$_LANG['out_false']."\\n".$_LANG['because']."({$start}-{$end})".$_LANG['allcard']."{$flg}".$_LANG['tip1'].$_LANG['liter']."{$count}!\\n".$_LANG['createnew']);
    }
    $sql = "SELECT * FROM ".TBL_SK_NEWCARD3." $where ORDER BY Id ASC LIMIT $start,$count  ";
    $result = $acc_db->getAll($sql);
	$cnt= date("Y-m-d H:i:s").">>>".$_SG['admname'].$_LANG['out']."{$count}".$_LANG['tip2']."{$totalnum}".$_LANG['tip3']."<{$start}-{$end}>. \r\n";
	$cnt .= "ID\t".$_LANG['card_id']."\r\n";
	foreach($result as $key => $value){
	    $cnt .= $value['Id']."\t";
		$cnt .= $value['picked']>0? $_LANG['card_used']:$value['newcard'];
		$cnt .= "\r\n";
	}
    //记录这次的最后一个区间
    swritefile($filepath,$end);
    //保存今日的导出日志
    $logfile = ROOT_PATH.'data/upload/newcard/dumplog_TW'.$cardtype.'_'.date("Ymd").".php";
    swritefile($logfile,"<?php exit()?>\n\n".$cnt."\n\n------------------------------------------\n\n",'a');
    header("Content-type: text/plain"); //纯文本格式
    header("Content-Disposition: attachment; filename=NewcardDump_TW".$cardtype."_".date("Ymd")."_[".($start+1)."-".$end."].txt");
    echo $cnt;
    exit();


}
if($_REQUEST['act'] == 'dianjuan'){
	admin_priv('dianjuan');
	$pay_type = $_POST['pay_type']; //补偿类型 通过角色还是账号
    $user_name = $_POST['platform_account']; //账号名或角色名
    $count = $_POST['zb_amount']; //点券数量
    $links[] = array('text' => $_LANG['back_dianjaun'], 'href' => 'resource.php?act=dianjuan');
    switch($pay_type){
        case "1":
            foreach($user_name as $key => $value){
        	    $sql = "select `id` from ".TBL_ACCOUNT." where `name` = '".$value."'";
            	$account_id = $acc_db -> getOne($sql);
            	if($account_id&&$count[$key]){
            	    $sql = "update ".TBL_SK_VAS_TABLE." set `VAS_Point_juan` = `VAS_Point_juan` + ".$count[$key]." where `Account_ID` = ".$account_id;
            	    $rs=$acc_db -> query($sql);
            	    if($rs){
            			$ok=1;
            		}else{
            			$ok=0;
            		}
            	}else{
            	    $error[] = $value;
            	}          		
            	$paynum++;
        	}
           	break;
        case "2":
            foreach($user_name as $key => $value){
        	    $sql = "select `account_id` from ".TBL_WB_USER." where `name` = '".$value."'";
            	$account_id = $wangpu_db -> getOne($sql);
            	if($account_id&&$count[$key]){
            	    $sql = "update ".TBL_SK_VAS_TABLE." set `VAS_Point_juan` = `VAS_Point_juan` + ".$count[$key]." where `Account_ID` = ".$account_id;
            		$rs=$acc_db -> query($sql);
            		if($rs){
            			$ok=1;
            		}else{
            			$ok=0;
            		}
            	}else{
            	    $error[] = $value;
            	} 
            		$paynum++;
        		}
           break;
    }
    
    if(count($error) > 0){
    	foreach($error as $key => $value){
    		$err_msg .= $value ."\n";
    	}
    	$msg = $_LANG['err_dianjian'] .$err_msg;
    	sys_msg($msg,1,$links,1);
    }
   
//    print_r($_POST);
    if($ok==1){
        sys_msg($_LANG['success'],0,$links);
    }
    $smarty -> assign('here',$_LANG['dianjuan']);
    $smarty -> display('res_dianjuan.htm');
}
?>
 
