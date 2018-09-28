<?php




//只允许POST请求
if($_SERVER['REQUEST_METHOD'] != 'POST' || !isset($_POST['lineid']) || !isset($_POST['u'])) exit('403');

$lineid = intval($_POST['lineid']);
$lineArr = array(1,2,3,4);
if(!in_array($lineid,$lineArr)) exit('403');

require_once './ptcommon.php';
dbconn_pt();

$ptuser = new pt_user;

//换线
$ptuser->changeLine($lineid);
sexit('200');
?>