#include "stdafx.h"
#include "World.h"
#include <algorithm>

void World::addEntity(Entity e)
{
	std::lock_guard<std::mutex> g(m);
	entities.push_back(e);
}

void World::updateEntityPosition(int id, double x, double y, double z)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	(*it).setPositionState(x, y, z);
}

World::World()
{
}


World::~World()
{
}
