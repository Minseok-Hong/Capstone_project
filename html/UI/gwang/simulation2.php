<!DOCTYPE html>
<html lang="ko">
<head>
   <meta charset="UTF-8">
   <title>모두의 엘리베이터</title>
   <script src="https://code.jquery.com/jquery-1.10.2.js"></script>
</head>
<body>
   <div id="container">
   	<p>모두의 엘리베이터</p>
   </div>

해당 엘리베이터 선택
<?php
    	$host = 'localhost';
    	$user = 'root';
    	$pw = 'root';
    	$dbName = 'capstone';
	
	mysql_connect($host, $user, $pw);
	mysql_select_db($dbName);
	$sql = "SELECT Building_Id, Building_Name FROM building";
	$result = mysql_query($sql);
	echo "<select id='myDropDown' name = 'myDropDown'>";
	while ($row = mysql_fetch_array($result)) {
    		echo "<option value='" . $row['Building_Id'] . "'>" . $row['Building_Name'] . "</option>";
	}
	echo "</select>";
	
	//여기가 위에가 드롭다운 리스트 만드는 php
?>

<form method ="post" action = "/gwang/3.html">
<td> <input type="submit" value = "보기"/></td>
</body>
</html>
