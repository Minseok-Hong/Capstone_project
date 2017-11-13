#!/usr/bin/php -q
<?php
define("_IP",    "127.0.0.1");
define("_PORT",  "60000");
$sSock      = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_bind($sSock, _IP, _PORT);
socket_listen($sSock);
while($cSock = socket_accept($sSock)){    
	socket_getpeername($cSock, $addr, $port);    
	echo "SERVER >> client connected $addr:$port \n";    
	//$date = date("Y/m/d H:i:s");  
	$len = stlen($date);
 	$date = "A1#2#3#4"; //여기다가 이런방식으로 logic.c에 전달할 데이터 넣으면 된다.
                     // 예를 들면 1 id빌딩에서 2층에서 3층으로 4명이 이동한다.

 	$date = $len . "" . $date; 
	socket_write($cSock, $date);    
	socket_close($cSock);    
	echo "SERVER >> client Close.\n";
	}
?> 