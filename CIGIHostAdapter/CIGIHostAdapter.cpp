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
#include <boost\algorithm\string.hpp>

int _tmain(int argc, _TCHAR* argv[])
{
	std::atomic_bool quit;
	quit = false;
	Semaphore s;
	std::queue<DataPacket> data;
	World worldData;
	World ghostData;
	CigiHost server(&worldData, &ghostData, &s, &data);
	ModelUpdater model(&s, &data);

	std::thread cigiThread(&CigiHost::run, &server, std::ref(quit));
	std::thread modelThread(&ModelUpdater::run, &model, std::ref(quit));

	std::string command = "";
	do
	{
		std::getline(std::cin, command);
		if (command.substr(0, 6) == "thresh")
		{
			float thresh = 0;
			std::vector<std::string> fields;
			boost::split(fields, command, boost::is_any_of("\t "), boost::token_compress_on);
			if (fields.size() == 2)
			{
				try
				{
					thresh = std::stof(fields[1]);
					server.changeThreshold(thresh);
					std::cout << "Threshold changed to " << thresh << std::endl;
				}
				catch (...)
				{
				}
			}
		}
		if (command == "dr")
		{
			bool value = server.toggleDR();
			if (value)
				std::cout << "Dead Reckoning ON" << std::endl;
			else
				std::cout << "Dead Reckoning OFF" << std::endl;
		}
		if (command == "predict")
		{
			bool value = server.togglePredictionMethod();
			if (value)
				std::cout << "Prediction Method = QUADRATIC" << std::endl;
			else
				std::cout << "Prediction Method = LINEAR" << std::endl;
		}
	} while (command != "quit");

	model.stop();
	quit = true;
	cigiThread.join();
	modelThread.join();

	return 0;
}

