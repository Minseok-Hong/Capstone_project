<?php
	$con = mysqli_connect("localhost","root","root","capstone");
	$result = mysqli_query($con, "SELECT * FROM building");
	
	$response = array();
	
	while($row = mysqli_fetch_array($result)){
		array_push($response,
	array('Building_Id'=>$row[0], 'Building_Name'=>$row[1],'Building_Floor' =>$row[2], 'Elevator_Number'=>$row[3],'Elevator_Capacity'=>$row[4]));}

	echo json_encode(array("response"=>$response));
	mysqli_close($con);
?>
