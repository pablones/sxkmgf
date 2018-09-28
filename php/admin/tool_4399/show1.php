<?php

require_once dirname(dirname(dirname(__FILE__))).'./common.php';

$filename = "1.txt";
$filepath = S_ROOT.'./admin/LOG/'.$filename;
$str = file_get_contents($filepath);
$str = serialize($str);
echo $str;
exit;
