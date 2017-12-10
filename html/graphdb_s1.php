<?php
        $host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';

        mysql_connect($host, $user, $pw);
        mysql_select_db($dbName);

        $sql = "select data from graph_s1 where id = 1";

        $result = mysql_query($sql);

        while($row = mysql_fetch_array($result))
        {
                $res = $row[0];
        }
        echo $res;

?>


