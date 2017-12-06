<?php

	$host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';
	$inputValue = $_POST['inputValue'];

        mysql_connect($host, $user, $pw);
        mysql_select_db($dbName);

	$sql = "select Current_Floor from getCurr where Building_Id=1";

	$result = mysql_query($sql);

	while($row = mysql_fetch_array($result))
	{
		$current_floor[] = $row[ Current_Floor];

	}
//	for($i=0;$i<2;$i++)
//	{
	echo json_encode($current_floor);
//	}
//	echo json_encode($current_floor);
?>
