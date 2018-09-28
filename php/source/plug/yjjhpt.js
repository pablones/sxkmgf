// yjjhpt  @charse=utf8
// 2010-9-20 by xiaofu [ver 1.0]

var yjjhpt = {
  ajRooturi:'',
  ajUserPHP:'_ajUser.php',
  ajSummaryPHP:'_ajSummary.php',
  ajUserSend:function(){ $.post(site.rooturi+'yjjhpt/'+ajUser+(new Date()).valueOf()); },
  ajSummarySend:function(){ $.post(site.rooturi+'yjjhpt/'+ajSummaryPHP+(new Date()).valueOf()); },
  };
