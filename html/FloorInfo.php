<?php

	$con = mysqli_connect("localhost","root","root","capstone");

	$userID = $_POST["userID"];
	$Current_Floor = $_POST["Current_Floor"];
	$Go_Floor = $_POST["Go_Floor"];
	$Building_Id = $_POST["Building_Id"];
	$Flag = $_POST["Flag"];
	
	$statement = mysqli_prepare($con, "UPDATE Calling SET Current_Floor = '$Current_Floor', Go_Floor = '$Go_Floor',
	Building_Id = '$Building_Id', Flag = '$Flag' WHERE userID = '$userID'");

	mysqli_stmt_bind_param($statement, "sssss", $userID, $Current_Floor, $Go_Floor, $Building_Id, $Flag);
	mysqli_stmt_execute($statement);	
	$response = array();
	$response["success"] = true;
	
	echo json_encode($response);
?>	
