<?php
/**
 * 消费统计
 * @author sun.D
 */

define('IN_ECS', true);
require( '../includes/init.php');
date_default_timezone_set('PRC');

$where = getIp($_CFG['allowIpArr']);
$sid = $_REQUEST['sid'];
$bdate = $_REQUEST['bdate'];
$edate = $_REQUEST['edate'];
$time_lang=$edate-$bdate;
$res=getOneMonthTime($edate,$time_lang);

for($i=0;$i<count($res);$i++){
    $date=date('Y-m-d',$res[$i]);
    $edate=date('Y-m-d',$res[$i]+3600*24);
    $sql = "select sum(Spend_VAS_Point) as sum_uselog from ".TBL_SER_CONSUMELOG_TABLE." where writeTime>'$date' and writeTime<'$edate'  ";
    $row=$acc_db -> getOne($sql);
    $time=date('Y-m-d',$res[$i]);
    if(!empty($row)){
        $info[$time]=$row; 
    }else{
    	$info[$time]=0;
    }
}	
if(!empty($info)){
    $info=array_reverse($info); 
}

$result = $info;

//print_r($result);
//echo '<br>'.array_sum($info);
//exit;
$result = serialize($result);
echo $result;


function getOneMonthTime($now_time,$time_rang){
    $start_time = $now_time - $time_rang - ONE_DAY;
   
    $tem = $start_time + ONE_DAY;
    while($tem <= $now_time){
        $result[] = $tem;
        $tem += ONE_DAY;
    }
    return $result;
}

?>