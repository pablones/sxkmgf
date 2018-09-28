<?php echo $this->fetch('pageheader.htm'); ?>
<form action="pet.php?act=edit_query" method="post" enctype="multipart/form-data" name="searchForm">
<table width="90%" id="mix-table"  align="center">
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_name']; ?></strong></td>
        <td align="left"><input type ="text" name="pet_name" id="pet_name" value="<?php echo $this->_var['res']['name']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_type']; ?></strong></td>
        <td align="left"><input type ="text" name="pet_type" id="pet_type" value="<?php echo $this->_var['res']['pet_type']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_life']; ?></strong></td>
        <td align="left"><input type ="text" name="life" id="life" value="<?php echo $this->_var['res']['life']; ?>"/></td>
    </tr>
    
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_level']; ?></strong></td>
        <td align="left"><input type ="text" name="level" id="level" value="<?php echo $this->_var['res']['level']; ?>"/></td>
    </tr>     
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_exp']; ?></strong></td>
        <td align="left"><input type ="text" name="exp" id="exp" value="<?php echo $this->_var['res']['exp']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_happy']; ?></strong></td>
        <td align="left"><input type="text" id="happy" name="happy" value="<?php echo $this->_var['res']['happy']; ?>"><?php echo $this->_var['lang']['boy']; ?></td>
    </tr>
    
    <tr>
    	<td width = "15%"><strong><?php echo $this->_var['lang']['huanshou']; ?></strong></td>
    	<td align ="left">
    		<input type="radio" value="2" name="huanshou" id = "huanshou" <?php if ($this->_var['res']['sex'] == 2): ?>checked<?php endif; ?> onchange="SelectOne();"><?php echo $this->_var['lang']['yes']; ?>
			<input type="radio" value="0" name="huanshou" id = "huanshou" <?php if ($this->_var['res']['sex'] == 0 || $this->_var['res']['sex'] == 1): ?>checked<?php endif; ?> onchange="SelectOne();"><?php echo $this->_var['lang']['no']; ?>
		</td>
    </tr>
   
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_understand']; ?></strong></td>
        <td align="left"><input type ="text" name="understand" id="understand" value="<?php echo $this->_var['res']['understand']; ?>"/></td>
    </tr>
    <tr id = "pet_intelligence" <?php if ($this->_var['res']['sex'] != 2): ?>style = "display:none;"<?php endif; ?>>
    	<td width="15%"><strong><?php echo $this->_var['lang']['pet_intelligence']; ?></strong></td>
    	<td align="left"><input type ="text" name = "intelligence" id = "intelligence" value ="<?php echo $this->_var['res']['intelligence']; ?>"/></td>
    </tr>    
    <tr id = "pet_fusion" <?php if ($this->_var['res']['sex'] != 2): ?>style = "display:none;"<?php endif; ?>>
    	<td width ="15%"><strong><?php echo $this->_var['lang']['pet_fusion']; ?></strong></td>
    	<td align="left"><input type = "text" name ="fusion" id = "fusion" value ="<?php echo $this->_var['res']['fusion']; ?>"/></td>
    </tr>
   
	<tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_bone']; ?></strong></td>
        <td align="left"><input type ="text" name="bone" id="bone" value="<?php echo $this->_var['res']['bone']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_lifespan']; ?></strong></td>
        <td align="left"><input type ="text" name="lifespan" id="lifespan" value="<?php echo $this->_var['res']['lifespan']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['pet_strchg']; ?></strong></td>
        <td align="left"><input type ="text" name="strchg" id="strchg" value="<?php echo $this->_var['res']['strchg']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['intchg']; ?></strong></td>
        <td align="left"><input type ="text" name="intchg" id="intchg" value="<?php echo $this->_var['res']['intchg']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['stachg']; ?></strong></td>
        <td align="left"><input type ="text" name="stachg" id="stachg" value="<?php echo $this->_var['res']['stachg']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['spichg']; ?></strong></td>
        <td align="left"><input type ="text" name="spichg" id="spichg" value="<?php echo $this->_var['res']['spichg']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['agichg']; ?></strong></td>
        <td align="left"><input type ="text" name="agichg" id="agichg" value="<?php echo $this->_var['res']['agichg']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['gift']; ?></strong></td>
        <td align="left"><input type ="text" name="gift" id="gift" value="<?php echo $this->_var['res']['gift']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['grow']; ?></strong></td>
        <td align="left"><input type ="text" name="grow" id="grow" value="<?php echo $this->_var['res']['grow']; ?>"/></td>
    </tr>
    <tr>
    	<td width="15%"><strong><?php echo $this->_var['lang']['addaddtion']; ?></strong></strong></td>
    	<td align="left"><input type ="text" name ="addaddtion" id ="addaddtion" value="<?php echo $this->_var['res']['addpoint']; ?>"/></td>
    	<td align="left"><span><?php echo $this->_var['lang']['level_notice']; ?></span></td>
    </tr>
  	<tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill1']; ?></strong></td>
        <td align="left"><input type ="text" name="skill1" id="skill1" value="<?php echo $this->_var['res']['skill_1']; ?>"/></td>
    </tr>
  	<tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill2']; ?></strong></td>
        <td align="left"><input type ="text" name="skill2" id="skill2" value="<?php echo $this->_var['res']['skill_2']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill3']; ?></strong></td>
        <td align="left"><input type ="text" name="skill3" id="skill3" value="<?php echo $this->_var['res']['skill_3']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill4']; ?></strong></td>
        <td align="left"><input type ="text" name="skill4" id="skill4" value="<?php echo $this->_var['res']['skill_4']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill5']; ?></strong></td>
        <td align="left"><input type ="text" name="skill5" id="skill5" value="<?php echo $this->_var['res']['skill_5']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill6']; ?></strong></td>
        <td align="left"><input type ="text" name="skill6" id="skill6" value="<?php echo $this->_var['res']['skill_6']; ?>"/></td>
    </tr>   
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill7']; ?></strong></td>
        <td align="left"><input type ="text" name="skill7" id="skill7" value="<?php echo $this->_var['res']['skill_7']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill8']; ?></strong></td>
        <td align="left"><input type ="text" name="skill8" id="skill8" value="<?php echo $this->_var['res']['skill_8']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill9']; ?></strong></td>
        <td align="left"><input type ="text" name="skill9" id="skill9" value="<?php echo $this->_var['res']['skill_9']; ?>"/></td>
    </tr>
    <tr>
        <td width="15%"><strong><?php echo $this->_var['lang']['skill10']; ?></strong></td>
        <td align="left"><input type ="text" name="skill10" id="skill10" value="<?php echo $this->_var['res']['skill_10']; ?>"/></td>
    </tr>
    <tr>
        <td colspan="4">
        	<input name = "resid" id="resid" type ="hidden" value ="<?php echo $this->_var['res']['id']; ?>"/>
            <input name="query" type="submit" class="button" id="query" value="<?php echo $this->_var['lang']['button_submit']; ?>"/>
            <input name="reset" type="reset" class='button' value='<?php echo $this->_var['lang']['button_reset']; ?>' />
        </td>
    </tr>
</table>
</form>
<script type="text/javascript">
	function SelectOne(){
		var huanshou_state = document.getElementsByName("huanshou");		
		if(huanshou_state[0].checked == true){			
			document.getElementById("pet_intelligence").style.display = "block";
			document.getElementById("pet_fusion").style.display = "block";
		}else{
			document.getElementById("pet_intelligence").style.display = "none";
			document.getElementById("pet_fusion").style.display = "none";
		}
		
		
		
	}
</script>
<?php echo $this->fetch('pagefooter.htm'); ?>