<?php
/**
 * 充值消费统计
 * @author King.L
 */
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');
require(dirname(__FILE__) . '/admin_class/pay_consume_class.php');
$pc=new pay_consume_class();

if($_REQUEST['act'] == 'py_all_statics'){
    $smarty -> display("py_all_statics.htm");
}

/**
 * 充值消费统计列表
 */
if ($_REQUEST['act'] == 'py_statics'){	
    admin_priv('py_statics');
    $result = $pc -> get_py_statics($_CFG['allowIpArr']);  
    $smarty -> assign('here',$_LANG['statics']);
    $smarty -> assign('rs' , $result[0]);
    $smarty -> assign('res' , $result[1]);
    $smarty -> display('py_statics.htm');
}

/**
 * 充值查询列表
 * 默认查询前30天数据
 */
if($_REQUEST['act'] == 'py_list'){
	admin_priv('py_list');
    $now_time = time();
    setcookie('search_time',$now_time);
    if($_CFG['allowIpArr']!= ''){
    	$Iparray = explode("|", $_CFG['allowIpArr']);
    }else{
    	$Iparray = array();
    }
    
    
    $result = $pc -> get_py_list($now_time,'',0,30,$Iparray);
    $filter['record_count'] =$result[1];
    $filter = page_and_size($filter);
    $record_count = count($result);
    $sumres = $pc -> getSum($result[2]);
    
    $smarty -> assign('here',$_LANG['list']);
	$smarty -> assign('prev',1);
	$smarty -> assign('next',2);
    $smarty -> assign('filter',       $filter);
    $smarty -> assign('sumres',$sumres);
    $smarty -> assign('record_count', $filter['record_count']);
    $smarty -> assign('page_count',   $filter['page_count']);
    $smarty -> assign('full_page',    1);
    $smarty -> assign('search_time',$now_time);
    $smarty -> assign('res',$result[0]);
    $smarty -> display('py_list.htm');
}
/**
 * 元宝充值记录
 * 默认列出所有记录
 */
if($_REQUEST['act'] == 'py_yb'){
	admin_priv("py_yb");
	$smarty -> assign('here' ,$_LANG['yb']);
	$smarty -> assign('pagenum',$pagenum);
    $smarty -> assign('res',$result);
    $smarty -> display('py_yb.htm');
}

/**
 * 元宝充值记录查询
 */
if($_REQUEST['act'] == 'ybsearch'){
	date_default_timezone_set('PRC');
    $start_time = $_POST['start_time'];
    $end_time = $_POST['end_time'];
    $username = $_POST['username'];
    $rolename = $_POST['rolename'];
    $paynum = $_POST['paynum'];
//    if(!empty($start_time)&&!empty($end_time)){
//        $start_time = strtotime($start_time);
//        $end_time = strtotime($end_time)+ONE_DAY;	
//    }

    $post="username=$username&rolename=$rolename&start_time=$start_time&end_time=$end_time&paynum=$paynum";

    $url="username=$_GET[username]&rolename=$_GET[rolename]&start_time=$_GET[start_time]&end_time=$_GET[end_time]&paynum=$_GET[paynum]";
    if(!$_POST[query]){
       $count=$pc -> get_yb_count($_GET[start_time],$_GET[end_time],$_GET[username],$_GET[rolename],$_GET[paynum]);
    }else{
   	   $count=$pc -> get_yb_count($start_time,$end_time,$username,$rolename,$paynum);
    }
    //print_r($count);
    //exit;
    $page=$_REQUEST['page']?$_REQUEST['page']:1;
    $start=($page-1)*30;
    $num=intval($count['count']/30)+1;
    
    for($i=0;$i<10;$i++){
    	if(($page+$i)>$num) break;
    	$pagenum[]=$page+$i;
    }
    if(!$_POST[query]){
    	$info=$_GET;
    	$post=$url;
    	$result = $pc -> get_yb_search($_GET[start_time],$_GET[end_time],$_GET[username],$_GET[rolename],$_GET[paynum],$start,30);
    }else{
    	$info=$_POST;
    	$result = $pc -> get_yb_search($start_time,$end_time,$username,$rolename,$paynum,$start,30);
    }

    $smarty -> assign('count',$count);
    $smarty -> assign('info',$info);
    $smarty -> assign('post',$post);
    $smarty -> assign('pagenum1',$pagenum);
    $smarty -> assign('url', $url);
    $smarty -> assign('page',$page);
    $smarty -> assign('lastpage',$page-1);
    $smarty -> assign('nextpage',$page+1);
    $smarty -> assign('num',$num);
    $smarty -> assign('here' ,$_LANG['yb']);
    $smarty -> assign('res',$result);
    $smarty -> display('py_yb.htm');
}

/*------------------------------------------------------ */
//-- 充值查询功能
/*------------------------------------------------------ */
if($_REQUEST['act'] == 'list_search'){
    $search_rang = isset($_POST['search_range']) ? $_POST['search_range'] : 1;
    $end_time = strtotime($_POST['start_time']);
    $start_time = strtotime($_POST['end_time']);
    $time_rang = $start_time - $end_time;
    $links[] = array('text' => $_LANG['back_list'], 'href' => 'pay_consume.php?act=py_list');   
    $Iparray = explode("|", $_CFG['allowIpArr']);
    
    if($time_rang < 0){
    	sys_msg($_LANG['err_time'],1,$links);
    }
    if($search_rang){
    	//get_py_list($start_time,$time_rang,$start=0,$number=10,$Iparray)
        $result = $pc -> get_py_list($start_time,$time_rang,0,10,$Iparray);        
    }else{
        $result = $pc -> get_all_pylist($start_time,$time_rang,0,10,$Iparray);
    }

    $filter['record_count'] =$result[1];
    $filter = page_and_size($filter);
    $record_count = count($result);

    $sumres = $pc -> getSum($result[0]);

    $smarty -> assign('filter',       $filter);
    $smarty -> assign('sumres',$sumres);
    $smarty -> assign('record_count', $filter['record_count']);
    $smarty -> assign('page_count',   $filter['page_count']);
    $smarty -> assign('full_page',    1);
    $smarty -> assign('search_time',$now_time);
    $smarty -> assign('res',$result[0]);
    $smarty -> display('py_list.htm');
}

/**
 * 供外站调用的充值记录查询
 */
if($_REQUEST['act'] == 'all_server_pylist'){
    $start_time = $_POST['start_time'];
    $time_rang = $_POST['time_rang'];
    $result = $pc -> get_py_list($start_time,$time_rang);
    echo $result;
}

if($_REQUEST['act'] =='list_query'){
	admin_priv('py_list');
	$page = $_REQUEST['pages'];
	if($page == 'first' || $page == 'last'){
		if($page == 'first'){
			$start = 0;
			$prev = 1;
			$next = 2;
		}else{
			setcookie('search_time',$now_time);
    		$res = $pc -> get_py_list($now_time,'',0,30);
    		$start = count($res[2]);
    		$prev = 1;
    		$next = 2;
		}
	}else{
		$start = ($page-1)*30;
		$prev = $page;
		$next = $page + 1;
	}
    $now_time = time();
    setcookie('search_time',$now_time);
    $result = $pc -> get_py_list($now_time,'',$start,30);
    $filter['record_count'] =$result[1];
    $filter = page_and_size($filter);
    $record_count = count($result);
    $result[0] = array_reverse($result[0]);
    $sumres = $pc -> getSum($result[2]);

	$smarty -> assign('prev',$prev);
	$smarty -> assign('next',$next);
    $smarty -> assign('filter',       $filter);
    $smarty -> assign('sumres',$sumres);
    $smarty -> assign('record_count', $filter['record_count']);
    $smarty -> assign('page_count',   $filter['page_count']);
    $smarty -> assign('full_page',    1);
    $smarty -> assign('search_time',$now_time);
    $smarty -> assign('res',$result[0]);
    $smarty -> display('py_list.htm');
	
}

if($_REQUEST['act'] == 'yb_query'){
	admin_priv("py_yb");
	$action = $_REQUEST['action'];
	if($_REQUEST['pages'] == 'first' || $_REQUEST['pages'] == 'last'){
		if($_REQUEST['pages'] == first){
			$start = 0;
			$prev = 1;
			$next = 2;
		}else{
			$sql = "select count(id) from `".TBL_ACCOUNT."` ";
			$count = $acc_db -> getOne($sql);
			$start = $count - 30;
			$prev = 1;
			$next = 2;
		}
	}else{
			$start = ($_REQUEST['pages']-1)*30;
			$prev = $_REQUEST['pages'];
			$next = $_REQUEST['pages'] + 1;
	}
	$now_time = $_COOKIE['search_time'];
    $result = $pc -> get_py_yb($now_time,'',$start,30);
	$result_count = count($result);
	$filter['record_count'] = $record_count;
	$filter = page_and_size($filter);
	
	
	$smarty -> assign('feiye' , 1);
	$smarty -> assign('prev' , $prev);
	$smarty -> assign('next' , $next);
	$smarty -> assign('filter',$filter);
	$smarty -> assign('page_count',   $filter['page_count']);
	$smarty -> assign('record_count',$result_count);
    $smarty -> assign('res',$result);
    $smarty -> display('py_yb.htm');
}
/*------------------------------------------------------ */
//-- 排序、分页、查询
/*------------------------------------------------------ */
elseif ($_REQUEST['act'] == 'query')
{
    $now_time = $_COOKIE['search_time'];
    $start = ($_POST['page'] - 1)*10;
    $result = $pc -> get_py_list($now_time,THIRTY_DAY,$start,10);
    $filter['record_count'] = $result[1];
    $filter = page_and_size($filter);
    $record_count = $result[1];
    $result[0] = array_reverse($result[0]);
    $tpl ='py_list.htm';

    $smarty -> assign('filter',       $filter);
    $smarty -> assign('record_count', $filter['record_count']);
    $smarty -> assign('page_count',   $filter['page_count']);
    $smarty -> assign('record_count',$record_count);
    $smarty -> assign('res',$result[0]); 
    make_json_result($smarty->fetch($tpl), '',array('filter' => $filter, 'page_count' =>$filter['page_count']));
}





?>