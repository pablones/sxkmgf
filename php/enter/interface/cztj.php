<?php
/**
 * 充值统计
 * @author sun.D
 */

define('IN_ECS', true);
require( '../includes/init.php');
date_default_timezone_set('PRC');//设置时区为北京时间

$where = getIp($_CFG['allowIpArr']);
$sid = $_REQUEST['sid'];
$bdate = $_REQUEST['bdate'];
$edate = $_REQUEST['edate'];
$time_lang=$edate-$bdate;
$res=getOneMonthTime($edate,$time_lang);

for($i=0;$i<count($res);$i++){
    $date=$res[$i];
    $sql="select sum(paymoney) as summoney from   ".TBL_SK_PAYLOG."  where paygold>0  and `logdate`>=$date and `logdate`<$date+86400 and ($where) ";      
    $row1=$acc_db -> getOne($sql);
    $sql="select sum(paymoney) as summoney from   ".TBL_SK_PAYLOG."  where paygold=0  and `logdate`>=$date and `logdate`<$date+86400 and ($where) ";      
    $row2=$acc_db -> getOne($sql);
    $row=$row1+($row2/2);
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