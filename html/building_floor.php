<?php
	
	$db_host = "localhost";
        $db_user = "root";
        $db_password = "root";
        $db_name = "capstone";

        $conn = mysqli_connect($db_host,$db_user,$db_password,$db_name);

        $result = mysqli_query($conn, "select *from building");

        while($row = mysqli_fetch_array($result))
        {
                echo $row['Building_Floor'];
                echo "<br>";

        }
        mysqli_close($conn);
?>

