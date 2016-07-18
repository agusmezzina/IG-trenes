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
	entity.getPositionState(x0, y0, z0);
	ghostEntity.getPositionState(x1, y1, z1);
	if (sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2) + pow(z0 - z1, 2)) > rThreshold)
	{
		return true;
	}
	return false;
}

DeadReckoning::DeadReckoning()
{

}


DeadReckoning::~DeadReckoning()
{
}
