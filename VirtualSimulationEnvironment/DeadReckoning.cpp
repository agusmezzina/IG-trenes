#include "stdafx.h"
#include "DeadReckoning.h"
#include <cmath>

void DeadReckoning::updateGhost(float deltaT)
{
	ghost->firstOrderPredictUpdate([deltaT](float p, float v) {return p + v * deltaT; });
}

bool DeadReckoning::isThresholdViolated(int entityID)
{
	auto entity = model->getEntity(entityID);
	auto ghostEntity = ghost->getEntity(entityID);
	auto p0 = entity.getPosition();
	auto p1 = ghostEntity.getPosition();
	if (sqrt(pow(p0.x() - p1.x(), 2) + pow(p0.y() - p1.y(), 2) + pow(p0.z() - p1.z(), 2)) > rThreshold)
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
