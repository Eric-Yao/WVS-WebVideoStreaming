<?php

include("pagecookie.php");
include("pageheader.php");
include("vheader.php");
include("func_query_word.php");
?>


<html>
<head><title>Svenska</title></head>

<body>
<?php echo $page_head;?>
<br>

<?php 
?>

<div align="center">
<fieldset style="width:600px;">
<legend>Verb</legend>
  <table style="width:600px;"> <col width="80px"/> <col width="520px" />
   <tr>
      <td><?php echo $verb_head;?></td>
      <td>
       <?php
         if(isset($_POST['submit'])){
            $searched_word = $_POST["searched_word"];
            //echo "results";
            //echo "results".$searched_word;
            echo '<table align="center"> <tr> 
              <td>| Infinitiv |</td>
              <td>| Presens |</td>
              <td>| Infinitiv |</td>
              <td>| Imperfekt |</td>
              <td>| Supinum |</td>
              <td>| Imperativum |</td>
              <td>| English |</td>
              </tr>';
            $prep = "SELECT infinitive FROM swedish_verbs WHERE";
            $prep = $prep." infinitive='".$searched_word;
            $prep = $prep."' OR presens='".$searched_word;
            $prep = $prep."' OR imperfekt='".$searched_word;
            $prep = $prep."' OR supinum='".$searched_word;
            $prep = $prep."' OR imperativum='".$searched_word."'";
            $tag = 1;
           func_query_word($prep, $tag);
            echo '</table>';
         }
       ?> 
       </td>
    </tr>
   <table>
</fieldset>
</div>

</body>
</html>