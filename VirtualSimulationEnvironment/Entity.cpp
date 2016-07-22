#include "stdafx.h"
#include "Entity.h"

osg::Vec3f Entity::getPosition() const
{
	return this->position;
}

void Entity::setPosition(osg::Vec3f newPosition)
{
	this->position = newPosition;
}

osg::Vec3f Entity::getVelocity() const
{
	return this->velocity;
}

void Entity::setVelocity(osg::Vec3f newVelocity)
{
	this->velocity = newVelocity;
}

int Entity::getID() const
{
	return this->id;
}

Entity::Entity(int id)
{
	this->id = id;
}

Entity::~Entity()
{
}
