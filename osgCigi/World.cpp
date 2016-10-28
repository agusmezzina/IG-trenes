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

void World::updateEntityOrientation(int id, osg::Vec3f orientation)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	if ((*it).getID() == id)
		(*it).setOrientation(orientation);
}

void World::updateEntityAngularVelocity(int id, osg::Vec3f velocity)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	if ((*it).getID() == id)
		(*it).setAngularVelocity(velocity);
}

void World::updateEntityAcceleration(int id, osg::Vec3f acceleration)
{
	std::lock_guard<std::mutex> g(m);
	auto it = std::find_if(std::begin(entities), std::end(entities),
		[&](Entity const& e) { return e.getID() == id; });
	if ((*it).getID() == id)
		(*it).setAcceleration(acceleration);
}

void World::firstOrderPredictUpdate(
	const std::function<float (float p, float v)>& predictionFunction)
{
	std::lock_guard<std::mutex> g(m);
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		auto p = (*it).getPosition();
		auto v = (*it).getVelocity();

		float x = predictionFunction(p.x(), v.x());
		float y = predictionFunction(p.y(), v.y());
		float z = predictionFunction(p.z(), v.z());

		(*it).setPosition(osg::Vec3f(x, y, z));
	}
}

void World::setTimestamp(float t)
{
	this->timestamp = t;
}

float World::getTimestamp() const
{
	return this->timestamp;
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
	Entity ownship(1);
	entities.push_back(ownship);
	timestamp = 0.0f;
}


World::~World()
{
}
