<?php

if(!defined('IN_ZBPHP'))
{
	exit('Access Denied');
}


//汇总报告
class pt_summary {

  public $count_online;       //统计瞬间 [在线人数]
  public $count_wbuser;       //统计瞬间 [wangpudb.wb_user记录数]
  public $count_accuser;      //统计瞬间 [acc_db.account记录数]

  public $daysum_paymoney;
  public $daysum_paygold;

  public $total_notloss;      //统计瞬间  [总-未流失]
  public $total_mayloss;      //统计瞬间  [总-可疑流失]
  public $total_isloss;       //统计瞬间  [总-已流失]

  public $timestamp;          //当前时间戳
  public $curdate;            //今日日期 2010-09-16

  public $lossLevelRec;       //每级别的流失率的汇总



  function __construct(){
    $this->count_online = $this->count_wbuser   = $this->count_accuser  = 0;
    $this->timestamp    = $GLOBALS['_SG']['timestamp'];
    $this->lossLevelRec = array();
    $this->total_notloss= $this->total_mayloss = $this->total_isloss = 0;
    $this->curdate      = date("Y-m-d");
    }

  //TB:@#__session
  function countOnline(){
    if($this->count_online == 0)  $this->count_online = $GLOBALS['db']->result_first("select count(*) from @#__session where unix_timestamp() - lastactive <= '".PT_SESSDEL_LIFE."' ");
    return $this->count_online;
    }

  //DB:wangpudb
  //TB:wb_user
  function countWbuser(){
    if($this->count_wbuser == 0){
      dbconn_game();
      $this->count_wbuser = $GLOBALS['db']->result_first("select count(*) from wb_user ");
      dbconn_pt();
      }
    return $this->count_wbuser;
    }

  //DB:acc_db
  //TB:account
  function countAccuser(){
    if($this->count_accuser == 0){
      dbconn_acc();
      $this->count_accuser = $GLOBALS['db']->result_first("select count(*) from account ");
      dbconn_pt();
      }
    return $this->count_accuser;
    }

  //TB:@#__minlog
  function ckMinlog(){
    $time = mktime(date("H"),date("i"),0);
    $sql  = "select 1 from @#__minlog where `time`='$time' limit 1 ";
    if($GLOBALS['db']->result_first($sql) == 1) return TRUE;
    $setarr = array(
              'time'          => $time,
              'count_online'  => $this->countOnline(),
              );
    return affectrow('minlog',$setarr);
    }

  //检测是否存在此分钟的日志
  function existMinlog($min_timestamp){
    return $GLOBALS['db']->result_first("select 1 from @#__minlog where `time`='$min_timestamp' limit 1 ");
    }

  //检测是否存在此小时的日志
  function existHourlog($hour_timestamp){
    return $GLOBALS['db']->result_first("select 1 from @#__hourlog where `hour`='$hour_timestamp' limit 1 ");
    }

  //检测是否存在当前小时的小时流失日志
  function existLosslog($hour_timestamp){
    return $GLOBALS['db']->result_first("select 1 from @#__losslog where `hour`='$hour_timestamp' limit 1 ");
    }

  //检测是否存在此日的日志
  function existGamedaylog($date){
    return $GLOBALS['db']->result_first("select logid from @#__gamedaylog where `date`='$date' limit 1");
    }

  function existLinelog($hour_timestamp){
    return $GLOBALS['db']->result_first("select 1 from @#__linelog where `hour`='$hour_timestamp' limit 1 ");
    }

  //检测是否存在未汇总的日志 gamedaylog
  function existNonSumGamedaylog(){
    return $GLOBALS['db']->result_first("select 1 from @#__gamedaylog where summary='0' and `date` < curdate() limit 1 ");
    }

  //检测是否存在未汇总的用户日志
  function existNonSumUserdaylog(){
    return $GLOBALS['db']->result_first("select 1 from @#__userdaylog where `summary`='0' and `date` < curdate() limit 1 ");
    }

  //创建全服某日日志
  function writeGamedaylog($date){
    $setarr = array(
              'date'      => $date,
              'logtime1'  => $this->timestamp,
              'logtime2'  => $this->timestamp,
              'summary'   => '0',
              );
    return affectrow('gamedaylog',$setarr);
    }

  //TB:@#__losslog
  function ckLosslog($hour_timestamp){

    }

  //TB:@#__hourlog
  //TB:@#__losslog
  function writeHourlog($hour_timestamp,$hourSetArr,$linelogSetArr){
    global $db;

    if(!$this->existHourlog($hour_timestamp)) affectrow('hourlog',$hourSetArr);
    if(!$this->existLosslog($hour_timestamp)){
      foreach($this->lossLevelRec as $key => $val){
        $setarr = array(
                  'hour'=> $hour_timestamp,
                  'level'=> $key,
                  'notloss'=> $val['notloss'],
                  'mayloss'=> $val['mayloss'],
                  'isloss'=> $val['isloss'],
                  'total'=> $val['total'],
                  );
        affectrow('losslog',$setarr);
        }
    }

   //@#__linelog
   if(!$this->existLinelog($hour_timestamp)){
     foreach($linelogSetArr as $key => $val){
        $setarr = array(
                  'hour'          =>  $hour_timestamp,
                  'count_online'  =>  $val,
                  'lineid'        =>  $key,
                  );
       affectrow('linelog',$setarr);
       }
     }
   }


  //DB:wangpudb
  //TB:wb_user
  function getLossLevelRec(){
    if(count($this->lossLevelRec) > 0) return $this->lossLevelRec;

    $tm2 = $this->timestamp - 3600*24*2;//24*2小时未登录算[疑似流失]
    $tm4 = $this->timestamp - 3600*24*4;//24*4小时未登录算[已经流失]

    $this->lossLevelRec   = array();
    $this->count_wbuser   = 0;

    $this->total_notloss  = $this->total_mayloss  = $this->total_isloss = 0;

    global $db;
    $sql = "select level,last_login,all_online_time from wb_user ";
    $query = $db->query($sql);
    while($rs = $db->fetch_array($query)){
        if(!array_key_exists($rs['level'],$this->lossLevelRec)){
          $this->lossLevelRec[$rs['level']] = array('mayloss' => 0, 'isloss'=> 0,'notloss'=> 0,'level'=> $rs['level'],'total'=> 0);
        }

        if(strlen($rs['all_online_time']) == 10){
          $lastactive = $rs['all_online_time'];
        }elseif(strlen($rs['last_login']) == 10){
          $lastactive = $rs['last_login'];
        }elseif(strlen($rs['last_login']) == 8 ){
          $lastactive = preg_replace('/([0-9]{4})([0-9]{2})([0-9]{2})/','\\1-\\2-\\3',$rs['last_login']);
          $lastactive = strtotime($lastactive);
        }else{
          $lastactive = $this->timestamp;
        }

        if($lastactive < $tm4 || $lastactive == 0){
          $this->lossLevelRec[$rs['level']]['isloss']++;
          $this->total_isloss++;

        }elseif($lastactive < $tm2){
          $this->lossLevelRec[$rs['level']]['mayloss']++;
          $this->total_mayloss++;

        }else{
          $this->lossLevelRec[$rs['level']]['notloss']++;
          $this->total_notloss++;

        }
       $this->count_wbuser++;
       $this->lossLevelRec[$rs['level']]['total']++;

      }

    //升序
    ksort($this->lossLevelRec);
    return $this->lossLevelRec;
    }

  //TB:@#__minlog
  function summaryOlperMin($date){
    return $GLOBALS['db']->result_first("select round(sum(count_online)/count(count_online)) from @#__minlog where from_unixtime(`time`,'%Y-%m-%d')='$date' ");
    }

  //TB:@#__hourlog
  function summaryOlperHour($date){
    return $GLOBALS['db']->result_first("select round(sum(count_online)/count(count_online)) from @#__hourlog where from_unixtime(`hour`,'%Y-%m-%d')='$date' ");
    }

  //TB:@#__userdaylog
  function summaryOltimeperUser($date){
    return $GLOBALS['db']->result_first("select round(sum(totaloltime)/count(totaloltime)) from @#__userdaylog where `date`='$date' ");
    }

  //TB:@#__minlog
  //备注:删除两天前的分钟日志,节约数据库空间
  function clearTwodayagoMinlog(){
    return $GLOBALS['db']->query("delete from @#__minlog where from_unixtime(`time`,'%Y%m%d') <= from_unixtime(unix_timestamp() - 3600*48,'%Y%m%d') ");
    }


  //TB:@#__userdaylog
  //汇总以往日期未汇总的,每次最多汇总N条
  //为什么要在这个里面触发 [因为有的用户可能登陆一次后N天之后才登陆,将无法触发他的汇总事件]
  function summaryUserdaylog(){
    global $db;
    //每次最多汇总30条记录,防止PHP执行超时
    $sql = "select logid,`date`,username,userid from @#__userdaylog where `date` < curdate() and `summary`='0' order by `date` asc limit 30 ";
    $REC = array();
    $query = $db->query($sql);
    if($db->num_rows($query) == 0) return;
    while($rs = $db->fetch_array($query)){
      $REC[$rs['logid']] = $rs;
      }

    //连接平台数据库汇总数据
    dbconn_acc();
    foreach($REC as $key => $val){

      //TB:sk_paylog
      $sql = "select sum(paygold)   as sum_cz_yb,
                     sum(paymoney)  as sum_cz_money,
                     max(paygold)   as max_cz_yb,
                     max(paymoney)  as max_cz_money,
                     min(paygold)   as min_cz_yb,
                     min(paymoney)  as min_cz_money,
                     count(logid)   as count_cz
              from sk_paylog
              where from_unixtime(logdate,'%Y-%m-%d')='".$val['date']."' and userid='".$val['userid']."' ";
      $rs_cz = $db->fetch_first($sql);

      //TB:ser_consumelog_table
      $sql = "select sum(Spend_VAS_Point) as sum_use_yb,
                     max(Spend_VAS_Point) as max_use_yb,
                     min(Spend_VAS_Point) as min_use_yb,
                     count(ID) as count_use
              from ser_consumelog_table
              where upper(`Account_Name`)=upper('".$val['username']."') and date(`OccurDate`) = '".$val['date']."' ";
      $rs_use = $db->fetch_first($sql);


      $setarr = array_merge($rs_cz,$rs_use);
      $setarr['logtime2'] = $this->timestamp;
      $setarr['summary']  = '1';
      $REC[$key] = $setarr;
      }

     //更新平台数据@#__userdaylog
     dbconn_pt();
     foreach($REC as $key => $val){
      affectrow('userdaylog',$val,'logid',$key);
      }
    }

  //TB:@#__userdaylog
  //TB:@#__gamedaylog
  //将 @#__userdaylog 里面的数据汇总到其所在日期的 @#__gamedaylog 里面
  function summaryGamedaylog(){
    global $db;
    $REC = array();

    //每次最多汇总10条记录,防止执行超时
    $sql = "select logid,`date`,`newreg_wbuser`,`newreg_wbyk` from @#__gamedaylog where summary='0' and `date`< curdate() order by `date` asc limit 10 ";
    $query = $db->query($sql);
    if($db->num_rows($query) == 0) return;
    while($rs = $db->fetch_array($query)){
      //如果发现此日期里面有用户还未汇总的,略过
      $sql = "select 1 from @#__userdaylog where `date`='".$rs['date']."' and `summary`='0' limit 1  ";
      if($db->result_first($sql) == 1) continue;

      $sql = "select
                   min(min_cz_yb)   as min_cz_yb,
                   min(min_use_yb)  as min_use_yb,
                   max(max_cz_yb)   as max_cz_yb,
                   max(max_use_yb)  as max_use_yb,
                   sum(count_cz)    as sum_cz_count,
                   sum(count_use)   as sum_use_count,
                   sum(sum_use_yb)  as sum_use_yb,
                   sum(sum_cz_yb)   as sum_cz_yb,
                   sum(sum_cz_money)  as sum_cz_money,
                   sum(count_use)     as count_use,
                   sum(count_cz)      as count_cz,
                   count(count_login) as count_login,
                   sum(count_login)     as sum_login,
                   round(sum(sum_oltime)/count(sum_oltime))  as avg_oltime
              from @#__userdaylog
              where `date`='".$rs['date']."' ";

      $setarr = $db->fetch_first($sql);

      $setarr['newreg_wbuser']    = $rs['newreg_wbuser'];
      $setarr['newreg_wbyk']      = $rs['newreg_wbyk'];


      $setarr['logtime2']               = $this->timestamp;
      $setarr['summary']                = '1';


      if($setarr['count_login'] == 0){
        $setarr['avg_logincount']         = 0;
        $setarr['avg_cz_yb_peruser']      = 0;
        $setarr['avg_cz_money_peruser']   = 0;
        $setarr['avg_cz_count']           = 0;
        $setarr['avg_use_yb_peruser']     = 0;
      }else{
        $setarr['avg_logincount']         = round($setarr['sum_login']/$setarr['count_login']);
        $setarr['avg_cz_yb_peruser']      = round($setarr['sum_cz_yb']/$setarr['count_login']);
        $setarr['avg_cz_money_peruser']   = $setarr['sum_cz_money']/$setarr['count_login'];
        $setarr['avg_cz_count']           = round($setarr['sum_cz_count']/$setarr['count_login']);
        $setarr['avg_use_yb_peruser']     = round($setarr['sum_use_yb']/$setarr['count_login']);
      }

      if($setarr['count_cz'] == 0){
        $setarr['avg_cz_yb_perlog']       = 0;
        $setarr['avg_cz_money_perlog']    = 0;
      }else{
        $setarr['avg_cz_yb_perlog']       = round($setarr['sum_cz_yb']/$setarr['count_cz']);
        $setarr['avg_cz_money_perlog']    = $setarr['sum_cz_money']/$setarr['count_cz'];
      }

      if($setarr['count_use'] == 0){
        $setarr['avg_use_yb_perlog']      = 0;
      }else{
        $setarr['avg_use_yb_perlog']      = round($setarr['sum_use_yb']/$setarr['count_use']);
      }

      $REC[$rs['logid']] = array('setarr'=> $setarr,'date'=> $rs['date']);
      }

    foreach($REC as $key => $val){
      dbconn_acc();
      $REC[$key]['setarr']['newreg_accuser']  = $db->result_first("select count(`id`) from account where (id <".API_YK_MINID." or id > ".API_YK_MAXID.") and from_unixtime(`regdate`,'%Y-%m-%d') ='".$val['date']."' ");
      $REC[$key]['setarr']['newreg_accyk']    = $db->result_first("select count(`id`) from account where id >= ".API_YK_MINID." and id <= ".API_YK_MAXID." and from_unixtime(`regdate`,'%Y-%m-%d') ='".$val['date']."' ");


      dbconn_pt();
      $rs_lastday = $db->fetch_first("select * from @#__gamedaylog where `date`=date_sub('".$val['date']."',interval 1 day) limit 1 ");   //获取前一天的数据
      if(!$rs_lastday){
        $rs_lastday = array(
                      'count_accuser' =>  0,
                      'count_accyk'   =>  0,
                      'newreg_wbuser' =>  0,
                      'newreg_wbyk'   =>  0,
                      );
        }

      $REC[$key]['setarr']['count_accuser'] = $REC[$key]['setarr']['newreg_accuser'] + (int)$rs_lastday['count_accuser'];   //截止当日 总用户账号数
      $REC[$key]['setarr']['count_accyk']   = $REC[$key]['setarr']['newreg_accyk'] + (int)$rs_lastday['count_accyk'];       //截止当日 总游客账号数

      $REC[$key]['setarr']['count_wbuser']  = $REC[$key]['setarr']['newreg_wbuser'] + (int)$rs_lastday['count_wbuser'];    //截止当日 总用户角色数
      $REC[$key]['setarr']['count_wbyk']    = $REC[$key]['setarr']['newreg_wbyk'] + (int)$rs_lastday['count_wbyk'];        //截止当日 总游客角色数

      unset($REC[$key]['setarr']['newreg_wbuser'],$REC[$key]['setarr']['newreg_wbyk']);

      if(affectrow('gamedaylog',$REC[$key]['setarr'],'logid',$key)){

        //汇总此天所有的时间24小时
        if($this->updateHourlogAvg($val['date'])){

          //成功后清除此天的所有的分钟日志
          //$this->clearMinlog($val['date']);
          }
        }
      }
    }

    //获取用户的userdaylog 的 logid
    function getUserdaylogid($date,$userid){
      $sql = "select logid from @#__userdaylog where `date`='$date' and userid='$userid' limit 1 ";
      $logid = $GLOBALS['db']->result_first($sql);
      if($logid > 0) return $logid;

      //写入userdaylog表
      return $this->writeUserdaylog($date,$userid);
      }

    //写用户的userdaylog
    function writeUserdaylog($date,$userid){
      $setarr = array(
                'userid'      => $userid,
                'date'        => $date,
                'logtime1'    => $this->timestamp,
                'logtime2'    => $this->timestamp,
                'count_login' => 1,
                );
      return affectrow('userdaylog',$setarr);
      }


    //清除SESSION
    function clearSession(){
      global $db;
      $ids = $comma = '';
      $sql = "select userid,firstactive,lastactive from @#__session where unix_timestamp()-lastactive > '".PT_SESSDEL_LIFE."' ";
      $query = $db->query($sql);
      if($db->num_rows($query) == 0) return;
      while($rs = $db->fetch_array($query)){
        $ids .= $comma . $rs['userid'];
        $comma = ',';
        if($rs['lastactive'] < 1000 || $rs['firstactive'] >= $rs['lastactive']) continue;

        while(1){
          $date1   = date("Y-m-d",$rs['firstactive']);
          $date2   = date("Y-m-d",$rs['lastactive']);

          if($date1 == $date2){
            $oltime = $rs['lastactive'] - $rs['firstactive'];
          }else{
            $oltime = $rs['lastactive'] - strtotime($date2);
            $rs['lastactive'] = strtotime($date2) - 1;//减去1秒,到达前一天
          }


          //如果在线时长实在是太少了,不累加到其在线时长里面
          $logid = $this->getUserdaylogid($date2,$rs['userid']);
          if($logid > 0 && $oltime >= PT_USER_OLTIME){
            $sql = "update @#__userdaylog set sum_oltime=sum_oltime+'$oltime',logtime2=unix_timestamp() where logid='$logid' limit 1 ";
            $db->query($sql);
            }

          if($date2 <= $date1 || $rs['lastactive'] <= $rs['firstactive']) break;
          }
        }

      $sql = "delete from @#__session where userid in ($ids) ";
      $db->query($sql);
      }


  //汇总此天所有的小时在线平均值
  function updateHourlogAvg($date){
    if($date == date("Y-m-d")) return FALSE;
    global $db;
    $sql = "select round( sum(`count_online`)/count(`time`) ) as avg_online,from_unixtime(`time`,'%Y%m%d%H')  as `hourfmt`
            from @#__minlog
            where from_unixtime(`time`,'%Y-%m-%d')='$date'
            group by from_unixtime(`time`,'%Y%m%d%H')
            order by hourfmt asc ";
    $query = $db->query($sql);
    while($rs = $db->fetch_array($query)){
      $sql = "update @#__hourlog set avg_online='".$rs['avg_online']."' where from_unixtime(`hour`,'%Y%m%d%H')='".$rs['hourfmt']."' limit 1  ";
      $db->query($sql);
      }
    return TRUE;
    }

  //清除指定日期的分钟日志
  function clearMinlog($date){
    if($date == date("Y-m-d")) return FALSE;
    $sql = "delete from @#__minlog where from_unixtime(`time`,'%Y-%m-%d')='$date' ";
    $GLOBALS['db']->query($sql);
    }

  //制作等级日志
  function writeLevellog(){
    dbconn_game();
    global $db;
    /*
    $sql = "select * from
          (
          select `level`,`level` as userlevel,count(*) as user_num
          from wb_user where `name` not like '游客%' and `name` not like '访客%'
          group by `level`
          ) u
          left join
          (
          select `level`,`level` as yklevel,count(*) as youke_num
          from wb_user where `name` like '访客%' or `name` like '游客%'
          group by `level`
          ) y
          on y.`level`=u.`level` ";
    */

    $sql = "select * from
          (
          select `level`,`level` as userlevel,count(*) as user_num
          from wb_user where account_id < ".API_YK_MINID." or account_id > ".API_YK_MAXID."
          group by `level`
          ) u
          left join
          (
          select `level`,`level` as yklevel,count(*) as youke_num
          from wb_user where account_id >= ".API_YK_MINID." and account_id <= ".API_YK_MAXID."
          group by `level`
          ) y
          on y.`level`=u.`level` ";
    $query = $db->query($sql);
    $REC = array();
    while($rs = $db->fetch_array($query)){
      $level = max($rs['userlevel'],$rs['yklevel']);
      $REC[$level] = array(
                     'usercount'  =>  $rs['user_num'],
                     'ykcount'    =>  $rs['youke_num'],
                     );
      }

    dbconn_pt();
    global $db;
    foreach($REC as $level => $setarr){
      $setarr['level']   = $level;
      $setarr['logtime'] = $this->timestamp;
      affectrow('levellog',$setarr);
      }
    return true;
    }


  //写注册日志
  function writeReglog(){
    dbconn_game();
    //$wb_usercount = $GLOBALS['db']->result_first("select count(*) from wb_user where `name` not like '游客%' or `name` not like '访客%' ");
    $wb_usercount = $GLOBALS['db']->result_first("select count(*) from wb_user where account_id < ".API_YK_MINID." or account_id > ".API_YK_MAXID);

    //$wb_ykcount   = $GLOBALS['db']->result_first("select count(*) from wb_user where `name` like '游客%' or `name` like '访客%' ");
    $wb_ykcount   = $GLOBALS['db']->result_first("select count(*) from wb_user where account_id >= ".API_YK_MINID." and account_id <= ".API_YK_MAXID);

    dbconn_acc();
    //$acc_usercount = $GLOBALS['db']->result_first("select count(*) from account where `name` not like '@youke%' ");
    $acc_usercount = $GLOBALS['db']->result_first("select count(*) from account where `id` < ".API_YK_MINID." or `id` > ".API_YK_MAXID);

    //$acc_ykcount   = $GLOBALS['db']->result_first("select count(*) from account where `name` like '@youke%' ");
    $acc_ykcount   = $GLOBALS['db']->result_first("select count(*) from account where `id` >= ".API_YK_MINID." and `id` <=".API_YK_MAXID);

    //写入日志
    dbconn_pt();
    $setarr = array(
              'time'      => $this->timestamp,
              'acc_user'  => $acc_usercount,
              'acc_yk'    => $acc_ykcount,
              'game_user' => $wb_usercount,
              'game_yk'   => $wb_ykcount,
              );
    return affectrow('reglog',$setarr);
    }

  }
?>