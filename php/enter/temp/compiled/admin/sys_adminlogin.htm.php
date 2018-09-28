<?php echo $this->fetch('pageheader.htm'); ?>
<script type="text/javascript ">
function check_input(){

    if(document.forms['frm_center'].username.value != ''){
        return true;
    }else{
        alert('<?php echo $this->_var['lang']['js_checkusernam']; ?>');
        return false;
    }
}   
</script>
<form onsubmit="return check_input();" target="_self" id="frm_center" name="frm_center" method="get" action="sys_config.php">
    <table width="90%" id="mix-table"  align="center">
        <tr>
            <td>
                <input type="hidden" value="yjjh" name="game_src"/>
                <input type="hidden" value="<?php echo $this->_var['server_id']; ?>" name="serverid"/> <!-- <?php echo $this->_var['server_id']; ?>-->
                <strong><?php echo $this->_var['lang']['username']; ?></strong>
                <input type="text"  id="username" name="username"/>
            </td>
        </tr>
        <tr>
            <td>
                <input type="hidden" name="act"  value="login_query"/>
                <input type="submit" id="button" class='button' name="button" value="<?php echo $this->_var['lang']['login']; ?>" />
            </td>
        </tr>
        <tr>
        	<td><strong><?php echo $this->_var['lang']['tips']; ?></strong></td>
        </tr>
    </table>
</form>
<?php echo $this->fetch('pagefooter.htm'); ?>