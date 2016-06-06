#pragma once
#include "SceneData.h"

class ModelUpdater
{
public:
	ModelUpdater(SceneData* data);
	void run();
	virtual ~ModelUpdater();
private:
	SceneData* data;
};

