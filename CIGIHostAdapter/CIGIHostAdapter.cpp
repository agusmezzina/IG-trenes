// CIGIHostAdapter.cpp: define el punto de entrada de la aplicaci�n de consola.
//

#include "stdafx.h"
#include "CigiHost.h"
#include "SceneData.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//UDPServer server;
	SceneData data;
	CigiHost server(&data);
	server.run();
	return 0;
}

