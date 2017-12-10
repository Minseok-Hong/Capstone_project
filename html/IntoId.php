<?php
	$con = mysqli_connect("localhost","root","root","capstone");

	$userID = $_POST["userID"];
	$NULL = "0";
	$FLAG = "3";		

	$statement = mysqli_prepare($con, "INSERT INTO Calling VALUES (?, ?, ?, ?, ?, ?, ?)");
	mysqli_stmt_bind_param($statement, "sssssss", $userID, $NULL, $NULL, $NULL, $NULL, $NULL, $FLAG);
	mysqli_stmt_execute($statement);
	
	$response = array();
	$response["success"] = true;
	
	echo json_encode($response);
?>	
