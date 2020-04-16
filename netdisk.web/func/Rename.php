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

	if(isset($_COOKIE['rename'])){
		$rename = $_COOKIE['rename'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();
	}
	
	$sName = $_GET['sName'];

	if(checkDir($username,$rename,$curDir) == 1){
		AlertBack('Folder name has been used','UserPage.php');
		exit();
	}

	echo "user: ".$username."<br>"; 
	echo "filename: ".$rename."<br>"; 
	echo "sourname: ".$sName."<br>";
	echo "curDir: ".$curDir."<br>";

	$conn = getConnect();
    $loginSQL = "update $username set FileName='$rename' where FileName='$sName' and FatherNum='$curDir'";
    $resultLogin = mysqli_query($conn, $loginSQL);
    closeConnect();
	
	header("refresh:0;../page/UserPage.php");
?>