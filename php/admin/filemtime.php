<?php

require_once '../common.php';
?>
<pre>
<?php
$filearr = array('minlog','sesslog','hourlog','levellog','reglog','todaylog','noneedsummary');
foreach($filearr as $val){
  $val .= '.log';
  $file = S_ROOT.'./data/timerfile/'.$val;
  echo $val."����ʱ�䣺 --> ";
  if(file_exists($file)){
    echo date("Y-m-d H:i:s",filemtime($file));
    }else{
      echo "���ļ���δ������";
    }
  echo "\r\n----------------------------------------\r\n";
  }

?>
</pre>