<?php
	$con = mysql_connect("localhost","db아이디","db비밀번호","db아이디");
	
	$userID = $_POST["userID"];

	$statement = mysqli_prepare($con,"SELECT * FROM USER WHERE userID = ?");
	mysqli_stmt_bind_param($statement, "s", $userID);
	mysqli_stmt_execute($statement);
	mysqli_stmt_store_result($statement);
	mysqli_stmt_bind_result($statement, $userID);
	
	$response = array();
	$response["success"] = true;
	
	while(mysqli_stmt_fetch($statement)){
		$response["success"] = false;
		$response["userID"] = $userID;
	}
	
	echo json_encode($response);
?>