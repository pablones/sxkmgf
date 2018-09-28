<?php

require_once '../common.php';
?>
<pre>
<?php
$filearr = array('minlog','sesslog','hourlog','levellog','reglog','todaylog','noneedsummary');
foreach($filearr as $val){
  $val .= '.log';
  $file = S_ROOT.'./data/timerfile/'.$val;
  echo $val."创建时间： --> ";
  if(file_exists($file)){
    echo date("Y-m-d H:i:s",filemtime($file));
    }else{
      echo "该文件尚未创建！";
    }
  echo "\r\n----------------------------------------\r\n";
  }

?>
</pre>