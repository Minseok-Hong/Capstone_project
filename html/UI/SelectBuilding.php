<?php
	header("Context-Type: text/html; charset=UTF-8");
	$con = mysqli_connect("localhost","root","root","capstone");
	
	$Building_Id = $_GET["Building_Id"];
	
	$result = mysqli_query($con, "SELECT Building_Floor, Elevator_Number from building WHERE Building_Id = '$Building_Id'");
	
	$response = array();
	while($row = mysqli_fetch_array($result)){
		array_push($response, array("Building_Floor"=>$row[0],"Elevator_Number"=>$row[1]));
	}
	
	echo json_encode(array("response"=>$response), JSON_UNESCAPED_UNICODE);
	mysqli_close($con);	
?>
