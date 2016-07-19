#pragma once
#include <list>
#include <mutex>
#include <functional>
#include "Entity.h"

class World
{
public:
	void addEntity(Entity e);
	const Entity getEntity(int id) const;
	void updateEntityState(int id, double x, double y, double z, double vx, double vy, double vz);
	void firstOrderPredictUpdate(
		const std::function<double (double p, double v)>& predictionFunction);
	//void instantCorrect(World* reference, double threshold);
	World();
	virtual ~World();
private:
	std::list<Entity> entities;
	mutable std::mutex m;
};

