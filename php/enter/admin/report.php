<?php
/**
 * @author King.L
 */

define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

date_default_timezone_set('PRC');
set_time_limit(0);
//在线人数
if($_REQUEST['act'] == 'online'){
	
    $date=$_POST['time'];
	if(empty($date)){
	    $date=date('Y-m-d');
	}else{
		$date=strtotime($date);
	    $date=date('Y-m-d',$date);
	}
    $sql="select time,sum(count_online) as sum  from ".TBL_ACC_LINELOG." group by time having(from_unixtime(`time`,'%Y-%m-%d')='$date') order by time desc ";
	$time=$acc_db->getAll($sql);
	//将时间戳转换
	for($i=0;$i<count($time);$i++){
	    $tmp=$time[$i]['time'];
    	$time[$i]['times']=date('Y-m-d H:i:s',$tmp);
	}
	
	$sql="select time,count_online,line from ".TBL_ACC_LINELOG." where from_unixtime(`time`,'%Y-%m-%d')='$date' order by time desc";
    $row=$acc_db->getAll($sql);
    //获取同一时间段六个区的在线人数
    foreach ($row as $key=>$value){
        $tmp=$value['time'];
    	$tmp=date('Y-m-d H:i:s',$tmp);
    	$line=$value['line'];
    	$info[$tmp][$line]=$value['count_online'];	
    }
    $smarty -> assign('here',$_LANG['online']);
    $smarty -> assign('line',$arr_sites);
	$smarty -> assign('time',$time);
	$smarty -> assign('info',$info);
    $smarty -> display('report_online.htm');
}

//各等级人数
if($_REQUEST['act'] == 'ratenum'){
	
    $sql="select userid from ".TBL_SK_PAYLOG." GROUP by userid";
	$row=$acc_db -> getAll($sql);
	
	foreach ($row as $key => $value){
	    $payid[]=$value['userid'];
	}
    if(is_array($payid)){
	    $where = implode(",",$payid);
	}
    $sql="select `level`,count(*) as count from ".TBL_WB_USER." group by `level` order by level ";
	$row=$wangpu_db -> getAll($sql);
    foreach ($row as $key => $value){
	    $level=$value['level'];
		$info[$level][all]=$value['count'];
		$info[$level][pt]=$value['count'];
		$info[$level][pay]=0;
	}
	if($where!=''){
	    $sql="select `level`,count(*) as count from ".TBL_WB_USER." where account_id in ($where) group by `level`";
	    $pay=$wangpu_db -> getAll($sql);
        foreach ($pay as $key => $value){
	        $level=$value['level'];
		    $info[$level]['pt']=$info[$level]['all']-$value['count'];
		    $info[$level]['pay']=$value['count'];
	    }
	}
    
	$allcount="[0";
	$paycount='[0';
	if(!empty($info)){
	    $key=array_keys($info);
	}
	
	for($i=1;$i<=100;$i++){
	    if(isset($info[$i]['pay'])){
		    $paycount.=','.$info[$i]['pay'];
		}else{
			$paycount.=',0';
		}
		if(isset($info[$i]['all'])){
			$allcount.=','.$info[$i]['all'];
		}else{
			$allcount.=',0';
		}
    }
    $allcount.=']';
    $paycount.=']';

	$smarty -> assign('all',$allcount);
	$smarty -> assign('pay',$paycount);
    $smarty -> assign('here',$_LANG['ratenum']);
	$smarty -> assign('info',$info);
	$smarty -> assign('row',$row);
    $smarty -> display('report_ratenum.htm');
    //$smarty -> display('test.htm');
}


//新增玩家
if($_REQUEST['act'] == 'new'){
	
    $date=$_POST['time'];
    if(empty($date)){
		$date=date('Y-m-d');
	}else{
	    $date=strtotime($date);
        $date=date('Y-m-d',$date);
	}
	$edate=date("Y-m-d",strtotime($date)+ONE_DAY);
	//$sql="select regdate,count(id) as count from ".TBL_ACCOUNT." group by regdate".$having;
	$sql="select count(id) as count from ".TBL_ACCOUNT." where FROM_UNIXTIME(regdate,'%Y-%m-%d')>= '$date'  and  FROM_UNIXTIME(regdate,'%Y-%m-%d')<='$edate'" ;
    $row=$acc_db -> getAll($sql);
	//print_r($row);

	//获取付费玩家
	$sql="select userid from ".TBL_SK_PAYLOG." where  FROM_UNIXTIME(logdate,'%Y-%m-%d')<'$date' GROUP by userid HAVING sum(paymoney)<30 ";
	$opay30=count($acc_db -> getAll($sql));
	$sql="select userid from ".TBL_SK_PAYLOG." where  FROM_UNIXTIME(logdate,'%Y-%m-%d')<'$edate' GROUP by userid HAVING sum(paymoney)<30 ";
	$npay30=count($acc_db -> getAll($sql));
	$pay30=$npay30-$opay30;
	
	$sql="select userid from ".TBL_SK_PAYLOG." where FROM_UNIXTIME(logdate,'%Y-%m-%d')<'$date'  GROUP by userid having  sum(paymoney)<=100 and sum(paymoney)>=30";
	$opay50=count($acc_db -> getAll($sql));
	$sql="select userid from ".TBL_SK_PAYLOG." where FROM_UNIXTIME(logdate,'%Y-%m-%d')<'$edate'  GROUP by userid having  sum(paymoney)<=100 and sum(paymoney)>=30";
	$npay50=count($acc_db -> getAll($sql));
	$pay50=$npay50-$opay50;
	
	$sql="select userid from ".TBL_SK_PAYLOG." where FROM_UNIXTIME(logdate,'%Y-%m-%d')<'$date'  GROUP by userid HAVING sum(paymoney)>100";
	$opay100=count($acc_db -> getAll($sql));
	$sql="select userid from ".TBL_SK_PAYLOG." where FROM_UNIXTIME(logdate,'%Y-%m-%d')<'$edate'  GROUP by userid HAVING sum(paymoney)>100";
	$npay100=count($acc_db -> getAll($sql));
	$pay100=$npay100-$opay100;
	
	$pay=$pay30+$pay50+$pay100;
	
    $smarty -> assign('here',$_LANG['new']);	
	$smarty -> assign('date',$date);
	$smarty -> assign('pay',$pay);
	$smarty -> assign('row',$row);
    $smarty -> display('report_new.htm');
}

//创建角色流失率统计
if($_REQUEST['act'] == 'create'){

    $stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	
	if(empty($stime)||empty($etime)){
	    $stime=strtotime(now)-6*ONE_DAY;
		$etime=strtotime(now);
	}else{
		$stime=strtotime($stime);
		$etime=strtotime($etime);
	}

	$time_lang=$etime-$stime;
	$res=getOneMonthTime($etime,$time_lang);
	$sql_1 = "select count(count1) from ".TBL_YJ_USERNUMLOG."";
	$result = $yjjhtp_db -> getAll($sql_1);
	

	//循环获取每天的人数
	for($i=0;$i<count($res);$i++){			
	    $etime=date("Y-m-d ",$res[$i]);
	    $btime4=date('ymd',$res[$i]).'0000';
	    $etime4=date('ymd',$res[$i]).'2359';
	    
	    $sql_2="select count(`id`) from ".TBL_YJ_USERNUMLOG." where  from_unixtime(time,'%Y-%m-%d')='$etime'";
	    $query=$yjjhtp_db ->getOne($sql_2);
		
	
	    if($query != 0){
	    	$sql_3="select count('id') from ".TBL_ACCOUNT." where from_unixtime(regdate,'%Y-%m-%d')='$etime'";
	        $sum1=$acc_db -> getOne($sql_3);
			
            
	        
	        $sql_4="select sum(count2) as sum2,sum(count3) as sum3 from ".TBL_YJ_USERNUMLOG." where from_unixtime(time,'%Y-%m-%d')='$etime'";
	        $rs=$yjjhtp_db ->getRow($sql_4);
	       
	        $sum2=$rs['sum2'];
	        $sum4=$rs['sum3'];
	        
	        
	        $sql_5 ="select count('id') from ".TBL_WB_USER." where Creat_Time>'$btime4' and Creat_Time<'$etime4' ";
	        $sum3=$wangpu_db -> getOne($sql_5);
	        
	        
	        $row[$etime][time]=$etime;
		    $row[$etime][sum1]=intval($sum1);
		    $row[$etime][sum2]=intval($sum2);
		    $row[$etime][sum3]=intval($sum3);
		    $row[$etime][sum4]=intval($sum4);
		    if($sum1!=0){
		        $row[$etime][regloss]=number_format(($sum1-$sum2)/$sum1*100,2);
		        $row[$etime][allloss]=number_format(($sum1-$sum4)/$sum1*100,2);
		    }
		    if($sum2!=0){
		       $row[$etime][createloss]=number_format(($sum2-$sum4)/$sum2*100,2);
		    }
		   if(empty($row[$etime][regloss])){
		       $row[$etime][regloss]='--';
		   }
           if(empty($row[$etime][createloss])){
		       $row[$etime][createloss]='--';
		   }
           if(empty($row[$etime][allloss])){
		       $row[$etime][allloss]='--';
		   }
         
	   }
	}
    $smarty -> assign('here',$_LANG['create']);
    $smarty -> assign('row',$row);
    $smarty -> display('report_create.htm');
}

//当天创建流失率
if($_REQUEST['act'] == 'daytj'){
    $date = $_GET ['date'];
    $sql = "select * from ".TBL_YJ_USERNUMLOG." where from_unixtime(time,'%Y-%m-%d')='$date'";
    $rs = $yjjhtp_db->getAll ( $sql );
    if(!empty($rs)){
        foreach ( $rs as $k => $v ) {
            $row [$k] [time] = date ( 'Y-m-d H:i:s', $v [time] + 8 * 3600 );
	        $row [$k] [sum1] = intval ( $v [count1] );
	        $row [$k] [sum2] = intval ( $v [count2] );
	        $row [$k] [sum3] = intval ( $v [count3] );
	        $row [$k] [sum4] = intval ( $v [count4] );

	        if ($row [$k][sum1] != 0 ) {
	            $row [$k][regloss] = number_format ( ($row [$k] [sum1] -$row [$k] [sum2]) / $row [$k] [sum1] * 100, 2 );
		        $row [$k][allloss] = number_format ( ($row [$k] [sum1] - $row [$k] [sum4]) / $row [$k] [sum1] * 100, 2 );
	        }
	        if ($row[$k][sum2] != 0){
	            $row [$k][createloss] = number_format ( ($row [$k] [sum2] -$row [$k] [sum4]) / $row [$k] [sum2] * 100, 2 );
	        }
        }
    }

    $smarty -> assign('here',$_LANG['create']);
    $smarty -> assign('action_link', array('href'=>'report.php?act=create', 'text' => $_LANG['daytj_back']));
    $smarty  -> assign('here',$_LANG['create']);
    $smarty -> assign ( 'row', $row );
    $smarty -> display ( 'report_daytj.htm' );
	
}


//流失率统计
if($_REQUEST['act'] == 'lost'){
	
	$tm2 =ONE_DAY;
    $tm4 =ONE_DAY*2;
    $nowtime=time();
    $isloss=0;
    $mayloss=0;
    $noloss=0;
	$sql = "SELECT Exit_Time,level FROM ".TBL_WB_USER." order by level";
	$row=$wangpu_db -> getAll($sql);
	foreach($row as $key => $value){
	   	$level=$value['level'];
		$exittime=$value['Exit_Time'];
		if(empty($info[$level]['isloss'])){
			$info[$level]['isloss']=0;
		}
	    if(empty($info[$level]['noloss'])){
			$info[$level]['noloss']=0;
		}
	   if(empty($info[$level]['mayloss'])){
			$info[$level]['mayloss']=0;
	   }
	   if($exittime!='0'){
		    $lasttime = preg_replace('/([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})/','\\1-\\2-\\3 \\4:\\5:00','20'.$exittime);
		    $lasttime = strtotime($lasttime);
	    }else{
		    $lasttime=$nowtime;
	    }
        $lastactive = $nowtime-$lasttime;

	    if($lastactive > $tm4){
            $info[$level]['isloss']++;
            $isloss++;
        }elseif($lastactive > $tm2){
            $info[$level]['mayloss']++;
            $mayloss++;
        }else{
            $info[$level]['noloss']++;
            $noloss++;
        }  
    } 
    $x='[';
    $yisloss='[0,';
    $ymayloss='[0,';
    $ynoloss='[0,';
    $ychurn='[0,';
    for($i=1;$i<=100;$i++){
    	if(!empty($info[$i])){
 	    $loss=$info[$i]['isloss']+$info[$i]['mayloss'];
   	    $all=$info[$i]['isloss']+$info[$i]['mayloss']+$info[$i]['noloss'];
   	    $info[$i]['churn']=number_format($loss/$all*100,2);	
   	    $yisloss.=$info[$i]['isloss'].',';
   	    $ymayloss.=$info[$i]['mayloss'].',';
   	    $ynoloss.=$info[$i]['noloss'].',';
   	    $ychurn.=$info[$i]['churn'].',';	
    	}else{
    	$yisloss.='0'.',';
   	    $ymayloss.='0'.',';
   	    $ynoloss.='0'.',';
   	    $ychurn.='0'.',';
    	}
    }
    
    $x = substr($x,0,strlen($x)-1); 
    $x.=']';
    $yisloss=substr($yisloss,0,strlen($yisloss)-1);
    $yisloss.=']';
    $ymayloss=substr($ymayloss,0,strlen($ymayloss)-1);
    $ymayloss.=']';
    $ynoloss=substr($ynoloss,0,strlen($ynoloss)-1);
    $ynoloss.=']';
    $ychurn=substr($ychurn,0,strlen($ychurn)-1);
    $ychurn.=']';
   
    $smarty -> assign('here',$_LANG['lost']);	    
    $smarty -> assign('ychurn',$ychurn);
    $smarty -> assign('ynoloss',$ynoloss);
    $smarty -> assign('ymayloss',$ymayloss);
    $smarty -> assign('yisloss',$yisloss);
    $smarty -> assign('x',$x);
	$smarty -> assign('noloss',$noloss);
	$smarty -> assign('mayloss',$mayloss);
	$smarty -> assign('isloss',$isloss);
	$smarty -> assign('info',$info);
    $smarty -> display('report_lost.htm');
    //$smarty -> display('test.htm');
}

if($_REQUEST['act'] == 'daysell'){
	
    $date=$_REQUEST['time'];
	
	if(empty($date))
	{
		$date=date("Y-m-d");
	}
	else
	{
		$date=date("Y-m-d",strtotime($date));
    }
	
	$sql="select ItemType_ID,sum(ItemType_Amount) as count,sum(Spend_VAS_Point) as value from ".TBL_SER_CONSUMELOG_TABLE.
	" where writeTime LIKE '$date%' group by ItemType_ID order by value desc";
	
	$rs=$acc_db ->getAll($sql);
	if(!empty($rs)){
		foreach($rs as $key => $value){
			$id=$value[ItemType_ID];
			$info[$id][count]=$value['count'];
			$info[$id]['yb']=intval($value['value']);
			$info[$id]['rmb']=$value['value']/10;
			$sum+=$value['value'];
		}
		$itemid=array_keys($info);
		$where= implode(",",$itemid);
	}
	if(!empty($where)){
	    $sql="select id,name from ".TBL_WB_ITEMTYPE."    where id in ($where)";
	    $row=$wangpu_db ->getAll($sql);
	    foreach ($row as $k=> $v){
	    	$id=$v['id'];
	    	$info[$id]['name']=$v['name'];
	    	//$info[$id]['price']=intval($v['price']);    	
	    }
	    $sql="select itemtype,price from ".TBL_WB_GOOD_VAS."  where itemtype in ($where)";
	    $row=$wangpu_db ->getAll($sql);
	    foreach ($row as $k=> $v){
	    	$id=$v['itemtype'];
	    	$info[$id]['price']=intval($v['price']);    	
	    }
	}
	
	if($sum){
		$sumrmb=$sum/10;
	}else{
		$sum=0;
		$sumrmb=0;
	}

	
	$smarty -> assign('sum',$sum);
	$smarty -> assign('sumrmb',$sumrmb);
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['daysell']);
    $smarty -> display('report_daysell.htm');
	
}

if($_REQUEST['act'] == 'allsell'){
	
	$sql="select ItemType_ID,sum(ItemType_Amount) as count,sum(Spend_VAS_Point) as value from ".TBL_SER_CONSUMELOG_TABLE. "  group by ItemType_ID order by value desc";
	
	$rs=$acc_db ->getAll($sql);
	if(!empty($rs)){
		foreach($rs as $key => $value){
			$id=$value[ItemType_ID];
			$info[$id][count]=$value['count'];
			$info[$id]['yb']=intval($value['value']);
			$info[$id]['rmb']=$value['value']/10;
			$sum+=$value['value'];
		}
		$itemid=array_keys($info);
		$where= implode(",",$itemid);
	}
	if(!empty($where)){
	    $sql="select id,name from ".TBL_WB_ITEMTYPE."  where id in ($where)";
	    $row=$wangpu_db ->getAll($sql);
	    foreach ($row as $k=> $v){
	    	$id=$v['id'];
	    	$info[$id]['name']=$v['name'];
	    	//$info[$id]['price']=intval($v['price']);    	
	    }
	    $sql="select itemtype,price from ".TBL_WB_GOOD_VAS."  where itemtype in ($where)";
	    $row=$wangpu_db ->getAll($sql);
	    foreach ($row as $k=> $v){
	    	$id=$v['itemtype'];
	    	$info[$id]['price']=intval($v['price']);    	
	    }
	}
	
	if($sum){
		$sumrmb=$sum/10;
	}else{
		$sum=0;
		$sumrmb=0;
	}
	
	$smarty -> assign('sum',$sum);
	$smarty -> assign('sumrmb',$sumrmb);
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['sell_all']);
    $smarty -> display('report_allsell.htm');
	
	
}

//全平台充值统计
if($_REQUEST['act'] == 'all_ptcz'){
	admin_priv(report_all_ptcz);
    //require("./includes/all_server_num.php");
    set_time_limit(0);
    $geturl = $_CFG['apiurl']."getserverconfig.php"; 
    $serverconfig = gethttpcnt($geturl);
	$serverconfig = unserialize($serverconfig);
	$stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
        $stime=date('Y-m-01', strtotime(now)); 
	    $stime=strtotime($stime);
		$etime=strtotime('now');
	}else{
	    $stime=strtotime($stime);
		$etime=strtotime($etime);
	}
	$time_lang=$etime-$stime;
	$res=getOneMonthTime($etime,$time_lang);
	foreach ($res as $key => $value){
		if(date('Y-m-d',$value)>date('Y-m-d')) break;
		$date[]=date('Y-m-d',$value);
	}

	//记录查询日期
    if(!empty($date)){
        $date=array_reverse($date); 
    }
    foreach ($serverconfig['arr_s'] as $key => $value){
    	$i=0;
    	while($i<=$value){
    	    if($i<=$serverconfig['hefu'][$key]&&$i%2==0){
				$i++;
				continue; 
			}
			if(empty($serverconfig['prevpages'][$key])){
			    $i++;
				continue;
			}
		    $str = "http://".$serverconfig['prevpages'][$key]['prev'].$i.$serverconfig['prevpages'][$key]['last']."/interface/cztj.php?bdate=$stime&edate=$etime ";
			$url[$key][$i] = $str;
			$i++;	
    	}
    }
    foreach ($url as $pt => $server){
    	foreach ($server as $k =>$v){
    		$result = gethttpcnt($v);
		    $data[$pt][$k] = unserialize($result);
    	}
    }
    if(!empty($date)){
        foreach ($data as $pt => $server){
    	    foreach ($server as $k =>$v){
    		    if(!empty($v)){
    			    foreach ($v as $day => $rmb){
    			        $info[$pt][$day]+=$rmb;
    		        }
    		    }else{
    		    	$err_data[$pt][]=$k;
    		    }
    	    }
        }
    }
    if(!empty($info)){
    	foreach ($info as $pt => $cz){
    		$sum[$pt]=array_sum($cz);
    	    foreach ($cz as $day => $rmb){
			    $sum_day[$day]+=$rmb;
			}
    	}
    }
	if(!empty($sum_day)){
		$sum_all=array_sum($sum_day);
	}
    
	$smarty -> assign('date',$date);
	$smarty -> assign('here',$_LANG['all_ptcz']);
	$smarty -> assign('err' , $err_data);
	$smarty -> assign('info' ,$info);
	$smarty -> assign('sum' ,$sum);
	$smarty -> assign('sum_all' ,$sum_all);
	$smarty -> assign('sum_day' ,$sum_day);
	$smarty -> display('report_all_ptcz.htm');
    
}

if($_REQUEST['act'] == 'czall'){
	admin_priv('report_czall');
    //require("./includes/all_server_num.php");
	set_time_limit(0);	
	$geturl = $_CFG['apiurl']."getserverconfig.php"; 
    $serverconfig = gethttpcnt($geturl);
	$serverconfig = unserialize($serverconfig); 
	if(empty($serverconfig)){
		require("./includes/all_server_num.php");
		$serverconfig = array(
		'all_server_name' => $all_server_name,
		'pages' => $pages,
		'prevpages' => $prevpages,
		'arr_s' => $arr_s,
		'hefu' => $hefu,  
	    );
	}  
	$stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
        $stime=date('Y-m-01', strtotime(now)); 
	    $stime=strtotime($stime);
		$etime=strtotime(now);
	}else{
	    $stime=strtotime($stime);
		$etime=strtotime($etime);
	}
	$time_lang=$etime-$stime;
	$res=getOneMonthTime($etime,$time_lang);
	foreach ($res as $key => $value){
		if(date('Y-m-d',$value)>date('Y-m-d')) break;
		$date[]=date('Y-m-d',$value);
	}
	//记录查询日期
    if(!empty($date)){
        $date=array_reverse($date); 
    }
    
	if (strpos(',' . $_SESSION['action_list'] . ',', ',view_all_server,') === false){
		//$_CFG['s_name']='4399';
	    $server_num = $serverconfig['arr_s'][$_CFG['s_name']];
		$i = 1;	
		//获取调用地址
		while($i <= $server_num){
			if($i<=$serverconfig['hefu'][$_CFG['s_name']]&&$i%2==0){
				$i++;
				continue;
			}
		    $str = "http://".$serverconfig['prevpages'][$_CFG['s_name']]['prev'].$i.$serverconfig['prevpages'][$_CFG['s_name']]['last']."/interface/cztj.php?sid=$i&bdate=$stime&edate=$etime ";
			$url[$i] = $str;
			$i++;
	    }
	    //调用各服获取充值信息的文件
		if(!empty($url)){
		    foreach($url as $key => $value){
			    $result = gethttpcnt($value);
			    if(!empty($result)){
				    $data[$key] = unserialize($result);
			    }
		    }
		    //获取调用不成功的服及求和
		    foreach ($data as $k => $v){
		        if(!$v){
			        $err_data[] = $k;
			    }else{
			    	$sum[$k]=array_sum($v);
			    	foreach ($v as $day => $rmb){
			    		$sum_day[$day]+=$rmb;
			    	}
			    }
		   }
		   	
		}
    }else{
	}
	if(!empty($sum_day)){
		$sum_all=array_sum($sum_day);
	}

	$smarty -> assign('date',$date);
	$smarty -> assign('here',$_LANG['czall']);
	$smarty -> assign('err' , $err_data);
	$smarty -> assign('data' ,$data);
	$smarty -> assign('sum' ,$sum);
	$smarty -> assign('sum_all' ,$sum_all);
	$smarty -> assign('sum_day' ,$sum_day);
	$smarty -> display('report_czall.htm');
	
	
}

if($_REQUEST['act'] == 'czday'){
	admin_priv('report_czday');
    $stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
		$etime=date('Y-m-d 0:0:0',time());
		$etime=strtotime($etime);
		$stime=$etime-30*ONE_DAY;
	}else{
		$stime=strtotime($stime);
		$etime=strtotime($etime);
	}
	$time_lang=$etime-$stime;
	$res=getOneMonthTime($etime,$time_lang);
	$Iparray = explode("|", $_CFG['allowIpArr']);
	foreach ($Iparray as $key => $value){
		$value=trim($value);
		if($key<count($Iparray)-1){
			$ip.=" ip='$value' or";
		}else{
			$ip.=" ip='$value' ";
		}	
	}
	//循环获取每天的充值
	for($i=0;$i<count($res);$i++){
	    $date=$res[$i];
	    $sql="select sum(paymoney) as summoney from ".TBL_SK_PAYLOG." where paygold=0 and userid>1 and `logdate`>$date and `logdate`<$date+86400 and ($ip)";
	    $mpay=$acc_db -> getOne($sql);
	    $mpay=$mpay/2;//获取手机充值的一半
	    
        $sql="select count(distinct userid) as countuser,count(logid) as count,sum(paymoney) as summoney,sum(paygold) as sumgold  from   ".TBL_SK_PAYLOG.
        "  where userid>1 and `logdate`>$date and `logdate`<$date+86400 and ($ip) order by userid desc";      
        $row=$acc_db -> getRow($sql);
        $time=date('Y-m-d',$res[$i]);;
        if(!empty($row['summoney'])){
        	$info[$time]=$row;
        	$info[$time]['summoney']=$row['summoney']-$mpay; 
            $info[$time]['arpu']= round(($row['summoney']-$mpay) / $row['countuser'],2);  
        }
        
	}	
    if(!empty($info)){
    	$info=array_reverse($info); 
    }
	
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['czday']);
    $smarty -> display('report_czday.htm');	
}

if($_REQUEST['act'] == 'czmonth'){
	admin_priv('report_allconsume');
    $stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
		$stime=date('Y-m-01', strtotime(now)); 
	    $stime=strtotime($stime);
		$etime=strtotime(now)+ONE_DAY;
	}else{
		$stime=strtotime($stime);
		$etime=strtotime($etime)+ONE_DAY;
	}
	$Iparray = explode("|", $_CFG['allowIpArr']);
	
	foreach ($Iparray as $key => $value){
		$value=trim($value);
		if($key<count($Iparray)-1){
			$ip.=" ip='$value' or";
		}else{
			$ip.=" ip='$value' ";
		}	
	}
	
    $sql="select FROM_UNIXTIME(logdate ,'%Y-%m' )as month, count(distinct userid) as countuser,count(logid) as count,sum(paymoney) as summoney,
    sum(paygold) as sumgold  from   ".TBL_SK_PAYLOG."  where userid>1 and logdate>'$stime' and logdate<'$etime' and ($ip) 
    GROUP BY  FROM_UNIXTIME( logdate, '%Y-%m' ) ";
    $row=$acc_db -> getAll($sql);
    if(!empty($row)){
        foreach ($row as $key => $value){
            $month=$value['month'];
            $sql="select sum(paymoney) from  ".TBL_SK_PAYLOG." 
            where userid>1 and FROM_UNIXTIME(logdate ,'%Y-%m' )='$month' and paygold=0 and logdate>'$stime' and logdate<'$etime' and ($ip)";
            $mpay=$acc_db -> getOne($sql);
            $mpay=$mpay/2;//获取手机充值的一半
            
            $res[$month]=$value;
            $res[$month]['summoney']= $value['summoney']-$mpay;
            $res[$month]['arpu']= round(($value['summoney']-$mpay) / $value['countuser'],2);  
        }
    	
    }
    if(!empty($res)){
    	$res=array_reverse($res); 
    }
    
	$smarty -> assign('info',$res);
	$smarty -> assign('here',$_LANG['czmonth']);
    $smarty -> display('report_czmonth.htm');	
}


//全平台消费统计
if($_REQUEST['act'] == 'all_ptxf'){
	admin_priv('report_all_ptxf');
	//require("./includes/all_server_num.php");
	set_time_limit(0);	
	$geturl = $_CFG['apiurl']."getserverconfig.php"; 
    $serverconfig = gethttpcnt($geturl);
	$serverconfig = unserialize($serverconfig); 
	 
	$stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
        $stime=date('Y-m-01', strtotime(now)); 
	    $stime=strtotime($stime);
		$etime=strtotime('now');
	}else{
	    $stime=strtotime($stime);
		$etime=strtotime($etime);
	}
	$time_lang=$etime-$stime;
	$res=getOneMonthTime($etime,$time_lang);
	foreach ($res as $key => $value){
		if(date('Y-m-d',$value)>date('Y-m-d')) break;
		$date[]=date('Y-m-d',$value);
	}

	//记录查询日期
    if(!empty($date)){
        $date=array_reverse($date); 
    }
    foreach ($serverconfig['arr_s'] as $key => $value){
    	$i=0;
    	while($i<=$value){
    	    if($i<=$serverconfig['hefu'][$key]&&$i%2==0){
				$i++;
				continue;
			}
			if(empty($serverconfig['prevpages'][$key])){
			    $i++;
				continue;
			}
		    $str = "http://".$serverconfig['prevpages'][$key]['prev'].$i.$serverconfig['prevpages'][$key]['last']."/interface/consumetj.php?bdate=$stime&edate=$etime ";
			$url[$key][$i] = $str;
			$i++;	
    	}
    }
    foreach ($url as $pt => $server){
    	foreach ($server as $k =>$v){
    		$result = gethttpcnt($v);
		    $data[$pt][$k] = unserialize($result);
    	}
    }
    if(!empty($data)){
        foreach ($data as $pt => $server){
    	    foreach ($server as $k =>$v){
    		    if(!empty($v)){
    			    foreach ($v as $day => $rmb){
    			        $info[$pt][$day]+=$rmb;
    		        }
    		    }else{
    		    	$err_data[$pt][]=$k;
    		    }
    	    }
        }
    }
    if(!empty($info)){
    	foreach ($info as $pt => $cz){
    		$sum[$pt]=array_sum($cz);
    	    foreach ($cz as $day => $rmb){
			    $sum_day[$day]+=$rmb;
			}
    	}
    }
	if(!empty($sum_day)){
		$sum_all=array_sum($sum_day);
	}
    
	$smarty -> assign('date',$date);
	$smarty -> assign('here',$_LANG['all_ptxf']);
	$smarty -> assign('err' , $err_data);
	$smarty -> assign('info' ,$info);
	$smarty -> assign('sum' ,$sum);
	$smarty -> assign('sum_all' ,$sum_all);
	$smarty -> assign('sum_day' ,$sum_day);
	$smarty -> display('report_all_ptxf.htm');  
    
	
}

if($_REQUEST['act'] == 'allconsume'){
	admin_priv('report_allconsume');
	//require("./includes/all_server_num.php");
	set_time_limit(0);	
	$geturl = $_CFG['apiurl']."getserverconfig.php"; 
    $serverconfig = gethttpcnt($geturl);
	$serverconfig = unserialize($serverconfig);
	 	    
	$stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
        $stime=date('Y-m-01', strtotime(now)); 
	    $stime=strtotime($stime);
		$etime=strtotime(now);
	}else{
	    $stime=strtotime($stime);
		$etime=strtotime($etime);
	}
	$time_lang=$etime-$stime;
	$res=getOneMonthTime($etime,$time_lang);
	foreach ($res as $key => $value){
		if(date('Y-m-d',$value)>date('Y-m-d')) break;
		$date[]=date('Y-m-d',$value);
	}
	//记录查询日期
    if(!empty($date)){
        $date=array_reverse($date); 
    }
    
	if (strpos(',' . $_SESSION['action_list'] . ',', ',view_all_server,') === false){
		//$_CFG['s_name']='4399';
	    $server_num = $serverconfig['arr_s'][$_CFG['s_name']];
		$i = 1;	
		//获取调用地址
		while($i <= $server_num){
			if($i<=$serverconfig['hefu'][$_CFG['s_name']]&&$i%2==0){
				$i++;
				continue;
			}
		    $str = "http://".$serverconfig['prevpages'][$_CFG['s_name']]['prev'].$i.$serverconfig['prevpages'][$_CFG['s_name']]['last']."/interface/consumetj.php?sid=$i&bdate=$stime&edate=$etime ";
			$url[$i] = $str;
			$i++;
	    }
	    //调用各服获取充值信息的文件
		if(!empty($url)){
		    foreach($url as $key => $value){
			    $result = gethttpcnt($value);
			    if(!empty($result)){
				    $data[$key] = unserialize($result);
			    }
		    }
		    //获取调用不成功的服及求和
		    foreach ($data as $k => $v){
		        if(!$v){
			        $err_data[] = $k;
			    }else{
			    	$sum[$k]=array_sum($v);
			    	foreach ($v as $day => $rmb){
			    		$sum_day[$day]+=$rmb;
			    	}
			    }
		   }
		   	
		}
    }else{
	}
	if(!empty($sum_day)){
		$sum_all=array_sum($sum_day);
	}

	$smarty -> assign('date',$date);
	$smarty -> assign('here',$_LANG['all_consume']);
	$smarty -> assign('err' , $err_data);
	$smarty -> assign('data' ,$data);
	$smarty -> assign('sum' ,$sum);
	$smarty -> assign('sum_all' ,$sum_all);
	$smarty -> assign('sum_day' ,$sum_day);
	$smarty -> display('report_allconsume.htm');

}

if($_REQUEST['act'] == 'dayconsume'){
	admin_priv('report_dayconsume');
	$stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
		$etime=date('Y-m-d 0:0:0',time());
		$etime=strtotime($etime);
		$stime=$etime-30*ONE_DAY;
	}else{
		$stime=strtotime($stime);
		$etime=strtotime($etime);
	}
	$time_lang=$etime-$stime;
	$res=getOneMonthTime($etime,$time_lang);
	//循环获取每天的消费
	for($i=0;$i<count($res);$i++){
	    $date=date('Y-m-d',$res[$i]);
	    $edate=date('Y-m-d',$res[$i]+ONE_DAY);
        $sql="select count(ID) as count_uselog,sum(Spend_VAS_Point) as sum_uselog,ROUND(max(Spend_VAS_Point),0) as max_uselog,ROUND(min(Spend_VAS_Point),0) as min_uselog
        ,ROUND(sum(Spend_VAS_Point)/count(ID),0) as avg_uselog from ".TBL_SER_CONSUMELOG_TABLE. "  where writeTime>'$date' and writeTime<'$edate' ";
        $row=$acc_db -> getRow($sql);
        $time=date('Y-m-d',$res[$i]);
        $info[$time]=$row;   
	}	

    if(!empty($info)){
    	$info=array_reverse($info); 
    }
	$smarty -> assign('info',$info);
	$smarty -> assign('here',$_LANG['day_consume']);
    $smarty -> display('report_dayconsume.htm');	
	
}


if($_REQUEST['act'] == 'monthconsume'){
	admin_priv('report_monthconsume');
	$stime=trim($_POST['start_time']);
	$etime=trim($_POST['end_time']);
	if(empty($stime)||empty($etime)){
		$stime=date('Y-m-01', strtotime(now)); 
	    $stime=strtotime($stime);
		$etime=strtotime(now)+ONE_DAY;
	}else{
		$stime=strtotime($stime);
		$etime=strtotime($etime)+ONE_DAY;
	}

	$date=date('Y-m-d',$stime);
	$edate=date('Y-m-d',$etime);
	    
    $sql="select DATE_FORMAT( writeTime, '%Y-%m' ) AS month,count(ID) as count_uselog,sum(Spend_VAS_Point) as sum_uselog,ROUND(max(Spend_VAS_Point),0) as max_uselog,
    ROUND(min(Spend_VAS_Point),0) as min_uselog,ROUND(sum(Spend_VAS_Point)/count(ID),0) as avg_uselog from ".TBL_SER_CONSUMELOG_TABLE. "  where writeTime>'$date' and writeTime<'$edate' 
    GROUP BY DATE_FORMAT( writeTime, '%Y-%m') ";
        
    $row=$acc_db -> getAll($sql);

	$smarty -> assign('info',$row);
	$smarty -> assign('here',$_LANG['month_consume']);
    $smarty -> display('report_monthconsume.htm');	
	
}



?>