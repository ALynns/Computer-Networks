<?php
	include_once("../func/sys.php");
    include_once("../func/mariadbOpt.php");	

	$filename = $_GET['sName'];

	// current dict
	if(isset($_COOKIE['curDir'])){
		$curDir = $_COOKIE['curDir'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();		
	}

	setcookie("ctrlC","",time()-3600,"/");
	setcookie("ctrlCFolder","",time()-3600,"/");

	setcookie("ctrlX",$filename,time()+3600,"/");
	setcookie("ctrlXFolder",$curDir,time()+3600,"/");

    AlertBack("ctrl + X successfully","UserPage.php");
?>
