<?php echo $this->fetch('pageheader.htm'); ?>
<table width="90%" id="mix-table"  align="center">
    <form action="resource.php?act=activity_query" method="post" enctype="multipart/form-data" name="searchForm">
    <tr>
        <td width="80"><strong><?php echo $this->_var['lang']['act_type']; ?></strong></td>
        <td>
            <!-- TODO 后期处理-->
            <select name="cardtype" id="cardtype" >
                <option value="1">签名活动</option>
                <option value="3">万兽之王</option>
            </select>
        </td>
    </tr>
    <tr>
        <td width="80"><strong><?php echo $this->_var['lang']['act_account']; ?></strong></td>
        <td><input type ="file" name="act_account" id="act_account" /></td>
    </tr>
    <tr>
        <td colspan="2">
            <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
            <input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />
        </td>
    </tr>
    </form>
</table>
<?php if ($this->_var['display'] == 1): ?>
<div class="list-div">
<table cellspacing='1' cellpadding='3'>
    <tr>
        <th><?php echo $this->_var['lang']['error_account']; ?></th>
    </tr>
    <?php $_from = $this->_var['error']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>
    <tr>    
        <td width = "100%" align="center"><?php echo $this->_var['result']; ?></td>
    </tr>
    <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
</table>
</div>      
<?php endif; ?>
<?php echo $this->fetch('pagefooter.htm'); ?>