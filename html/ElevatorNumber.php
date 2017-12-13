<?php
	$con = mysqli_connect("localhost","root","root","capstone");
	
	$Building_Id = $_POST["Building_Id"];
	$Elevator_id = $_POST["Elevator_Number"];
	$Building_Floor = $_POST["Building_Floor"];
	$NULL = "0";
	$FLOOR = "1";
	
	for($i=1; $i<=$Elevator_id; $i++){
	$statement = mysqli_prepare($con, "INSERT INTO getCurr VALUES (?, ?, ?, ?, ?)");
	
	mysqli_stmt_bind_param($statement, "iiiii", $Building_Id, $i, $FLOOR, $NULL, $Building_Floor);
	mysqli_stmt_execute($statement);
	
	$response = array();
	$response["success"] = true;
	
	echo json_encode($response);
	}
	
?>
