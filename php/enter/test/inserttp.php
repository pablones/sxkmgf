<?PHP
define('IN_ECS', true);
require('../includes/init.php');
//require(dirname(__FILE__) . '/includes/init.php');
set_time_limit(0);



$filename='yjjhp.txt';

$file_handle = fopen($filename, "r"); 
if($file_handle){
	while (!feof($file_handle)) { 
	    $card=trim(fgets($file_handle));
	    if(!empty($card)){
		    $line.= ",".$card; 
	    }
    } 
}

$yjjhp= explode(',',$line); 
fclose($file_handle);


$filename='4399.txt';
$line='';
$file_handle = fopen($filename, "r"); 
if($file_handle){
	while (!feof($file_handle)) { 
	    $card=trim(fgets($file_handle));
	    if(!empty($card)){
		    $line.= ",".$card; 
	    }
    } 
}
$y4399= explode(',',$line);
fclose($file_handle);


echo count($y4399);
echo '<p>';
echo count($yjjhp);
echo '<p>';
//foreach ($yjjhp as $k => $v){
//	echo $v;
//	echo "<p>";
//	foreach ( $y4399 as $key => $value){
//		$flg=0;
//		if($v == $vaule){
//			$flg=1;
//			break;
//		}
//	}
//	if($flg==0){
//		$ex[]=$v;
//	}
//
//}

print_r(array_diff($yjjhp,$y4399));
echo '<p>';
print_r(array_diff($y4399,$yjjhp));



?>