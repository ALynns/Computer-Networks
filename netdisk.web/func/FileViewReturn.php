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

	if(isset($_COOKIE['Password'])){
		$password = $_COOKIE['Password'];
	}
	else{
        AlertBack('User login expired','Login.php');
        exit();		
	}

	$curFolder = $_GET['folder'];

    $conn = getConnect();
    $Opt = "select * from $username";
    $result = mysqli_query($conn, $Opt);
    $num = mysqli_num_rows($result);
    // FileKey FileName FileType FileStatus FatherNum RealFile
    for($x=0;$x<$num;$x++){
    	$row[$x] = mysqli_fetch_assoc($result);
	}
    closeConnect();

    for($x = 0; $x < $num; $x++){
    	if($row[$x]['FileKey'] == $curFolder){
    		$curFolder = $row[$x]['FatherNum'];
			setcookie('curDir',$curFolder,time()+3600,"/");
			header("refresh:0;../page/UserPage.php");
    	}
	}

?>