<?php

require_once dirname( dirname(__FILE__) ).'./common.php';

dbconn_game();

$sql="select `level`,count(*) as user_num from wb_user group by `level`";
echo $sql;

$rs=$db->query($sql);

if($db->num_rows($rs))
{
	while($row=$db->fetch_array($rs))
	{
		$level[$row['level']]=$row['user_num'];
	}
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>玩家等级查询</title>
<style>
th{ background-color:#999}
</style>
</head>

<body>
<table border="1" width="30%" align="center">
<tr><th>等级</th><th>人数</th></tr>
<?php
	foreach($level as $key => $value)
	{
		echo "<tr align='center'>";
		echo "<td>".$key."</td><td>".$value."</td>";
		echo "</tr>";
	}
?>
</table>
</body>
</html>
