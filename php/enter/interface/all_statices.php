<?php
/**
 * 全平台统计
 * @author King.L
 */

define('IN_ECS', true);
require( '../includes/init.php');

$where = getIp($_CFG['allowIpArr']);
$sid = $_REQUEST['sid'];
$bdate = $_REQUEST['bdate'];
$edate = $_REQUEST['edate'];
$btime = date('Y-m-d',$bdate);
$etime = date('Y-m-d',$edate);

$sql = "SELECT COUNT(logid)  AS count_paylog,SUM(paygold)  AS sum_paygold,SUM(paymoney) AS sum_paymoney,MAX(paygold)  AS max_paygold,MAX(paymoney) AS max_paymoney,
MIN(paygold)  AS min_paygold,MIN(paymoney) AS min_paymoney,ROUND(SUM(paygold)/COUNT(logid),0) AS avg_paygold,ROUND(SUM(paymoney)/COUNT(logid),2) AS avg_paymoney FROM ".TBL_SK_PAYLOG.
" where logdate>'$bdate' and logdate<'$edate' and ".$where;
$rs = $GLOBALS['acc_db'] -> getRow($sql);
       
$sql = "select count(ID) as count_uselog,sum(Spend_VAS_Point) as sum_uselog,max(Spend_VAS_Point) as max_uselog,min(Spend_VAS_Point) as min_uselog,
ROUND(sum(Spend_VAS_Point)/count(ID),0) as avg_uselog from ".TBL_SER_CONSUMELOG_TABLE." where writeTime>'$btime' and writeTime<'$etime' ";
$res = $GLOBALS['acc_db'] -> getRow($sql);

$result = array(
	'rs' => $rs,
	'res' => $res,
	'sid' => $sid,
);
$result = serialize($result);
echo $result;
?>