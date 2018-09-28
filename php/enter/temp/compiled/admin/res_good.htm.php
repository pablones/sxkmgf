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

<form action="resource.php?act=goods_query" method="post" enctype="multipart/form-data" name="searchForm">
<table width="90%" id="mix-table"  align="center">
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['is_username']; ?></strong></td>
        <td><input type ="text" name="username" id="username" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['goods_id']; ?></strong></td>
        <td><input type ="text" name="goods_id" id="goods_id" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['goods_amount']; ?></strong></td>
        <td><input type ="text" name="goods_amount" id="goods_amount" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['overlay_amount']; ?></strong></td>
        <td><input type ="text" name="overlay_amount" id="overlay_amount" /></td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['goods_color']; ?></strong></td>
        <td>
            <select name="goods_color" id="goods_color" >
                <option value="1"><?php echo $this->_var['lang']['white']; ?></option>
                <option value="2"><?php echo $this->_var['lang']['blue']; ?></option>
                <option value="3"><?php echo $this->_var['lang']['green']; ?></option>
                <option value="4"><?php echo $this->_var['lang']['purple']; ?></option>
                <option value="5"><?php echo $this->_var['lang']['orange']; ?></option>
            </select>
        </td>
    </tr>
    <tr>
        <td width="90"><strong><?php echo $this->_var['lang']['is_binding']; ?></strong></td>
        <td>
            <input type="radio" value="1" name="binding" checked='true'><?php echo $this->_var['lang']['bin']; ?>
            <input type="radio" value="0" name="binding"><?php echo $this->_var['lang']['notbin']; ?>
        </td>
    </tr>
	<tr>
        <td width="90"><strong><?php echo $this->_var['lang']['goods_position']; ?></strong></td>
        <td><input type="text" name="goods_position" id="goods_position" /></td>
	</tr>
	<tr>
		<td width="90"><strong><?php echo $this->_var['lang']['input_account']; ?>:</strong></td>
		<td><input type="file" name="input_account" id="input_account" /></td>
	</tr>
	<tr>
         <td width="60"> <strong><?php echo $this->_var['lang']['add_type']; ?>:</strong> </td>
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
        <td> <input type="text" name="request" id="request"  /> </td>
    </tr>
    <tr class="id2">
        <td width="100"> <strong><?php echo $this->_var['lang']['act_man']; ?>:</strong> </td>
        <td> <input type="text" name="action" id="action"  /> </td>
    </tr>
    <tr>
        <td colspan="4">
            <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
            <input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />
        </td>
    </tr>
    <tr>
    	<td colspan="4">
    		<strong><?php echo $this->_var['lang']['goods_notic']; ?><br/><?php echo $this->_var['lang']['goods_notic_1']; ?><br/><?php echo $this->_var['lang']['goods_notic_3']; ?><br/><?php echo $this->_var['lang']['goods_notic_2']; ?><br/></strong>
    	</td>
    </tr>
</table>
</form>
<?php $_from = $this->_var['res']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>

<strong><?php echo $this->_var['result']; ?></strong><br />

<?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>


<?php echo $this->fetch('pagefooter.htm'); ?>