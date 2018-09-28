<?php echo $this->fetch('pageheader.htm'); ?>
<script type="text/javascript" src="../templates/js/jquery.min.js"></script>
<div class="list-div">
	<table cellspacing='1' cellpadding='3'>
		<tr>
			<td width="10%"><strong><?php echo $this->_var['lang']['servername']; ?></strong></td>
			<td width="15%">
				<?php if ($this->_var['view_all_server'] == 1): ?>
				<select name="pingtai" id="pingtai">
					<option value="0"><?php echo $this->_var['lang']['please_select']; ?></option>
					<?php $_from = $this->_var['all_server']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'server');if (count($_from)):
    foreach ($_from AS $this->_var['server']):
?>
				  	<option value="<?php echo $this->_var['server']; ?>" <?php if ($this->_var['server'] == $this->_var['servers']): ?> selected="selected" <?php endif; ?>><?php echo $this->_var['server']; ?></option>
				  	<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
			 	</select>
			 	<?php else: ?>
			 	<strong><?php echo $this->_var['all_server']; ?></strong>
			 	<input type="hidden" id = "pingtai" name ="pingtai" value="<?php echo $this->_var['all_server']; ?>"/>
			 	<?php endif; ?>
			</td>
					
			<td width="10%"><strong><?php echo $this->_var['lang']['server']; ?></strong></td>
			<td width ="15%">
				<input type ="text" id ="fuwuqi" name="fuwuqi"/>
			</td>
			<td>
				<input type="button" id="jump" name="jump" class="button" value="<?php echo $this->_var['lang']['jump']; ?>" onclick="jumpto();"/>
			</td>
		</tr>
	</table>
</div>
<script>	
	function jumpto(){		
		var servername = document.getElementById('pingtai').value;
		var fuwuqi = document.getElementById('fuwuqi').value;
		if(fuwuqi == ''){
			alert("err_notic");
			return false;
		}
		var url = "jump_server.php?act=jump_query&servername="+servername+"&fuwuqi="+fuwuqi;
		window.location.href = url;
	}
</script>
<?php echo $this->fetch('pagefooter.htm'); ?>