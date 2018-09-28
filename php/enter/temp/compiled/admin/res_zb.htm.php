<?php echo $this->fetch('pageheader.htm'); ?>
<?php echo $this->smarty_insert_scripts(array('files'=>'../js/utils.js,selectzone.js,colorselector.js')); ?>
<?php echo $this->smarty_insert_scripts(array('files'=>'../js/transport.js,common.js')); ?>
<link type="text/css" href="./templates/js/jquery-ui-1.8.14.custom.css" rel="stylesheet" />	
<script type="text/javascript" src="./templates/js/jquery.min.js"></script>
<script type="text/javascript" src="./templates/js/jquery-ui-1.8.14.custom.min.js"></script>

<script>
$(document).ready(function(){
	$('.id2').hide();
	$('#select1').change(function() {
		if($('#select1  option:selected').val() == "<?php echo $this->_var['lang']['staff']; ?>"){
			$('.id2').show();
		}else{
			$('.id2').hide();
		}
		//alert('#select1 option:selected').val());
	});
	$('#query').click(function(){
		if($('#select1  option:selected').val() == "<?php echo $this->_var['lang']['staff']; ?>"){
			if($('#action').val()==''||$('#request').val()==''){
				alert('<?php echo $this->_var['lang']['user_null']; ?>');
				return false;
			}
	    }
	});
});
</script>


<form action="resource.php?act=zb_query" method="post" enctype="multipart/form-data" name="searchForm">
<table width="90%" id="mix-table"  align="center">
    <tr>
		<td>  
           <a href="javascript:;" onclick="addImg(this)">[+]</a>
            <strong><?php echo $this->_var['lang']['platform_account']; ?>:</strong><input type="text" name="platform_account[]" id="platform_account" >
            <strong><?php echo $this->_var['lang']['zb_amount']; ?>:</strong><input type ="text" name="zb_amount[]" id="yb_amount" />           
        </td>
    </tr>
	<tr>
		<td> <strong><?php echo $this->_var['lang']['input_account']; ?>:</strong><input type="file" name="input_account" id="input_account" /></td>
	</tr>
	<tr>
        <td width="30%"> 
            <strong><?php echo $this->_var['lang']['add_type']; ?>:</strong> 
            <select id="select1" name="add_type">
                 <option value="<?php echo $this->_var['lang']['reward']; ?>"><?php echo $this->_var['lang']['reward']; ?></option>
                 <option value="<?php echo $this->_var['lang']['compensation']; ?>" selected="selected"><?php echo $this->_var['lang']['compensation']; ?></option>
                 <option value="<?php echo $this->_var['lang']['staff']; ?>"><?php echo $this->_var['lang']['staff']; ?></option>
             </select>
         </td>
    </tr>
    <tr class="id2">
        <td width="60"> 
            <strong><?php echo $this->_var['lang']['request']; ?>:</strong> 
            <input type="text" name="request" id="request"  />
            <strong><?php echo $this->_var['lang']['act_man']; ?>:</strong>
            <input type="text" name="action" id="action"  />
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

  function removeImg(obj)
  {
      var row = rowindex(obj.parentNode.parentNode);
      var tbl = document.getElementById('mix-table');
      //var tbl  = document.getElementById('test');

      tbl.deleteRow(row);
  }
</script>
<?php echo $this->fetch('pagefooter.htm'); ?>