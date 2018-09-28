<?php
/**
 * ╥юЁацт
 */
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');
require(dirname(__FILE__) . '/class/fc_class.php');


$id =isset($_POST['card'])?$_POST['card'] :'';
$id = trim($id);
$fc = new fc_class($id);
echo $fc -> is_idcard();


?>