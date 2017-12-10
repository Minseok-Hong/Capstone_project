<?php

	$host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';
	$inputValue = $_POST['inputValue'];


        mysql_connect($host, $user, $pw);

        mysql_select_db($dbName);

	$sql = "select Building_Id from building where flag = 1";

	$result = mysql_query($sql);

	//echo $result;
	while($row =mysql_fetch_array($result))
	{
		$response = $row[0];	
	}

	$sql2 = "select Elevator_Number from building where flag = 1";
	
	$result2 = mysql_query($sql2);

	while($row =mysql_fetch_array($result2))
	{
		$elevator_number[] = $row[ Elevator_Number];
	}

	$sql1 = "select Building_Floor, Current_Floor from getCurr where Building_Id=$response";
	
	$result1 = mysql_query($sql1);

	while($row = mysql_fetch_array($result1))
	{
	        $Building_Floor[] = $row[ Building_Floor];
		$current_floor[] = $row[ Current_Floor];
	}
	
	$merge = array_merge($current_floor, $Building_Floor);
	$merge1 = array_merge($elevator_number, $merge);
//	for($i=0;$i<2;$i++)
//	{
	//echo json_encode($Building_Floor);
	echo json_encode($merge1);
//	}
//	echo json_encode($current_floor);
?>
