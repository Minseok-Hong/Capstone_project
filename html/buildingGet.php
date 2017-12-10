<?php
   header("Context-Type: text/html; charset=UTF-8");
   $con = mysqli_connect("52.79.39.200","root","root","capstone");
   
   $Building_Name = $_GET["Building_Name"];
   
   $result = mysqli_query($con, "SELECT * FROM building WHERE Building_Name = '$Building_Name'");
	
   echo $result;  
//	 $response = array();
//   while($row = mysqli_fetch_array($result)){
//      array_push($response, array("Building_Id"=>$row[0], "Building_Name"=>$row[1],
//      "Building_Floor"=>$row[2],"Elevator_Number"=>$row[3]));
//   }
   
//   echo json_encode(array("response"=>$response), JSON_UNESCAPED_UNICODE);
//   mysqli_close($con);   
?>
