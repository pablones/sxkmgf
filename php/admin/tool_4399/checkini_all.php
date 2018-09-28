<?php
	set_time_limit(300);
	require_once dirname(dirname(dirname(__FILE__))).'./common.php';

	$arr_servers=array(
		'4399',
		'4399-m1',
		'4399-m2',
		'91wan',
		'91wan-m1',
		'kuaiwan',
		'kuaiwan-m1',
		'37wan',
		'37wan-m1',
		'360',
		'360-m1',
		'aosheng',
		'aosheng-m1',
		'kuwo',
		'kuwo-m1',
		'IS',
		'IS-m1',
		'woyo',
		'woyo-m1',
		'fengxing',
		'fengxing-m1',
		'PPTV',
		'PPTV-m1',
		'05wan',
        '05wan-m1',
		'FL',
        'FL-m1',
	);
	
	
	$filename = 'Test.ini';
	$filepath = S_ROOT.'./admin/'.$filename;
	if(!file_exists($filepath)) swritefile($filepath,"");
	$pwd=$_POST['psw'];
	$begintime=time();
	$str="";

	foreach($arr_servers as $site)
	{
		$arr_servernum[$site]=gethttpcnt('http://www.yujiangame.com:8099/servernum-ini.php?s='.$site,'','',1);
		$arr_siteid=$_POST['siteid'.$site];
		for($i=1;$i<=$arr_servernum[$site];$i++){
			switch($site)
			{
				case '4399':$surl[$site][$i]="http://yjjh".$i."-s.my4399.com/admin/tool_4399/show.php";break;
				case '4399-m1':$surl[$site][$i]="http://yjjh".$i."-m1.my4399.com/admin/tool_4399/show.php";break;
				case '4399-m2':$surl[$site][$i]="http://yjjh".$i."-m2.my4399.com/admin/tool_4399/show.php";break;
				case '91wan':$surl[$site][$i]="http://yjjh".$i."-s.91wan.com/admin/tool_4399/show.php";break;
				case '91wan-m1':$surl[$site][$i]="http://yjjh".$i."-m1.91wan.com/admin/tool_4399/show.php";break;
				case 'kuaiwan':$surl[$site][$i]="http://yjjh".$i."-s.kuaiwan.com/admin/tool_4399/show.php";break;
				case 'kuaiwan-m1':$surl[$site][$i]="http://yjjh".$i."-m1.kuaiwan.com/admin/tool_4399/show.php";break;
				case '37wan':$surl[$site][$i]="http://s".$i.".yjjh.37wan.com/admin/tool_4399/show.php";break;
				case '37wan-m1':$surl[$site][$i]="http://s".$i."-m1.yjjh.37wan.com/admin/tool_4399/show.php";break;
				case '360':$surl[$site][$i]="http://s".$i.".yjjh.wan.360.cn/admin/tool_4399/show.php";break;
				case '360-m1':$surl[$site][$i]="http://s".$i."-m1.yjjh.wan.360.cn/admin/tool_4399/show.php";break;
				case 'aosheng':$surl[$site][$i]="http://yjjh".$i."-s.kx1d.com/admin/tool_4399/show.php";break;
				case 'aosheng-m1':$surl[$site][$i]="http://yjjh".$i."-m1.kx1d.com/admin/tool_4399/show.php";break;
				case 'kuwo':$surl[$site][$i]="http://s".$i.".yjjh.kuwo.cn/admin/tool_4399/show.php";break;
				case 'kuwo-m1':$surl[$site][$i]="http://s".$i."-m1.yjjh.kuwo.cn/admin/tool_4399/show.php";break;
				case 'IS':$surl[$site][$i]="http://s".$i.".yjjh.iugame.cn/admin/tool_4399/show.php";break;
				case 'IS-m1':$surl[$site][$i]="http://s".$i."-m1.yjjh.iugame.cn/admin/tool_4399/show.php";break;
				case 'woyo':$surl[$site][$i]="http://yjjh".$i."-s.web.woyo.com/admin/tool_4399/show.php";break;
				case 'woyo-m1':$surl[$site][$i]="http://yjjh".$i."-m1.web.woyo.com/admin/tool_4399/show.php";break;
				case 'fengxing':$surl[$site][$i]="http://s".$i.".yjjh.funshion.com/admin/tool_4399/show.php";break;
				case 'fengxing-m1':$surl[$site][$i]="http://s".$i."-m1.yjjh.funshion.com/admin/tool_4399/show.php";break;
				case 'PPTV':$surl[$site][$i]="http://s".$i.".yjjh.g.pptv.com/admin/tool_4399/show.php";break;
				case 'PPTV-m1':$surl[$site][$i]="http://s".$i."-m1.yjjh.g.pptv.com/admin/tool_4399/show.php";break;
				case '05wan':$surl[$site][$i]="http://s".$i.".yjjh.05wan.com/admin/tool_4399/show.php";break;
				case '05wan-m1':$surl[$site][$i]="http://s".$i."-m1.yjjh.05wan.com/admin/tool_4399/show.php";break;
				case 'FL':$surl[$site][$i]="http://yjjh".$i."-s.fy5.com/admin/tool_4399/show.php";break;
				case 'FL-m1':$surl[$site][$i]="http://yjjh".$i."-m1.fy5.com/admin/tool_4399/show.php";break;
			}
//			$url123=str_replace("show.php","show1.php",$surl[$site][$i]);
////			$url1234="http://yjjh1-s.my4399.com/admin/tool_4399/show1.php";
//			$echo1=gethttpcnt($url123);
//			$echo1=unserialize($echo1);
//			print_r($echo1);
			if(!empty($arr_siteid) && in_array($i,$arr_siteid)){
				$url=str_replace("show.php","checkini.php?psw=".$pwd."&site=".$site,$surl[$site][$i]);
                $ret=gethttpcnt($url,'','',1);
				if($ret!=1){
					$tmpstr.=$site."  ".$i."服、";
				}
			
			}
		}
	}
	if(!empty($_POST)){
		if(empty($tmpstr)){
			$str="所选服务器全部更新成功！";
		}
		else{
			$str="服务器".$tmpstr."更新失败";
		}
	}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>更新ini</title>
<style type="text/css">
<!--
body {	font-family: Arial, Helvetica, sans-serif;	font-size: 17px;	margin: auto;	width: 1100px;}
h1 {	font-size: 19px;}
ul li {	margin: 5px;}
th,td {	font-size: 17px;	border: 1px solid #EEEEEE;	padding: 5px;}
th {	background-color: #EEE;	text-align: left;}
#form1 #psw {	font-size: 17px;}
-->
</style>
<script type="text/javascript">
//checkback为设置反选

function checkback(oj)
{
	for (var i=0;i<oj.length;i++ )
	{
		if (oj[i].checked)
		{
			oj[i].checked=false;
		}
		else
		{
			oj[i].checked=true;
		}
	}
}
</script>
</head>

<body>
<h1>御剑江湖-ini配置文件 自动更新：</h1>
<form id="form1" name="form1" method="post" action="checkini_all.php">
<table width="80%" border="1">
  <tr>
    <th width="120" nowrap="nowrap">服务器:</th>
    <td><table><?php
	foreach($arr_servers as $site){
		echo "<tr><th colspan='10'>".$site."<input type=\"checkbox\" onclick=\"checkback(document.getElementsByName('siteid".$site."[]'));\" /></th></tr>";
		for($i=1;$i<=$arr_servernum[$site];$i++)
		{
			if($i%10==1)
			{
				echo "<tr>";
			}
			echo '<td><input type="checkbox" name="siteid'.$site.'[]" value="'.$i.'"/>'.$i.'服<a href="'.$surl[$site][$i].'" target="_blank"><span style="color:red">日志</span></a></td>';
//			echo '<td><input type="checkbox" name="siteid'.$site.'[]" value="'.$i.'"/>'.$i.'服<a href="'.$surl[$site][$i].'" target="_blank"><span style="color:red">日志</span></a><'.$echo1.'></td>';

			echo $echo1.'</td>';


			if($i%10==0)
			{
				echo "</tr>";
			}
		}
	}
	?></table></td>
  </tr>  <tr>
    <th nowrap="nowrap">手动更新版本</th>
    <td>
      <table width="100%" border="1">
        <tr>
          <td width="60">密码</td>
          <td><label>
            <input name="psw" type="text" id="psw" />
            <input type="submit" name="submit" value="提交" />
            <input name="formhash" type="hidden" id="formhash" value="<?php echo formhash()?>" />
          </label>
          <em>版本文件txt <a href="<?php echo $filepath?>" target="_blank"><?=$filename?></a></em>
          </td>
          </tr>
      </table>
      </td>
  </tr>
</table></form>
<?php
	$endtime=time();
	$runtime="所用时间为：".($endtime-$begintime)."秒<br/>";
	echo $runtime.$str;
?>
</body>
</html>