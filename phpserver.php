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
	$date = date("Y/m/d H:i:s");    
	socket_write($cSock, $date);    
	socket_close($cSock);    
	echo "SERVER >> client Close.\n";
	}
?> 