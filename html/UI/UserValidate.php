<?php
	$con = mysql_connect("localhost","root","root","capstone");
	
	$userID = $_POST["ID"];

	$statement = mysqli_prepare($con,"SELECT * FROM USER WHERE ID = ?");
	mysqli_stmt_bind_param($statement, "s", $userID);
	mysqli_stmt_execute($statement);
	mysqli_stmt_store_result($statement);
	mysqli_stmt_bind_result($statement, $userID);
	
	$response = array();
	$response["success"] = true;
//	echo 1;
	while(mysqli_stmt_fetch($statement)){
		$response["success"] = false;
//		echo 2; 
		$response["userID"] = $userID;
//		echo 3;
	}
	
	echo json_encode($response);
?>
