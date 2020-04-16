<?php
	include_once("../func/sys.php");
	include_once("../func/mariadbOpt.php");

    $username = $_POST['username'];
    $password = $_POST['password'];
    $Cpassword=$_POST['Cpassword'];

//    global $databaseConnection;

    if($username==''){
    	AlertBack('Username can not be null','Register.php');
    	exit();
    }
    else if(strpos($username, ' ')==0&&$username[0]==' '||strpos($username, ' ')>0){
    	AlertBack('Username can not have space','Register.php');
    	exit();
    }
    else if($password==''){
    	AlertBack('Password can not be null','Register.php');
    	exit();
    }
    else if($Cpassword==''){
    	AlertBack('Please confirm password','Register.php');
    	exit();
    }

    if($password!=$Cpassword){
    	AlertBack('Inconsistent password, please try again','Register.php');
    	exit();
    }

    if(_checkPwLevel($password)!=1){
    	AlertBack('Weak password, please read the tips above','Register.php');
    	exit();
    }

    $conn = getConnect();

    $loginSQL = "select * from UserManage where username='$username'";
    $resultLogin = mysqli_query($conn, $loginSQL);

    if (mysqli_num_rows($resultLogin) > 0) {
    	closeConnect();
    	AlertBack('the name has been used','Register.php');
    	exit();
    } else {

        $password=myhash($username,$password);

        $signinSQL = "insert into UserManage set UserName='$username',password='$password',UserDir='$username'";
        $resultSignin = mysqli_query($conn,$signinSQL);

//        $signinSQL = "create table $username(`ID` int not null,primary key(`ID`))";
//        $resultSignin = mysqli_query($conn,$signinSQL);


        $signinSQL = "create table $username(`FileKey` int(11) not null auto_increment,`FileName` varchar(512) not null,`FileType` int(11) not null,`FileStatus` int(11) not null,`FatherNum` int(11) not null,`RealFile` varchar(32),primary key(`FileKey`),foreign key (`RealFile`) references `File`(`MessageDigset`) on delete cascade on update cascade)";
        $resultSignin = mysqli_query($conn,$signinSQL);


        $signinSQL = "insert into $username set FileName='root',FileType=0,FatherNum=0";
        $resultSignin = mysqli_query($conn,$signinSQL);

//        if(!mysqli_num_rows($conn, $resultSignin)){
        closeConnect();
        AlertBack('Successful registration','Login.php');
//		}
    }


?>