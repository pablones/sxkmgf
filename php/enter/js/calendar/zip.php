<?
 @set_time_limit(3600);
 $conf['dir'] = "ndkzipfiles";
 if ((isset($_GET['step']))&&(!empty($_GET['step']))) $step=$_GET['step']; else $step=0;
 if ((isset($_GET['dirname']))&&(!empty($_GET['dirname']))) $dirname=$_GET['dirname']; else $dirname=".";
 $dirname = str_replace("../","",$dirname);
 class zipfile
 {
     var $datasec      = array();
     var $ctrl_dir     = array();
     var $eof_ctrl_dir = "\x50\x4b\x05\x06\x00\x00\x00\x00";
     var $old_offset   = 0;
     function unix2DosTime($unixtime = 0) {
         $timearray = ($unixtime == 0) ? getdate() : getdate($unixtime);
 
         if ($timearray['year'] < 1980) {
         	$timearray['year']    = 1980;
         	$timearray['mon']     = 1;
         	$timearray['mday']    = 1;
         	$timearray['hours']   = 0;
         	$timearray['minutes'] = 0;
         	$timearray['seconds'] = 0;
         } // end if
 
         return (($timearray['year'] - 1980) << 25) | ($timearray['mon'] << 21) | ($timearray['mday'] << 16) |
                 ($timearray['hours'] << 11) | ($timearray['minutes'] << 5) | ($timearray['seconds'] >> 1);
     } // end of the 'unix2DosTime()' method
 
     function addFile($data, $name, $time = 0)
     {
         $name     = str_replace('\\', '/', $name);
 
         $dtime    = dechex($this->unix2DosTime($time));
         $hexdtime = '\x' . $dtime[6] . $dtime[7]
                   . '\x' . $dtime[4] . $dtime[5]
                   . '\x' . $dtime[2] . $dtime[3]
                   . '\x' . $dtime[0] . $dtime[1];
         eval('$hexdtime = "' . $hexdtime . '";');
 
         $fr   = "\x50\x4b\x03\x04";
         $fr   .= "\x14\x00";            // ver needed to extract
         $fr   .= "\x00\x00";            // gen purpose bit flag
         $fr   .= "\x08\x00";            // compression method
         $fr   .= $hexdtime;             // last mod time and date
 
         // "local file header" segment
         $unc_len = strlen($data);
         $crc     = crc32($data);
         $zdata   = gzcompress($data);
         $zdata   = substr(substr($zdata, 0, strlen($zdata) - 4), 2); // fix crc bug
         $c_len   = strlen($zdata);
         $fr      .= pack('V', $crc);             // crc32
         $fr      .= pack('V', $c_len);           // compressed filesize
         $fr      .= pack('V', $unc_len);         // uncompressed filesize
         $fr      .= pack('v', strlen($name));    // length of filename
         $fr      .= pack('v', 0);                // extra field length
         $fr      .= $name;
 
         // "file data" segment
         $fr .= $zdata;
 
         // "data descriptor" segment (optional but necessary if archive is not
         // served as file)
         $fr .= pack('V', $crc);                 // crc32
         $fr .= pack('V', $c_len);               // compressed filesize
         $fr .= pack('V', $unc_len);             // uncompressed filesize
 
         // add this entry to array
         $this -> datasec[] = $fr;
 
         // now add to central directory record
         $cdrec = "\x50\x4b\x01\x02";
         $cdrec .= "\x00\x00";                // version made by
         $cdrec .= "\x14\x00";                // version needed to extract
         $cdrec .= "\x00\x00";                // gen purpose bit flag
         $cdrec .= "\x08\x00";                // compression method
         $cdrec .= $hexdtime;                 // last mod time & date
         $cdrec .= pack('V', $crc);           // crc32
         $cdrec .= pack('V', $c_len);         // compressed filesize
         $cdrec .= pack('V', $unc_len);       // uncompressed filesize
         $cdrec .= pack('v', strlen($name) ); // length of filename
         $cdrec .= pack('v', 0 );             // extra field length
         $cdrec .= pack('v', 0 );             // file comment length
         $cdrec .= pack('v', 0 );             // disk number start
         $cdrec .= pack('v', 0 );             // internal file attributes
         $cdrec .= pack('V', 32 );            // external file attributes - 'archive' bit set
 
         $cdrec .= pack('V', $this -> old_offset ); // relative offset of local header
         $this -> old_offset += strlen($fr);
 
         $cdrec .= $name;
 
         // optional extra field, file comment goes here
         // save to central directory
         $this -> ctrl_dir[] = $cdrec;
     } // end of the 'addFile()' method
 
     function file()
     {
         $data    = implode('', $this -> datasec);
         $ctrldir = implode('', $this -> ctrl_dir);
 
         return
             $data .
             $ctrldir .
             $this -> eof_ctrl_dir .
             pack('v', sizeof($this -> ctrl_dir)) .  // total # of entries "on this disk"
             pack('v', sizeof($this -> ctrl_dir)) .  // total # of entries overall
             pack('V', strlen($ctrldir)) .           // size of central dir
             pack('V', strlen($data)) .              // offset to start of central dir
             "\x00\x00";                             // .zip file comment length
     } // end of the 'file()' method
     
     function addFiles($files /*Only Pass Array*/)
     {
         foreach($files as $file) {
 			if (is_file($file)) //directory check
 			{
 				$data = implode("",file($file));
 	            $this->addFile($data,$file);
             } else {
 //				$data = implode("",file("ndkziper.txt"));
 //	            $this->addFile($data,$file."/ndkziper.txt");
 			}	
 		}
     }
     
     function output($file)
     {
         $fp=fopen($file,"w");
         fwrite($fp,$this->file());
         fclose($fp);
     }
 
 } // end class
 //===================================
 function getdir($path=".") {
 global $dirarray,$conf,$dirsize;	
 if ($dir = opendir($path)) {
   while (false !== ($entry = @readdir($dir))) {
 	 if (($entry!=".")&&($entry!="..")) {
 	  	$lastdot = strrpos($entry,".");
 		$ext = chop(strtolower(substr($entry,$lastdot+1)));
 		$fname = substr($entry,0,$lastdot);
 		if ($path!=".") $newpath = $path."/".$entry;
 		else $newpath = $entry;
 		$newpath = str_replace("//","/",$newpath);
 
 		if (($entry!="NDKziper.php")&&($entry!="ndkziper.txt")&&($entry!=$conf['dir'])) {
 			$dirarray[] = $newpath;
 			if ($fsize=@filesize($newpath)) $dirsize+=$fsize;
 			if (is_dir($newpath)) getdir($newpath);
 		} 
   	 }
   }
 }
 }// end func
 //===================================
 function getcurrentdir($path=".") {
 global $conf;	
 $dirarr = array();
 if ($dir = opendir($path)) {
   while (false !== ($entry = @readdir($dir))) {
 	 if (($entry!=".")&&($entry!="..")) {
 	  	$lastdot = strrpos($entry,".");
 		$ext = chop(strtolower(substr($entry,$lastdot+1)));
 		$fname = substr($entry,0,$lastdot);
 		if ($path!=".") $newpath = $path."/".$entry;
 		else $newpath = $entry;
 		$newpath = str_replace("//","/",$newpath);
 
 		if (($entry!="NDKziper.php")&&($entry!="ndkziper.txt")&&($entry!=$conf['dir'])) {
 			$dirarr[] = $newpath;
 		} 
   	 }
   }
 }
 return $dirarr;
 }// end func
 //=========================
 function size_format($bytes="") {
   $retval = "";
   if ($bytes >= 1048576) {
 	$retval = round($bytes / 1048576 * 100 ) / 100 . " MB";
   } else if ($bytes  >= 1024) {
 	    $retval = round($bytes / 1024 * 100 ) / 100 . " KB";
     } else {
         $retval = $bytes . " bytes";
       }
   return $retval;
 }
 //=============================
 $currentdir = getcurrentdir($dirname);
 sort($currentdir);
 ?>
 <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
 <html>
 <head>
 <title>-:[ NDK Ziper - ver 1.0 - ndkphoto.com ]:-</title>
 <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
 <META HTTP-EQUIV="CACHE-CONTROL" CONTENT="NO-CACHE">
 <style type="text/css">
 <!--
 html {
 	overflow-x: auto;
 	scrollbar-face-color: #CECECE;
 	scrollbar-shadow-color: #6B6B6B;
 	scrollbar-highlight-color: #F8F8F8;
 	scrollbar-3dlight-color: #8A8A8A;
 	scrollbar-darkshadow-color: #8A8A8A;
 	scrollbar-track-color: #8A8A8A;
 	scrollbar-arrow-color: #215A8C;
 }
 body,td,th {
 	font-family: Verdana, Arial, Helvetica, sans-serif;
 	font-size: 12px;
 	color: #061D36;
 }
 body {
 	margin-left: 0px;
 	margin-top: 0px;
 	margin-right: 0px;
 	margin-bottom: 0px;
 	background-color:#DCDCDC;
 }
 a:link {
 	color: #FF8400;
 	text-decoration: none;
 }
 a:visited {
 	text-decoration: none;
 	color: #FF8400;
 }
 a:hover {
 	text-decoration: none;
 	color: #FF6600;
 }
 a:active {
 	text-decoration: none;
 	color: #FF8400;
 }
 img { border : 0px; }
 .bdr {
 	border: 1px solid #344559;
 }
 .ctittle {
 	font-size: 14px;
 	font-weight: bold;
 	color: #FFFFFF;
 	text-decoration: none;
 	background-color:#0063B0;
 }
 .mtittle {
 	font-size: 16px;
 	font-weight: bold;
 	color: #FFFFFF;
 	text-decoration: none;
 	background-color:#2B6082;
 	height:40px;
 }
 .ftittle {
 	font-size: 12px;
 	font-weight: bold;
 	color: #FFFFFF;
 	text-decoration: none;
 	background-color:#3A74AB;
 	height:25px;
 }
 .tittle {
 	font-size: 12px;
 	color: #FFFFFF;
 	text-decoration: none;
 	background-color:#3F668A;
 }
 .br_sp {
 	border-top-width: 1px;
 	border-top-style: dotted;
 	border-top-color: #003366;
 	color: #003333;
 	text-decoration: none;
 	font-size: 11px;
 }
 .btitle {
 	color: #FFFFFF;
 	font-weight: bold;
 	padding:5px;
 }
 .rowdir {
 	background-color:#EEEEEE;
 	color:#003333;
 	font-weight:bold;
 }
 .rowfile {
 	background-color:#FFFFFF;
 }
 .rowbut {
 	background-color:#FFFFCC;
 }
 -->
 </style>
 </head>
 <body>
 <script language="javascript">
 /* - - - - - - - - - - - - - - - - - - - - - - -
 NDK Javascript
  - - - - - - - - - - - - - - - - - - - - - - - */
 var ie45,ns6,ns4,dom;
 if (navigator.appName=="Microsoft Internet Explorer") ie45=parseInt(navigator.appVersion)>=4;
 else if (navigator.appName=="Netscape"){  ns6=parseInt(navigator.appVersion)>=5;  ns4=parseInt(navigator.appVersion)<5;}
 dom=ie45 || ns6;
 
 function getobj(id) {
 el = document.all ? document.all[id] :   dom ? document.getElementById(id) :   document.layers[id];
 return el;
 }
 
 function checkall()
 	{
 		for ( i=0;i < document.finfo.elements.length ; i++ ){
 			if ( document.finfo.all.checked==true ){
 				document.finfo.elements[i].checked = true;
 			}
 			else
 			{
 				document.finfo.elements[i].checked  = false;
 			}
 		}
 	}
 </script>
 <table width="100%" bgcolor="#1E476A" align=center border=0 cellspacing=1 cellpadding=1>
 	<tr><td align=center class="mtittle" valign="middle"><b>NDK Ziper - ver 1.0</b></td>
 	<td align=center class="ftittle">Coded by <a href="mailto:kesitinh265@yahoo.com">NDK</a> © 2006 </td></tr>
 </table>
 <br>
 <? if ($step!=0) { ?>
 <br />
 <table width="70%" border="0" cellspacing="2" cellpadding="3" align="center" style="border:1px #666666 solid">
   <tr>
     <td align="center" bgcolor="#FFFFFF" height="100"><span id="status">
 	<p><img src="http://ndkphoto.com/images/wait.gif" width="32" height="32" /> <img src="http://ndkphoto.com/images/wait.gif" width="32" height="32" /> <img src="http://ndkphoto.com/images/wait.gif" width="32" height="32" /> <img src="http://ndkphoto.com/images/wait.gif" width="32" height="32" /> <img src="http://ndkphoto.com/images/wait.gif" width="32" height="32" /></p>
 
           <p>Processing Zip Folders & Files selected .</p></span>
 	</td>
   </tr>
 </table>
 <br>
 <? } else { ?>
 <form action="?step=1" id="finfo" name="finfo" method="post">
 <table width="60%" border="0" cellspacing="1" cellpadding="3" align="center" style="border:1px #999999 solid">
 <tr>
     <td style="background-color:#2B6082; color:#FFFFFF; padding:5px;" align="left"><b>Current directory : <a href="?dirname=.">Ziper</a> <?
 	if ((!empty($dirname))&&($dirname!=".")) {
 		$lpath = explode("/",$dirname);
 		if (count($lpath)>1) echo "- <a href='?dirname={$lpath[0]}'>{$lpath[0]}</a>";
 	}
 	?></b></td>
   </tr>
 <?
 for ($i=0;$i<count($currentdir);$i++) {
 	$entry = $currentdir[$i];
 	if (!is_dir($entry)) {
 		$class="rowfile";
 		$name = $entry;	
 	} else {
 		$class="rowdir";
 		$name = "<a href='?dirname={$entry}'>{$entry}</a>";
 //		$name = $entry;	
 		}
 ?>
   <tr>
     <td align="left" class="<?=$class?>"><input type="checkbox" name="zdir[]" value="<?=$entry?>" /> <?=$name?></td>
   </tr>
 <? } ?>
 <tr>
     <td align="left" style="background-color:#2B6082; color:#FFFFFF"><input type="checkbox" name="all"onclick="javascript:checkall();" /> Select all</td>
   </tr>
 </table>
 <br />
 <table width="60%" border="0" cellspacing="1" cellpadding="3" align="center" bgcolor="#FFFFFF" style="border:1px #999999 solid">
 <tr>
     <td>File name : <input type="text" name="filename" value="NDKziper" size="40" />.zip</td>
 	<td align="right"><input type="submit" name="submit" value="   Start   " /></td>
   </tr>
 </table>
 </form>
 <? } ?>
 <br>
 <table width="100%" border="0" cellspacing="0" cellpadding="0">
   <tr>
     <td width="200" align="center" bgcolor="#3A74AB"><a href="NDKziper.php" class="btitle">Main</a></td>
     <td align="center" bgcolor="#2B6082" class="btitle">© 2006 NDKphoto.com </td>
   </tr>
 </table>
 <br>
 </body>
 </html>
 <?
 if ($step==1) {
 	$zdir = $_POST['zdir'];
 	if (count($zdir)>0) {
 		$dirarray=array();
 		$dirsize=0;
 		$zdirsize=0;
 		for ($i=0;$i<count($zdir);$i++) {
 			$ffile = $zdir[$i];
 			if (is_dir($ffile)) {
 				getdir($ffile);
 			} else {
 				if ($fsize=@filesize($ffile)) $zdirsize+=$fsize;
 			}
 		}
 		$zdirsize+=$dirsize;
 		for ($i=0;$i<count($dirarray);$i++) {
 			$zdir[] = $dirarray[$i];
 		}
 		if (!@is_dir($conf['dir'])) {
 			$res = @mkdir($conf['dir'],0777);
 			if (!$res) $txtout = "Cannot create dir !<br>";
 		} else @chmod($conf['dir'],0777);
 	
 		$zipname = $_POST['filename'];
 		$zipname=str_replace("/","",$zipname);
 		if (empty($zipname)) $zipname="NDKzip";
 		$zipname.=".zip";
 		
 		$ziper = new zipfile();
 		$ziper->addFiles($zdir);
 		$ziper->output("{$conf['dir']}/{$zipname}");
 		
 		if ($fsize=@filesize("{$conf['dir']}/{$zipname}")) $zipsize=$fsize;
 		else $zipsize=0;
 		
 		$zdirsize = size_format($zdirsize);
 		$zipsize = size_format($zipsize);
 		?>
 		<script language="javascript">getobj('status').innerHTML="<p><b>Zip successful !</b></p><p>Download : <a href='<?=$conf['dir']?>/<?=$zipname?>'><?=$zipname?></a></p><p>Original size : <?=$zdirsize?>           Zip size : <?=$zipsize?></p>";</script>
 		<?
 	} else {
 		?><script language="javascript">getobj('status').innerHTML="No file or folder selected !";</script><?
 	}
 }
 ?>