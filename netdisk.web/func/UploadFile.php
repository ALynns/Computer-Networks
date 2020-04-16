<?php
//	define('ROOT',"/home/u1751763/pan/temp/");
//	define('Real',"/home/u1757163/pan/file/");
	define('ROOT',dirname(__FILE__)."/../pan/temp/");
	define('Real',dirname(__FILE__)."/../pan/file/");

    include_once("../func/sys.php");
    include_once("../func/mariadbOpt.php");	


	if(isset($_COOKIE['Username'])){
		$username = $_COOKIE['Username'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();
	}

	if(isset($_COOKIE['Password'])){
		$password = $_COOKIE['Password'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();
	}

	if(isset($_COOKIE['fname'])){
		$filename = ($_COOKIE['fname']);
	}
	else{
        AlertBack('User login expired2','Login.php');
        exit();		
	}
	$filename=iconv("UTF-8","gbk//TRANSLIT",$filename);
	
	// current dict
	if(isset($_COOKIE['curDir'])){
		$curDir = $_COOKIE['curDir'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();		
	}



    if($_FILES["upload_file"]["error"]>0){
		AlertBack("upload file error","UserPaga.php");
		exit();
	}else{
		echo "upload file name: ".$_FILES["upload_file"]["name"]."<br>";
		echo "       file type: ".$_FILES["upload_file"]["type"]."<br>";
		echo "       file size: ".$_FILES["upload_file"]["size"]."<br>";
		echo "   tmp file load: ".$_FILES["upload_file"]["tmp_name"]."<br>";


		if(file_exists("../pan/temp/".$_FILES["upload_file"]["name"])){
			echo $_FILES["upload_file"]["name"]." exists";
		}
		else{
			$stroed_path = ROOT.basename($filename);

			move_uploaded_file($_FILES["upload_file"]["tmp_name"], $stroed_path);

			$md5 = md5_file($stroed_path);

			// move&save file 
			copy($stroed_path, Real.$md5);
			unlink($stroed_path);

			addFile($md5);
			addUserFile($username,$filename,$curDir,$md5);
		}
		
		header("refresh:0;../page/UserPage.php");
	}

?>