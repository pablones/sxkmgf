<?php

require_once dirname(dirname(dirname(__FILE__))).'./common.php';


$rs = array();
$username = '';
if(submitcheck()){
  $username = shtmlspecialchars( trim( $_POST['username'] ) );
  if(empty($username)) echo jump('?'.$_SG['timestamp'],'用户名不能为空！');

  dbconnect();

  $sql = "select * from account where `name`='$username' limit 1 ";
  $rs = $db->fetch_first($sql);
  if(!$rs) echo jump('?'.$_SG['timestamp'],'该账号不存在！');
  }

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>密码查询</title>
<link href="style/global.css" rel="stylesheet" type="text/css" />
</head>

<body>
<div id="content">
<h3>御剑江湖&lt;账号数据库 &gt; &raquo;账号密码查询[1服官网实时] 数据库{<span class="red"> 115.238.101.102 </span> }</h3>
<form id="form1" name="form1" method="post" action="">
 <table width="450" border="1" summary="" class="tbBorder">
    <tr>
      <th width="120" nowrap="nowrap">4399账号名</th>
      <td><label>
        <input name="username" type="text" id="username" value="<?php echo $username?>" />
      </label></td>
    </tr>
    <?php if($rs['password']){ ?>
    <tr>
      <th width="120" nowrap="nowrap">查询到的密码</th>
      <td><input name="password" id="password" value="<?php echo $rs['password']?>" /></td>
    </tr>
    <?php } ?>
    <tr>
      <th nowrap="nowrap">&nbsp;</th>
      <td><label>
        <input type="submit" name="submit" value="提交" />
        <input name="formhash" type="hidden" id="formhash" value="<?php echo formhash()?>" />
      </label></td>
    </tr>
  </table>
</form>
</div>
<div id="footer"></div>
</body>
</html>
