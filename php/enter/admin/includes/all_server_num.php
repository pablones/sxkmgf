<?php
	
	//平台名数组
	$all_server_name = array(
		'4399' => '4399', 
		'91wan' => '91wan',
		'37wan' => '37wan',
		'kuaiwan' => 'kuaiwan',
		'360' => '360',
		'aosheng' => 'aosheng',
		'kuwo' => 'kuwo',
		'IS' => 'IS',
		'woyo' => 'woyo',
		'fengxing' => 'fengxing',
		'TW' => 'TW',
		'PPTV' => 'PPTV',
		'05wan' => '05wan',
		'FL' => 'FL',
		'131' => '131',
	    'yuenan' =>'yuenan',
	    'Wtest' => 'Wtest', //外网测试服
	    
	);
	
	//后端域名 
	$pages = array(
		'4399' => array(
			'prev' => 'yjjh', //前缀
			'last' => 'bak.my4399.com', //后缀
		),	
		'91wan' => array(
			'prev' => 'yjjh',
			'last' => 'bak.91wan.com',
		),
		'37wan' => array(
			'prev' => 's',
			'last' => 'bak.yjjh.37wan.com',
		),
		'kuaiwan' => array(
			'prev' => 'yjjh',
			'last' => 'bak.kuaiwan.com',
		),
		'360' => array(
			'prev' => 's',
			'last' => 'bak.yjjh.wan.360.cn',
		),
		'aosheng' => array(
			'prev' => 'yjjh',
			'last' => 'bak.kx1d.com',
		),
		'kuwo' => array(
			'prev' => 's',
			'last' => 'bak.yjjh.kuwo.cn',
		),
		'TW' => array(
			'prev' => 'yjjh',
			'last' => 'bak.6mp.com.tw',
		),
		'131' => array(
			'prev' => 'yjjh',
			'last' => 'bak.131.com',
		),
		'FL' => array(
			'prev' => 'yjjh',
			'last' => 'bak.fy5.com',
		),
		'IS' => array(
			'prev' => 's',
			'last' => 'bak.yjjh.iugame.cn',
		),
		'PPTV' => array( 
			'prev' => 's',
			'last' => 'bak.yjjh.g.pptv.com',
		),
		'fengxing' => array( 
			'prev' => 's',
			'last' => 'bak.yjjh.funshion.com',
		),
		'woyo' => array( 
			'prev' => 'yjjh',
			'last' => 'bak.web.woyo.com',
		),
		'05wan' => array(
			'prev' => 's',
			'last' => 'bak.yjjh.05wan.com',
		),
		'yuenan' => array(
			'prev' => 'yjjh',
			'last' => 's.nhatkiem.com/enter/admin',
		),
	);
  
	//前端域名
	$prevpages = array(
		'4399' => array(
			'prev' => 'yjjh', //前缀
			'last' => '-s.my4399.com/4399', //后缀
		),
		'131' => array(
			'prev' => 'yjjh',
			'last' => '-s.131.com/enter',
		),
		'kuaiwan' => array(
			'prev' => 'yjjh',
			'last' => '-s.kuaiwan.com/enter',
		),
		'TW' => array(
			'prev' => 'yjjh',
			'last' => '-s.6mp.com.tw/enter',
		),
		'360' =>array(
			'prev' => 's',
			'last' => '.yjjh.wan.360.cn/enter',
		),	
		'FL' =>array(
			'prev' => 'yjjh',
			'last' => '-s.fy5.com/enter',
		),
		'kuwo' =>array(
			'prev' => 's',
			'last' => '.yjjh.kuwo.cn/enter',
		),
		'91wan' => array(
			'prev' => 'yjjh',
			'last' => '-s.91wan.com/enter',
		),	  
		'IS' => array(
			'prev' => 's',
			'last' => '.yjjh.iugame.cn/enter',
		),
		'PPTV' => array(
			'prev' => 's',
			'last' => '.yjjh.g.pptv.com/enter',
		),
		'fengxing' => array( 
			'prev' => 's',
			'last' => '.yjjh.funshion.com/enter',
		),
		'37wan' => array( 
			'prev' => 's',
			'last' => '.yjjh.37wan.com/enter',
		),
		'aosheng' => array( 
			'prev' => 'yjjh',
			'last' => '-s.kx1d.com/enter',
		),
		'woyo' => array(
			'prev' => 'yjjh',
			'last' => '-s.web.woyo.com/enter',
		),
		'05wan' => array(
			'prev' => 's',
			'last' => '.yjjh.05wan.com/enter',
		),
		'yuenan' => array(
			'prev' => 'yjjh',
			'last' => '-s.nhatkiem.com/enter',
		),
		
	);
	//开服数量
	$arr_s=array(
		'4399'=> 62,
		'91wan'=> 33,
		'37wan'=>1,
		'kuaiwan'=>10,
		'360'=>15,
		'aosheng'=>6,
		'kuwo'=>5,
		'IS'=>6,
		'woyo'=>1,
		'fengxing'=>3,
		'TW'=>4,
        'PPTV'=>1,
        '05wan'=>1,
        'FL'=>5,
		'131' => 1,
	    'Wtest' => 1, //外网测试服
		'yuenan' => 9,
	);
	
	//记录合服到几服
	$hefu = array(
		'4399' => 30,
        '91wan' => 24,
        'IS' => 4,
        '05wan'=>2,
        'aosheng'=>4,
	    '360' => 6,
	    'FL' => 2,
		'kuwo'=>4,
	    'fengxing'=>2,
	);
	
	$modes_ticket = array(
		'4399'=> '4399*:yjjh*:aX4L*:41@-504',
		'91wan'=> '91wan_weedong*:yjjh*:aX4L*:41@-504',
		'37wan'=> '37wan*:yjjh*:aX4L*:41@-504',
		'kuaiwan'=> 'kuaiwan:yujianjianghu:login:073c8df7-8a54-4776-b0d8-db15e4b92fec',
		'360'=> '360*:yjjh&:aX4L*:41@-504',
		'aosheng'=> 'aoshengwan*:yjjh*:aX5F*:51@-5962',
		'kuwo'=> 'kuwo*:yjjh&:aX4L*:41@-kw',
		'IS'=> 'IS*:yjjh&:aX4isL*:41@-652',
		'woyo'=> 'woyo*:yjjh*:OWYOL*:41@-FDS',
		'fengxing'=> 'fengxing*#yjjh&:aX4L*$41@-kw',
		'TW'=> 'taiwan::wJCz@pay::frhSQFDATgrfnnUBZ',
        'PPTV'=> 'yujianjianghuo:44%^@#***(77',
        '05wan'=> '05*:yjjh&:aX4L*:41@-05-king-@#$%^&()*_+',
        'FL'=> 'feile*!@#$%11235813*yujian(*feile*)',
		'131' =>  'yujian:131@#$%&*qn48z!7authorking.L',
		'yuenan' => 'yjjh-yuenan!@#$%^&*()11235813xxyy',
	);

?>