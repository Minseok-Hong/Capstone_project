<!DOCTYPE html>
<html lang="ko">
<head>
   <meta charset="UTF-8">
   <title>모두의 엘리베이터</title>
   <style>
      #container {
         width:960px;
         margin:0 auto;
         text-align:center;
      }
      #content1 {
      	width: 300px;
      	position: absolute;
      	top: 100; left: 0;
      	padding: 10px;
      }
      #content2 {
      	width: 500px;
      	position: absolute;
      	top: 100; right: 0;
      	padding: 10px;
      }
   </style>
   <script src="https://code.jquery.com/jquery-1.10.2.js"></script>
</head>
<body>
   <div id="container">
   	<p>모두의 엘리베이터</p>
</div>

<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
        <script>
            $(document).ready(function(){
            $('#myDropDown').change(function(){
                //Selected value
                var inputValue = $(this).val();
                //alert("value in js "+inputValue);

                //Ajax for calling php function
                $.post('submit.php', { dropdownValue: inputValue }, function(data){
                    //alert('ajax completed. Response:  '+data);
                    //do after submission operation in DOM
                });
            });
        });
        </script>


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

echo "<select id='myDropDown'>";
while ($row = mysql_fetch_array($result)) {
    echo "<option value='" . $row['Building_Id'] . "'>" . $row['Building_Name'] . "</option>";
}
echo "</select>";


?>


</body>
</html>
