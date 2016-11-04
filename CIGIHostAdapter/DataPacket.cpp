#include "DataPacket.h"


DataPacket::DataPacket(int id, float x, float y, float z, float vx, float vy, float vz, float ax, float ay, float az, float alpha, float alphaV, float t, bool last)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->z = z;
	this->vx = vx;
	this->vy = vy;
	this->vz = vz;
	this->ax = ax;
	this->ay = ay;
	this->az = az;
	this->alpha = alpha;
	this->alphaV = alphaV;
	this->t = t;
	this->last = last;
}

int DataPacket::getID() const
{
	return this->id;
}

float DataPacket::getX() const
{
	return this->x;
}

float DataPacket::getY() const
{
	return this->y;
}

float DataPacket::getZ() const
{
	return this->z;
}

float DataPacket::getVx() const
{
	return this->vx;
}

float DataPacket::getVy() const
{
	return this->vy;
}

float DataPacket::getVz() const
{
	return this->vz;
}

float DataPacket::getAx() const
{
	return this->ax;
}

float DataPacket::getAy() const
{
	return this->ay;
}

float DataPacket::getAz() const
{
	return this->az;
}

float DataPacket::getAlpha() const
{
	return this->alpha;
}

float DataPacket::getAlphaV() const
{
	return this->alphaV;
}

float DataPacket::getTime() const
{
	return this->t;
}

bool DataPacket::getLast() const
{
	return this->last;
}

DataPacket::~DataPacket()
{
}
