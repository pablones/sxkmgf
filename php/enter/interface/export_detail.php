<?php
/**
 * 导出充值明细
 * @author DXJ
 */

define('IN_ECS', true);
require( '../includes/init.php');
date_default_timezone_set('PRC');

$where = getIp($_CFG['allowIpArr']);
$sid = $_REQUEST['sid'];
$bdate = $_REQUEST['bdate'];
$edate = $_REQUEST['edate'];

//echo $sql="select paynum, logdate, userid, ip,serverid,paygold, paymoney from   ".TBL_SK_PAYLOG."  where `logdate`>'$bdate' and `logdate`<'$edate' and ($where) ";     
$sql="select paynum, logdate, userid, ip,serverid,paygold, paymoney,serverid from   ".TBL_SK_PAYLOG."  where  `logdate`>'$bdate' and `logdate`<'$edate' and ($where) ";    
$row=$acc_db -> getAll($sql);
if(!empty($row)){
    foreach ($row as $key => $value){
	    $sql="select name from ".TBL_ACCOUNT." where id='$value[userid]' ";
	    $row[$key]['username']=$acc_db ->getOne($sql);
    }
}

if(!empty($row)){
    foreach ($row as $key => $rs){
      if($rs[paygold]==0){
      	$rs[paymoney]=$rs[paymoney]/2;
      }
      $result.="<tr align='center'>";
      $result.="<td>".date('Y-m-d H:i:s',$rs[logdate])."</td>";
      $result.="<td style='vnd.ms-excel.numberformat:@'>".$rs[paynum]."</td>";
      $result.="<td>".$rs[ip]."</td>";
      $result.="<td>".$rs[logdate]."</td>";
      $result.="<td>".$rs[username]."</td>";
      $result.="<td>".$rs[paymoney]."</td>";
      $result.="<td>".$rs[paygold]."</td>";
      $result.="<td>".$_CFG['s_name']."</td>";
      $result.="<td>".$rs[serverid]."</td>";
      $result.="</tr>";
      
    }
}

$result = serialize($result);
echo $result;




?>