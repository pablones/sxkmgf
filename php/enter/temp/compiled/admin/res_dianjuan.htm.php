<?php echo $this->fetch('pageheader.htm'); ?>
<?php echo $this->smarty_insert_scripts(array('files'=>'../js/utils.js,selectzone.js,colorselector.js')); ?>
<?php echo $this->smarty_insert_scripts(array('files'=>'../js/transport.js,common.js')); ?>
<form action="resource.php?act=dianjuan" method="post" enctype="multipart/form-data" name="searchForm">
<table width="90%" id="mix-table"  align="center">
	<tr>
		<td colspan="4">
			<strong><?php echo $this->_var['lang']['add_type']; ?></strong>
			<input type="radio" value="1" name="pay_type" checked='true'><?php echo $this->_var['lang']['is_account']; ?>
			<input type="radio" value="2" name="pay_type"><?php echo $this->_var['lang']['is_username']; ?>
		</td>
	</tr>
    <tr>
		<td>  
           <a href="javascript:;" onclick="addImg(this)">[+]</a>
           <?php echo $this->_var['lang']['account_role']; ?>:<input type="text" name="platform_account[]" id="platform_account" >
           <?php echo $this->_var['lang']['dianjuan_count']; ?>:<input type ="text" name="zb_amount[]" id="yb_amount" />           
        </td>
    </tr>
	<tr>
		<td colospan = 2>
		    <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
			<input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />
		</td>
	</tr>
</table>
</form>


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
<?php echo $this->fetch('pagefooter.htm'); ?>