<?PHP 
ini_set('display_errors', 'On');
error_reporting(E_ALL);

header('Content-Type: text/html; charset=utf-8');

echo "md5:" . md5_file("./PloucsMessenger.exe") . "\n";

?>
