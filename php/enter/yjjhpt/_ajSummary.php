<?php


/** 汇总,小时表,分钟日志表,客户端每隔5分钟请求一次
  *
  * 所需处理的事务
  * [分钟日志] => 记录分钟日志
  * [清除离线] => 清除过期的SESSION,每隔多长时间清除一次
  * [小时日志] => 记录小时日志
  * [每日汇总] => <用户汇总><当日全服汇总>
  */

//只允许POST请求
if($_SERVER['REQUEST_METHOD'] != 'POST') exit('403');

require_once './ptcommon.php';
//error_reporting(E_ALL);

if(!isset($_SCOOKIE['summary'])) $_SCOOKIE['summary'] = 0;

//如果已经掉线或者请求间隔小于允许,那么直接PASS掉,预设请求间隔为 <5分钟 [ PT_UPDATEMIN=5*60 ] >
if($_SG['timestamp'] - intval($_SCOOKIE['summary']) < PT_UPDATEMIN) sexit('400');

dbconn_pt();
$ptsummary = new pt_summary;


//----------------------------------------------------------------------------------------
//分钟日志 <检查频率: 预设1分钟>
$minlogfile = S_ROOT.'./data/timerfile/minlog.log';
if(file_exists($minlogfile) && date("i") == date("i",filemtime($minlogfile))){
  //不执行

}else{
  if($ptsummary->ckMinlog()) swritefile($minlogfile,'.');

  //----------------------------------------------------------------------------------------
  //清除离线 <检查频率: 预设5分钟>
  $sesslogfile = S_ROOT.'./data/timerfile/sesslog.log';
  if(!file_exists($sesslogfile)) swritefile($sesslogfile,'-');
  if($ptsummary->timestamp - filemtime($sesslogfile) >= PT_SESSDEL_LIFE){

    //清理离线用户 <清除条件: 预设5分钟未响应>
    $ptsummary->clearSession();
    swritefile($sesslogfile,'-');

    $H = date("H");

    //---------------------------------------------------------------------------------------------
    //小时日志 <流失率,小时日志>
    $hourlogfile = S_ROOT.'./data/timerfile/hourlog.log';
    if(file_exists($hourlogfile) && date("H",filemtime($hourlogfile)) == $H){
      //不执行

    }elseif( !$ptsummary->existHourlog($_SG['timestamp']) ){
      dbconn_game();
      $ptsummary->getLossLevelRec();
      $hourSetArr = array(
                    'count_wbuser'  => $ptsummary->countWbuser(),
                    'count_accuser' => $ptsummary->countAccuser(),
                    'count_online'  => $ptsummary->countOnline(),
                    'total_notloss' => $ptsummary->total_notloss,
                    'total_mayloss' => $ptsummary->total_mayloss,
                    'total_isloss'  => $ptsummary->total_isloss,
                    'logtime'       => $ptsummary->timestamp,
                    );
      dbconn_pt();
      $sql = "SELECT count( * ) as total , lineid FROM yjjhpt_session GROUP BY lineid order by lineid asc  ";
      $query = $db->query($sql);
      $linelogSetArr = array();
      while($rs = $db->fetch_array($query)){
        $linelogSetArr[$rs['lineid']] = $rs['total'];
        }

      $nowtime = mktime($H,0,0);
      $hourlogmtime = file_exists($hourlogfile) ? filemtime($hourlogfile):$_SG['timestamp'];
      while(1){
        //记录时间
        $hourSetArr['hour']  = $nowtime;
        $ptsummary->writeHourlog($nowtime,$hourSetArr,$linelogSetArr);
        if($nowtime < 1 || date("YmdH",$nowtime) == date("YmdH",$hourlogmtime)) break;
        $nowtime -= 3600;
        }

       swritefile($hourlogfile,'.');
      }

    //-----------------------------------------------------------------------------------------------------------

    //汇总进行, <预设: 每5分钟>,每天晚上22点到翌日9点进行汇总[如果没有访客访问将无法汇总]
    if($H >= 22 || $H <= 10){

      //指针文件,今天全服日志是否创建了
      $todaylogfile = S_ROOT.'./data/timerfile/todaylog.log';
      if(file_exists($todaylogfile) && date("Y-m-d",filemtime($todaylogfile)) == $ptsummary->curdate){
        //不执行

        }elseif($db->result_first("select 1 from @#__gamedaylog where `date`='".$ptsummary->curdate."' limit 1 ") != 1){
          //创建成功,标志今天的全服日志已经创建了,无需再次检测是否创建
          if($ptsummary->writeGamedaylog($ptsummary->curdate))  swritefile($todaylogfile,'.');
        }

      //今天是否需要检测汇总
      $noneedsummaryfile = S_ROOT.'./data/timerfile/noneedsumary.log';
      if(file_exists($noneedsummaryfile) && date("Y-m-d",filemtime($noneedsummaryfile)) == $ptsummary->curdate){
        //今天不需要进行汇总了,不执行

      }elseif($ptsummary->existNonSumGamedaylog()){

        //汇总用户日志,先汇总未汇总的用户,再汇总每日日志
        if($ptsummary->existNonSumUserdaylog()){
          $ptsummary->summaryUserdaylog();
         }else{
          $ptsummary->summaryGamedaylog();
         }
      }else{
        //标志今天没有需要汇总的日志了
        swritefile($noneedsummaryfile,'.');
      }
    }
  }

  //----------------------------------------------------------------------------------------
  //每10分钟执行一次reglog[注册日志]
  $reglogfile = S_ROOT.'./data/timerfile/reglog.log';
  if(file_exists($reglogfile) && $ptsummary->timestamp - filemtime($reglogfile) < 10*60){
    //不执行

    }else{
      if($ptsummary->writeReglog()) swritefile($reglogfile,'.');
    }


  //每10分钟执行一次levellog[等级日志]
  $levellogfile = S_ROOT.'./data/timerfile/levellog.log';
  if(file_exists($levellogfile) && $ptsummary->timestamp - filemtime($levellogfile) < 10*60){
    //不执行

    }else{
      if($ptsummary->writeLevellog()) swritefile($levellogfile,'.');
    }

  //----------------------------------------------------------------------------------------
}


$db->close();
ssetcookie('summary',$_SG['timestamp']);
sexit('200');
?>