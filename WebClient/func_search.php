
<?php

include("pagecookie.php");


function func_search() {
echo '<form method = "post" action="'.$_SERVER['PHP_SELF'].'">
<div align="center">     
  <label for="Input word">Input word here: </label><br>
  <input type="text" name="searched_word"/>
  <input type="submit" value="Search" name="submit"/> 
</fieldset> 
</div>
</form>';

} 
?>