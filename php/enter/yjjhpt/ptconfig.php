<?php


define('PT_UPDATEMIN',3*60);                     //非心跳包的请求间隔,单位[s]
define('PT_SESSDEL_LIFE',3*60);                  //超过此时间间隔的算离线,每隔分钟清理一次
define('PT_USER_OLTIME',3*60);                   //每次在线时长大于180秒才进行汇总,少于就忽略
define('PT_USER_TM',40);                         //心跳包的时间间隔,单位[s]

?>