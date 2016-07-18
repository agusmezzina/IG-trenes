#include "stdafx.h"
#include "World.h"
#include <algorithm>

void World::addEntity(Entity e)
{
	std::lock_guard<std::mutex> g(m);
	entities.push_front(e);
}

const Entity World::getEntity(int id) const
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	return *it;
}

void World::updateEntityPosition(int id, double x, double y, double z)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	if ((*it).getID() == id)
		(*it).setPositionState(x, y, z);
}

void World::updateEntityVelocity(int id, double vx, double vy, double vz)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	if ((*it).getID() == id)
		(*it).setVelocityState(vx, vy, vz);
}

void World::firstOrderPredictUpdate(
	const std::function<double (double p, double v)>& predictionFunction)
{
	std::lock_guard<std::mutex> g(m);
	for (auto entity : entities)
	{
		double x, y, z;
		double vx, vy, vz;
		entity.getPositionState(x, y, z);
		entity.getVelocityState(vx, vy, vz);
		entity.setPositionState(
			predictionFunction(x, vx),
			predictionFunction(y, vy),
			predictionFunction(z, vz));
	}
}

//void World::instantCorrect(World* reference, double threshold)
//{
//	std::lock_guard<std::mutex> g(m);
//	for (auto entity : entities)
//	{
//		if (1) //threshold violated
//		{
//			//correct entity state
//		}
//	}
//}

World::World()
{
	Entity ownship(0);
	entities.push_back(ownship);
}


World::~World()
{
}
