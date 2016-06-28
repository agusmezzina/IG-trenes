// CIGIHostAdapter.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "SimulationTimer.h"
#include "CigiHost.h"
#include "ModelUpdater.h"
#include "SceneData.h"
#include <thread>
#include <queue>

int _tmain(int argc, _TCHAR* argv[])
{
	//UDPServer server;
	SceneData data;
	CigiHost server(&data);
	ModelUpdater model(&data);
	SimulationTimer timer(&data);

	std::thread cigiThread(&CigiHost::run, &server);
	std::thread modelThread(&ModelUpdater::run, &model);
	std::thread timerThread(&SimulationTimer::run, &timer);

	cigiThread.join();
	modelThread.join();
	timerThread.join();

	return 0;
}

