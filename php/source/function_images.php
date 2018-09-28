<?php


if(!defined('IN_ZBPHP'))
{
	exit('Access Denied');
}

//生成缩略图
function makethumb($srcfile,$thumbsource=0)
{
	global $_SG,$_Cfg;

	$srcfile = S_ROOT.$_Cfg['uploaddir'].$srcfile;

	//判断文件是否存在
	if (!file_exists($srcfile))
	{
		return '';
	}

	$dstfile = $srcfile.'.thumb.jpg';

	//缩略图大小
	$tow = intval($_Cfg['thumbwidth']);
	$toh = intval($_Cfg['thumbheight']);
	if($tow < 100) $tow = 100;
	if($toh < 100) $toh = 100;

	$make_max = 0;
	$maxtow = intval($_Cfg['maxthumbwidth']);
	$maxtoh = intval($_Cfg['maxthumbheight']);
	if($maxtow >= 450 && $maxtoh >= 450)
	{
		$make_max = 1;
	}

	//获取图片信息
	$im = '';
	if($data = getimagesize($srcfile))
	{
		if($data[2] == 1) {
			$make_max = 0;//gif不处理
			if(function_exists("imagecreatefromgif"))
			{
				$im = imagecreatefromgif($srcfile);
			}
		}
		 elseif($data[2] == 2)
		 {
			if(function_exists("imagecreatefromjpeg"))
			{
				$im = imagecreatefromjpeg($srcfile);
			}
		}
		 elseif($data[2] == 3)
		 {
			if(function_exists("imagecreatefrompng"))
			{
				$im = imagecreatefrompng($srcfile);
			}
		}
	}
	if(!$im) return '';

	$srcw = imagesx($im);
	$srch = imagesy($im);

	$towh = $tow/$toh;
	$srcwh = $srcw/$srch;
	if($towh <= $srcwh)
	{
		$ftow = $tow;
		$ftoh = $ftow*($srch/$srcw);

		$fmaxtow = $maxtow;
		$fmaxtoh = $fmaxtow*($srch/$srcw);
	}
	 else
	 {
		$ftoh = $toh;
		$ftow = $ftoh*($srcw/$srch);

		$fmaxtoh = $maxtoh;
		$fmaxtow = $fmaxtoh*($srcw/$srch);
	}

	if($srcw <= $maxtow && $srch <= $maxtoh)
	{
		$make_max = 0;//不处理
	}

	if($srcw > $tow || $srch > $toh)
	{
		if(function_exists("imagecreatetruecolor") && function_exists("imagecopyresampled") && @$ni = imagecreatetruecolor($ftow, $ftoh))
		{
			imagecopyresampled($ni, $im, 0, 0, 0, 0, $ftow, $ftoh, $srcw, $srch);
			//大图片
			if($make_max && @$maxni = imagecreatetruecolor($fmaxtow, $fmaxtoh))
			{
				imagecopyresampled($maxni, $im, 0, 0, 0, 0, $fmaxtow, $fmaxtoh, $srcw, $srch);
			}
		}
		 elseif(function_exists("imagecreate") && function_exists("imagecopyresized") && @$ni = imagecreate($ftow, $ftoh))
		 {
			imagecopyresized($ni, $im, 0, 0, 0, 0, $ftow, $ftoh, $srcw, $srch);
			//大图片
			if($make_max && @$maxni = imagecreate($fmaxtow, $fmaxtoh))
			{
				imagecopyresized($maxni, $im, 0, 0, 0, 0, $fmaxtow, $fmaxtoh, $srcw, $srch);
			}
		}
		 else
		 {
			return '';
		}

		if(function_exists('imagejpeg'))
		{
			imagejpeg($ni, $dstfile);
			//大图片
			if($make_max && $thumbsource)
			{
				imagejpeg($maxni, $srcfile);
			}

		}
		 elseif(function_exists('imagepng'))
		 {
			imagepng($ni, $dstfile);
			//大图片
			if($make_max && $thumbsource)
			{
				imagepng($maxni, $srcfile);
			}
		}
		imagedestroy($ni);
		if($make_max)
		{
			imagedestroy($maxni);
		}
	}

	imagedestroy($im);

	if(!file_exists($dstfile))
	{
		return '';
	}
	 else
	 {
		return $dstfile;
	}
}

//图片水印

function makewatermark($srcfile)
{
	global $_SG,$_Cfg;

	//水印图片
	$watermarkfile = S_ROOT.(empty($_Cfg['watermarkfile'])? './images/watermark.png':$_Cfg['watermarkfile']);
	$srcfile = S_ROOT.$_Cfg['uploaddir'].$srcfile;
    if(!file_exists($watermarkfile) || !$water_info = getimagesize($watermarkfile))
    {
    	return '';
    }

    $water_w = $water_info[0];
    $water_h = $water_info[1];
    $water_im = '';
    switch($water_info[2])
    {
        case 1:@$water_im = imagecreatefromgif($watermarkfile);break;
        case 2:@$water_im = imagecreatefromjpeg($watermarkfile);break;
        case 3:@$water_im = imagecreatefrompng($watermarkfile);break;
        default:break;
    }

	if(empty($water_im))
	{
		return '';
	}
    //原图
    if(!file_exists($srcfile) || !$src_info = getimagesize($srcfile))
    {
    	return '';
    }
    $src_w = $src_info[0];
    $src_h = $src_info[1];
    $src_im = '';
    switch($src_info[2])
    {
        case 1:
        	//判断是否为动画
        	$fp = fopen($srcfile, 'rb');
			$filecontent = fread($fp, filesize($srcfile));
			fclose($fp);
			if(strpos($filecontent, 'NETSCAPE2.0') === FALSE)
			{//动画图不加水印
        		@$src_im = imagecreatefromgif($srcfile);
			}
        	break;
        case 2:@$src_im = imagecreatefromjpeg($srcfile);break;
        case 3:@$src_im = imagecreatefrompng($srcfile);break;
        default:break;
    }

    if(empty($src_im))
    {
    	return '';
    }

    //加水印的图片的长度或宽度比水印小150px
    if(($src_w < $water_w + 100) || ($src_h < $water_h + 100))
    {
    	return '';
    }

    //位置
	switch($_Cfg['watermarkpos'])
	{
		case 1://顶端居左
			$posx = 0;
			$posy = 0;
			break;
		case 2://顶端居右
			$posx = $src_w - $water_w;
			$posy = 0;
			break;
		case 3://底端居左
			$posx = 0;
			$posy = $src_h - $water_h;
			break;
		case 4://底端居右
			$posx = $src_w - $water_w;
			$posy = $src_h - $water_h;
			break;
		default://随机
			$posx = mt_rand(0, ($src_w - $water_w));
			$posy = mt_rand(0, ($src_h - $water_h));
			break;
	}

    //设定图像的混色模式
	@imagealphablending($src_im, true);
	//拷贝水印到目标文件
	@imagecopy($src_im, $water_im, $posx, $posy, 0, 0, $water_w, $water_h);
    switch($src_info[2])
    {
        case 1:@imagegif($src_im, $srcfile);break;
        case 2:@imagejpeg($src_im, $srcfile);break;
        case 3:@imagepng($src_im, $srcfile);break;
        default:return '';
    }
	@imagedestroy($water_im);
	@imagedestroy($src_im);
	return true;
}


//图片上传,单个单个上传
function upload($file,$folder='images',$maxSize=0,$minSize=0,$extarr='')
{
  global $_Cfg,$_SG;
  $fileurl = $filepath = '';
  if(!array_key_exists('name',$file)) return false;
  !$minSize && $minSize = 120;
  !$maxSize && $maxSize = 3*1024*1024;
  if(!$extarr) $extarr = array('gif','jpg','jpeg','png','bmp');
  $fileurl = ($folder ? $folder.'/':'').date("YmdHi").random(2).'.'.fileExt($file['name']);
  $filepath = S_ROOT.$_Cfg['uploaddir'];
  if(!is_dir($filepath)) mkdir($filepath,0777);
  $filepath .= $fileurl;
  if(!is_dir(dirname($filepath))) mkdir(dirname($filepath),0777);
  if(!move_uploaded_file($file['tmp_name'],$filepath)) return false;
  if(!file_exists($filepath)) return '';
  return $fileurl;
  }

?>