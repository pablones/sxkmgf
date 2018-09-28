<?


$arr = array(
		'maomao1t' => array(0001,75890863),
        'mysql3301'=> array( 2914 , 82398774),
        'mysql3305'=> array( 4655 ,	82849682),
        'mysql3306'=> array( 0029 , 75890863),
        'mysql3307'=> array( 1320 , 80683061),
        'mysql3308'=> array( 15404, 80967256),
        'mysql3309'=> array( 8508 ,	80967363),

        //以下存放在测试网数据库
        'mysql9905'=> array( 301435,86493917),
        );

  require_once '../common.php';
  $un = trim($_GET['un']);
  if(!array_key_exists($un,$arr)) $un = 'maomao1t';
  $serverid = 'S2';
  $debug = trim($_GET['debug']) == 'yes' ? '.debug':'';

    $pm = authcode($arr[$un][1],'ENCODE');
    $arr2 = array(
           'userid'   => $arr[$un][0],
           'username' => $un,
           'pm'       => $pm,
           'fromlogin'=> 'yes',
           );

    ssetcookie("timestamp",$GLOBALS['_SG']['timestamp']);
    ssetcookie("userid",$arr[$un][0]);
    ssetcookie("fromlogin",'yes'); //是登陆而不是刷新
    ssetcookie("4399uid",$pm ); //加密
    ssetcookie("username",$un);
    ssetcookie("serverid",$serverid);
    foreach($arr2 as $key => $val){
      ssetcookie('u'.$arr[$un][0]."[$key]",$val);
      }
    jump("../4399/play".$debug.".php?username=".$un.'&userid='.$arr[$un][0]);
    exit();
    ?>