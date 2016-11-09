#include "World.h"
#include <algorithm>

using namespace osgCigi;

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

void World::updateEntityPosition(int id, osg::Vec3f position)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	if ((*it).getID() == id)
		(*it).setPosition(position);
}

void World::updateEntityVelocity(int id, osg::Vec3f velocity)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	if ((*it).getID() == id)
		(*it).setVelocity(velocity);
}

void World::updateEntityAcceleration(int id, osg::Vec3f acceleration)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	if ((*it).getID() == id)
		(*it).setAcceleration(acceleration);
}

void World::setTimestamp(long t)
{
	 this->timestamp = t;
}

long World::getTimestamp() const
{
	return this->timestamp;
}

World::World()
{
	Entity ownship(1);
	entities.push_back(ownship);
}


World::~World()
{
}
