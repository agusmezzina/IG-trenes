#include "stdafx.h"
#include "DeadReckoning.h"
#include <cmath>

void DeadReckoning::updateGhost(double deltaT)
{
	ghost->firstOrderPredictUpdate([deltaT](double p, double v) {return p + v * deltaT; });
}

bool DeadReckoning::isThresholdViolated(int entityID)
{
	auto entity = model->getEntity(entityID);
	auto ghostEntity = ghost->getEntity(entityID);
	double x0, y0, z0, x1, y1, z1;
	double vx, vy, vz;
	entity.getState(x0, y0, z0, vx, vy, vz);
	ghostEntity.getState(x1, y1, z1, vx, vy, vz);
	if (sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2) + pow(z0 - z1, 2)) > rThreshold)
	{
		return true;
	}
	return false;
}

DeadReckoning::DeadReckoning(World* model, World* ghost) : model(model), ghost(ghost)
{

}


DeadReckoning::~DeadReckoning()
{
}
