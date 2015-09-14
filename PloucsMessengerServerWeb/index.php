<?PHP 
	setcookie("pass", "ok", 0);
?>
Cookie ? <?php
		if (isset($_COOKIE['pass']) and $_COOKIE['pass'] == "ok") echo "YEAH" else "NOPE";
?>