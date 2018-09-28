<?php
/**
 * 充值消费统计类
 * @author King.L
 */

class pay_consume_class {

    /**
     * 获取充值消费统计列表
     * @param  $allowIpArr 充值消费IP
     * @return
     */
    public function get_py_statics($allowIpArr){
        $result = $this -> _get_py_statics($allowIpArr);
        return $result;
    }

    /**
     * 获取充值查询列表数据
     */
    public function get_py_list($start_time,$time_rang,$start=0,$number=10,$Iparray){
        $result = $this -> _getpylist($start_time,$time_rang,$start,$number,$Iparray);
        return $result;
    }

    /**
     * 获取充值汇总
     * @param  $res
     * @return
     */
    public function getSum($res){
        $result = $this -> _getSum($res);
        return $result;
    }

    /**
     * 获取元宝列表
     * @return
     */
    public function get_py_yb($now_time,$time_rang='',$start,$num){
        $result = $this -> _get_py_yb($now_time,$time_rang,$start,$num);
        return $result;
    }
        /**
     * 获取元宝记录数量,珠宝,元宝总和
     * @return
     */
    public function get_yb_count($start_time,$end_time,$username,$rolename,$paynum){
        $result = $this -> _get_yb_count($start_time,$end_time,$username,$rolename,$paynum);
        return $result;
    }

    private function _get_yb_count($start_time,$end_time,$username,$rolename,$paynum){  
      	date_default_timezone_set('PRC'); 
    	//$ip = getIp($_CFG['allowIpArr']); 	
        //$where = " where 1 and $ip";
        $where = " where 1 ";
        if(!empty($start_time) && !empty($end_time)){
        	$start_time = strtotime($start_time);
            $end_time = strtotime($end_time)+ONE_DAY;
            $where .= "and (`logdate` <= ".$end_time." and `logdate` >= ".$start_time.")";
        }
        if(!empty($paynum)){
            $where .= "and `paynum` = '".$paynum."'"; 
        }
        if(!empty($rolename)){
            $sql = "select `account_id`,`name` from `".TBL_WB_USER."` where `name` like '%".str_replace(' ','%',$rolename)."%'";
            $wb_userinfo = $GLOBALS['wangpu_db'] -> getAll($sql);
            $i = count($wb_userinfo) -1;
            if(!empty($wb_userinfo)){
            	$where1=" and (";
                foreach($wb_userinfo as $key => $value){
                    if($i > 0){
                        $where1 .= " `userid` = ".$value['account_id']." or ";
                    }else{
                        $where1 .= " `userid` = ".$value['account_id'].") ";
                    }
                    $i--;
                }
            }else{
                return  0;
            }
 
           $sql = "select count(*) from ".TBL_SK_PAYLOG.$where .$where1;
           $result1 = $GLOBALS['acc_db'] -> getOne($sql);
           $sql=" SELECT sum(paygold) AS yb, sum(paymoney) AS rmb FROM ".TBL_SK_PAYLOG.$where.$where1;
           $result = $GLOBALS['acc_db'] -> getRow($sql);
           $result['count']=$result1;
           return $result;
        }else{
        	if(!empty($username)){    
            	$sql = "select `id` from `".TBL_ACCOUNT."` where `name` = '".$username."'";
            	$account_id = $GLOBALS['acc_db'] -> getOne($sql);
            	if(empty($account_id)){
            	    return 0;
            	}           	  
            	$sql = "select count(*) from ".TBL_SK_PAYLOG." $where  and userid = '$account_id'  order by userid desc";
            	$res = $GLOBALS['acc_db'] -> getOne($sql);
        		$sql=" SELECT sum(paygold) AS yb, sum(paymoney) AS rmb FROM ".TBL_SK_PAYLOG." $where  and userid = '$account_id'  order by userid desc";
           		$result = $GLOBALS['acc_db'] -> getRow($sql);
           		$result['count']=$res;
          		return $result;
            }else{
            	$sql = "select count(*) from ".TBL_SK_PAYLOG.$where." order by logdate desc";
            	$res = $GLOBALS['acc_db'] -> getOne($sql);
            	$sql=" SELECT sum(paygold) AS yb, sum(paymoney) AS rmb FROM ".TBL_SK_PAYLOG.$where;
           		$result = $GLOBALS['acc_db'] -> getRow($sql);
           		$result['count']=$res;
          		return $result;
            }	
        }
    }
    
    
    /**
     * 获取元宝搜索记录
     * @return
     */
    public function get_yb_search($start_time,$end_time,$username,$rolename,$paynum,$start,$num=30){
        $result = $this -> _get_yb_search($start_time,$end_time,$username,$rolename,$paynum,$start,$num);
        return $result;
    }

    private function _get_yb_search($start_time,$end_time,$username,$rolename,$paynum,$start,$num){ 
    	date_default_timezone_set('PRC'); 
    	//$ip = getIp($_CFG['allowIpArr']); 	
        //$where = " where 1 and $ip";
        $where = " where 1 ";
        if(!empty($start_time) && !empty($end_time)){
        	$start_time = strtotime($start_time);
            $end_time = strtotime($end_time)+ONE_DAY;
            $where .= "and (`logdate` <= ".$end_time." and `logdate` >= ".$start_time.")";
        }
        if(!empty($paynum)){
            $where .= "and `paynum` = '".$paynum."'"; 
        }
        if(!empty($rolename)){
            $sql = "select `account_id`,`name` from `".TBL_WB_USER."` where `name` like '%".str_replace(' ','%',$rolename)."%'";
            $wb_userinfo = $GLOBALS['wangpu_db'] -> getAll($sql);
            $i = count($wb_userinfo) -1;
            if(!empty($wb_userinfo)){
            	$where1=" and (";
                foreach($wb_userinfo as $key => $value){
                if($i > 0){
                    $where1 .= " `userid` = ".$value['account_id']." or ";
                }else{
                    $where1 .= " `userid` = ".$value['account_id'].") ";
                }
                $i--;
                }
            }else{
                return 0;
            }
            $sql = "select * from ".TBL_SK_PAYLOG.$where ." $where1 order by logdate desc limit $start,$num";
            
            $result = $GLOBALS['acc_db'] -> getAll($sql);
            foreach($wb_userinfo as $key => $value){
                $userinfo[$value['account_id']] = $value['name'];
            }
            foreach($result as $key => $value){
            	$sql = "select `name` from ".TBL_ACCOUNT." where `id` = '".$value['userid']."'";
            	$usernameinfo = $GLOBALS['acc_db'] -> getOne($sql);
            	$result[$key]['username'] = isset($usernameinfo)? $usernameinfo : '';
                $result[$key]['rolename'] = $userinfo[$value['userid']];
                $result[$key]['logdate'] = date("Y-m-d H:i:s",$value['logdate']);
            }
            //$result = $this -> fenye($result, $start, $num);
        	return $result;
        }else{
        	if(!empty($username)){    
            	$sql = "select `id` from `".TBL_ACCOUNT."` where `name` = '".$username."'";
            	$account_id = $GLOBALS['acc_db'] -> getOne($sql);
            	if(empty($account_id)){
            	    return 0;
            	}            	  
            	$sql = "select * from ".TBL_SK_PAYLOG." $where  and userid = '$account_id'  order by logdate desc limit $start,$num";
            	$res = $GLOBALS['acc_db'] -> getAll($sql);
            	foreach($res as $key => $value){
            		$value['logdate'] = date("Y-m-d H:i:s",$value['logdate']);
            		$sql = "select `name` from ".TBL_WB_USER." where `account_id` = '".$value['userid']."'";
            		$roleinfo = $GLOBALS['wangpu_db'] -> getOne($sql);
            		$value['rolename'] = isset($roleinfo)? $roleinfo : '';
            		$value['username']=$username;
            		$result[] = $value;
            	}   	
            	//$result = $this -> fenye($result, $start, $num);
        		return $result;
            }else{
            	$sql = "select * from ".TBL_SK_PAYLOG.$where." order by logdate desc limit $start,$num";
            	$res = $GLOBALS['acc_db'] -> getAll($sql);
            	foreach($res as $key => $value){
            	    $value['logdate1'] = $value['logdate'];
            		$value['logdate'] = date("Y-m-d H:i:s",$value['logdate']);
            		$sql = "select `name` from ".TBL_ACCOUNT." where `id` = '".$value['userid']."'";
            		$usernameinfo = $GLOBALS['acc_db'] -> getOne($sql);
            		$value['username'] = isset($usernameinfo)? $usernameinfo : '';
            		$sql = "select `name` from ".TBL_WB_USER." where `account_id` = '".$value['userid']."'";
            		$roleinfo = $GLOBALS['wangpu_db'] -> getOne($sql);
            		$value['rolename'] = isset($roleinfo)? $roleinfo : '';
            		$result[] = $value;
            	}					       	
            	//$result = $this -> fenye($result, $start, $num);
        		return $result;
            }	
        }
    }

    private function _get_py_yb($now_time,$time_rang,$start,$num){

        $sql = "select * from ".TBL_SK_PAYLOG." order by logdate desc  limit $start,$num ";
        $res = $GLOBALS['acc_db'] -> getAll($sql);
        foreach($res as $key => $value){
            $value['logdate'] = date("Y-m-d h:i:s",$value['logdate']);
            $sql = "select `name` from ".TBL_ACCOUNT." where `id` = '".$value['userid']."'";
            $usernameinfo = $GLOBALS['acc_db'] -> getOne($sql);
            $value['username'] = isset($usernameinfo)? $usernameinfo : '';
            $sql = "select `name` from ".TBL_WB_USER." where `account_id` = '".$value['userid']."'";
            $roleinfo = $GLOBALS['wangpu_db'] -> getOne($sql);
            $value['rolename'] = isset($roleinfo)? $roleinfo : '';
            $result[] = $value;
            }
        //$result = $this-> fenye($result,$start,$num);
        return $result;
    }

    private function _get_py_statics($allowIpArr){
    	if($allowIpArr){
    		$where = getIp($allowIpArr);
    	}
        //查询最大的充值元宝数
        if($where){
        	$sql = "SELECT COUNT(logid)  AS count_paylog,SUM(paygold)  AS sum_paygold,SUM(paymoney) AS sum_paymoney,MAX(paygold)  AS max_paygold,MAX(paymoney) AS max_paymoney,MIN(paygold)  AS min_paygold,MIN(paymoney) AS min_paymoney,SUM(paygold)/COUNT(logid) AS avg_paygold,SUM(paymoney)/COUNT(logid) AS avg_paymoney FROM ".TBL_SK_PAYLOG." where ".$where;
        }else{
        	$sql = "SELECT COUNT(logid)  AS count_paylog,SUM(paygold)  AS sum_paygold,SUM(paymoney) AS sum_paymoney,MAX(paygold)  AS max_paygold,MAX(paymoney) AS max_paymoney,MIN(paygold)  AS min_paygold,MIN(paymoney) AS min_paymoney,SUM(paygold)/COUNT(logid) AS avg_paygold,SUM(paymoney)/COUNT(logid) AS avg_paymoney FROM ".TBL_SK_PAYLOG;
        }

        $rs = $GLOBALS['acc_db'] -> getRow($sql);
       
        $sql = "select count(ID) as count_uselog,sum(Spend_VAS_Point) as sum_uselog,max(Spend_VAS_Point) as max_uselog,min(Spend_VAS_Point) as min_uselog,sum(Spend_VAS_Point)/count(ID) as avg_uselog from ".TBL_SER_CONSUMELOG_TABLE;
        $res = $GLOBALS['acc_db'] -> getRow($sql);
        
        return array($rs,$res);
    }

    private function _getSum($res){
    	if($res){
    		foreach($res as $key => $value){
            	$yb_sum += $value['summoney'];
            	$rmb_sum += $value['sumgold'];
        	}    	
    	}

        $result = array(
            'yb_sum' => $yb_sum,
            'rmb_sum' => $rmb_sum,
        );
        return $result;
    }

    private function _getpylist($start_time,$time_rang ='',$start=0,$number=10,$Iparray){
    	if(empty($time_rang) || $time_rang == ''){
    		$sql = "select min(logdate)  from ".TBL_SK_PAYLOG."";
    		$minlogdate = $GLOBALS['acc_db'] -> getOne($sql);  
    		$time_rang = $start_time - $minlogdate; 		
    	}		
    	
        $time_zend = getOneMonthTime($start_time, $time_rang);
        $i = count($time_zend) - 1;
        $ip_count = count($Iparray) - 1;
        if($Iparray[0]!=''){
        	print_r("ffff");
	        foreach($Iparray as $key => $value){
	    	    $value=trim($value);
	            if($ip_count != 0){
	                $where .= " ip = '$value' or ";
	            }else{
	                $where .= " ip = '$value' ";
	            }
	             $ip_count --;
	        }
        }
      
        while($i > 0){
        	if($where){
        		$sql = "select count(distinct userid) as countuser,count(logid) as count,sum(paymoney) as summoney,sum(paygold) as sumgold  from ".
            		TBL_SK_PAYLOG." where logdate <=".$time_zend[$i]." and logdate >=".$time_zend[$i-1]." and ($where) order by userid desc";
        	}else{
        		$sql = "select count(distinct userid) as countuser,count(logid) as count,sum(paymoney) as summoney,sum(paygold) as sumgold  from ".
            		TBL_SK_PAYLOG." where logdate <=".$time_zend[$i]." and logdate >=".$time_zend[$i-1]."  order by userid desc";
        	}
       		
            $result[] = $GLOBALS['acc_db'] -> getRow($sql);

            $i--;
        }
        if($result){
        	$result = array_reverse($result);
        	 //组装数据
	        foreach($result as $key => $value){
	            $result[$key]['countuser'] = isset($value['countuser']) ? $value['countuser'] : 0;
	            $result[$key]['count'] = isset($value['count']) ? $value['count']: 0;
	            $result[$key]['summoney'] = isset($value['summoney']) ? $value['summoney']: 0;
	            $result[$key]['sumgold'] = $value['sumgold'];
	            
	            $result[$key]['logdate'] = date("Y-m-d", $time_zend[$key+1]);
	            if($result[$key]['countuser'] != 0){
	                $result[$key]['arpu'] = round($result[$key]['summoney'] / $result[$key]['countuser'],2);
	            }else{
	                $result[$key]['arpu'] = 0;
	            }
	        } 
        }
     	
       
 		if($result){
 			$result = array_reverse($result);
 		}
        
        $rs_num = count($result);
        $i = 1;
        while($i < $number && $start<=$rs_num){
            $res[] = $result[$start];
            $start++;
            $i++;
        }
        $result = array ($res,$rs_num,$result);
        return $result;
    }
    
    /**
     * 分页处理
     * 
     */
    private function fenye($result,$start,$num){
    	if($result){
    		$result = array_slice($result,$start,$num);
    	}	   	
    	return $result;
    }
}