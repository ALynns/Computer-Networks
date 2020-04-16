<!DOCTYPE html>
<html>
	<head>
		<meta charset="gbk">
		<title>My Pan Login</title>

		<link href="../styles/label-form.css" rel="stylesheet" type="text/css">
		<link href="../styles/login.css" rel="stylesheet" type="text/css">

		<script src="../scripts/Login.js" defer></script>
	</head>
	
	<body>
	<div class="container">	
		<div class="content">
			<img src="../images/qaq.jpg" alt="My test image">
			<form action="../func/LoginCheck.php" method="post">
				<h1>Welcome to MyPan</h1>

				<p>The MyPan is created by <strong><abbr title="designers">MY XZH CW</abbr></strong>.</p>

				<section>
    				<h2>information</h2>
    				<p>
     	 				<label for="name">
        					<span>Username: </span>
      					</label>
      					<input type="text" id="Username" name="username" size="18" value="" maxlength="16" placeholder="please input username">
    				</p>

    				<p>
      					<label for="pwd">
        					<span>Password: </span>
      					</label>
      					<input type="password" id="Password" name="password" size="18" value="" maxlength="16" placeholder="please input password">
    				</p>
				</section>
			
				<div class="button">
					<button type="submit">Login</button><button type="button" onclick=jumpToRegister()>Register</button>
				</div>	
			</form>	

			<a href="../../../u1751763/server.c" download="test">
    			Download img
			</a>
		</div>

		<div class="footer">
			<span>@2019-2019 MyPan</span>
		</div>
	</div>
	</body>		
</html>

<!%%% color select: https://developer.mozilla.org/zh-CN/docs/Web/CSS/CSS_Colors/Color_picker_tool>
