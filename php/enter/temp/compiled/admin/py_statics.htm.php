<?php echo $this->fetch('pageheader.htm'); ?>
<ul style="padding:0; margin: 0; list-style-type:none; color: #CC0000;">

</ul>
<!-- directory install end -->

<div class="list-div">
<table cellspacing='1' cellpadding='3'>
  <tr>
    <th colspan="4" class="group-title"><?php echo $this->_var['lang']['system_info']; ?></th>
  </tr>
  <tr>
    <td width="10%"><?php echo $this->_var['lang']['all_pay_amount']; ?></td>
    <td  colspan="3"><?php echo $this->_var['rs']['count_paylog']; ?></td>
  </tr>
  <tr>
    <td width="10%"><?php echo $this->_var['lang']['all_pay_rmb']; ?></td>
    <td width="20%"><?php echo $this->_var['rs']['sum_paymoney']; ?></td>
    <td width="10%"><?php echo $this->_var['lang']['all_pay_yb']; ?></td>
    <td width="20%"><?php echo $this->_var['rs']['sum_paygold']; ?></td>
  </tr>
  <tr>
    <td><?php echo $this->_var['lang']['each_pay_rmb']; ?></td>
    <td><?php echo $this->_var['rs']['avg_paymoney']; ?></td>
    <td><?php echo $this->_var['lang']['each_pay_yb']; ?></td>
    <td><?php echo $this->_var['rs']['avg_paygold']; ?></td>
  </tr>
  <tr>
    <td><?php echo $this->_var['lang']['max_pay_rmb']; ?></td>
    <td><?php echo $this->_var['rs']['max_paymoney']; ?></td>
    <td><?php echo $this->_var['lang']['max_pay_yb']; ?></td>
    <td><?php echo $this->_var['rs']['max_paygold']; ?></td>
  </tr>
  <tr>
    <td><?php echo $this->_var['lang']['min_pay_rmb']; ?></td>
    <td><?php echo $this->_var['rs']['min_paymoney']; ?></td>
    <td><?php echo $this->_var['lang']['min_pay_yb']; ?></td>
    <td><?php echo $this->_var['rs']['min_paygold']; ?></td>
  </tr>
  <tr>
    <td><?php echo $this->_var['lang']['all_consume_yb']; ?></td>
    <td><?php echo $this->_var['res']['count_uselog']; ?></td>
    <td><?php echo $this->_var['lang']['each_consume_yb']; ?></td>
    <td><?php echo $this->_var['res']['avg_uselog']; ?></td>
  </tr>
  <tr>
    <td><?php echo $this->_var['lang']['max_consume_yb']; ?></td>
    <td><?php echo $this->_var['res']['max_uselog']; ?></td>
    <td><?php echo $this->_var['lang']['min_consume_yb']; ?></td>
    <td><?php echo $this->_var['res']['min_uselog']; ?></td>
  </tr>
</table>
</div>

<?php echo $this->fetch('pagefooter.htm'); ?>
