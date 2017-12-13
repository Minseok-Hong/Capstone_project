<!DOCTYPE html>
<html lang="ko">
<head>
   <meta charset="UTF-8">
 <script src="https://code.jquery.com/jquery-1.10.2.js"></script>

</head>

<body>
<?php

	$poo = $_POST["poo"];
	
	$host = 'localhost';
        $user = 'root';
        $pw = 'root';
        $dbName = 'capstone';
	
	mysql_connect($host, $user, $pw);
        mysql_select_db($dbName);
	
	for($count =1; $count <=($poo[1]); $count++)
	{
		$i = $count -1;
		$k = $count +30;
		echo '<link rel="stylesheet" type="text/css" href="style.css">';
		echo '<script src="jquery-2.0.2.min.js"></script>';
		echo '<script src="script.js"></script>';
		echo '<div id= "'.$i.'" div class="tower">';
		echo '<p style="color:white;" align="center">People = <span id = "'.$k.'" value="" style="color:white"></span></p>';

  	for($floor=1; $floor <= ($poo[2]); $floor++)
	{
		$f = $floor +20;
		echo '<div id= "'.$f.'" class="floor">';
		echo $poo[2]-$floor+1;
		echo '</div> ';
	}
		$incre = $count + 10;

		echo '<img id="'.$incre.'" class = "elevator"  src="elevator_main.jpg">';

		echo '</div> ';
	}	
	
?> 

<script src = "http://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js"></script>
	
	<script>
		(function() {
                        var poll = function(){

                        $.ajax({
                                url:'/gwang/sample1.php',
                                dataType: 'json',
                                type: 'get',
                                async:false,
                                success: function(data){
					take(data);
                                },
                                error: function(data){
					alert(data);
                                }
                                });
                        };
                        setInterval(function(){
                                poll();
                        },100);

                        }
                )();
		
			var take = (function(s1){
				
				var p1 = $('#31');
				p1.text(s1[0]);

			});

	</script>
</body>
</html>


