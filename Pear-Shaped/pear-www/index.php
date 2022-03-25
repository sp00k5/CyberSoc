<?php session_start(); /* Starts the session */

if(!isset($_SESSION['UserData']['Username'])){
	header("location:login.php");
	exit;
}

?>


<!DOCTYPE html>
<html>
<head>
<style>
body {background-color: rgb(14, 14, 15);}
h1   {
    color: rgb(199,199,199);
    font-family: Helvetica, sans-serif;
    font-weight: 700;
}
p    {
    color: rgb(199,199,199);
    font-family: Helvetica, sans-serif;
    font-weight: 700;
    font-size: 20px;
}

hr {
width: 100%;
height: 1px;
background-color: rgb(199,199,199);
margin-right: auto;
margin-left: auto;
margin-top: 5px;
margin-bottom: 5px;
border: 1px rgb(199,199,199);
}

a {
font-family: Helvetica, sans-serif;
font-weight: 700;
font-size: 10px;
color: #FFF;
}

.center {
  margin: auto;
  width: 60%;
  padding: 10px;
  text-align: center;
}
</style>
</head>
<body>

<div class="center">
<h1>!! Congratulations !!</h1>
<hr>
<p> Here at Pear Inc we believe in trust and security. cybersoc{S5L_15_K3Y_H3R3}</p>
<a href="logout.php">LOGOUT</a> 
</div>
</body>
</html>





