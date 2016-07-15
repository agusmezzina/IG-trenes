#pragma once
#include <list>
#include <mutex>
#include <functional>
#include "Entity.h"

class World
{
public:
	void addEntity(Entity e);
	void updateEntityPosition(int id, double x, double y, double z);
	void updateEntityVelocity(int id, double vx, double vy, double vz);
	void firstOrderPredictUpdate(
		const std::function<double (double p, double v)>& predictionFunction);
	void instantCorrect(World* reference, double threshold);
	World();
	virtual ~World();
private:
	std::list<Entity> entities;
	std::mutex m;
};

