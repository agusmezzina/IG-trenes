#include "SceneData.h"


SceneData::SceneData() : x(0), y(0.5), z(0)
{
}

void SceneData::getData(double& x, double& y, double& z) const
{
	std::lock_guard<std::mutex> lock(m);
	x = this->x;
	y = this->y;
	z = this->z;
}

void SceneData::setData(double x, double y, double z)
{
	std::lock_guard<std::mutex> lock(m);
	this->x = x;
	this->y = y;
	this->z = z;
}

SceneData::~SceneData()
{
}
