#!/usr/bin/php -q
<?php
set_time_limit(0);

define("_IP",    "127.0.0.1");
define("_PORT",  "60000");
define("_TIMEOUT", 10);


$cSock = array();
$cInfo = array();
$sSock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

socket_bind($sSock, _IP, _PORT);
socket_listen($sSock);

while(1)
{
    $sockArr = array_merge(array($sSock), $cSock);
    if(socket_select($sockArr, $tWrite = NULL, $tExcept = NULL, _TIMEOUT) > 0)
    {
        foreach($sockArr as $sock)
        {
            // Listen 하고 있는 서버 소켓일 경우
            // 새로운 클라이언트의 접속을 의미
            if($sock == $sSock)
            {
                $tSock = socket_accept($sSock);

                socket_getpeername($tSock, $sockIp, $sockPort);

                $cSock[$tSock] = $tSock;
                $cInfo[$tSock] = array('ip'=>$sockIp, 'port'=>$sockPort);

                msg("client connect : ".$sockIp.":".$sockPort."\n");
            }
            // 클라이언트 접속해 있는 소켓중 하나일경우
            // 해당 클라이언트에서 이벤트가 발생함을 의미
            else
            {
                $buf = socket_read($sock, 4096);

                // 접속 종료
                if(!$buf)
                {
                    exceptSocket(&$cSock, &$cInfo, $sock);
                    msg("client connection broken : ".$sockIp.":".$sockPort."\n");
                }
                // 메시지 수신 이벤트
                else
                {
                    msg("recive data : ".$buf."\n");
                    $thisSockInfo = $cInfo[$sock];
                    $cmd = substr($buf, 0, 4);
                    switch($cmd)
                    {
                        // 시간전송
                        case "time":
                            msg("client(".$thisSockInfo['port'].") time data request\n");
                            socket_write($sock, date("Y/m/d H:i:s"));
                            break;

                        // 종료
                        case "quit":
                            msg("client(".$thisSockInfo['port'].") quit request\n");
                            socket_write($sock, "quit");
                            socket_close($sock);
                            exceptSocket(&$cSock, &$cInfo, $sock);
                            break;
                        default:
                            msg("client(".$thisSockInfo['port'].") invalid command $cmd\n");
                            break;
                    }
                }
            }
        }
    }
}

function exceptSocket(&$sockSet, &$infoSet, $sock)
{
    unset($sockSet[$sock]);
    unset($infoSet[$sock]);
    // array_merge 함수에서 error 발생을 막기위한 처리
    if(count($sockSet)==0)
    {
        $sockSet = array();
        $infoSet = array();
    }
}

function msg($msg)
{
    echo "SERVER >> ".$msg;
}

?>
