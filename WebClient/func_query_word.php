<?php

include("pagecookie.php");



  
function func_query_word($prep, $tag) {
	
  include("conninfo.php");
  
  $result_tag = 0;
  
  $host = $datahost;
  $user = $datauser;
  $pass = $datapass;
  $name = $dataname;

  $mysqli = new mysqli($host, $user, $pass, $name);
  /* check connection */
  if ($mysqli->connect_errno) {
  	printf("The login server is busy now, please try later");
    exit();
  }
    
  $stmt = $mysqli->prepare($prep);
  $stmt->bind_result($cnt_result);
  $stmt->execute();
  $stmt->store_result();

  $num_of_rows = $stmt->num_rows;
  //echo "nums: ".$num_of_rows;
  if ($num_of_rows > 0){
  //	if ($tag==1) 
  	{
  	  echo "<tr>"; 
      $stmt->fetch(); 
      echo "<td>".$cnt_result."</td>";
   //   echo "<td>".$cnt_result['infinitive']."</td>";
   //   echo "<td>".$cnt_result['infinitive']."</td>";
   //   echo "<td>".$cnt_result['infinitive']."</td>";
   //   echo "<td>".$cnt_result['infinitive']."</td>";
   //   echo "<td>".$cnt_result['infinitive']."</td>";
      //}
      echo "</tr>";
      
      $result_tag = 1;
  	}
  } else {
  	echo "Not found the word!";
  }
  $stmt->close();
  $mysqli->close();
  
  if ($result_tag == 1) {
  	return 1;
  } else {
  	return 0;
  }
}
?>
