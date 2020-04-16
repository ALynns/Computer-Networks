<?php
// pay attention to del cookie
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

	if(isset($_COOKIE['ctrlX'])){
		$ctrlX = $_COOKIE['ctrlX'];
		if(isset($_COOKIE['ctrlXFolder'])){
			$ctrlXFolder = $_COOKIE['ctrlXFolder'];
			
			if(checkDir($username,$ctrlX,$curDir) == 1){
				AlertBack('same name file/folder exits','UserPage.php');
				exit();
			}

	        $conn = getConnect();
    	    $loginSQL = "select * from $username where FileName='$ctrlX' and FatherNum='$ctrlXFolder'";
        	$resultLogin = mysqli_query($conn, $loginSQL);
        	closeConnect();
            // FileKey FileName FileType FileStatus FatherNum RealFile
            $row = mysqli_fetch_assoc($resultLogin);
            $md5 = $row['RealFile'];

			addUserFile($username,$ctrlX,$curDir,$md5);
			delUserFile($username,$ctrlX,$ctrlXFolder);
		}
		else{
	        AlertBack('crtl expired','UserPage.php');
    	    exit();	
		}
	}
	else if(isset($_COOKIE['ctrlC'])){
		$ctrlC = $_COOKIE['ctrlC'];
		if(isset($_COOKIE['ctrlCFolder'])){
			$ctrlCFolder = $_COOKIE['ctrlCFolder'];
			
			if(checkDir($username,$ctrlC,$curDir) == 1){
				AlertBack('same name file/folder exits','UserPage.php');
				exit();
			}

	        $conn = getConnect();
    	    $loginSQL = "select * from $username where FileName='$ctrlC' and FatherNum='$ctrlCFolder'";
        	$resultLogin = mysqli_query($conn, $loginSQL);
        	closeConnect();
            // FileKey FileName FileType FileStatus FatherNum RealFile
            $row = mysqli_fetch_assoc($resultLogin);
            $md5 = $row['RealFile'];

			addUserFile($username,$ctrlC,$curDir,$md5);
		}
		else{
	        AlertBack('crtl expired','UserPage.php');
    	    exit();	
		}		
	} else {
	    AlertBack('nothing ctrl V','UserPage.php');
	    exit();
	}
	
	// del cookies
	setcookie("ctrlX","",time()-3600,"/");
	setcookie("ctrlC","",time()-3600,"/");
	setcookie("ctrlXFolder","",time()-3600,"/");
	setcookie("ctrlCFolder","",time()-3600,"/");

    AlertBack("ctrl + V successfully","UserPage.php");
?>