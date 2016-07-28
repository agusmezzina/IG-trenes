#include "stdafx.h"
#include "DeadReckoning.h"
#include <cmath>

void DeadReckoning::updateGhost(int entityID, float deltaT)
{
	//ghost->firstOrderPredictUpdate([deltaT](float p, float v) {return p + v * deltaT; });
	auto ghostEntity = ghost->getEntity(entityID);
	auto p = ghostEntity.getPosition();
	auto v = ghostEntity.getVelocity();
	ghost->updateEntityPosition(entityID, p + v * deltaT);
}

void DeadReckoning::correctGhost(int entityID, int step)
{
	auto entity = model->getEntity(entityID);
	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	auto pg = ghost->getEntity(entityID).getPosition();
	ghost->updateEntityPosition(entityID, pg + (p - pg) * step / smoothness);
	ghost->updateEntityVelocity(entityID, v);
}

void DeadReckoning::correctGhost(int entityID)
{
	auto entity = model->getEntity(entityID);
	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	ghost->updateEntityPosition(entityID, p);
	ghost->updateEntityVelocity(entityID, v);
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
	rThreshold = 0.1f;
	smoothness = 10;
}


DeadReckoning::~DeadReckoning()
{
}
