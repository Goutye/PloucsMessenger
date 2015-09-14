<?PHP 

ini_set('display_startup_errors',1);
ini_set('display_errors',1);
error_reporting(-1);

include_once("includes/db.php");

setcookie("pass", "ok", 0);

function sendCMDTo($cmd, $idUser, $msg) {
	$query = "INSERT INTO ploucs_messages VALUES('". $idUser ."', '". $msg ."', '". $cmd ."')";
	try {
		$req = $db->exec($query);
	} catch(PDOException $e) {
		echo $e->getMessage();
		return 1;
	}
}

function getUsers() {
	$users = {};
	$i = 0;
	try {
		$req = $db->query("SELECT id FROM ploucs_users");
		while($rep = $req->fetch(PDO::FETCH_OBJ)) {
			$users[$i++] = $rep->id;
		}
	} catch(PDOException $e) {
		echo $e->getMessage();
		return;
	}
}

function getConnectedUsers() {
	$users = {};
	$i = 0;
	try {
		$req = $db->query("SELECT id FROM ploucs_connected");
		while($rep = $req->fetch(PDO::FETCH_OBJ)) {
			$users[$i++] = $rep->userId;
		}
	} catch(PDOException $e) {
		echo $e->getMessage();
		return;
	}
}

if (isset($_COOKIE['pass']) && $_COOKIE['pass'] == "ok") {
	if (isset($_POST['cmd'])) {
		echo "OK";

		$cmdFull = htmlspecialchars($_POST['cmd']);
		$cmd = explode($cmdFull, ':');

		switch($cmd[0]) {
			case "con":
				//Get the user with the pseudo $cmd[1]
				try {
					$req = $db->query("SELECT * FROM ploucs_users WHERE pseudo = '".$cmd[1]."'");

					while($rep = $req->fetch(PDO::FETCH_OBJ)) {
						$id = $rep->id;
						$pwd = $rep->pwd;
					}
				} catch(PDOException $e) {
					echo $e->getMessage();
					return;
				}

				$loginPwd = md5($cmd[2]);

				//If no pwd
				if ($pwd == "" and $cmd[2] != "") {
					try {
						$query = "UPDATE ploucs_users SET pwd = '". $loginPwd ."' WHERE id = '". $id ."'";
						$req = $db->exec($query);
					} catch(PDOException $e) {
						echo $e->getMessage();
						return;
					}
				} //If isset pwd
				else {
					if ($pwd == $loginPwd) {
						//cookie own id
						setcookie("id", $id . "", 0);
						echo "OK";

						//Add to the connected users list
						$query = "INSERT INTO ploucs_connected VALUES('". $i ."', GETUTCDATE())";
						try {
							$req = $db->exec($query);
						}catch(PDOException $e) {
							echo $e->getMessage();
							return;
						}

						//Notify the others connected users
						$users = getConnectedUsers();
						foreach ($users as $u)
							sendCMDTo("con", $u, "con:". $id .":". $cmd[1]);
					}
					else
						echo "BAD_PWD";
				} 
				break;
			case "dis":
				break;
			case "msg":
				break;
			case "pm":
				break;
			default:
				echo "NO MATCHING CMD FOR '". $cmd[0] ."'";
		}		
		//evaluate cmd
	}
	else {
		//unstack
	}	
}
else echo "NO";

?>