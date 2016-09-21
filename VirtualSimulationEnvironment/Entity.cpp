#include "stdafx.h"
#include "Entity.h"
#include <cmath>

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

osg::Vec3f Entity::getAcceleration() const
{
	return this->acceleration;
}

void Entity::setAcceleration(osg::Vec3f newAcceleration)
{
	this->acceleration = newAcceleration;
}

int Entity::getID() const
{
	return this->id;
}

float Entity::getSR() const
{
	return this->sr;
}

float Entity::getAOI() const
{
	return this->aoi;
}

Entity::Entity(int id)
{
	this->id = id;
	this->aoi = 0.0f;
	this->sr = 0.0f;
	this->updated = false;
}

Entity::Entity(int id, float areaOfInterest, float sensitiveRegion)
{
	this->id = id;
	this->aoi = areaOfInterest;
	this->sr = sensitiveRegion;
	this->updated = false;
}

Entity::~Entity()
{
}
