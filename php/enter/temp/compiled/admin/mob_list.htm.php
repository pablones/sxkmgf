<?php echo $this->fetch('pageheader.htm'); ?>
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
	<form action="mobsearch.php?act=query" method="post" enctype="multipart/form-data" name="searchForm">
    	<table>
       		<tr>
                <td width="60"> <strong><?php echo $this->_var['lang']['username']; ?>：</strong> </td>
                <td> <input type="text" name="username" value=""  /> </td>
              
            </tr>
			<tr>
                <td colspan="3">
                    <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_search']; ?>"/>
                	<input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />                    
                </td>
             </tr>   
        </table>
    </form>
</div>
<div class="list-div" >
	<table cellspacing='1' cellpadding='3'>
  		<tr>
  	  		<th><?php echo $this->_var['lang']['account_id']; ?></th>
  	  		<th><?php echo $this->_var['lang']['account_name']; ?></th>
  	  		<th><?php echo $this->_var['lang']['type']; ?></th>
  	 	 	<th><?php echo $this->_var['lang']['typename']; ?></th>
  	  		<th><?php echo $this->_var['lang']['generalize_id']; ?></th>
  	  		<th><?php echo $this->_var['lang']['stateid']; ?></th>
  	  		<th><?php echo $this->_var['lang']['state']; ?></th>
  		</tr>
		
    	<tr align="center">
        	<td><?php echo $this->_var['result']['account_id']; ?></td>
        	<td><?php echo $this->_var['result']['rolename']; ?></td>
        	<td><?php echo $this->_var['result']['type']; ?></td>
        	<td><?php echo $this->_var['result']['typename']; ?></td>
        	<td><?php echo $this->_var['result']['generalize_id']; ?></td>
        	<td><?php echo $this->_var['result']['state']; ?></td>
        	<td><?php echo $this->_var['result']['lstate']; ?></td>
    	</tr>
    	
  </table>
</div>
<?php echo $this->fetch('pagefooter.htm'); ?>