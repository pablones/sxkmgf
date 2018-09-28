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
    <form action="report.php?act=allconsume" method="post" enctype="multipart/form-data" name="searchForm">
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
				<?php $_from = $this->_var['err']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'data_0_43415100_1318571283');if (count($_from)):
    foreach ($_from AS $this->_var['data_0_43415100_1318571283']):
?>
				<span><?php echo $this->_var['data_0_43415100_1318571283']; ?>,</span>
				<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
			</td>
		</tr>
	</table>
</div>

<div class="list-div">
	<table>
		<tr>
		    <th>日&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;期</th>
			<?php $_from = $this->_var['data']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('key', 'data_0_43429000_1318571283');if (count($_from)):
    foreach ($_from AS $this->_var['key'] => $this->_var['data_0_43429000_1318571283']):
?>
			<th><?php echo $this->_var['key']; ?>服</th>
			<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
			<th>合计</th>
		</tr>
		<?php $_from = $this->_var['date']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'date_0_43440700_1318571283');if (count($_from)):
    foreach ($_from AS $this->_var['date_0_43440700_1318571283']):
?>
		<tr>
		    <td><?php echo $this->_var['date_0_43440700_1318571283']; ?></td>
		<?php $_from = $this->_var['data']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'data_0_43474600_1318571283');if (count($_from)):
    foreach ($_from AS $this->_var['data_0_43474600_1318571283']):
?>
		    <td><?php echo $this->_var['data_0_43474600_1318571283'][$this->_var['date_0_43440700_1318571283']]; ?></td>
		<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
		  <td><?php echo $this->_var['sum_day'][$this->_var['date_0_43440700_1318571283']]; ?></td>
		</tr>   
		<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
		<tr>
		<td>合计</td>
		<?php $_from = $this->_var['data']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('id', 'data_0_43507700_1318571283');if (count($_from)):
    foreach ($_from AS $this->_var['id'] => $this->_var['data_0_43507700_1318571283']):
?>
		    <td><?php echo $this->_var['sum'][$this->_var['id']]; ?></td>
		<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
		<td><?php echo $this->_var['sum_all']; ?></td>
		</tr>  
	</table>
	
</div>

<?php echo $this->fetch('pagefooter.htm'); ?>