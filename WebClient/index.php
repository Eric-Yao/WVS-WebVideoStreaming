<?php
//session_start();

//注销登录
if($_GET['action'] == "logout"){
//    unset($_SESSION['cnt_id']);
//    unset($_SESSION['user']);
    unset($_COOKIE['cnt_id']);
   // setcookie('cnt_id', NULL);
    setcookie("cnt_id", "", time()-3600);
    echo '<br/> <br/> <div align="center"">Logout succeed. Press here to <a href="index.php">Login In</a></div>';
    exit;
}

//phpinfo();
if(!isset($_COOKIE['cnt_id'])) {
  if(isset($_POST['submit'])) {
      
    //$today=date("Y-m-d H:i:s");
    //echo($today);
    $user = $_POST["user"];
    $pass = $_POST["pass"];
    
    setcookie('cnt_id',1);
    setcookie('user',$user);
    // echo "<hr><p>Logined User: $user<p>";  
    $home_url = 'loged.php';
    header('Location: '.$home_url);
  } else {
    $error_msg = '</br>';//Sorry, you must enter a username and password to log in.';
  }
}else {
  $home_url = 'loged.php';
  header('Location: '.$home_url);
  //echo "<hr><p>Logined User: $user<p>";
  //echo "<hr><p>Logined User: $user<p>";
  //  echo "<br>Now, you can <a href=loged.php>Go to user center</a>";
}
?>



<html>
<head><title>Svenska</title></head>

<body>

<?php 
if(empty($_COOKIE['cnt_id'])){
echo '<div align="center""><p class="error">'.$error_msg.'</p></div>'; 
?> 

<form method = "post" action="<?php echo $_SERVER['PHP_SELF'];?>">
<div align="center">     

<fieldset style="width:250px;"> 
<legend>Cloud Gaming</legend> 
</br>
<input type="submit" value="Start test" name="submit"/>
</br> 
</fieldset> 
</div>
</form> 

<?php 
} else {
echo "aaaaa";

}
?>



</body>
</html>
