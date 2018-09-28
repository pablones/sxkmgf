<?php echo $this->fetch('pageheader.htm'); ?>
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


<form action="compensation.php?act=item" method="post" enctype="multipart/form-data" name="searchForm">
<table width="90%" id="mix-table"  align="center">

    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['is_username']; ?></strong></td>
        <td align="left"><input type ="text" name="username" id="username" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['item_id']; ?></strong></td>
        <td align="left"><input type ="text" name="typeid" id="typeid" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['compensation_amount']; ?></strong></td>
        <td align="left"><input type ="text" name="amount" id="amount" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['is_binding']; ?></strong></td>
        <td>
            <input type="radio" value="1" name="binding" checked='true'><?php echo $this->_var['lang']['bin']; ?>
            <input type="radio" value="0" name="binding"><?php echo $this->_var['lang']['notbin']; ?>
        </td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['quality']; ?></strong></td>
        <td align="left"><input type ="text" name="quality" id="quality" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['star']; ?></strong></td>
        <td align="left"><input type ="text" name="star" id="star" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['addlev']; ?></strong></td>
        <td align="left"><input type ="text" name="addlev" id="addlev" /></td>
    </tr>
   
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itembase']; ?>1</strong></td>
        <td align="left"><input type ="text" name="itembase1" id="itembase1" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itembase']; ?>2</strong></td>
        <td align="left"><input type ="text" name="itembase2" id="itembase2" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemAddition']; ?>1</strong></td>
        <td align="left"><input type ="text" name="itemAddition1" id="itemAddition1" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemAddition']; ?>2</strong></td>
        <td align="left"><input type ="text" name="itemAddition2" id="itemAddition2" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemAddition']; ?>3</strong></td>
        <td align="left"><input type ="text" name="itemAddition3" id="itemAddition3" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemAddition']; ?>4</strong></td>
        <td align="left"><input type ="text" name="itemAddition4" id="itemAddition4" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemAddition']; ?>5</strong></td>
        <td align="left"><input type ="text" name="itemAddition5" id="itemAddition5" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemAddition']; ?>6</strong></td>
        <td align="left"><input type ="text" name="itemAddition6" id="itemAddition6" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemAddition']; ?>7</strong></td>
        <td align="left"><input type ="text" name="itemAddition7" id="itemAddition7" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemAddition']; ?>8</strong></td>
        <td align="left"><input type ="text" name="itemAddition8" id="itemAddition8" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemGem']; ?>1</strong></td>
        <td align="left"><input type ="text" name="itemGem1" id="itemGem1" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemGem']; ?>2</strong></td>
        <td align="left"><input type ="text" name="itemGem2" id="itemGem2" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['itemGem']; ?>3</strong></td>
        <td align="left"><input type ="text" name="itemGem3" id="itemGem3" /></td>
    </tr>
    <tr>
            <td> <strong><?php echo $this->_var['lang']['add_type']; ?>:</strong></td>
            <td> 
            <select id="select1" name="add_type">
                 <option value="<?php echo $this->_var['lang']['reward']; ?>"><?php echo $this->_var['lang']['reward']; ?></option>
                 <option value="<?php echo $this->_var['lang']['compensation']; ?>" selected="selected"><?php echo $this->_var['lang']['compensation']; ?></option>
                 <option value="<?php echo $this->_var['lang']['staff']; ?>"><?php echo $this->_var['lang']['staff']; ?></option>
             </select>
            </td>
    </tr>
    <tr class="id2">
        <td width="60"> <strong><?php echo $this->_var['lang']['request']; ?>:</strong> </td>
        <td><input type="text" name="request" id="request"  /></td>
    </tr>
    <tr class="id2">
        <td><strong><?php echo $this->_var['lang']['act_man']; ?>:</strong></td>
        <td><input type="text" name="action" id="action"  /></td>
    </tr>
    
    <tr>
        <td colspan="4">
            <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>" onclick="return confirm('<?php echo $this->_var['lang']['whether']; ?>')"/>
            <input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />
        </td>
    </tr>
</table>
</form>

   <?php if ($this->_var['ok'] == 1): ?>
   <script type="text/javascript">
       alert('<?php echo $this->_var['lang']['compensation_success']; ?>');
   </script>
   <?php endif; ?>



<?php echo $this->fetch('pagefooter.htm'); ?>