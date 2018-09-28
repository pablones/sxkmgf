<?php echo $this->fetch('pageheader.htm'); ?>
<?php echo $this->smarty_insert_scripts(array('files'=>'../js/utils.js,selectzone.js,colorselector.js')); ?>
<?php echo $this->smarty_insert_scripts(array('files'=>'../js/transport.js,common.js')); ?>

<script language="JavaScript">
 /**
   * 新增一个充值工具
   */
  function addImg(obj)
  {
      var src  = obj.parentNode.parentNode;
      var idx  = rowindex(src);
      var tbl  = document.getElementById('mix-table');
      //var tbl  = document.getElementById('test');
      var row  = tbl.insertRow(idx + 1);
      var cell = row.insertCell(-1);
      cell.innerHTML = src.cells[0].innerHTML.replace(/(.*)(addImg)(.*)(\[)(\+)/i, "$1removeImg$3$4-");
  }

  /**
   * 删除充值
   */
  function removeImg(obj)
  {
      var row = rowindex(obj.parentNode.parentNode);
      var tbl = document.getElementById('mix-table');
      //var tbl  = document.getElementById('test');

      tbl.deleteRow(row);
  }
</script>



<div class="main-div">
	<span><?php echo $this->_var['lang']['notices']; ?></span> 
<form action="tool.php?act=newuser" method="post" enctype="multipart/form-data" name="searchForm">
<table width="90%" id="mix-table"  align="center">
	<tr>
		<td colspan="4">
			<strong><?php echo $this->_var['lang']['type']; ?></strong>
			<input type="radio" value="1" name="tool_type" checked='true'><?php echo $this->_var['lang']['zdy']; ?>
			<input type="radio" value="2" name="tool_type"><?php echo $this->_var['lang']['GM']; ?>
		</td>
	</tr>
    <tr>
		<td>  
           <a href="javascript:;" onclick="addImg(this)">[+]</a>
           <?php echo $this->_var['lang']['rolename']; ?>:<input type="text" name="rolename[]" id="rolename" >
        </td>
    </tr>
	<tr>
		<td colspan='2'>
			<input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
			<input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />
		</td>
	</tr>
</table>

</form>
</div>
<strong><?php echo $this->_var['str']; ?></strong>
<div class="list-div">
<table>
  	<tr>
  	    <th width="100" align="left"><?php echo $this->_var['lang']['rolename']; ?></th>
  	    <th width="100" align="left"><?php echo $this->_var['lang']['tooltype']; ?></th>
  	    <th align="left"><?php echo $this->_var['lang']['newuser_act']; ?></th>
  	</tr>

<?php $_from = $this->_var['rs']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>
    <tr>
        <td><?php echo $this->_var['result']['name']; ?></td>
        <td>
        <?php if ($this->_var['result']['identity'] == 1): ?>
           <?php echo $this->_var['lang']['zdy']; ?>
        <?php elseif ($this->_var['result']['identity'] == 2): ?>
           <?php echo $this->_var['lang']['GM']; ?>
        <?php endif; ?>
        </td>
        <td>
        <?php if ($this->_var['result']['identity'] == 1): ?>
           <a href="tool.php?act=del&role=<?php echo $this->_var['result']['name']; ?>"><?php echo $this->_var['lang']['newuser_del']; ?></a>
        <?php elseif ($this->_var['result']['identity'] == 2): ?>
           <?php echo $this->_var['lang']['newuser_un_del']; ?>                  
        <?php endif; ?>
        </td>
    </tr>
    <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
  </table>

<?php echo $this->fetch('pagefooter.htm'); ?>
