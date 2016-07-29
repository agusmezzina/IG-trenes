#pragma once
#include <osg\Vec3f>

class Entity
{
public:
	osg::Vec3f getPosition() const;
	void setPosition(osg::Vec3f newPosition);
	osg::Vec3f getVelocity() const;
	void setVelocity(osg::Vec3f newVelocity);
	osg::Vec3f getAcceleration() const;
	void setAcceleration(osg::Vec3f newAcceleration);
	int getID() const;
	Entity(int id);
	virtual ~Entity();
private:
	osg::Vec3f position;
	osg::Vec3f orientation;
	osg::Vec3f velocity;
	osg::Vec3f acceleration;
	int id;
};

