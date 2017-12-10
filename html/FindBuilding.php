<?php
	header("Content-Type: text/html; charset=UTF-8");
	$con = mysqli_connect("localhost","root","root","capstone");

	$Building_Name = $_GET["Building_Name"];

	$result = mysqli_query($con, "SELECT * FROM building WHERE Building_Name = '$Building_Name'");
	$response = array();
	while($row = mysqli_fetch_array($result)){
		array_push($response, array("Building_Id" => $row[0], "Building_Name" => $row[1], "Building_Floor" => $row[2], "Elevator_Number" => $row[3]));
}

	echo json_encode(array("response"=>$response), JSON_UNESCAPED_UNICODE);
	mysqli_close($con);
?>
