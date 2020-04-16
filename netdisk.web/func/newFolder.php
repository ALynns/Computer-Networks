<?php
	include_once("../func/sys.php");
    include_once("../func/mariadbOpt.php");	


	if(isset($_COOKIE['Username'])){
		$username = $_COOKIE['Username'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();
	}
	
	// current dict
	if(isset($_COOKIE['curDir'])){
		$curDir = $_COOKIE['curDir'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();		
	}

	// current dict
	if(isset($_COOKIE['folderName'])){
		$dirname = $_COOKIE['folderName'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();
	}

	if(checkDir($username,$dirname,$curDir) == 1){
		AlertBack('Folder name has been used','UserPage.php');
		exit();
	}
	addFolder($username,$dirname,$curDir);
	
	header("refresh:0;../page/UserPage.php");
?>