#pragma once
#include "SceneData.h"
#include "DataPacket.h"
#include <queue>

class ModelUpdater
{
public:
	ModelUpdater(std::queue<DataPacket>* data);
	void run();
	virtual ~ModelUpdater();
private:
	std::queue<DataPacket>* data;
};

