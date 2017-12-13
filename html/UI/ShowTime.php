<?php
	header("Context-Type: text/html; charset = UTF-8");
	$con = mysqli_connect("localhost","root","root","capstone");

	$userID = $_GET["userID"];	

	$result = mysqli_query($con, "SELECT Time, Elevator_Id, Flag FROM Calling WHERE userID = '$userID' AND Flag ='2'");
	$response = array();
	
	while($row = mysqli_fetch_array($result))
	{
		array_push($response,array("Time"=>$row[0], "Elevator_Id"=>$row[1],));}	
	echo json_encode(array("response"=>$response), JSON_UNESCAPED_UNICODE);

	$statement = mysqli_prepare($con, "UPDATE Calling SET Flag = '3' WHERE userID = '$userID'");
	mysqli_stmt_execute($statement);
	mysqli_close($con);
?>
