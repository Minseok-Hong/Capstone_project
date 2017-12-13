<?php
	$con = mysqli_connect("localhost","root","root","capstone");

	if(mysqli_connect_errno($con)){
		echo "Failed to connect to MySQL : ".mysqli_connect_error();
	}	

	$userID = $_POST["userID"];
	$Building_Id = $_POST["Building_Id"];
	
	$statement = mysqli_prepare($con, "INSERT INTO MyBuilding VALUES (?, ?)");
	mysqli_stmt_bind_param($statement, "si", $userID, $Building_Id);
	mysqli_stmt_execute($statement);
	
	$response = array();
	$response["success"] = true;
	
	echo json_encode($response);
?>
