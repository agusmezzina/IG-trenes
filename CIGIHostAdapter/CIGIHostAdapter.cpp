// CIGIHostAdapter.cpp: define el punto de entrada de la aplicaci�n de consola.
//

#include "stdafx.h"
#include "UDPServer.h"

int _tmain(int argc, _TCHAR* argv[])
{
	UDPServer server;
	server.run();
	return 0;
}

