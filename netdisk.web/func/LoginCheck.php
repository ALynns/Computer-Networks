<?php
    include_once("../func/sys.php");
    include_once("../func/mariadbOpt.php");

    $username = $_POST['username'];
    $password = $_POST['password'];

//    global $databaseConnection;

    if($username==''){
        AlertBack('Username can not be null','Login.php');
        exit();
    }
    else if($password==''){
        AlertBack('Password can not be null','Login.php');
        exit();
    }


    $conn = getConnect();

    $loginSQL = "select * from UserManage where username='$username'";
    $resultLogin = mysqli_query($conn, $loginSQL);

    if (mysqli_num_rows($resultLogin) == 0) {
        closeConnect();
        AlertBack('user not exists','Login.php');
        exit();
    } else {

        $password = myhash($username,$password);

        $signinSQL = "select * from UserManage where username='$username'";
        $resultSignin = mysqli_query($conn,$signinSQL);

        $row = mysqli_fetch_assoc($resultSignin);
        $Cpassword = $row['Password'];

        if($Cpassword != $password){
            closeConnect();

            // $resultSignin" Inconsistent password, please try again
            AlertBack('Wrong password, please try again','Login.php');
            exit();
        }
        

        closeConnect();



        setcookie('Username',$username,time()+3600,"/");
        setcookie('Password',$password,time()+3600,"/");

        AlertBack('Successful login','UserPage.php');
    }
?>