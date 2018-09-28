<?php
/**
 * 
 * @author King.L
 */
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');
date_default_timezone_set('PRC');
   


if($_REQUEST['act'] == 'list'){
	if($_POST[query]){
		$username=trim($_POST['username']);
        $rolename=trim($_POST['rolename']);
        $startlevel=trim($_POST['startlevel']);
        $endlevel=trim($_POST['endlevel']);
        $startdate=trim($_POST['startdate']);
        $enddate=trim($_POST['enddate']);
        $regip=trim($_POST['regip']);
        $info=$_POST;
	}else{
	    $username=trim($_REQUEST['username']);
        $rolename=trim($_REQUEST['rolename']);
        $startlevel=trim($_REQUEST['startlevel']);
        $endlevel=trim($_REQUEST['endlevel']);
        $startdate=trim($_REQUEST['startdate']);
        $enddate=trim($_REQUEST['enddate']);
        $regip=trim($_REQUEST['regip']);
        $info=$_GET;
	}
    $where=" where 1";
    
    //获取用户信息
    if(!empty($username)){
        $where.= " and `name` LIKE '%".$username."%'"; 
    } 
    if(!empty($regip)){
    	$where.= " and netbar_ip like '%".str_replace(' ','%',$regip)."%' ";
    }    
    if(!empty($startdate)){
    	$startdate1=strtotime($startdate);
    	$where .= " and regdate >= '$startdate1' ";
    }   
    if(!empty($enddate)){
       $enddate1=strtotime($enddate);
	   $where .= " and regdate <= '$enddate1' ";
    }  

    //角色信息
    $where_gamedb = " ";
    if($rolename){
        $where_gamedb .= "and `name` LIKE '%$rolename%' "; 
    } 
    if($startlevel > 0){
    	$where_gamedb .= " and level >= '$startlevel' ";
    }
    if($endlevel > 0){
    	$where_gamedb .= " and level <= '$endlevel' ";
    }
    //获取结果总数
    if($where_gamedb!=' '){
    	$sql="select count(*) from ".TBL_WB_USER."  where 1 $where_gamedb ";
  	    $count=$GLOBALS['wangpu_db']-> getOne($sql);
  	    
    }else{
        $sql="select count(id) from ".TBL_ACCOUNT.$where;
        $count =$GLOBALS['acc_db'] -> getOne($sql);
    }
    $post="username=$username&rolename=$rolename&startdate=$startdate&enddate=$enddate&startlevel=$startlevel&endlevel=$endlevel&regip=$regip"; 
    
    $page=$_REQUEST['page']?$_REQUEST['page']:1;
    $start=($page-1)*30;
    $num=$count%30==0?intval($count/30):(intval($count/30)+1);
    for($i=0;$i<10;$i++){
    	if(($page+$i)>$num) break;
    	$pagenum[]=$page+$i;
    }
    
    if($where_gamedb!=' '){
        $sql="select id,account_id,name,level from ".TBL_WB_USER." where 1 $where_gamedb order by account_id desc limit $start,30";
  	    $res=$GLOBALS['wangpu_db']-> getAll($sql);

        if(!empty($res)){
  	        foreach ($res as $key => $value){
  	            $accountid=$value['account_id'];
  	            $sql = "select id,name,netbar_ip,regdate from ".TBL_ACCOUNT."$where and id ='$accountid' ";
  	            $rs1=$GLOBALS['acc_db']->getRow($sql);
  		        if(!empty($rs1)){
 		            $rs[$key]['id']=$rs1[id];
		            $rs[$key]['name']=$rs1[name];
		            $rs[$key]['netbar_ip']=$rs1[netbar_ip];
	                $rs[$key]['regdate']=date('Y-m-d H:i:s',$rs1['regdate']);
	                $rs[$key]['role']=$value['name'];  
	                $rs[$key]['level']=$value['level'];
	                $rs[$key]['roleid']=$value['id'];
                 }
            }
        }
    }else{
        $sql="select id,name,netbar_ip,regdate from ".TBL_ACCOUNT.$where." order by id desc limit $start,30";
        $rs1=$GLOBALS['acc_db']->getAll($sql);

        if(!empty($rs1)){
           foreach ($rs1 as $key => $value){
  	            $id=$value['id'];
                $sql="select id,name,level from ".TBL_WB_USER." where account_id ='$id' $where_gamedb";
	            $res=$GLOBALS['wangpu_db']-> getRow($sql);
	            if(!empty($res)){
		            $rs[$key]['id']=$value[id];
		            $rs[$key]['name']=$value[name];
		            $rs[$key]['netbar_ip']=$value[netbar_ip];
	                $rs[$key]['regdate']=date('Y-m-d H:i:s',$value['regdate']);
	                $rs[$key]['role']=$res['name'];  
	                $rs[$key]['level']=$res['level'];
	                $rs[$key]['roleid']=$res['id'];
	            }
	        }
        	
        }

    }
    
    
    
    $smarty -> assign('post',$post);
    $smarty -> assign('pagenum1',$pagenum);
    $smarty -> assign('url', $url);
    $smarty -> assign('page',$page);
    $smarty -> assign('lastpage',$page-1);
    $smarty -> assign('nextpage',$page+1);
    $smarty -> assign('num',$num);
    $smarty-> assign('info',$info);
    $smarty -> assign('here',$_LANG['list']);
	$smarty-> assign('rs',$rs);
    $smarty -> display('player_list.htm');
}
//玩家充值排行
if($_REQUEST['act'] == 'ranking'){

    $sql="select `userid` from `".TBL_SK_PAYLOG."` group by `userid`";
    $total=$acc_db->getRow($sql);
    $allowIpArr = $_CFG['allowIpArr'];
    $allowIpArr = explode('|' , $allowIpArr);
    $ip_count = count($allowIpArr) - 1;
 	if($allowIpArr[0] !=''){
 	    foreach($allowIpArr as $key => $value){
	        $value=trim($value);
	        if($ip_count != 0){
	            $where .= " ip = '$value' or ";
	        }else{
	            $where .= " ip = '$value' ";
	        }
	        $ip_count --;
	    }
	}
	if($where){
		$sql="select sum(a.paymoney) as pay_rmb,sum(a.paygold) as pay_yb,b.name,a.userid from ".TBL_SK_PAYLOG." as a , ".TBL_ACCOUNT." as b   WHERE a.userid=b.id and (".$where." ) group by a.userid ORDER by pay_rmb desc";		
	}else{
		$sql = "select sum(a.paymoney) as pay_rmb,sum(a.paygold) as pay_yb,b.name,a.userid from ".TBL_SK_PAYLOG." as a , ".TBL_ACCOUNT." as b   WHERE a.userid=b.id  group by a.userid ORDER by pay_rmb desc";
	}
    
    $rs = $acc_db->getAll($sql);
    foreach($rs as $k => $v){
        $info[$v['userid']]=$v;
    }
    //获取角色信息
    if($info){
        $where = implode(",",array_keys($info));
        $sql = "select level,name,account_id from ".TBL_WB_USER." where account_id in ($where)";
        $rs = $wangpu_db -> getAll($sql);
    }
    //以用户id为键值存储角色
    foreach($rs as $k1 => $v1){

        $account_id=$v1['account_id'];
        $level=$v1['level'];
        $info[$account_id]['level']=$level;
        $info[$account_id]['rolename']=$v1['name'];
    }
	


    
    $smarty -> assign('here',$_LANG['ranking']);
    $smarty -> assign('full_page' , 1);
    $smarty -> assign('info' , $info);
    $smarty -> display('player_ranking.htm');
}

//玩家装备查询
if($_REQUEST['act'] == 'equire'){
	
    $username=trim($_POST['username']);
    $rolename=trim($_POST['rolename']);
    $arr_pro=array(
		1 => $_LANG[Addition1],
		2 => $_LANG[Addition2],
		3 => $_LANG[Addition3],
		4 => $_LANG[Addition4],
		5 => $_LANG[Addition5],
		6 => $_LANG[Addition6],
		7 => $_LANG[Addition7],
		8 => $_LANG[Addition8],
		9 => $_LANG[Addition9],
		10 => $_LANG[Addition10],
		11 => $_LANG[Addition11],
		12 => $_LANG[Addition12],
		13 => $_LANG[Addition13],
		14 => $_LANG[Addition14],
		15 => $_LANG[Addition15],
		16 => $_LANG[Addition16],
		17 => $_LANG[Addition17],
		18 => $_LANG[Addition18],
		19 => $_LANG[Addition19],
		20 => $_LANG[Addition20],
		21 => $_LANG[Addition21],
		22 => $_LANG[Addition22],
		23 => $_LANG[Addition23],
		24 => $_LANG[Addition24],
		25 => $_LANG[Addition25],
		26 => $_LANG[Addition26],
		27 => $_LANG[Addition27],
		28 => $_LANG[Addition28],
    
	);
    
    
    //输入用户名
    if(!empty($username) && empty($rolename)){
	    //$sql="select id from ".TBL_ACCOUNT." where name='$username' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= " select id from ".TBL_ACCOUNT." where name LIKE CONVERT( _utf8 '".$username."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= "select id from ".TBL_ACCOUNT." where name LIKE '".$username."'"; 
    	}
		$row1=$acc_db -> getRow($sql);
		if(!empty($row1)){
		    $id = $row1['id'];
			$sql = "select id,name,level from ".TBL_WB_USER." where account_id='$id'";
			$row = $wangpu_db-> getRow($sql);
			$row['username']=$username;
			$sql="select * from ".TBL_WB_ITEM." where owner_id=".$row['id'];
			//$sql="select * from wb_magic where ownerid";
			$rs=$wangpu_db-> getAll($sql);
		    //获取装备名称
		    $rs1=$rs;
			for($i=0;$i<count($rs1);$i++){
			    $sql = "select name from ".TBL_WB_ITEMTYPE." where id =".$rs1[$i]['TYPE'];
			    $res = $wangpu_db-> getRow($sql);
			    $rs[$i]['type'] = $res['name'];  
			    for($j=1;$j<=8;$j++){
			        if($rs[$i]['itemAddition'.$j]==0){
			        }else if(strlen($rs[$i]['itemAddition'.$j])>0 && strlen($rs[$i]['itemAddition'.$j])==5){
				        $rs[$i]['itemAddition'.$j]="0".$rs[$i]['itemAddition'.$j];
				        $rs[$i]['itemAddition'.$j]=$arr_pro[intval(substr($rs[$i]['itemAddition'.$j],0,2))]."+".intval(substr($rs[$i]['itemAddition'.$j],2));
			        }else{
				        $rs[$i]['itemAddition'.$j]=$arr_pro[intval(substr($rs[$i]['itemAddition'.$j],0,2))]."+".intval(substr($rs[$i]['itemAddition'.$j],2));
			        }
			        if($j<=2){
				        if($rs[$i]['itemBase'.$j]==0){
				        }else if(strlen($rs[$i]['itemBase'.$j])>0 && strlen($rs[$i]['itemBase'.$j])==5){
					        $rs[$i]['itemBase'.$j]="0".$rs[$i]['itemBase'.$j];
					        $rs[$i]['itemBase'.$j]=$arr_pro[intval(substr($rs[$i]['itemBase'.$j],0,2))]."+".intval(substr($rs[$i]['itemBase'.$j],2));
				       }else{
					       $rs[$i]['itemBase'.$j]=$arr_pro[intval(substr($rs[$i]['itemBase'.$j],0,2))]."+".intval(substr($rs[$i]['itemBase'.$j],2));
				       }
			       }
		       }
            }
        }		
    }
	//输入角色名
    if(empty($username) && !empty($rolename)){
		
        //$sql="select id,account_id,name,level from ".TBL_WB_USER." where name='$rolename' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= "select id,account_id,name,level from ".TBL_WB_USER." where name LIKE CONVERT( _utf8 '".$rolename."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= " select id,account_id,name,level from ".TBL_WB_USER." where name LIKE '".$rolename."'"; 
    	}
		$row1=$wangpu_db -> getRow($sql);
		if(!empty($row1)){
		    $id=$row1['account_id'];
			$row=$row1;
			$sql="select name from ".TBL_ACCOUNT." where id='$id'";
			$row1=$acc_db-> getRow($sql);
			$row['username']=$row1['name'];
			$row['rolename']=$rolename;
			$sql="select * from ".TBL_WB_ITEM." where owner_id=".$row['id'];
			//$sql="select * from wb_magic where ownerid";
			$rs=$wangpu_db-> getAll($sql);
		    //获取装备名称
		    $rs1=$rs;
			for($i=0;$i<count($rs1);$i++){
			    $sql="select name from ".TBL_WB_ITEMTYPE." where id =".$rs1[$i]['TYPE'];
			    $res=$wangpu_db-> getRow($sql);
			    $rs[$i]['type']=$res['name'];  
			    for($j=1;$j<=8;$j++){
			        if($rs[$i]['itemAddition'.$j]==0){
			        }else if(strlen($rs[$i]['itemAddition'.$j])>0 && strlen($rs[$i]['itemAddition'.$j])==5){
				        $rs[$i]['itemAddition'.$j]="0".$rs[$i]['itemAddition'.$j];
				        $rs[$i]['itemAddition'.$j]=$arr_pro[intval(substr($rs[$i]['itemAddition'.$j],0,2))]."+".intval(substr($rs[$i]['itemAddition'.$j],2));
			        }else{
				        $rs[$i]['itemAddition'.$j]=$arr_pro[intval(substr($rs[$i]['itemAddition'.$j],0,2))]."+".intval(substr($rs[$i]['itemAddition'.$j],2));
			        }
			        if($j<=2){
				        if($rs[$i]['itemBase'.$j]==0){
				        }else if(strlen($rs[$i]['itemBase'.$j])>0 && strlen($rs[$i]['itemBase'.$j])==5){
					        $rs[$i]['itemBase'.$j]="0".$rs[$i]['itemBase'.$j];
					        $rs[$i]['itemBase'.$j]=$arr_pro[intval(substr($rs[$i]['itemBase'.$j],0,2))]."+".intval(substr($rs[$i]['itemBase'.$j],2));
				       }else{
					       $rs[$i]['itemBase'.$j]=$arr_pro[intval(substr($rs[$i]['itemBase'.$j],0,2))]."+".intval(substr($rs[$i]['itemBase'.$j],2));
				       }
			       }
		       }
			}
        }
	} 
	//输入用户名和角色名	
    if(!empty($username) && !empty($rolename)){
        //$sql="select id,account_id,name,level from ".TBL_WB_USER." where name='$rolename' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= "select id,account_id,name,level from ".TBL_WB_USER." where name LIKE CONVERT( _utf8 '".$rolename."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= " select id,account_id,name,level from ".TBL_WB_USER." where name LIKE '".$rolename."'"; 
    	}
		$row1=$wangpu_db -> getRow($sql);
		//$sql="select id from ".TBL_ACCOUNT." where name='$username' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= " select id from ".TBL_ACCOUNT." where name LIKE CONVERT( _utf8 '".$username."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= "select id from ".TBL_ACCOUNT." where name LIKE '".$username."'"; 
    	}
		$row2=$acc_db -> getRow($sql);
		if(!empty($row1)&&!empty($row2)&&$row1['account_id']==$row2['id']){
		    $id=$row1['account_id'];
			$row=$row1;
			$row['username']=$username;
			$row['rolename']=$rolename;
			$sql="select * from ".TBL_WB_ITEM." where owner_id=".$row['id'];
			//$sql="select * from wb_magic where ownerid";
			$rs=$wangpu_db-> getAll($sql);
		    //获取装备名称
		    $rs1=$rs;
			for($i=0;$i<count($rs1);$i++){
			    $sql="select name from ".TBL_WB_ITEMTYPE." where  id =".$rs1[$i]['TYPE'];
			    $res=$wangpu_db-> getRow($sql);
			    $rs[$i]['type']=$res['name']; 
			    for($j=1;$j<=8;$j++){
			        if($rs[$i]['itemAddition'.$j]==0){
			        }else if(strlen($rs[$i]['itemAddition'.$j])>0 && strlen($rs[$i]['itemAddition'.$j])==5){
				        $rs[$i]['itemAddition'.$j]="0".$rs[$i]['itemAddition'.$j];
				        $rs[$i]['itemAddition'.$j]=$arr_pro[intval(substr($rs[$i]['itemAddition'.$j],0,2))]."+".intval(substr($rs[$i]['itemAddition'.$j],2));
			        }else{
				        $rs[$i]['itemAddition'.$j]=$arr_pro[intval(substr($rs[$i]['itemAddition'.$j],0,2))]."+".intval(substr($rs[$i]['itemAddition'.$j],2));
			        }
			        if($j<=2){
				        if($rs[$i]['itemBase'.$j]==0){
				        }else if(strlen($rs[$i]['itemBase'.$j])>0 && strlen($rs[$i]['itemBase'.$j])==5){
					        $rs[$i]['itemBase'.$j]="0".$rs[$i]['itemBase'.$j];
					        $rs[$i]['itemBase'.$j]=$arr_pro[intval(substr($rs[$i]['itemBase'.$j],0,2))]."+".intval(substr($rs[$i]['itemBase'.$j],2));
				       }else{
					       $rs[$i]['itemBase'.$j]=$arr_pro[intval(substr($rs[$i]['itemBase'.$j],0,2))]."+".intval(substr($rs[$i]['itemBase'.$j],2));
				       }
			       }
		       } 
			}
	    }
    } 	
	
    $smarty -> assign('here',$_LANG['equire']);	
	$smarty-> assign('row',$row);
	$smarty-> assign('rs',$rs);
    $smarty -> display('player_equire.htm');
}



//玩家技能查询
if($_REQUEST['act'] == 'skill'){
	$arr_mp=array(
	1 => $_LANG['TM'],
	2 => $_LANG['QZ'],
	4 => $_LANG['EM'],
	8 => $_LANG['GB'],
	16 => $_LANG['SL'],
	32 => $_LANG['DC'],
	4096=>$_LANG['XS']
    );
    $username=trim($_POST['username']);
    $rolename=trim($_POST['rolename']);
	
    //输入用户名
    if(!empty($username) && empty($rolename)){
        //$sql="select id from ".TBL_ACCOUNT." where name='$username' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= " select id from ".TBL_ACCOUNT." where name LIKE CONVERT( _utf8 '".$username."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= "select id from ".TBL_ACCOUNT." where name LIKE '".$username."'"; 
    	}
        $row1=$acc_db -> getRow($sql);
        if(!empty($row1)){
		    $id=$row1['id'];
			$sql="select id,name,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv from ".TBL_WB_USER." where account_id='$id'";
			$row=$wangpu_db-> getRow($sql);
			$row['username']=$username;
			$sql="select * from ".TBL_WB_MAGIC." where ownerid=".$row['id']." order by level desc";
			//$sql="select * from wb_magic where ownerid";
			$rs=$wangpu_db-> getAll($sql);
		    //获取技能名称
		    $rs1=$rs;
			for($i=0;$i<count($rs1);$i++){
			    $sql="select name from ".TBL_WB_MAGICTYPE." where type =".$rs1[$i]['type'];
			    $res=$wangpu_db-> getRow($sql);
			    $rs[$i]['type']=$res['name'];  
			}
        }	
	}
	//输入角色名
    if(empty($username) && !empty($rolename)){
        //$sql="select id,account_id,name,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv from ".TBL_WB_USER." where name='$rolename' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= "select id,account_id,name,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv  from ".TBL_WB_USER." where name LIKE CONVERT( _utf8 '".$rolename."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= " select id,account_id,name,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv from ".TBL_WB_USER." where name LIKE '".$rolename."'"; 
    	}
		$row1=$wangpu_db -> getRow($sql);
		if(!empty($row1)){
		    $id=$row1['account_id'];
			$row=$row1;
			$sql="select name from ".TBL_ACCOUNT." where id='$id'";
			$row1=$acc_db-> getRow($sql);
			$row['username']=$row1['name'];
			$row['rolename']=$rolename;
			$sql="select * from ".TBL_WB_MAGIC." where ownerid=".$row['id']." order by level desc";
			//$sql="select * from wb_magic where ownerid";
			$rs=$wangpu_db-> getAll($sql);
		    //获取技能名称
		    $rs1=$rs;
			for($i=0;$i<count($rs1);$i++){
			    $sql="select name from ".TBL_WB_MAGICTYPE." where type =".$rs1[$i]['type'];
			    $res=$wangpu_db-> getRow($sql);
			    $rs[$i]['type']=$res['name'];  
			}
        }
	} 
	//输入用户名和角色名	
    if(!empty($username) && !empty($rolename)){
        //$sql="select id,account_id,name,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv from ".TBL_WB_USER." where name='$rolename' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= "select id,account_id,name,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv from ".TBL_WB_USER." where name LIKE CONVERT( _utf8 '".$rolename."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= " select id,account_id,name,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv from ".TBL_WB_USER." where name LIKE '".$rolename."'"; 
    	}
		$row1=$wangpu_db -> getRow($sql);
		//$sql="select id from ".TBL_ACCOUNT." where name='$username' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= " select id from ".TBL_ACCOUNT." where name LIKE CONVERT( _utf8 '".$username."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= "select id from ".TBL_ACCOUNT." where name LIKE '".$username."'"; 
    	}
		$row2=$acc_db -> getRow($sql);
		if(!empty($row1)&&!empty($row2)&&$row1['account_id']==$row2['id']){
		    $id=$row1['account_id'];
			$row=$row1;
			$row['username']=$username;
			$row['rolename']=$rolename;
			$sql="select * from ".TBL_WB_MAGIC." where ownerid=".$row['id']." order by level desc";
			//$sql="select * from wb_magic where ownerid";
			$rs=$wangpu_db-> getAll($sql);
		    //获取技能名称
		    $rs1=$rs;
			for($i=0;$i<count($rs1);$i++){
			    $sql="select name from ".TBL_WB_MAGICTYPE." where type =".$rs1[$i]['type'];
			    $res=$wangpu_db-> getRow($sql);
			    $rs[$i]['type']=$res['name'];  
			}
        }
			
	} 	
		   
	$smarty -> assign('mp',$arr_mp);	
    $smarty -> assign('here',$_LANG['skill']);	
	$smarty-> assign('row',$row);
	$smarty-> assign('rs',$rs);
    $smarty -> display('player_skill.htm');
}


//玩家宠物查询
if($_REQUEST['act'] == 'pet'){
	
    $username=$_REQUEST['username'];
    $rolename=$_REQUEST['rolename'];
    //输入用户名
    if(!empty($username) && empty($rolename)){
	    //$sql="select id from ".TBL_ACCOUNT." where name='$username' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= " select id from ".TBL_ACCOUNT." where name LIKE CONVERT( _utf8 '".$username."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= "select id from ".TBL_ACCOUNT." where name LIKE '".$username."'"; 
    	}
		$row1=$acc_db -> getRow($sql);
		if(!empty($row1)){
		    $id=$row1['id'];
			$sql="select id,name,level from ".TBL_WB_USER." where account_id='$id'";
			$row=$wangpu_db-> getRow($sql);
			if(!empty($row)){
			    $row['username']=$username;
			    $sql="select * from ".TBL_WB_EUDEMON." where owner_id =".$row['id']." ";
			    $rs=$wangpu_db-> getAll($sql);
			    //获取宠物类型
			    $rs1=$rs;
			    for($i=0;$i<count($rs1);$i++){
			        $sql="select name from ".TBL_WB_EUDEMONTYPE." where Id =".$rs1[$i]['typeid'];
			        $res=$wangpu_db-> getRow($sql);
			        $rs[$i]['type']=$res['name'];  
			    }
			}
        }
	}
    //输入角色名
    if(empty($username) && !empty($rolename)){
        //$sql="select id,account_id,name,level from ".TBL_WB_USER." where name='$rolename' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= "select id,account_id,name,level from ".TBL_WB_USER." where name LIKE CONVERT( _utf8 '%".$rolename."%' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= " select id,account_id,name,level from ".TBL_WB_USER." where name LIKE '%".$rolename."%'"; 
    	}
		$row1=$wangpu_db -> getRow($sql);
		if(!empty($row1)){
		    $id=$row1['account_id'];
			$row=$row1;
			$sql="select name from ".TBL_ACCOUNT." where id='$id'";
			$row1=$acc_db-> getRow($sql);
			$row['username']=$row1['name'];
			$row['rolename']=$rolename;
			$sql="select * from ".TBL_WB_EUDEMON." where owner_id =".$row['id']." order by level desc";
			//$sql="select * from wb_magic where ownerid";
			$rs=$wangpu_db-> getAll($sql);
			//获取宠物类型
		    $rs1=$rs;
			for($i=0;$i<count($rs1);$i++){
			    $sql="select name from ".TBL_WB_EUDEMONTYPE." where Id =".$rs1[$i]['typeid'];
			    $res=$wangpu_db-> getRow($sql);
			    $rs[$i]['type']=$res['name'];  
			}
        }
	} 
	//输入用户名和角色名	
    if(!empty($username) && !empty($rolename)){
        //$sql="select id,account_id,name,level from ".TBL_WB_USER." where name='$rolename' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= "select id,account_id,name,level from ".TBL_WB_USER." where name LIKE CONVERT( _utf8 '%".$rolename."%' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= " select id,account_id,name,level from ".TBL_WB_USER." where name LIKE '%".$rolename."%'"; 
    	}
		$row1=$wangpu_db -> getRow($sql);
		//$sql="select id from ".TBL_ACCOUNT." where name='$username' limit 1";
        if($_CFG['s_name']=='TW'){
    		$sql= " select id from ".TBL_ACCOUNT." where name LIKE CONVERT( _utf8 '".$username."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= "select id from ".TBL_ACCOUNT." where name LIKE '".$username."'"; 
    	}
		$row2=$acc_db -> getRow($sql);
		if(!empty($row1)&&!empty($row2)&&$row1['account_id']==$row2['id']){
		    $id=$row1['account_id'];
		    $row=$row1;
			$row['username']=$username;
			$row['rolename']=$rolename;
			$sql="select * from ".TBL_WB_EUDEMON." where owner_id =".$row['id']." order by level desc";
			//$sql="select * from wb_magic where ownerid";
			$rs=$wangpu_db-> getAll($sql);
			//获取宠物类型
		    $rs1=$rs;
			for($i=0;$i<count($rs1);$i++){
			    $sql="select name from ".TBL_WB_EUDEMONTYPE." where Id =".$rs1[$i]['typeid'];
			    $res=$wangpu_db-> getRow($sql);
			    $rs[$i]['type']=$res['name'];  
			}
        }
	} 	
		   
	
    $smarty -> assign('here',$_LANG['pet']);	
	$smarty-> assign('row',$row);
	$smarty-> assign('rs',$rs);
    $smarty -> display('player_pet.htm');
}


//玩家商品购买查询
if($_REQUEST['act'] == 'goods'){
    $username=trim($_POST['username']);
	$rolename=trim($_POST['rolename']);
	
	//只输入角色名
	if(!empty($rolename) && empty($username)){
	    //$sql="select account_id from ".TBL_WB_USER." where name='$rolename'";
	    if($_CFG['s_name']=='TW'){
    		$sql= "select account_id from ".TBL_WB_USER." where name LIKE CONVERT( _utf8 '".$rolename."' USING big5) COLLATE big5_chinese_ci"; 
    	}else{
    		$sql= " select account_id from ".TBL_WB_USER." where name LIKE '".$rolename."'"; 
    	}
		$row=$wangpu_db->getRow($sql);
		$id=$row['account_id'];
	}
	
	
	if(!empty($id)){
		$sql="select name from ".TBL_ACCOUNT." where id=$id";
		$row=$acc_db->getRow($sql);
		$username=$row['name'];
	}
	//输入用户名不为空
	if(!empty($username)){
		if(empty($rolename)){
		   if($_CFG['s_name']=='TW'){
    		    $sql= " select id from ".TBL_ACCOUNT." where name LIKE CONVERT( _utf8 '".$username."' USING big5) COLLATE big5_chinese_ci"; 
    	   }else{
    		    $sql= "select id from ".TBL_ACCOUNT." where name LIKE '".$username."'"; 
    	   }
			$row=$acc_db->getRow($sql);
			$id=$row['id'];
		}
		$sql="select Account_Name,ItemType_ID,writeTime,ItemType_Amount,Spend_VAS_Point,Account_Name as value from ".TBL_SER_CONSUMELOG_TABLE." where Account_Name='$username' order by writeTime desc";
		$rs=$acc_db->getAll($sql);		
		$sql="select sum(Spend_VAS_Point) from ".TBL_SER_CONSUMELOG_TABLE." where Account_Name='$username' ";
		$sum=$acc_db->getOne($sql);
		$sum=intval($sum);
		//获取商品名和商品价格
	    $rs1=$rs;
	    for($i=0;$i<count($rs1);$i++){
	    	$rs[$i][ItemType_Amount]=intval($rs[$i][ItemType_Amount]);
	    	$rs[$i][Spend_VAS_Point]=intval($rs[$i][Spend_VAS_Point]);
	        $sql="select id,name from ".TBL_WB_ITEMTYPE." where id=".$rs[$i]['ItemType_ID'];
		    $res=$wangpu_db-> getRow($sql);
		    $rs[$i]['name']=$res['name'];
		    $sql="select price from ".TBL_WB_GOOD_VAS." where itemtype =".$rs[$i]['ItemType_ID'];
		    $res=$wangpu_db-> getRow($sql);
		    if(isset($res['price'])){
		        $rs[$i]['price']=intval($res['price']); 
		    }  
	    }		
	
	}
	
	
	
    $smarty -> assign('here',$_LANG['goods']);	
	$smarty-> assign('username',$username);
	$smarty-> assign('sum',$sum);
	$smarty-> assign('rs',$rs);
    $smarty -> display('player_goods.htm');
}

/* 玩家密码查询 */
if($_REQUEST['act'] == 'searchpwd'){
	
	$smarty -> assign('here' , $_LANG['searchpwd']);	
	$smarty -> display('player_searchpwd.htm');
}

if($_REQUEST['act'] == 'searchpwd_query'){
    $username = $_REQUEST['account'];
	$sql = "select password from ".TBL_ACCOUNT." where name = '".$username."'";
	$result = $acc_db -> getOne($sql);
	$smarty -> assign('username' , $username);
	$smarty -> assign('pwd' , $result);
	$smarty -> assign('display' , 1);
	$smarty -> display('player_searchpwd.htm');	
}

if($_REQUEST['act'] == 'setitempwd'){
	
    $fileinfo = $_FILES['input_rolename'];
    $role_name = $_POST['rolename']; //获取角色名
    if($fileinfo['name']){
        	/* 批量导入 */
    	$filepath = ROOT_PATH."data/upload/reset/";
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
        foreach($role_name as $key => $value){
            if($value=='') continue;
            $sql="select name from ".TBL_WB_USER." where name='$value' ";
            $rs=$wangpu_db ->getOne($sql);
            //$str=array();
            if($rs){
                $sql="update " .TBL_WB_USER." set `lockpwd`='EE399A85CA1C57FD'  where name='$value' ";
        	    $wangpu_db -> query($sql);
        	    $row=$wangpu_db ->affected_rows();
        	    if($row){
        	       $str1[]=$_LANG['reset_tip1'].$value.$_LANG['reset_tip2'];
        	    }else{
        	       $str1[]="[".$value."]".$_LANG['reset_tip3'] ;
        	    }
            }else{
                $str1[]=$_LANG['rolename']."[".$value."]".$_LANG['reset_tip4'] ;
            }

        }
    }elseif(!empty($role_name)){
        foreach($role_name as $key => $value){
            if($value=='') continue;
            $sql="select name from ".TBL_WB_USER." where name='$value' ";
            $rs=$wangpu_db ->getOne($sql);
            if($rs){
                $sql="update " .TBL_WB_USER." set `lockpwd`='EE399A85CA1C57FD'  where name='$value' ";
        	    $wangpu_db -> query($sql);
        	    $row=$wangpu_db ->affected_rows();
        	    if($row){
        	        $str1[]=$_LANG['reset_tip1'].$value.$_LANG['reset_tip2'];
        	    }else{
        	        $str1[]="[".$value."]".$_LANG['reset_tip3'] ;
        	    }
            }else{
            	$str1[]=$_LANG['rolename']."[".$value."]".$_LANG['reset_tip4'] ;
            }

        }

   }
    //print_r($str);
    $smarty -> assign('str',$str1);
	$smarty -> assign('here',$_LANG['setitempwd']);
	$smarty -> display('player_setitempwd.htm');	
	

}

if($_REQUEST['act'] == 'userdetail'){
	
	$arr_mp=array(
	1 => $_LANG['TM'],
	2 => $_LANG['QZ'],
	4 => $_LANG['EM'],
	8 => $_LANG['GB'],
	16 => $_LANG['SL'],
	32 => $_LANG['DC'],
	4096=>$_LANG['XS']
    );

    $userid = intval($_GET['userid']);
    $sql = "select * from ".TBL_ACCOUNT." where `id`='$userid' limit 1 ";
    $rs = $acc_db ->getAll($sql);

    //获取剩余元宝数
    $sql = "select * from ".TBL_SK_VAS_TABLE."  where Account_ID='$userid' order by VAS_Point asc ";
    $REC_VAS = array();
    $row = $acc_db -> getAll($sql);
    $vaspointsum = $vasmoneysum = 0;
    $REC_VAS['一线'] = 0;
    if(!empty($row)){
        foreach($row as $key => $rs_vas){
            $REC_VAS[$rs_vas['Server_Name']] = $rs_vas['VAS_Point'];
            $vaspointsum += $rs_vas['VAS_Point'];
            $vasmoneysum += $rs_vas['VAS_Point_juan'];
        }
    }


    $sql = "select * from ".TBL_WB_USER." where account_id='$userid' limit 1 ";
    $rs2 = $wangpu_db -> getAll($sql);
    $lastexitdate = "20".substr($rs2[0]['Exit_Time'],0,2)."-".substr($rs2[0]['Exit_Time'],2,2)."-".substr($rs2[0]['Exit_Time'],4,2)." "
    .substr($rs2[0]['Exit_Time'],6,2).":".substr($rs2[0]['Exit_Time'],8,2).":00";

    $info['name']=$rs[0]['name'];
    $info['role']=$rs2[0]['name'];
    $info['sex']=$rs2[0]['sex'] == 1 ? '女':'男';
    $info['reg_time']=date('Y-m-d H:i:s',$rs[0]['regdate']);
    $info['reg_ip']=$rs[0]['netbar_ip'];
    $info['last_exit']=$lastexitdate;
    
    $info['yb']=$vaspointsum.'YB';
    $info['rmb']=$vasmoneysum;
    $info['gold1']=$rs2[0]['Gold1'];
    $info['gold2']=$rs2[0]['Gold2'];
    $info['gold3']=$rs2[0]['Gold3'];
    $info['gold4']=$rs2[0]['Gold4'];
    
    $info['Feel']=$rs2[0]['Feel'];
    $info['Fir_Job']=$arr_mp[$rs2[0]['Fir_Job']] ;
    $info['Sec_Job']=$arr_mp[$rs2[0]['Sec_Job']] ;
    $info['level']=$rs2[0]['level'];
    $info['Fir_JobLv']=$rs2[0]['Fir_JobLv'];
    $info['Sec_JobLv']=$rs2[0]['Sec_JobLv'] ;
    
    $smarty->assign('action_link', array('href'=>'javascript:history.go(-1)', 'text' => $_LANG['detail_back']));     
    $smarty -> assign('info',$info);
    $smarty -> assign('rec_vas',$REC_VAS);
    $smarty -> assign('here',$_LANG['list_userdetail']);
    $smarty -> display('player_userdetail.htm');
	
}

if($_REQUEST['act'] == 'first_pay'){
	admin_priv("player_first_pay");
	$username=trim($_POST['username']);
	$rolename=trim($_POST['rolename']);
	
	if($username){
		$sql="select id from ".TBL_ACCOUNT." where name like '$username' ";
		$id1 =$acc_db -> getOne($sql);
		if($id1){
			$id=$id1;
		}
	}
	if($rolename){
		$sql="select account_id from ".TBL_WB_USER." where name like '$rolename' ";
		$id2 =$wangpu_db -> getOne($sql);
		if($id){
			if($id2!=$id){
				$id='';
			}
		}else{
			if($id2){
				$id=$id2;
			}
		}
	}
	
	if(!empty($id)){
		$sql= " select * from ".TBL_SK_GENERALIZE." where account_id='$id' and generalize_id='4006' and type='1000' ";
		$res= $acc_db -> getAll($sql);
	}
	if(!empty($res)){
	    $sql=" select name from ".TBL_ACCOUNT." where id ='$id' ";
		$name=$acc_db -> getOne($sql);
		$sql=" select name from ".TBL_WB_USER." where account_id ='$id' ";
		$role=$wangpu_db -> getOne($sql);
		foreach ($res as $key =>$value){
			$info[$key][id]=$value[id];
			$info[$key][state]=$value[state]==1000? $_LANG['no_get']:$_LANG['have_get'];
			$info[$key][generalize_id]=$value[generalize_id];
			$info[$key][type]=$value[type];
			$info[$key][name]=$name;
			$info[$key][role]=$role;
		}
	}
	$smarty -> assign('here',$_LANG['fpay']);
	$smarty -> assign('info',$info);
	$smarty -> assign('name',$username);
	$smarty -> assign('role',$rolename);
	$smarty -> display('player_fpay.htm');	
}


if($_REQUEST['act'] == 'game_log'){

	date_default_timezone_set('PRC');
	$date=trim($_POST['time']);
	$role=trim($_POST['rolename']);
	$date=date('Y-m-d',strtotime($date));
	$moth=intval(date('m',strtotime($date)));
	$year=date('Y_',strtotime($date));
	$day=intval(date('d',strtotime($date))/8)+1;
	$table='log_'.$year.$moth.'_'.$day;

	if($_POST[time]&&$role){
		$sql="SELECT * FROM   $table WHERE szOwner LIKE '$role' AND szTime LIKE '$date%' ";

	    $info=$gdb -> getAll($sql);
	}
	if(!empty($info)){
	    foreach ( $info as $key => $value){
		    $info[$key]['usOp']='usOp'.$value['usOp'];
		    $info[$key]['usOpType']='usOpType'.$value['usOpType'];
		    $info[$key]['usGetType']='usGetType'.$value['usGetType'];
	    }
	}

	$smarty -> assign('here',$_LANG['game_log']);
	$smarty -> assign('info',$info);
    $smarty -> display('player_game_log.htm');	
	
}





/*------------------------------------------------------ */
//-- 排序、分页、查询
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'query')
{
    
   
}




?>
 
