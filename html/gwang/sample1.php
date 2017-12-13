<?php

	$host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';

        mysql_connect($host, $user, $pw);

        mysql_select_db($dbName);

	$sql = "select People_Num from Simul_getCurr where Simul_Id=1";
	
	$result = mysql_query($sql);

	while($row = mysql_fetch_array($result))
	{
		$people_num[] = $row[ People_Num];
	}

	echo json_encode($people_num);


?>
