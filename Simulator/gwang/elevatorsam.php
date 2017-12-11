<!DOCTYPE html>
<html lang="ko">
<head>
 <meta charset="UTF-8">
 <title>실시간 운행정보</title>

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
//  echo " <div class ="tower">
//<div id = "f2" class="floor">3층</div>";
?>
  </div>
</body>
</html>

