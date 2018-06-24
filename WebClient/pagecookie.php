<?php

function check_cookie() {
if(!isset($_COOKIE['cnt_id'])){
  $home_url = 'index.php';
  header('Location: '.$home_url);
  exit;
  }
}


?>
