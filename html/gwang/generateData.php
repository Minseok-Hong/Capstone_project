<?php

   $host = 'localhost';  // MySQl host
   $user = 'root';  // MySQL username
   $pw = 'root';  // MySQL password
   $dbName = 'capstone';  // MySQL database name


   mysql_connect($host, $user, $pw);
   mysql_select_db($dbName);

   $query = "SELECT Building_Id, Elevator_id, Current_Floor FROM getCurr";
   $result  = mysql_query($query);

   $resultArray = array();

   while($row = mysql_fetch_assoc($result)){
    $arrayMiddle = array(
      "b" => (int)$row['Building_Id'],
      "e" => (int)$row['Elevator_id'],
      "v" => (int)$row['Current_Floor']
    );
    array_push($resultArray, $arrayMiddle);
   }
   $json = json_encode($resultArray);
   echo $json;
?>
