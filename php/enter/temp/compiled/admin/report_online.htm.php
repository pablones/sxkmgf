<?php echo $this->fetch('report_header.htm'); ?>
<link type="text/css" href="./templates/js/jquery-ui-1.8.14.custom.css" rel="stylesheet" />	
<script type="text/javascript" src="./templates/js/jquery.min.js"></script>
<script type="text/javascript" src="./templates/js/jquery-ui-1.8.14.custom.min.js"></script>
<script>
	$(function() {
		$("#start_time_id").datepicker();
		$("#end_time_id").datepicker();
	});
</script>

<div class="main-div">
    <form action="report.php?act=online" method="post" enctype="multipart/form-data" name="searchForm">
        <table>
             <tr>
                <td  width="40" align="left"><strong><?php echo $this->_var['lang']['online_date']; ?>：</strong></td>
                <td>
                    <input type="text" name="time" maxlength="60" size="20" readonly="readonly" id="start_time_id" />
                </td>
             <tr>
                <td colspan="3">
                        <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
                        <input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />
                    
                </td>
             </tr>   
        </table>
    </form>
</div>



<div class="list-div">
<table>
    <tr>
        <th width="150" align="left"><?php echo $this->_var['lang']['online_time']; ?></th>
        <th width="100" align="left"><?php echo $this->_var['lang']['online_one']; ?></th>
        <th width="100" align="left"><?php echo $this->_var['lang']['online_tow']; ?></th>
        <th width="100" align="left"><?php echo $this->_var['lang']['online_three']; ?></th>
        <th width="100" align="left"><?php echo $this->_var['lang']['online_four']; ?></th>
        <th width="100" align="left"><?php echo $this->_var['lang']['online_five']; ?></th>
        <th width="100" align="left"><?php echo $this->_var['lang']['online_six']; ?></th>
        <th align="left"><?php echo $this->_var['lang']['online_sum']; ?></th>
    </tr>
    <?php $_from = $this->_var['time']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>
    <tr>
        <td><?php echo $this->_var['result']['times']; ?></td>       
        <td>
        <?php if ($this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['one']]): ?>
            <?php echo $this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['one']]; ?>
        <?php else: ?>
            0
        <?php endif; ?>
        </td>        
        <td>
        <?php if ($this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['two']]): ?>
            <?php echo $this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['two']]; ?>
        <?php else: ?>
           0
        <?php endif; ?>
        <td>
        <?php if ($this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['three']]): ?>
            <?php echo $this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['three']]; ?>
        <?php else: ?>
           0
        <?php endif; ?>
        </td>
        <td>
        <?php if ($this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['four']]): ?>
            <?php echo $this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['four']]; ?>
        <?php else: ?>
           0
        <?php endif; ?>
        </td>
         <td>
        <?php if ($this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['five']]): ?>
            <?php echo $this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['five']]; ?>
        <?php else: ?>
           0
        <?php endif; ?>
        </td>
        <td>
        <?php if ($this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['six']]): ?>
            <?php echo $this->_var['info'][$this->_var['result']['times']][$this->_var['lang']['six']]; ?>
        <?php else: ?>
           0
        <?php endif; ?>
        </td>
        <td><?php echo $this->_var['result']['sum']; ?></td>
    </tr>
    <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>
</table>
<!-- 分页 TOOD 未来处理 -->

<?php echo $this->fetch('pagefooter.htm'); ?>