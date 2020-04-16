<?php
    function getConnect() {
        $hostname = "localhost";
        $database = "db1751763";
        $userName = "u1751763";
        $password = "u1751763";

        $conn = mysqli_connect($hostname, $userName, $password, $database) or die (mysql_error());
        mysqli_query($conn,"set names gbk");

        return $conn;
    }

    function closeConnect() {
        global $databaseConnection;
        if ($databaseConnection) {
            mysqli_close($databaseConnection) or die (mysql_error());
        }
    }


    function addFile($md5){
        $conn = getConnect();
        $loginSQL = "insert into File set MessageDigset='$md5',FilePath='/home/httpd/www-1751763/pan/file/',ConNum=1";
        $resultLogin = mysqli_query($conn, $loginSQL);
        closeConnect();
    }

    function addUserFile($username,$filename,$curDir,$md5){
        $conn = getConnect();
        $loginSQL = "insert into $username set FileName='$filename',FileType=1,FatherNum=$curDir,RealFile='$md5'";
        $resultLogin = mysqli_query($conn, $loginSQL);
        closeConnect();
    }

    function delUserFile($username,$filename,$curDir){
        // del user label & del File conNum
        $conn = getConnect();
        $loginSQL = "select * from $username where FileName='$filename' and FatherNum='$curDir'";
        $resultLogin = mysqli_query($conn, $loginSQL);
        closeConnect();

        if(mysqli_num_rows($resultLogin) < 0)
            Alert("delete error");
        else {
            $f = mysqli_fetch_assoc($resultLogin);
            if($f['FileType'] == 0){
                $conn = getConnect();
                $ccDir = $f['FileKey'];
                $loginSQL = "select * from $username where FatherNum='$ccDir'";
                $resultLogin = mysqli_query($conn, $loginSQL);
                closeConnect();

                $num = mysqli_num_rows($resultLogin);
    
                // FileKey FileName FileType FileStatus FatherNum RealFile
                for($x = 0; $x<  $num; $x++){
                    $row[$x] = mysqli_fetch_assoc($resultLogin);
                    delUserFile($username,$row[$x]['FileName'],$ccDir);
                }

            }
            else{
                $realf = $f['RealFile'];

                $conn = getConnect();
                $loginSQL = "select ConNum from File where MessageDigset='$realf'";
                $resultLogin = mysqli_query($conn, $loginSQL);
                closeConnect();

                $ff = mysqli_fetch_assoc($resultLogin);
                $num = (int)$ff['ConNum'] - 1;

                $conn = getConnect();
                $loginSQL = "update File set ConNum='$num' where MessageDigset='$realf'";
                $resultLogin = mysqli_query($conn, $loginSQL);
                closeConnect();
            }

            $conn = getConnect();
            $loginSQL = "delete from $username where FileName='$filename' and FatherNum='$curDir'";
            $resultLogin = mysqli_query($conn, $loginSQL);
            closeConnect();
        }

    }

    function addFolder($username,$dirname,$curDir){
        $conn = getConnect();
        $loginSQL = "insert into $username set FileName='$dirname',FileType=0,FatherNum=$curDir";
        $resultLogin = mysqli_query($conn, $loginSQL);
        closeConnect();       
    }

    function search($tableName,$key,$value){
        $conn = getConnect();
        $loginSQL = "select * from $tableName where $key='$value'";
        $resultLogin = mysqli_query($conn, $loginSQL);
        closeConnect();

        $num = mysqli_num_rows($resultLogin);

            echo "sea:";
            echo $num;

        if($num == 0)
            return 0;
        else 
            return 1;
    }


    function checkDir($tableName,$dirName,$curDir){
        $conn = getConnect();
        $loginSQL = "select * from $tableName where FileName='$dirName'";
        $resultLogin = mysqli_query($conn, $loginSQL);
        closeConnect();

        echo "dir:";
        echo $tableName.$dirName.$curDir;
        echo "dirend";

//        if(mysqli_num_rows($resultLogin) <= 0)
        if($resultLogin === false)
            return 0;
        else {
            $num = mysqli_num_rows($resultLogin);

            // FileKey FileName FileType FileStatus FatherNum RealFile
            for($x = 0; $x<  $num; $x++){
                $row[$x] = mysqli_fetch_assoc($resultLogin);
                if($row[$x]['FatherNum']==$curDir)
                    return 1;
            }
            return 0;
        }
    }



?>
