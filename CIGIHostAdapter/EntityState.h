#pragma once
class EntityState
{
public:
	EntityState();
	EntityState(double x, double y, double z);
	void getState(double& x, double& y, double& z);
	void setState(double x, double y, double z);
	virtual ~EntityState();
private:
	double x, y, z;
};

