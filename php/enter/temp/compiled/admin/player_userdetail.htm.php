<?php echo $this->fetch('pageheader.htm'); ?>

<div class="list-div">
<table>
<tr> <td> <strong><?php echo $this->_var['lang']['detail_tip1']; ?>&nbsp;[<?php echo $this->_var['info']['name']; ?>]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(<?php echo $this->_var['lang']['detail_role']; ?>:&nbsp; "<?php echo $this->_var['info']['role']; ?>"&nbsp;<?php echo $this->_var['lang']['detail_tip2']; ?>)</strong></td></tr>

</table>
<table>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_account']; ?></th>
        <td><?php echo $this->_var['info']['name']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_role']; ?></th>
        <td><?php echo $this->_var['info']['role']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['sex']; ?></th>
        <td><?php echo $this->_var['info']['sex']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_reg_time']; ?></th>
        <td><?php echo $this->_var['info']['reg_time']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_reg_ip']; ?></th>  
        <td><?php echo $this->_var['info']['reg_ip']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_lastexit']; ?></th>  
        <td><?php echo $this->_var['info']['last_exit']; ?></td>
    </tr>
    

    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_yb']; ?></th>
        <td><?php echo $this->_var['info']['yb']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_rmb']; ?></th>
        <td><?php echo $this->_var['info']['rmb']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_gold1']; ?></th>
        <td><?php echo $this->_var['info']['gold1']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_gold2']; ?></th>  
        <td><?php echo $this->_var['info']['gold2']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_gold3']; ?></th>  
        <td><?php echo $this->_var['info']['gold3']; ?></td>
    </tr>   
     <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_gold4']; ?></th>
        <td><?php echo $this->_var['info']['gold4']; ?></td>
    </tr>    
    
    
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_feel']; ?></th>
        <td><?php echo $this->_var['info']['Feel']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_job']; ?></th>
        <td><?php echo $this->_var['info']['Fir_Job']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_joblevel']; ?></th>  
        <td><?php echo $this->_var['info']['Fir_JobLv']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_job2']; ?></th>
        <td><?php echo $this->_var['info']['Sec_Job']; ?></td>
    </tr>
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_joblevel2']; ?></th>  
        <td><?php echo $this->_var['info']['Sec_JobLv']; ?></td>
    </tr>   
    <tr>
        <th width="90" align="left"><?php echo $this->_var['lang']['detail_money']; ?></th>  
        <td>
            <table>
                <tr>
                    <td width="70" align="left"><?php echo $this->_var['lang']['detail_sid']; ?></td><td><?php echo $this->_var['lang']['detail_yb_sy']; ?></td>
                 </tr>
              <?php $_from = $this->_var['rec_vas']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('k', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['k'] => $this->_var['result']):
?>
                 <tr>
                    <td><?php echo $this->_var['k']; ?></td><td><?php echo $this->_var['result']; ?></td>
                 </tr>
              <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>          
            </table>
        </td>
    </tr>   

    
</table>
<!-- 分页 TOOD 未来处理 -->
</div>
<?php echo $this->fetch('pagefooter.htm'); ?>