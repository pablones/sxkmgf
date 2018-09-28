<?php

require_once dirname(dirname(dirname(__FILE__))).'./common.php';

$filename = "Result.txt";
//print_r ($filename);
$filepath = S_ROOT.'./admin/LOG/'.$filename;
//echo sreadfile($filepath);
$print = sreadfile($filepath);
print nl2br($print);