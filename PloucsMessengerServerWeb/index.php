<?PHP 
ini_set('display_errors', 'On');
error_reporting(E_ALL);

include_once("../includes/db.php");

setcookie("pass", "ok", 0);

function sendCMDTo($cmd, $idUser, $msg) {
	global $db;
	$query = "INSERT INTO ploucs_messages VALUES('". $idUser ."', '". $msg ."', '". $cmd ."', CURRENT_TIMESTAMP())";
	try {
		$req = $db->exec($query);
	} catch(PDOException $e) {
		echo $query. ": " .$e->getMessage();
		return 1;
	}
}

function getUsers() {
	global $db;
	$users = array();
	$i = 0;
	try {
		$req = $db->query("SELECT id FROM ploucs_users");
		while($rep = $req->fetch(PDO::FETCH_OBJ)) {
			$users[$i++] = $rep->id;
		}
	} catch(PDOException $e) {
		echo $query. ": " .$e->getMessage();
		return;
	}
	return $users;
}

function getConnectedUsers() {
	global $db;
	$users = array();
	$i = 0;
	try {
		$req = $db->query("SELECT userId FROM ploucs_connected");
		while($rep = $req->fetch(PDO::FETCH_OBJ)) {
			$users[$i++] = $rep->userId;
		}
	} catch(PDOException $e) {
		echo $query. ": " .$e->getMessage();
		return;
	}
	return $users;
}

if (isset($_COOKIE['pass']) && $_COOKIE['pass'] == "ok") {
	if (isset($_COOKIE['id']))
		$clientID = htmlspecialchars($_COOKIE['id']);
	else
		$clientID = "";

	if (isset($_POST['cmd'])) {
		$cmdFull = htmlspecialchars($_POST['cmd']);
		$cmd = explode(':', $cmdFull);

		//evaluate cmd
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
					echo $query. ": " .$e->getMessage();
					return;
				}

				$loginPwd = md5($cmd[2]);
				$passwordOK = false;

				//If no pwd
				if ($pwd == "" and $cmd[2] != "") {
					try {
						$query = "UPDATE ploucs_users SET pwd = '". $loginPwd ."' WHERE id = '". $id ."'";
						$req = $db->exec($query);
					} catch(PDOException $e) {
						echo $query. ": " .$e->getMessage();
						return;
					}

					$passwordOK = true;
				} //If isset pwd
				else {
					$passwordOK = $pwd == $loginPwd;
				}

				if ($passwordOK) {
					//cookie own id
					setcookie("id", $id . "", 0);
					echo "OK";

					//Add to the connected users list
					$query = "INSERT INTO ploucs_connected VALUES('". $id ."', CURRENT_TIMESTAMP())";
					try {
						$req = $db->exec($query);
					}catch(PDOException $e) {
						echo $query . ": " .$e->getMessage();
						return;
					}

					//Notify the others connected users
					$users = getConnectedUsers();
					foreach ($users as $u)
						sendCMDTo("con", $u, "con:". $id .":". $cmd[1]);
				}
				else {
					echo "BAD_PWD";
				}
				break;
			case "dcn":
				$query = "DELETE FROM ploucs_connected WHERE userId = '". $clientID ."'";
				try {
					$req = $db->exec($query);
				}catch(PDOException $e) {
					echo $query. ": " .$e->getMessage();
				}
				$users = getConnectedUsers();
				foreach ($users as $u)
					sendCMDTo("dcn", $u, "dcn:" . $clientID);
				unset($_COOKIE['mycookiename']);
				break;
			case "msg":
				$users = getUsers();
				foreach ($users as $u)
					sendCMDTo("msg", $u, "msg:" . $clientID . ": " . $cmd[1]);
				break;
			case "pm":
				sendCMDTo("pm", $cmd[1], "pm:" . $clientID . ": " . $cmd[2]);
				break;
			default:
				echo "NO MATCHING CMD FOR '". $cmd[0] ."'";
		}		
	}
	else {
		//unstack
		if ($clientID == "") {
			echo "FAIL: NO USER ID FOUND! Why did you eat the cookie? :/";
		} else {
			//Update the last_date we saw the user
			$query = "UPDATE ploucs_connected SET last_date = CURRENT_TIMESTAMP() WHERE userId = '". $clientID ."'";
			try {
				$req = $db->exec($query);

				//Get the msg from the stack
				$req = $db->query("SELECT msg FROM ploucs_messages WHERE toId = '". $clientID ."' ORDER BY date"); 
				while($rep = $req->fetch(PDO::FETCH_OBJ)) {
					echo $rep->msg . "\n";
				}
				$db->exec("DELETE FROM ploucs_messages WHERE toId = '". $clientID . "'");

				//Delete the users who are disconnected by time-out.
				$req = $db->query("SELECT userId FROM ploucs_connected WHERE last_date < CURRENT_TIMESTAMP() - INTERVAL 10 SECOND");
				if ($req->rowCount() > 0) {
					$query = $db->prepare("DELETE FROM ploucs_connected WHERE last_date < CURRENT_TIMESTAMP() - INTERVAL 10 SECOND");
					$query->execute();

					//If we are the one who was the first to delete the disconnected user (Case of someone else reached the SELECT before the right user reach the DELETE)
					if ($req->rowCount() == $query->rowCount()) {
						$users = getConnectedUsers();
						while($rep = $req->fetch(PDO::FETCH_OBJ)) {
							foreach ($users as $u)
								sendCMDTo("dcn", $u, "dcn:" . $rep->userId);
						}
					}
				}

			}catch(PDOException $e) {
				echo $query. ": " .$e->getMessage();
			}
		}
	}	
}
else echo "NO";
?>