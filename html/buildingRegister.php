<?php

	$db_host = "localhost";
        $db_user = "root";
        $db_password = "root";
        $db_name = "capstone";

        $conn = mysqli_connect($db_host,$db_user,$db_password,$db_name);

        $Building_Id = $_POST["Building_Id"];
	$Building_Name =  $_POST["Building_Name"];
        //$Building_Name = "Sungho";
	$Building_Floor =  $_POST["Building_Floor"];
        //$Building_Floor = "3";
	$Elevator_Number = $_POST["Elevator_Number"];
        //$Elevator_Number = "1";
	$Elevator_Capacity = $_POST["Elevator_Capacity"];
	$FLAG = '0';

        $mysql_qry = "insert into building(Building_Id,Building_Name,Building_Floor,Elevator_Number,Elevator_Capacity, flag) values( '$Building_Id','$Building_Name','$Building_Floor','$Elevator_Number','$Elevator_Capacity','$FLAG')";

        mysqli_query($conn,$mysql_qry);

        echo "insert building Successful";
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

