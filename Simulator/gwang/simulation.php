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
엘리베이터 아이디<p id="ele_id"></p>
<div id ="name_feedback"></div>

</form>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
        <script>//드롭다운 리스트가 변경되면 일어나는 스크립트
            $(document).ready(function(){
            $('#myDropDown').change(function(){
                //Selected value
                var inputValue = $(this).val();
                //alert("value in js "+inputValue);
		//document.write(inputValue);
                document.getElementById('ele_id').innerHTML = inputValue;
		//Ajax for calling php function
		
			
		$.post('/gwang/3.html', { inputValue: inputValue }, function(data){
                    //alert('ajax completed. Response:  '+data);
                    //do after submission operation in DOM
	//			var result = $('.result').html(data);
			$('#name_feedback').html(data);
					
		});
            });
        });
</script>
<?php
	
function processDrpdown($selectedVal) {
    //echo "Selected value in php<BR> ".$selectedVal;
	echo "1";
}
if ($_POST['dropdownValue']){
    //call the function or execute the code
    processDrpdown($_POST['dropdownValue']);
	echo "@@@@@@@@@@@@";
        //echo "Selected value in php<BR> ".$selectedVal;
}
?>

</body>
</html>
