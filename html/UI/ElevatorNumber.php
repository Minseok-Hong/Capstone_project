<?php
	$con = mysqli_connect("localhost","root","root","capstone");
	
	$Building_Id = $_POST["Building_Id"];
	$Elevator_id = $_POST["Elevator_Number"];
	$NULL = "0";
	$FLOOR = "1";
	
	for($i=1; $i<=$Elevator_id; $i++){
	$statement = mysqli_prepare($con, "INSERT INTO getCurr VALUES (?, ?, ?, ?)");
	
	mysqli_stmt_bind_param($statement, "iiii", $Building_Id, $i, $FLOOR, $NULL);
	mysqli_stmt_execute($statement);
	
	$response = array();
	$response["success"] = true;
	
	echo json_encode($response);
	}
	
?>
