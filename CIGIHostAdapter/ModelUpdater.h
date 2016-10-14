#pragma once
#include <queue>
#include "SceneData.h"
#include "DataPacket.h"
#include "Semaphore.h"

class ModelUpdater
{
public:
	ModelUpdater(Semaphore* sem, std::queue<DataPacket>* data);
	void run();
	virtual ~ModelUpdater();
private:
	DataPacket readData(std::string message);
	void enqueueData(DataPacket p);
	std::queue<DataPacket>* data;
	Semaphore* s;
};

