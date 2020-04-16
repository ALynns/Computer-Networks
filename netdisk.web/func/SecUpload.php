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

	if(isset($_COOKIE['MD5'])){
		$MD5 = $_COOKIE['MD5'];
	}
	else{
        AlertBack('upload file expired1','UserPage.php');
        exit();
	}
	
	if(isset($_COOKIE['SecFile'])){
		$SecFile = $_COOKIE['SecFile'];
	}
	else{
        AlertBack('upload file expired2','UserPage.php');
        exit();
	}
	$SecFile=iconv("UTF-8","gbk//TRANSLIT",$SecFile);

	// check user table whether exist same name first
	if(checkDir($username,$SecFile,$curDir) == 1){
        AlertBack('same name file exits','UserPage.php');
        exit();
	}

	echo $MD5;
	// sec upload
	if(search('File','MessageDigset', $MD5) == 1){
		addUserFile($username,$SecFile,$curDir,$MD5);

        $conn = getConnect();
        $loginSQL = "select ConNum from File where MessageDigset='$MD5'";
        $resultLogin = mysqli_query($conn, $loginSQL);
        closeConnect();

        $ff = mysqli_fetch_assoc($resultLogin);
        $num = (int)$ff['ConNum'] + 1;

        $conn = getConnect();
        $loginSQL = "update File set ConNum='$num' where MessageDigset='$MD5'";
        $resultLogin = mysqli_query($conn, $loginSQL);
        closeConnect();


        setcookie("MD5","",time()-3600,"/");

        AlertBack("SecUpload !","UserPage.php");
//	header("location:../page/UserPage.php");
	}
	else{
		AlertReturn('successfully select');
//        AlertBack("successfully select","UserPage.php");
	}
?>