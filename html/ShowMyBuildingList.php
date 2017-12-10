<?php
	header("Context-Type: text/html; charset=UTF-8");
	$con = mysqli_connect("localhost","root","root","capstone");
	
	$userID = $_GET["userID"];

	$result = mysqli_query($con, "SELECT building.Building_Id, building.Building_Name, building.Building_Floor, building.Elevator_Number FROM USER,building,MyBuilding WHERE USER.userID = '$userID' AND USER.userID = MyBuilding.userID AND MyBuilding.Building_Id = building.Building_Id");
	$response = array();
	while($row = mysqli_fetch_array($result))
	{
		array_push($response, array("Building_Id"=>$row[0], "Building_Name"=>$row[1], "Building_Floor"=>$row[2],"Elevator_Number"=>$row[3]));
	}
	
	echo json_encode(array("response"=>$response), JSON_UNESCAPED_UNICODE);
	mysqli_close($con);	
?>
