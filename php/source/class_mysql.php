<?php

if(!defined('IN_ZBPHP'))
{
	exit('Access Denied');
}

class dbstuff
{
	var $querynum = 0;
	var $link;
	var $charset;
	function connect($dbhost, $dbuser, $dbpw, $dbname = '', $pconnect = 0, $halt = TRUE)
	{
		if(1)
		{
			if(!$this->link = @mysql_pconnect($dbhost, $dbuser, $dbpw))
			{
				$halt && $this->halt('Can not pconnect to MySQL server ['.$dbhost.']');
			}
		}
		else
		{
			if(!$this->link = @mysql_connect($dbhost, $dbuser, $dbpw, 1))
			{
				$halt && $this->halt('Can not connect to MySQL server {'.$dbhost.'}');
			}
		}

		if($this->version() > '4.1')
		{
			if($this->charset)
			{
				@mysql_query("SET character_set_connection=$this->charset, character_set_results=$this->charset, character_set_client=binary", $this->link);
			}
			if($this->version() > '5.0.1')
			{
				@mysql_query("SET sql_mode=''", $this->link);
			}
		}
		if($dbname)
		{
			@mysql_select_db($dbname, $this->link);
		}
	}

	function select_db($dbname)
	{
		return mysql_select_db($dbname, $this->link);
	}

	function fetch_array($query, $result_type = MYSQL_ASSOC)
	{
		return mysql_fetch_array($query, $result_type);
	}

	function query($sql, $type = '')
	{
		if(D_BUG)
		{
			global $_SG;
			$sqlstarttime = $sqlendttime = 0;
			$mtime = explode(' ', microtime());
			$sqlstarttime = number_format(($mtime[1] + $mtime[0] - $_SG['starttime']), 6) * 1000;
		}
		$func = $type == 'UNBUFFERED' && @function_exists('mysql_unbuffered_query') ?
			'mysql_unbuffered_query' : 'mysql_query';
	  $sql = str_replace('@#__',$GLOBALS['_SC']['tablepre'],$sql);
		if(!($query = $func($sql, $this->link)) && $type != 'SILENT')
		{
			$this->halt('MySQL Query Error', $sql);
		}

		if(D_BUG)
		{
			$mtime = explode(' ', microtime());
			$sqlendttime = number_format(($mtime[1] + $mtime[0] - $_SG['starttime']), 6) * 1000;
			$sqltime = round(($sqlendttime - $sqlstarttime), 3);

      //查询时间大于1的
      if($sqltime > 30){
  			$explain = array();
  			$info = mysql_info();
  			/*
  			if($query && preg_match("/^(select )/i", $sql))
  			{
  				$explain = mysql_fetch_assoc(mysql_query('EXPLAIN '.$sql, $this->link));
  			}
  			*/
  			$_SG['debug_query'][] = array('sql' => $sql, 'time' => $sqltime, 'info' => $info, 'explain' => $explain);
  			$filecnt = "";
  			$filepath = S_ROOT.'./data/mysql_log/log_'.date("Ymd").".php";
  			if(!file_exists($filepath)) $filecnt .= "<?php exit()?>\n";
  			$filecnt .= "<h3>".date("Y-m-d H:i:s")."</h3>\n".
  			            "<pre style=\"font-family: 'Courier New', Courier, monospace;font-size:15px;\">\n".
  			            "SQL:\t".$sql."\n".
  			            "Time:\t".$sqltime."\n".
  			            "Info:\t".$info."\n".
  			            "</pre><hr />";
        swritefile($filepath,$filecnt,'a');
  		}
		}

		$this->querynum++;
		return $query;
	}

	function affected_rows()
	{
		return mysql_affected_rows($this->link);
	}

	function error()
	{
		return (($this->link) ? mysql_error($this->link) : mysql_error());
	}

	function errno()
	{
		return intval(($this->link) ? mysql_errno($this->link) : mysql_errno());
	}

  function fetch_first($sql)
  {
    return $this->fetch_array($this->query($sql));
  }

	function result($query, $row)
	{
		$query = @mysql_result($query, $row);
		return $query;
	}

	function result_first($sql)
	{
	  return $this->result($this->query($sql),0);
	}

	function num_rows($query)
	{
		$query = mysql_num_rows($query);
		return $query;
	}

	function num_fields($query)
	{
		return mysql_num_fields($query);
	}

	function free_result($query)
	{
		return mysql_free_result($query);
	}

	function insert_id()
	{
		return ($id = mysql_insert_id($this->link)) >= 0 ? $id : $this->result($this->query("SELECT last_insert_id()"), 0);
	}

	function fetch_row($query)
	{
		$query = mysql_fetch_row($query);
		return $query;
	}

	function fetch_fields($query)
	{
		return mysql_fetch_field($query);
	}

	function version()
	{
		return mysql_get_server_info($this->link);
	}

	function close() {
		return mysql_close($this->link);
	}

	function halt($message = '', $sql = '')
	{
		$dberror = $this->error();
		$dberrno = $this->errno();
		$help_link = "http://www.zbphp.com/?type=mysql&dberrno=".rawurlencode($dberrno)."&dberror=".rawurlencode($dberror);
	  $filecnt = "<div style=\"font-size:15px;background:#EEEEEE;padding:0.5em;margin:3px;font-family: 'Courier New', Courier, monospace;border:solid 2px #CCC;\">\n".
	            "<strong>MySQL Error : ".date("Y-m-d H:i:s")."</strong>\n<pre style=\"font-family: 'Courier New', Courier, monospace;\">\n".
	            "Msg:\t".$message."\n".
	            "SQL:\t".$sql."\n".
	            "Error:\t".$dberror."\n".
	            "Errno:\t".$dberrno."\n".
	            "</pre></div>\n";

    //------------ mysql error log start -----------------//
		$filepath = S_ROOT.'./data/mysql_log/err_'.date("Ymd").".php";
		if(!file_exists($filepath)) $filecnt = "<?php exit();?>\n".$filecnt."\n";
		swritefile($filepath,$filecnt,'a');
		//------------ mysql error log  end ------------------//

    //echo $filecnt;

    //不输出内容，而是直接跳转到官网首页
    jump("http://www.fy5.com/web/fy5/pay.php");//,"《御剑江湖》服务器爆满，请到官网登录后重试！"
		exit();
	}
}

?>