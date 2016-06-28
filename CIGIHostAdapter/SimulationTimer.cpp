#include "SimulationTimer.h"
#include <future>
#include <chrono>

SimulationTimer::SimulationTimer(SceneData* data)
{
	_data = data;
}

void SimulationTimer::run()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		_data->updateCurrent();
	}
}

SimulationTimer::~SimulationTimer()
{
}
