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

	// current dict
	if(isset($_COOKIE['curDir'])){
		$curDir = $_COOKIE['curDir'];
	}
	else{
		$curDir = 1;
	}
	setcookie('curDir',$curDir,time()+3600,"/");
    
    $conn = getConnect();
    $Opt = "select * from $username";
    $result = mysqli_query($conn, $Opt);

	if($result===false)
		$num = 0;
	else
    	$num = mysqli_num_rows($result);
    

    // FileKey FileName FileType FileStatus FatherNum RealFile
    for($x=0;$x<$num;$x++){
    	$row[$x] = mysqli_fetch_assoc($result);
	}

//	remember to del cookie while exit

    closeConnect();
?>
<!DOCTYPE html>
<html>
	<head>
		<meta charset="gbk">
		<title>My Pan UserPage</title>

		<link href="../styles/userPage.css" rel="stylesheet" type="text/css">

		<script type="text/javascript" src="../scripts/userPage.js"></script>
		<script type="text/javascript" src="../scripts/mariadbOpt.js"></script>
		<script src="../scripts/spark-md5.js"></script>
	</head>
	
	<body>
	<div class="container">	
		<div class="content">
			<?php
				echo "<h1>Hello, $username</h1>";
			?>


			<form id="sendFile" action="../func/UploadFile.php" method="post" enctype="multipart/form-data">
			<fieldset>				
				<legend>Upload File</legend>
				<div>
					<label for="file">Select file: </label>
					<input type="file" name="upload_file" id="upload_file" onchange="fileSelected()">
					<div id='box'></div>
				</div>

				<progress id="progressBar" value="0" max="100" style="width: 300px;"></progress>
				<span id="percentage"></span><span id="time"></span><br>

				<input type="button" onclick="UploadFile()" value="upload">
				<input type="button" onclick="cancelUploadFile()" value="cancel">
			</fieldset>
			</form>

			<h2>My Files</h2>

			<?php
				echo "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
				<a href='../func/ctrlV.php'>ctrl+V</a><br>";
				
				if($curDir == 1){
					echo "<a>/root</a>
					&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
					<a href='../func/newFolder.php' onclick='newFolder()'>new folder</a><br>";
				}
				else{
					echo "<a href = '../func/FileViewReturn.php?folder=$curDir'>../</a>
					&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
					<a href='../func/newFolder.php' onclick='newFolder()'>new folder</a><br>";
				}

				$isNull = 1;
				for($x = 0; $x < $num; $x++){
					if($row[$x]["FatherNum"] == $curDir){
						$FileDig = $row[$x]['RealFile'];
						$FileName = $row[$x]['FileName'];
						$FileName = urldecode($FileName);

						if($row[$x]["FileType"] == 0){		//folder
							$curFolder = $row[$x]['FileKey'];

							echo "<a href = '../func/FileViewNext.php?folder=$curFolder'>$FileName</a>
							&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
							&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
							<a href='../func/DelFile.php?delName=$FileName'>del</a>
							&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
							<a href='../func/Rename.php?sName=$FileName' onclick='Rename()'>rename</a>
							<br>";
							$isNull = 0;
						}
						else{
							echo "<a>$FileName</a>
							&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
							<a href='../pan/file/$FileDig' download='$FileName'>download</a>
							&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
							<a href='../func/DelFile.php?delName=$FileName'>del</a>
							&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
							<a href='../func/Rename.php?sName=$FileName' onclick='Rename()'>rename</a>
							&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
							<a href='../func/ctrlC.php?sName=$FileName'>ctrl+C</a>
							&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
							<a href='../func/ctrlX.php?sName=$FileName'>ctrl+X</a>
							<br>";
							$isNull = 0;
						}
					}
				}
				if($isNull == 1){
					echo "<a>(NULL)</a><br>";
				}

			?>

			<p></p><br>
			<p></p><br>
			<p></p><br>
			<p></p><br>
			<p></p><br>
		</div>

		<div class="footer">
			<span>@2019-2019 MyPan</span>
		</div>
	</div>


		<script type="text/javascript">
            function fileSelected() {
  				var file = document.getElementById('upload_file').files[0];
  
    			document.cookie="fname="+file.name+"; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";

                var fileReader = new FileReader(), box = document.getElementById('box');
                blobSlice = File.prototype.mozSlice || File.prototype.webkitSlice || File.prototype.slice, chunkSize = 2097152,

                // read in chunks of 2MB
                chunks = Math.ceil(file.size / chunkSize), currentChunk = 0, spark = new SparkMD5();

                fileReader.onload = function(e) {
                    console.log("read chunk nr", currentChunk + 1, "of", chunks);
                    spark.appendBinary(e.target.result);
                    // append binary string
                    currentChunk++;

                    if (currentChunk < chunks) {
                        loadNext();
                    } else {
                        $md5 = spark.end();

                        box.innerText = 'MD5 hash:' + $md5;


   						document.cookie = "MD5=" + $md5 + "; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";
   						document.cookie = "SecFile=" + file.name + "; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";
//   						document.cookie = "fileObj=" + document.getElementById('upload_file') + "; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";

						location.href = "../func/SecUpload.php";
                    }
                };

                function loadNext() {
                    var start = currentChunk * chunkSize, end = start + chunkSize >= file.size ? file.size : start + chunkSize;

                    fileReader.readAsBinaryString(blobSlice.call(file, start, end));
                }

                loadNext();
                
            }

        </script>
	</body>

</html>

