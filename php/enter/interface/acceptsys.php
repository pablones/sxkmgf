<?php
define('IN_ECS', true);
require('../includes/init.php');

$coflist = $_REQUEST['coflist'];
$coflist = stripslashes($coflist);
$coflist = unserialize($coflist);

foreach($coflist as $key => $value){
	$sql = "select * from ".TBL_SYS_CONFIG." where code = '".$value['code']."'";
	$result = $acc_db -> getRow($sql);
	if(!$result){
		$sql = "insert into ".TBL_SYS_CONFIG."(`parent_id`,`code`,`type`,`value`)values('".$value['parent_id']."','".$value['code']."','".$value['type']."','".$value['value']."')";	
		$acc_db -> query($sql);
	}
}

exit("ok");

?>