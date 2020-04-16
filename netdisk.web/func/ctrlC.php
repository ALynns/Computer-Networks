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

	setcookie("ctrlX","",time()-3600,"/");
	setcookie("ctrlXFolder","",time()-3600,"/");

	setcookie("ctrlC",$filename,time()+3600,"/");
	setcookie("ctrlCFolder",$curDir,time()+3600,"/");

    AlertBack("ctrl + C successfully","UserPage.php");
?>
