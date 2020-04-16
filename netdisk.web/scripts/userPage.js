function fileSelected1() {
  var file = document.getElementById('upload_file').files[0];
  if (file) {
    var fileSize = 0;
    if (file.size > 1024 * 1024){
      fileSize = (Math.round(file.size * 100 / (1024 * 1024)) / 100).toString() + 'MB';
    }else{
      fileSize = (Math.round(file.size * 100 / 1024) / 100).toString() + 'KB';
    }      
    document.getElementById('fileName').innerHTML = 'Name: ' + file.name;
    document.getElementById('fileSize').innerHTML = 'Size: ' + fileSize;
    document.getElementById('fileType').innerHTML = 'Type: ' + file.type;

    document.cookie="fname="+file.name+"; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";
  }
}


function UploadFile(){
	// sec upload



	fileObj = document.getElementById("upload_file").files[0];
	url = "http://10.60.102.252:20240" + "/func/UploadFile.php";

	form = new FormData();

	form.append("upload_file", fileObj);

	xhr = new XMLHttpRequest();
	xhr.open("POST", url, true);
	xhr.onload = uploadComplete;
	xhr.onerror = uploadFailed;
//  var fd = document.getElementById('sendFile').getFormData();

//  xhr.addEventListener("abort", uploadCanceled, false);	
	xhr.upload.onprogress = progressFunction;

//  	xhr.open("POST", url);
	xhr.upload.onloadstart = function(){
		ot = new Date().getTime();
		oloaded = 0;
	}

//	xhr.send(fd);
	xhr.send(form);
}

function cancelUploadFile(){
	fileObj.value='';

    document.getElementById("percentage").innerHTML = Math.round(0) + "%";
	document.getElementById("time").innerHTML = ', speed: '+ 0 + ', rest time: ' + 0 + 's';
	xhr.abort();
}

function uploadComplete(evt) {
//	alert(evt.target.responseText);

//	fileObj.value='';

	location.href="../func/UploadFile.php";
}

function uploadFailed(evt) {
// 	fileObj.value='';
  	alert("upload error2");	
}


function progressFunction(evt){
	var progressBar = document.getElementById("progressBar");
	var percentageDiv = document.getElementById("percentage");


	if(evt.lengthComputable){
		progressBar.max = evt.total;
		progressBar.value = evt.loaded;
		percentageDiv.innerHTML = Math.round(evt.loaded / evt.total * 100) + "%";
	}

	var time = document.getElementById("time");
	var nt = new Date().getTime();				// cur time

	var pertime = (nt - ot)/1000;				// cur used time

	ot = new Date().getTime();					
	var perload = evt.loaded - oloaded;
	oloaded = evt.loaded;

	var speed = perload / pertime;
	var bspeed = speed;
	var units = 'b/s'

	if(speed/1024 > 1){
		speed = speed /1024;
		units = 'k/s';
	}
	if(speed/1024 > 1){
		speed = speed /1024;
		units = 'M/s';
	}

	var restTime = ((evt.total - evt.loaded)/bspeed).toFixed(1);
	time.innerHTML = ', speed: '+ speed + units + ', rest time: ' + restTime + 's';
	if(bspeed == 0) time.innerHTML = 'upload cancel';
}


function newFolder(){
	var folderName = prompt("input folder name: ", "new folder");

    document.cookie = "folderName="+folderName+"; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";
 //   location.href = "../func/newFolder.php";
}

function Rename(){
	var rename = prompt("input new name: ", "newName");

    document.cookie = "rename="+rename+"; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";	
}


function delFile($delName){
   document.cookie = "delName=" + $delName + "; expires=Thu, 18 Dec 2043 12:00:00 GMT; path=/";

   location.href = "../func/DelFile.php";
}
