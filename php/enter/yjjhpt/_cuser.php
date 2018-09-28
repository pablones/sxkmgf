<?php


/*
  请求条件: 成功创建角色[即在wb_user表中成功创建记录,Flash调用网页的JS函数 cuser(userid);]

  处理创建角色(包括注册用户创建的角色、游客创建的角色)
  更新今日的角色创建数目

  TB: acc_db.acc_handle [UPDATE]
  TB: yjjhpt.@#__gamedaylog [UPDATE || INSERT INTO]
*/


  //限定请求类型
  if($_SERVER['REQUEST_METHOD'] != 'POST' || !isset($_POST['u'])) exit('403');

  require_once './ptcommon.php';
  $userid = intval($_POST['u']);
  if(0 == $userid) sexit('400');

  //渠道来源
  $cid = shtmlspecialchars( trim( $_POST['cid'] ) );


  //先判断是否有该用户的账号
  $sql = "SELECT 1 FROM account WHERE id='$userid' LIMIT 1 ";
  if($db->result_first($sql) != 1) sexit("400");


  if($userid >= API_MIN_YKID && $userid <= API_MAX_YKID){
    $accfn  = 'count_wbyk';       //总的wbuser表的游客角色数
    $ptfn   = 'newreg_wbyk';      //创建角色成功的游客数目

    if($cid){
        $db->query("UPDATE acc_ykcidlog SET total_wbyk=total_wbyk+1 WHERE cid='$cid' LIMIT 1 ");
        if(!$db->affected_rows()){
          $setarr = array(
                    'cid'           =>  $cid,
                    'total_regyk'   =>  1,
                    'total_reguser' =>  0,
                    'total_wbyk'    =>  1,
                    );
          affectrow('acc_ykcidlog',$setarr);
          }
        }
    }else{
      $accfn  = 'count_wbuser';   //总的wbuser表的用户角色数
      $ptfn   = 'newreg_wbuser';  //创建角色成功的用户数
    }

 //DB:acc_db
 //TB:acc_handle
 $db->query("update acc_handle set `$accfn`=`$accfn`+1 limit 1 ");

 dbconn_pt();
 $db->query("update @#__gamedaylog set `$ptfn`=`$ptfn`+1 where `date`=curdate() limit 1 ");

 //今日的日志未创建,那么创建
 if(!$db->affected_rows()){
  $setarr = array(
            'date'          =>  date("Y-m-d"),
            'logtime1'      =>  $_SG['timestamp'],
            'logtime2'      =>  $_SG['timestamp'],
            'newreg_wbuser' =>  ($ptfn == 'newreg_wbuser' ? 1:0),
            'newreg_wbyk'   =>  ($ptfn == 'newreg_wbyk' ? 1:0),
            );
  affectrow('gamedaylog',$setarr);
  }

 sexit("200");


?>