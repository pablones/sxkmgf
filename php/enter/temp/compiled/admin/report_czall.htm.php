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
    <form action="report.php?act=czall" method="post" enctype="multipart/form-data" name="searchForm">
        <table>
             <tr>
                <td width="80"><div align="left"><strong><?php echo $this->_var['lang']['create_stime']; ?>：</strong></div></td>
                <td align="left" width="240">
                    <input type="text" name="start_time" maxlength="60" size="20" readonly="readonly" id="start_time_id" />
                </td>
                <td  width="80"><div align="left"><strong><?php echo $this->_var['lang']['create_etime']; ?>：</strong></div></td>
               <td>
                    <input type="text" name="end_time" maxlength="60" size="20" readonly="readonly" id="end_time_id" />
                </td>
              
             <tr>
                <td colspan="3">
                        <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
                        <input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />
                    
                </td>
             </tr>   
        </table>
    </form>
</div>



<div class="main-div">
	<table>
		<tr>
			<td><strong><?php echo $this->_var['lang']['err_server']; ?></strong></td>
		</tr>
		<tr>
			<td>
				<?php $_from = $this->_var['err']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'data_0_46343400_1318571285');if (count($_from)):
    foreach ($_from AS $this->_var['data_0_46343400_1318571285']):
?>
				<span><?php echo $this->_var['data_0_46343400_1318571285']; ?>,</span>
				<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
			</td>
		</tr>
	</table>
</div>
<div class="list-div">
	<table>
		<tr>
		    <th align="left">日&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;期</th>
			<?php $_from = $this->_var['data']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('key', 'data_0_46357200_1318571285');if (count($_from)):
    foreach ($_from AS $this->_var['key'] => $this->_var['data_0_46357200_1318571285']):
?>
			<th align="left" width="60"><?php echo $this->_var['key']; ?>&nbsp;&nbsp;服</th>
			<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
			<th align="left" >&nbsp;&nbsp;合计</th>
		</tr>
		<?php $_from = $this->_var['date']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'date_0_46368700_1318571285');if (count($_from)):
    foreach ($_from AS $this->_var['date_0_46368700_1318571285']):
?>
		<tr>
		    <td><?php echo $this->_var['date_0_46368700_1318571285']; ?></td>
		<?php $_from = $this->_var['data']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'data_0_46378000_1318571285');if (count($_from)):
    foreach ($_from AS $this->_var['data_0_46378000_1318571285']):
?>
		    <td><?php echo $this->_var['data_0_46378000_1318571285'][$this->_var['date_0_46368700_1318571285']]; ?></td>
		<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
		  <td><?php echo $this->_var['sum_day'][$this->_var['date_0_46368700_1318571285']]; ?></td>
		</tr>   
		<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
		<tr>
		<td>合计</td>
		<?php $_from = $this->_var['data']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('id', 'data_0_46395000_1318571285');if (count($_from)):
    foreach ($_from AS $this->_var['id'] => $this->_var['data_0_46395000_1318571285']):
?>
		    <td><?php echo $this->_var['sum'][$this->_var['id']]; ?></td>
		<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
		<td><?php echo $this->_var['sum_all']; ?></td>
		</tr>  
	</table>
	
</div>

<?php echo $this->fetch('pagefooter.htm'); ?>