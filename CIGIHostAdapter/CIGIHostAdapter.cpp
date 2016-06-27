// CIGIHostAdapter.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "CigiHost.h"
#include "ModelUpdater.h"
#include "SceneData.h"
#include <thread>

int _tmain(int argc, _TCHAR* argv[])
{
	//UDPServer server;
	SceneData data;
	CigiHost server(&data);
	ModelUpdater model(&data);
	
	std::thread cigiThread(&CigiHost::run, &server);
	std::thread modelThread(&ModelUpdater::run, &model);

	cigiThread.join();
	modelThread.join();

	return 0;
}

