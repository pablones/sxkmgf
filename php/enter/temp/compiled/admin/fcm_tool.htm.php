<?php echo $this->fetch('pageheader.htm'); ?>
<?php echo $this->smarty_insert_scripts(array('files'=>'../js/utils.js,selectzone.js,colorselector.js')); ?>
<?php echo $this->smarty_insert_scripts(array('files'=>'../js/transport.js,common.js')); ?>
<div class="list-div">
	<form action="fcm_tool.php?act=fcm_query" method="post" enctype="multipart/form-data" name="searchForm">
		<table cellspacing='1' cellpadding='3' id="mix-table">
			<tr>
				<td>  
           			<a href="javascript:;" onclick="addImg(this)">[+]</a>
           			<?php echo $this->_var['lang']['account']; ?>:&nbsp;<input type="text" name="account[]" id="account" >
        		</td>
			</tr>
			<tr>
				<td colspan='2'>
					<input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['btn']; ?>"/>
					<input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['btn_reset']; ?>' />
				</td>
			</tr>
		</table>
	</form>
</div>
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