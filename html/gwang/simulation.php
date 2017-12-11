<!DOCTYPE html>
<html lang="ko">
<head>
   <meta charset="UTF-8">
   <title>
	</title>

		<p><img src="Title.png" alt="title"  width ="760" height="190" style = "margin-left: auto; margin-right: auto; display:block;"></p>
<!--   <style>
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
-->
   <script src="https://code.jquery.com/jquery-1.10.2.js"></script>
</head>
<body>
   <div id="container">

</div>
<div style = "text-align:center">
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
	echo "<select id='myDropDown' name = 'myDropDown' align='absmiddle'>";
	while ($row = mysql_fetch_array($result)) {
    		echo "<option value='" . $row['Building_Id'] . "'>" . $row['Building_Name'] . "</option>";
	}
	echo "</select>";
	
	//여기가 위에가 드롭다운 리스트 만드는 php
?>
</div>
<form method ="post" action = "/gwang/4.html">
<div id ="ele_id"></div>
<div id ="name_feedback"></div>
</form>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
        <script>//드롭다운 리스트가 변경되면 일어나는 스크립트
            $(document).ready(function(){
            $('#myDropDown').change(function(){
                var inputValue = $(this).val();
		$.post('/gwang/4.html', { inputValue: inputValue }, function(data){
		$('#name_feedback').html(data);
		});
            });
        });



</script>

</body>
</html>
