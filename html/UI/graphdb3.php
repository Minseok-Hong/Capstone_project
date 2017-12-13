<?php
        $host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';

        mysql_connect($host, $user, $pw);
        mysql_select_db($dbName);

        $sql = "select Current_Floor from getCurr where Building_Id = 2 AND Elevator_id = 1";

        $result = mysql_query($sql);

        while($row = mysql_fetch_array($result))
        {
                $res = $row[0];
        }
        echo $res;

?>


