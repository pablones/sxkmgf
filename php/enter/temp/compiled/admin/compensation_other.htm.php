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

<form action="compensation.php?act=other" method="post" enctype="multipart/form-data" name="searchForm">
<table width="90%" id="mix-table"  align="center">
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['compensation_type']; ?></strong></td>
        <td>
            <select name="type" id="type" align="left">
                <option value="3"><?php echo $this->_var['lang']['other3']; ?></option>
                <option value="4"><?php echo $this->_var['lang']['other4']; ?></option>
                <option value="5"><?php echo $this->_var['lang']['other5']; ?></option>
                <option value="7"><?php echo $this->_var['lang']['other7']; ?></option>
                <option value="8"><?php echo $this->_var['lang']['other8']; ?></option>
                <option value="9"><?php echo $this->_var['lang']['other9']; ?></option>
            </select>
        </td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['is_username']; ?></strong></td>
        <td align="left"><input type ="text" name="username" id="username" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['compensation_amount']; ?></strong></td>
        <td align="left"><input type ="text" name="amount" id="amount" /></td>
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
<?php echo $this->fetch('pagefooter.htm'); ?>