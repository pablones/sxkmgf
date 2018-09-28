
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
    <form action="player.php?act=list" method="post" enctype="multipart/form-data" name="searchForm">
        <table>
             <tr>
                <td width="60"> <strong><?php echo $this->_var['lang']['username']; ?>：</strong> </td>
                <td> <input type="text" name="username" value="<?php echo $this->_var['info']['username']; ?>"  /> </td>
                <td  width="60"> <div align="left"><strong><?php echo $this->_var['lang']['rolename']; ?>：</strong></div> </td>
                <td> <input type="text" name="rolename" value="<?php echo $this->_var['info']['rolename']; ?>" /></td>
                <td  width="80"><div align="left"><strong><?php echo $this->_var['lang']['list_slevel']; ?>：</strong></div></td>
                <td> <input type="text" name="startlevel" value="<?php echo $this->_var['info']['startlevel']; ?>"  /> </td>
                <td  width="80"><div align="left"><strong><?php echo $this->_var['lang']['list_elevel']; ?>：</strong></div></td>
                <td> <input type="text" name="endlevel" value="<?php echo $this->_var['info']['endlevel']; ?>" /> </td>
             </tr>
             <tr>
                <td width="60"> <div align="left"><strong><?php echo $this->_var['lang']['list_sdate']; ?>:</strong></div></td>
                <td width="60"> <input type="text" name="startdate" maxlength="60" size="20" readonly="readonly" id="start_time_id"  value="<?php echo $this->_var['info']['startdate']; ?>"/>  </td>
                <td  width="60"> <div align="left"><strong><?php echo $this->_var['lang']['list_edate']; ?>:</strong></div></td>
                <td> <input type="text" name="enddate" maxlength="60" size="20" readonly="readonly" id="end_time_id"  value="<?php echo $this->_var['info']['enddate']; ?>"/> </td>
                <td  width="60"><div align="left"><strong><?php echo $this->_var['lang']['list_regip']; ?>：</strong></div></td>
                <td >
                    <input type="text" name="regip" value="<?php echo $this->_var['info']['regip']; ?>" />
                </td>
             <tr>
                <td colspan="3">
                        <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
                        <input name="reset" type="button" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' onclick="funClear()"/> 
                        <input type="hidden" name="action" value="list">
                    
                </td>
             </tr>   
        </table>
    </form>
</div>



<div class="list-div" >


  
  <table cellspacing='1' cellpadding='3'>
  	<tr>
  	  <th><?php echo $this->_var['lang']['list_userid']; ?></th>
  	  <th><?php echo $this->_var['lang']['rolename']; ?>ID</th>
  	  <th><?php echo $this->_var['lang']['list_username']; ?></th>
  	  <th><?php echo $this->_var['lang']['list_role']; ?></th>
  	  <th><?php echo $this->_var['lang']['list_level']; ?></th>
  	  <th><?php echo $this->_var['lang']['list_regip']; ?></th>
  	  <th><?php echo $this->_var['lang']['list_regtime']; ?></th>
  	  <th><?php echo $this->_var['lang']['list_detail']; ?></th>
  	</tr>
 
<?php $_from = $this->_var['rs']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>
    <tr align="center">
        <td><?php echo $this->_var['result']['id']; ?></td>
        <td><?php echo $this->_var['result']['roleid']; ?></td>
        <td><?php echo $this->_var['result']['name']; ?></td>
        <td><?php echo $this->_var['result']['role']; ?></td>
        <td><?php echo $this->_var['result']['level']; ?></td>
        <td><?php echo $this->_var['result']['netbar_ip']; ?></td>
        <td><?php echo $this->_var['result']['regdate']; ?></td>
        <td>
            <?php if ($this->_var['result']['id']): ?>
                <a href="player.php?act=userdetail&userid=<?php echo $this->_var['result']['id']; ?>"><?php echo $this->_var['lang']['list_detail']; ?></a>
            <?php endif; ?>
        </td>
    </tr>
    <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>

  </table>

<table>
    <tr>
        <th>
                      共<?php echo $this->_var['num']; ?>页
        <a href='player.php?act=list&page=1&<?php echo $this->_var['post']; ?>'>首页&nbsp;&nbsp;</a>
        <?php if ($this->_var['lastpage'] >= 1): ?>
        <a href='player.php?act=list&page=<?php echo $this->_var['lastpage']; ?>&<?php echo $this->_var['post']; ?>'>上一页&nbsp;&nbsp;</a>
        <?php else: ?>
                      上一页&nbsp;&nbsp;
        <?php endif; ?>
        <?php $_from = $this->_var['pagenum1']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>
        <a href='player.php?act=list&page=<?php echo $this->_var['result']; ?>&<?php echo $this->_var['post']; ?>'><?php echo $this->_var['result']; ?>&nbsp;</a>
        <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
        <?php if ($this->_var['nextpage'] <= $this->_var['num']): ?>
        <a href='player.php?act=list&page=<?php echo $this->_var['nextpage']; ?>&<?php echo $this->_var['post']; ?>'>下一页&nbsp;&nbsp;</a>
        <?php else: ?>
                       下一页&nbsp;&nbsp;
        <?php endif; ?>
        <a href='player.php?act=list&page=<?php echo $this->_var['num']; ?>&<?php echo $this->_var['post']; ?>'>末页</a>
                    当前第<?php echo $this->_var['page']; ?>页
        </th>
    </tr>

</table>


<?php echo $this->fetch('pagefooter.htm'); ?>
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