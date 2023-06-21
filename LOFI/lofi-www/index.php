<html>
<title>I am so hackable it's ridiculous</title>
<body>
<h1>I am so hackable it's ridiculous</h1>
<h1> Seriously, it's embarrassing</h1>
<img src="casino.gif">

<?php
   $file = $_GET['page'];
   if(isset($file))
   {
       include("$file");
   }
?>
