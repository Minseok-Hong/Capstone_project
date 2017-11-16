<?php

        $db_host = "localhost";
        $db_user = "root";
        $db_password = "root";
        $db_name = "capstone";

        $conn = mysqli_connect($db_host,$db_user,$db_password,$db_name);

	$userID = $_POST["userID"];
//	$userID = "hwasu";
//	$userPassword = "1234";
	$userPassword = $_POST["userPassword"];

//	$userGender = "M";
//	$userEmail = "zzzzzk";
//	$userMajor = "azzz";
// $_POST["userMajor"];

//	echo $userEmail;	
// $_POST["userEmail"];

//	$mysql_qry = "insert into USER(userID,userPassword,userGender,userEmail,userMajor) values('$userID','$userPassword','$userGender','$userEmail','$userMajor')";

	$mysql_qry = "select *from USER where userID like '$userID' and userPassword like '$userPassword'";
	
	$result=mysqli_query($conn,$mysql_qry);

	if(mysqli_num_rows($result)>0)
	{
		echo "Login success WHOO!!!";
	}
	else
	{
		echo "FAIL";
	}
//	if($conn->mysqli_query($mysql_qry) === TRUE)
//	{
//		echo "insert Successful";
//	}
//	else
//		echo "Error: " . $mysql_qry . "<br>" . $conn->error;
//	}
	
//	$conn->close();

	mysqli_close($conn);	
?>


                                                                                                                                                    
          

