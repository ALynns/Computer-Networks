<!DOCTYPE html>
<html>
	<head>
		<meta charset="gbk">
		<title>My Pan Register</title>

		<link href="../styles/label-form.css" rel="stylesheet" type="text/css">
		<link href="../styles/register.css" rel="stylesheet" type="text/css">

		<script src="../scripts/Register.js" defer></script>
	</head>
	
	<body>
	<div class="container">	
		<div class="content">
			<form action="../func/RegisterCheck.php" method="post">
				<h1>MyPan Register</h1>
				<ul>
					<li>username should be longer than 4 and be less than 16</li>
					<li>pwd should be longer than 12 and be less than 18</li>
					<li>pwd should be at least with two of uppercase/lowercase/special letter</li>
				</ul>
				<section>
    				<h2>information</h2>
    				<p>
     	 				<label for="name">
        					<span>Username: </span>
      					</label>
      					<input type="text" id="username" name="username" size="18" value="" maxlength="16" placeholder="please input username">
    				</p>

    				<p>
      					<label for="pwd">
        					<span>Password: </span>
      					</label>
      					<input type="password" id="p
      					assword" name="password" size="18" value="" maxlength="16" placeholder="please input password">
    				</p>

    				<p>
      					<label for="pwd">
        					<span>Confirm: </span>
      					</label>
      					<input type="password" id="CPassword" name="Cpassword" size="18" value="" maxlength="16" placeholder="confirm password">
    				</p>
				</section>
			
				<div class="button">
					<button type="submit">Register</button><button type="button" onclick=jumpToLogin()>Login</button>
				</div>	

			</form>
		</div>

		<div class="footer">
			<span>@2019-2019 MyPan</span>
		</div>
	</div>
	</body>		
</html>


