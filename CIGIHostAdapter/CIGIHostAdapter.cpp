// CIGIHostAdapter.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
//#include "UDPServer.h"
#include "CigiHost.h"
#include "SimulationState.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	SimulationState ss;
	CigiHost ch(&ss);
	//UDPServer server;
	//server.run();
	ch.runCigi();
	char q;
	std::cout << "Listo" << std::endl;
	std::cin >> q;
	return 0;
}

