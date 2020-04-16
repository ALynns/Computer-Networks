<?php
	$curFolder = $_GET['folder'];
	setcookie('curDir',$curFolder,time()+3600,"/");

    header("refresh:0;../page/UserPage.php");
?>