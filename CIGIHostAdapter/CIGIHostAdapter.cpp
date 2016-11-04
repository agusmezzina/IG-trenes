// CIGIHostAdapter.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "SimulationTimer.h"
#include "CigiHost.h"
#include "ModelUpdater.h"
#include "SceneData.h"
#include "World.h"
#include "DataPacket.h"
#include <thread>
#include <queue>
#include <atomic>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	//UDPServer server;
	//SceneData data;
	std::atomic_bool quit;
	quit = false;
	Semaphore s;
	std::queue<DataPacket> data;
	World worldData;
	World ghostData;
	/*worldData.updateEntityPosition(1, osg::Vec3f(0.0f, 3.0f, 0.0f));
	ghostData.updateEntityPosition(1, osg::Vec3f(0.0f, 3.0f, 0.0f));*/
	CigiHost server(&worldData, &ghostData, &s, &data);
	ModelUpdater model(&s, &data);
	//SimulationTimer timer(&data, &worldData);

	std::thread cigiThread(&CigiHost::run, &server, std::ref(quit));
	std::thread modelThread(&ModelUpdater::run, &model, std::ref(quit));
	//std::thread timerThread(&SimulationTimer::run, &timer);
	char command = ' ';
	while (command != 'q')
		std::cin >> command;
	model.stop();
	quit = true;
	cigiThread.join();
	modelThread.join();
	//timerThread.join();

	return 0;
}

