#pragma once
class Entity
{
public:
	void getState(double& x, double& y, double& z, double& vx, double& vy, double& vz) const;
	void setState(double x, double y, double z, double vx, double vy, double vz);
	int getID() const;
	Entity(int id);
	virtual ~Entity();
private:
	double x, y, z;
	double yaw, pitch, roll;
	double vx, vy, vz;
	int id;
};

