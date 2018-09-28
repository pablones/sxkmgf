<?php echo $this->fetch('pay_header.htm'); ?>
<link type="text/css" href="./templates/js/jquery-ui-1.8.14.custom.css" rel="stylesheet" />	
<script type="text/javascript" src="./templates/js/jquery.min.js"></script>
<script type="text/javascript" src="./templates/js/jquery-ui-1.8.14.custom.min.js"></script>
<script>
	$(function() {
		$("#start_time_id").datepicker();
		$("#end_time_id").datepicker();
	});
</script>

<div class="main-div">
    <form action="pay_consume.php?act=ybsearch&<?php echo $this->_var['post']; ?>" method="post" enctype="multipart/form-data" name="searchForm">
       <table>
          <tr>
             <td width="80"> <strong><?php echo $this->_var['lang']['platform_account']; ?></strong> </td>
             <td width="326"><input type="text" name="username" id="username" value="<?php echo $this->_var['info']['username']; ?>"></td>
             <td width="324" align="left">
	             <strong><?php echo $this->_var['lang']['player_name']; ?></strong>  
                 <input type="text" name="rolename" id="rolename" value="<?php echo $this->_var['info']['rolename']; ?>">
              </td>
              <td width="324" align="left">
	             <strong><?php echo $this->_var['lang']['paynum']; ?></strong>  
                 <input type="text" name="paynum" id="paynum" value="<?php echo $this->_var['info']['paynum']; ?>">
              </td>
          </tr>
          <tr>
              <td width="80"> <div align="left"><strong><?php echo $this->_var['lang']['label_time']; ?></strong></div> </td>
              <td width="326">
	             <input type="text" name="start_time" maxlength="60" size="20" readonly="readonly" id="start_time_id"  value="<?php echo $this->_var['info']['start_time']; ?>"/> ~
                 <input type="text" name="end_time" maxlength="60" size="20" readonly="readonly" id="end_time_id"  value="<?php echo $this->_var['info']['end_time']; ?>" />
	          </td>
              <td width="324">  
                 <input type="radio" value="1" name="search_range" checked='true'><?php echo $this->_var['lang']['self_server']; ?>
                 <input type="radio" value="0" name="search_range"><?php echo $this->_var['lang']['all_server']; ?>
	          </td>
           </tr>
           <tr>
              <td colspan="3">
                  <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
                 <input name="reset" type="button" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' onclick="funClear()"/>  
               </td>
           </tr>              
       </table>
     </form>
</div>
<div class="list-div">
<?php if ($this->_var['count']['yb'] > 0): ?>
	<h1><?php echo $this->_var['lang']['all_pay_yb']; ?>：<?php echo $this->_var['count']['yb']; ?>&nbsp;&nbsp;<?php echo $this->_var['lang']['rmb_total']; ?>：￥<?php echo $this->_var['count']['rmb']; ?></h1>
<?php endif; ?>
<table cellspacing='1' cellpadding='3'>
    <tr>
        <th><?php echo $this->_var['lang']['platform_account']; ?></th>
        <th><?php echo $this->_var['lang']['player_name']; ?></th>
        <th><?php echo $this->_var['lang']['game_server']; ?></th>
        <th><?php echo $this->_var['lang']['pay_gold']; ?></th>
        <th><?php echo $this->_var['lang']['pay_money']; ?></th>
        <th><?php echo $this->_var['lang']['pay_time']; ?></th>
        <th><?php echo $this->_var['lang']['pay_ip']; ?></th>
        <th><?php echo $this->_var['lang']['paynum']; ?></th>
    </tr>
    <?php $_from = $this->_var['res']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>
    <tr>
    
        <td><?php echo $this->_var['result']['username']; ?></td>
        <td><?php echo $this->_var['result']['rolename']; ?></td>
        <td><?php echo $this->_var['result']['serverid']; ?></td>
        <td><?php echo $this->_var['result']['paygold']; ?></td>
        <td><?php echo $this->_var['result']['paymoney']; ?></td>
        <td><?php echo $this->_var['result']['logdate']; ?></td>
        <td><?php echo $this->_var['result']['ip']; ?></td>
        <td><?php echo $this->_var['result']['paynum']; ?></td>
    </tr>
    <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
</table>
<table>
    <tr>
        <th>
                      共<?php echo $this->_var['num']; ?>页
        <a href='pay_consume.php?act=ybsearch&page=1&<?php echo $this->_var['post']; ?>'>首页&nbsp;&nbsp;</a>
        <?php if ($this->_var['lastpage'] >= 1): ?>
        <a href='pay_consume.php?act=ybsearch&page=<?php echo $this->_var['lastpage']; ?>&<?php echo $this->_var['post']; ?>'>上一页&nbsp;&nbsp;</a>
        <?php else: ?>
                      上一页&nbsp;&nbsp;
        <?php endif; ?>
        <?php $_from = $this->_var['pagenum1']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>
        <a href='pay_consume.php?act=ybsearch&page=<?php echo $this->_var['result']; ?>&<?php echo $this->_var['post']; ?>'><?php echo $this->_var['result']; ?>&nbsp;</a>
        <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
        <?php if ($this->_var['nextpage'] <= $this->_var['num']): ?>
        <a href='pay_consume.php?act=ybsearch&page=<?php echo $this->_var['nextpage']; ?>&<?php echo $this->_var['post']; ?>'>下一页&nbsp;&nbsp;</a>
        <?php else: ?>
                       下一页&nbsp;&nbsp;
        <?php endif; ?>
        <a href='pay_consume.php?act=ybsearch&page=<?php echo $this->_var['num']; ?>&<?php echo $this->_var['post']; ?>'>末页</a>
                    当前第<?php echo $this->_var['page']; ?>页
        </th>
    </tr>

</table>
    
</div>    

<script >
  function funClear(){
      var txts=document.getElementsByTagName("input");  
      for(var i=0;i<txts.length;i++)  
      {  
        if(txts[i].type=="text")  
        {  
          txts[i].value ="";  
        }  
      }
 }
</script>
  
<?php echo $this->fetch('pagefooter.htm'); ?>