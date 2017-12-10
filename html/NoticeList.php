<?php
	$con = mysqli_connect("localhost","root","root","capstone");
	
	$result = mysqli_query($con, "SELECT * FROM NOTICE ORDER BY noticeDate DESC;");
	//building no 순서대로 내림차순
	$response = array();
	
	while($row = mysqli_fetch_array($result)){
		array_push($response,
		array('noticeContent'=>$row[0], 'noticeName'=>$row[1], 'noticeDate'=>$row[2]));
	}
	
	echo json_encode(array("response"=>$response));
	mysqli_close($con);
?>
