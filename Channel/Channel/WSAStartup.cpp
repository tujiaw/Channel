#include "stdafx.h"
#include "WsaStartup.h"
#include <Windows.h>
#include <crtdbg.h>

WsaStartup::WsaStartup(unsigned char majorVer, unsigned char minorVer)
{
	WSAData wsaData;
	_ASSERT(0 == WSAStartup(MAKEWORD(majorVer, minorVer), &wsaData));
}


WsaStartup::~WsaStartup(void)
{
	WSACleanup();
}
