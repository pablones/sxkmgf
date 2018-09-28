<?php echo $this->fetch('pageheader.htm'); ?>
<div class="main-div">
    <form action="item.php" method="post" enctype="multipart/form-data" name="searchForm">
        <table>
             <tr>
                <td width="60"> <strong><?php echo $this->_var['lang']['username']; ?>:</strong> </td>
                <td> <input type="text" name="username" value="<?php echo $this->_var['info']['username']; ?>"  /> </td>
                <td  width="60"> <div align="left"><strong><?php echo $this->_var['lang']['rolename']; ?>:</strong></div> </td>
                <td> <input type="text" name="rolename" value="<?php echo $this->_var['info']['rolename']; ?>" /></td>
             </tr>
             <tr>
                <td colspan="4">
                        <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
                        <input name="reset" type="button" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>'"/> 
                        <input type="hidden" name="act" value="search">                   
                </td>
             </tr>   
        </table>
    </form>
</div>
<h1>
<span class="action-span" style="float:left"><?php echo $this->_var['lang']['bag_info']; ?></span>
</h1>
<?php if ($this->_var['view'] == 1): ?>
<div class="list-div" >
	<table cellspacing='1' cellpadding='3'>
  		<tr>
  	  		<th><?php echo $this->_var['lang']['username']; ?></th>
  	  		<th><?php echo $this->_var['lang']['rolename']; ?></th>
  	  		<th><?php echo $this->_var['lang']['level']; ?></th>
  	  		<th><?php echo $this->_var['lang']['item_name']; ?></th>
  	  		<th><?php echo $this->_var['lang']['item_strong']; ?></th>
  	  		<th><?php echo $this->_var['lang']['item_start']; ?></th>
  	  		<th><?php echo $this->_var['lang']['handle']; ?></th>
  		</tr>
		<?php $_from = $this->_var['result']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'res');if (count($_from)):
    foreach ($_from AS $this->_var['res']):
?>
    	<tr align="center">
        	<td><?php echo $this->_var['res']['account']; ?></td>
        	<td><?php echo $this->_var['res']['playername']; ?></td>
        	<td><?php echo $this->_var['res']['level']; ?></td>
       	 	<td><?php echo $this->_var['res']['name']; ?></td>
			<td><?php echo $this->_var['res']['addlev']; ?></td>
			<td><?php echo $this->_var['res']['star']; ?></td>

        	<td>
            	<span><a href = "item.php?act=edit&id=<?php echo $this->_var['res']['id']; ?>&username=<?php echo $this->_var['info']['username']; ?>&rolename=<?php echo $this->_var['info']['rolename']; ?>"><?php echo $this->_var['lang']['edit']; ?></a></span>
        	</td>
    	</tr>
    	<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>

  </table>
</div>
<?php endif; ?>
<?php echo $this->fetch('pagefooter.htm'); ?>