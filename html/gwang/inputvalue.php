<?php
	
        $host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';

        mysql_connect($host, $user, $pw);
        mysql_select_db($dbName);

	$sql = "update building set flag=0 where flag=1";
	
	mysql_query($sql);
 		
?>
