#include "stdafx.h"
#include "Channel.h"
#include "WSAStartup.h"

#pragma comment(lib, "ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	WsaStartup wsaStartup(2, 2);


	return 0;
}
