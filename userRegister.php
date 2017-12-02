<?php

        $db_host = "localhost";
        $db_user = "root";
        $db_password = "root";
        $db_name = "capstone";

        $conn = mysqli_connect($db_host,$db_user,$db_password,$db_name);

        $userID =  $_POST["userID"];
        $userPassword =  $_POST["userPassword"];
        $userGender = $_POST["userGender"];
        $userEmail = $_POST["userEmail"]; 
        $userMajor =  $_POST["userMajor"];
//        

        $mysql_qry = "insert into USER(userID,userPassword,userGender,userEmail,userMajor) values('$userID','$userPassword','$userGender','$userEmail','$userMajor')";

        mysqli_query($conn,$mysql_qry);

	echo "success";
//      if($conn->mysqli_query($mysql_qry) === TRUE)
//      {
//              echo "insert Successful";
//      }
//      else
//              echo "Error: " . $mysql_qry . "<br>" . $conn->error;
//      }

//      $conn->close();

        mysqli_close($conn);
?>





