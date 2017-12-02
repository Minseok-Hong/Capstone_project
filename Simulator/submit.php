<?php

function processDrpdown($selectedVal) {
    //echo "Selected value in php<BR> ".$selectedVal;

	//echo ("<H5> 아무거나 : $selectedVar  <BR>");
	
}        

if ($_POST['dropdownValue']){

	//echo ("<H5> 아무거나 : $selectedVar  <BR>");

    //call the function or execute the code
    processDrpdown($_POST['dropdownValue']);
	//echo "@@@@@@@@@@@@";
	//echo "Selected value in php<BR> ".$selectedVal;


}

?>

<!DOCTYPE html>
<html lang="ko">
<head>
 <meta charset="UTF-8">
 <title>실시간 운행정보</title>
 

<style>
 body{
  background:#F5ECCE;
  color:#fff;
  padding:30px 20px 20px 30px;
  line-height:1.4;
  font-family: '나눔 고딕';
}
.tower{
  display: inline-block;
  background-color: #2A1B0A;
  left: 100px;
  position: relative;
}
.floor {
  width: 100px;
  height: 50px;
  background-color: #D29953;
  margin:10px 10px 10px 10px;
  font-size: 1.3em;
  font-weight:bold;
  cursor: default;
  position: relative;
  text-align: right;
  padding-right: 10px;
}
.elevator {
  width: 50px;
  height: 50px;
  background-color: #8B4F1D;
  margin: -10px 10px 10px 10px;
  font-size: 0.7em;
  font-weight:bold;
  cursor: default;
  position: absolute;
}

</style>


<script type="text/javascript" src="http://code.jquery.com/jquery-3.2.0.min.js" ></script>
<script type="text/javascript">
  $(document).ready(function(){
    var elevator = $('.elevator');
    var floors = $('.floor');
    var f1 = $('#f1');
    var move = 5;
    var count = 0;

    elevator.css({
      top: f1.position().top+20,
    })
    floors.each(function() {
      var pos = $(this).position();
      if(move==5){
        if(count==0){
          elevator.delay(500).animate({top: pos.top+20}, 1000);
          count++;
        }
      }
    });
  });
</script>
</head>

<body>
<link rel="stylesheet" type="text/css" href="style.css">
  <script src="jquery-2.0.2.min.js"></script>
  <script src="script.js"></script>
  <div class="tower">
    <div id="f10" class="floor">10층</div>
    <div id="f9" class="floor">9층</div>
    <div id="f8" class="floor">8층</div>
    <div id="f7" class="floor">7층</div>
    <div id="f6" class="floor">6층</div>
    <div id="f5" class="floor">5층</div>
    <div id="f4" class="floor">4층</div>
    <div id="f3" class="floor">3층</div>
    <div id="f2" class="floor">2층</div>
    <div id="f1" class="floor">1층</div>
    <img id="f0" class="elevator" src="elevator.png">
  </div>


<?php
	
	$selectedVar = $_POST[ "ele_id"];
	
	$host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';

	echo "입력된 이름 : $host ";
	echo "입력된 이름 : $selectedVar ";

	echo ("<H5> 아무거나 : $selectedVar <BR>");
        
	mysql_connect($host, $user, $pw);
        mysql_select_db($dbName);
        $sql = "SELECT Building_Name, Building_Floor, Elevator_Number FROM building where Building_Id = '$selectedVar'";
        $result = mysql_query($sql);


        while($row = MySQL_fetch_array($result)){

        $ele_Name = $row[ Building_Name];
        $ele_Floor = $row[ Building_Floor];
        $ele_Number = $row[ Elevator_Number];



        echo"

        <table border=1 width=600 cellspacing=0 cellpadding =2 align=center>
        <tr align = center>
                <td width = 50>빌딩이름</td><td width=50>$ele_Name</td>
                <td width = 50>빌딩층수</td><td width=50>$ele_Floor</td>
                <td width = 50>엘리베이터수</td><td width=50>$ele_Number</td>

        </tr>
        </table><br>";
}
?>


</body>
</html>
