<?
$key = '4399::XXQY::pay::frhSQBdJJmdnnUBZ';
$PayNum = date("YmdHis").rand(1000,9999);
$PayGold = 1209;
$PayMoney = 500;
$PayToUser = '龍龙abc123';
$ServerName = 'S2';
$Mode = 1;
$PayTime = time();
//md5($this->key.$this->Mode.$this->PayNum.$this->PayToUser.$this->PayMoney.$this->PayGold.$this->PayTime.$this->ServerName);
$ticket = md5($key.$Mode.$PayNum.$PayToUser.$PayMoney.$PayGold.$PayTime.$ServerName);


$url = 'http://www.youyuegame.com:8099/4399/pay_modes.php?PayNum=201008171704237417&Mode=1&PayToUser=%E9%BE%8D%E9%BE%99abc123&PayGold=100
&PayMoney=10&PayTime=1282035863&ServerName=S1&ticket=60cda1bf1dcc21f2525825e74e14c232';

$apiurl = 'http://www.youyuegame.com:8099/4399/pay_modes.php?a';
$apiurl .= '&PayNum='.$PayNum;
$apiurl .= '&Mode='.$Mode;
$apiurl .= '&PayToUser='.$PayToUser;
$apiurl .= '&PayGold='.$PayGold;
$apiurl .= '&PayMoney='.$PayMoney;
$apiurl .= '&PayTime='.$PayTime;
$apiurl .= '&ServerName='.$ServerName;
$apiurl .= '&ticket='.$ticket;

echo '<a href="'.$apiurl.'">'.$apiurl.'</a>';
?>


