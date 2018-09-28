<?php echo $this->fetch('pageheader.htm'); ?>

<div class="main-div">
    <form action="player.php?act=pet" method="post" enctype="multipart/form-data" name="searchForm">
        <table >
             <tr>
                <td  width="60"><div align="left"><strong><?php echo $this->_var['lang']['username']; ?>：</strong></div></td>
                <td width="60">
                    <input type="text" name="username" value=""  />
                </td>
                <td  width="60"><div align="left"><strong><?php echo $this->_var['lang']['rolename']; ?>：</strong></div></td>
                <td>
                    <input type="text" name="rolename" value="" />
                </td>
             </tr>
             <tr>
                <td colspan="3">
                        <input name="query" type="submit" class="button" id="query" value="提交"/>
                        <input name="reset" type="reset" class='button' value="重置" />
                    
                </td>
             </tr>   
        </table>
    </form>
</div>

<div class="list-div">

       <table >
  	       <tr>
  	          <th><?php echo $this->_var['lang']['account']; ?>
  	          </th><th><?php echo $this->_var['lang']['rolename']; ?></th>
  	          <th><?php echo $this->_var['lang']['level']; ?></th>
  	          <th><?php echo $this->_var['lang']['pet_name']; ?></th>
  	          <th><?php echo $this->_var['lang']['pet_type']; ?></th>
  	          <th><?php echo $this->_var['lang']['pet_gettype']; ?></th>
  	          <th><?php echo $this->_var['lang']['pet_grow']; ?></th>
  	          <th><?php echo $this->_var['lang']['pet_understand']; ?></th>
  	          <th><?php echo $this->_var['lang']['pet_maxscore']; ?></th>
  	          <th><?php echo $this->_var['lang']['handle']; ?></th>
  	       </tr>
 

<?php $_from = $this->_var['rs']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }; $this->push_vars('', 'result');if (count($_from)):
    foreach ($_from AS $this->_var['result']):
?>
          <tr>
             <td><?php echo $this->_var['row']['username']; ?></td>
             <td><?php echo $this->_var['row']['name']; ?></td>
             <td><?php echo $this->_var['row']['level']; ?></td>
             <td><?php echo $this->_var['result']['name']; ?></td>
             <td><?php echo $this->_var['result']['type']; ?></td>
             <td><?php echo $this->_var['result']['gettype']; ?></td>
             <td><?php echo $this->_var['result']['grow']; ?></td>
             <td><?php echo $this->_var['result']['understand']; ?></td>
             <td><?php echo $this->_var['result']['maxscore']; ?></td>
             <td><a href="pet.php?act=edit&id=<?php echo $this->_var['result']['id']; ?>"><?php echo $this->_var['lang']['edit']; ?></a></td>
          </tr>
    <?php endforeach; endif; unset($_from); ?><?php $this->pop_vars();; ?>

      </table>
<!-- 分页 TOOD 未来处理 -->  

</div>
<?php echo $this->fetch('pagefooter.htm'); ?>