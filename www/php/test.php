<?php

$salt1 = "mj&h*";
$salt2 = "pq!@";

if ( isset($_POST['username']) && isset($_POST['password']) ) {

	if ( $_POST['attempt'] == 0 ) {

		$username = $_POST['username'];
		$password = $_POST['password'];
		$token    = hash('ripemd128', "$salt1$password$salt2");
		
		$attempt  = 1;
		$message  = "Please enter your existing username and password.";
		
	} else {
	
		$username   = $_POST['prev_username'];
		$password   = $_POST['prev_password'];
		$token      = $_POST['prev_token'];
		$password_2 = $_POST['password'];
		$token_2    = hash('ripemd128', "$salt1$password_2$salt2");
	
		if ( ( $_POST['username'] == $username ) && 
		     ( $token_2 == $token ) ) {
		     
			$message  = "Success! 
			<a href='hash_test.php'>Clear and start over.</a>";
		     
		} else {
		
			$message  = "They don't match! 
			<a href='hash_test.php'>Clear and start over.</a>";
		
		}
		
		$attempt  = 0;
		
	}

} else {

	$username = "";
	$password = "";
	$token    = "";
	$attempt  = 0;
	$message  = "Please create your NEW username and password.";

}

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name=viewport content="width=device-width, initial-scale=1">
    <title> Hash Test </title>
</head>

<body>

<h1>Test the PHP hash() function</h1>

<p><?php echo $message ?></p>

<form id="hashform" method="post" autocomplete="off" action="hash_test.php">

    <label for="username">Username </label>
	<input type="text" name="username" id="username" maxlength="20" required>

    <label for="password">Password </label>
	<input type="password" name="password" id="password" maxlength="20" required>
	
	<!-- hidden fields used to preserve the previous values 
	     because there is no database here -->
	<input type="hidden" name="attempt" value="<?php echo $attempt ?>">
	<input type="hidden" name="prev_username" value="<?php echo $username ?>">
	<input type="hidden" name="prev_password" value="<?php echo $password ?>">
	<input type="hidden" name="prev_token" value="<?php echo $token ?>">

	<input type="submit" id="submit" value="Submit">
</form>

<p>For testing purposes, we'll show everything here:</p>
<p>Username: <?php echo $username ?></p>
<p>Password: <?php echo $password ?></p>
<p>Encrypted password: <?php echo $token ?></p>

</body>
</html>