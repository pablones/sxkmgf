<?php

/*
 +---------------------------------------------------
 *  查询充值汇总
 +---------------------------------------------------
 *
 *
 +--------------------------------------------------
 */


if(isset($_SERVER['PHP_AUTH_USER']) && $_SERVER['PHP_AUTH_USER'] == 'yjjh-xzj-2010' && $_SERVER['PHP_AUTH_PW'] == 'youyue-yjjh-2010'){

  }else{
    header('WWW-Authenticate: Basic realm="xzj"');
    header('HTTP/1.0 401 Unauthorized');
    echo '请输入密码';
    exit();
  }

require_once dirname(dirname(dirname(__FILE__))).'./common.php';
/*
$_SC['dbhost']  		= '115.238.101.102';
$_SC['dbuser']  		= 'server-phproot';   //用户 root server-phpadm
$_SC['dbpw'] 	 		  = 'fuzhaobin139XX9901'; //密码 test

if($db) unset($db);
*/
dbconnect();



$where  = " WHERE ip='121.10.143.59' OR ip='115.182.52.223' ";

/*
//查询充值总次数的！
$sql    = "SELECT COUNT(*) FROM sk_paylog $where ";
$count_paylog = $db->result_first($sql);

//查询充值总额(RMB)
$sql    = "SELECT SUM(paymoney) FROM sk_paylog $where ";
$sum_paymoney = $db->result_first($sql);

//查询充值总元宝数
$sql    = "SELECT SUM(paygold)  FROM sk_paylog $where ";
$sum_paygold  = $db->result_first($sql);
*/

//查询最大的充值元宝数
$sql    = "SELECT
                  COUNT(logid)  AS count_paylog,
                  SUM(paygold)  AS sum_paygold,
                  SUM(paymoney) AS sum_paymoney,
                  MAX(paygold)  AS max_paygold,
                  MAX(paymoney) AS max_paymoney,
                  MIN(paygold)  AS min_paygold,
                  MIN(paymoney) AS min_paymoney,
                  SUM(paygold)/COUNT(logid)   AS  avg_paygold,
                  SUM(paymoney)/COUNT(logid)  AS  avg_paymoney
          FROM sk_paylog $where ";
$rs     = $db->fetch_first($sql);

$sql	= "select count(ID) as count_uselog,sum(Spend_VAS_Point) as sum_uselog,max(Spend_VAS_Point) as max_uselog,min(Spend_VAS_Point) as min_uselog,sum(Spend_VAS_Point)/count(ID) as avg_uselog from ser_consumelog_table";

$res 	=$db->fetch_first($sql);


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>元宝充值小汇总（官网实时）</title>
<link href="style/global.css" rel="stylesheet" type="text/css" />
</head>

<body>
<div id="content">
<h3>官网账号服务器账号数据库实时充值小汇总
  {<span class="red"> <?php echo $_SC['dbhost']; ?> </span> }  <em><a href="javascript:;" onclick="window.location.reload();">刷新本页</a></em></h3>
 <table width="450" border="1" summary="" class="tbBorder">
  <tr>
    <th width="250">总充值次数</th>
    <td><?php echo $rs['count_paylog']?></td>
    </tr>
  <tr>
    <th>总充入金额（人民币）</th>
    <td><?php echo $rs['sum_paymoney']?></td>
    </tr>
  <tr>
    <th>总充入元宝数</th>
    <td><?php echo $rs['sum_paygold']?></td>
    </tr>
  <tr>
    <th>平均每次充入人民币</th>
    <td><?php echo number_format($rs['avg_paymoney'],2)?></td>
    </tr>
  <tr>
    <th>平均每次充入元宝数</th>
    <td><?php echo number_format($rs['avg_paygold'],2)?></td>
    </tr>
  <tr>
    <th>最大一笔充入金额</th>
    <td><?php echo $rs['max_paymoney']?></td>
    </tr>
  <tr>
    <th>最大一笔充入元宝数</th>
    <td><?php echo $rs['max_paygold']?></td>
    </tr>
  <tr>
    <th>最小一笔充入金额</th>
    <td><?php echo $rs['min_paymoney']?></td>
    </tr>
  <tr>
    <th>最小一笔充入元宝数</th>
    <td><?php echo $rs['min_paygold']?></td>
    </tr>
</table><br />
<table width="450" border="1" summary="" class="tbBorder">
  <tr>
    <th width="250">总消费元宝数</th>
    <td><?php echo $res['sum_uselog']?></td>
    </tr>
  <tr>
    <th>平均每次消费元宝数</th>
    <td><?php echo $res['avg_uselog']?></td>
    </tr>
  <tr>
    <th>最大一次消费元宝数</th>
    <td><?php echo $res['max_uselog']?></td>
    </tr>
  <tr>
    <th>最小一次消费元宝数</th>
    <td><?php echo $res['min_uselog']?></td>
    </tr>
  </table>
</div>
<!-- <hr /> -->
<ul>
  <li><a href="http://web.4399.com/yjjh/" target="_blank">官网首页</a></li>
  <li><a href="http://yjjh1-bak.my4399.com/" target="_blank">运营平台</a>(新的)</li>
  <li><a href="javascript:;" onclick="alert('由于旧版方案已经被废弃，旧版平台已停！请登录新平台进行操作！');">运营平台</a>(旧的)</li>
</ul>
<hr />
<p class="rt">本次查询时间<u><?php echo date("Y-m-d H:i:s")?></u>，更多详情请登录<a href="http://yjjh1-bak.my4399.com" target="_blank">运营平台查看&raquo; </a> </p>
</body>
</html>
