<?php
        $host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';

        mysql_connect($host, $user, $pw);
        mysql_select_db($dbName);

        $sql = "select Current_Floor from getCurr";

        $result = mysql_query($sql);

        while($row = mysql_fetch_array($result))
        {
                $res = $row[3];
        }
        echo $res;

?>


