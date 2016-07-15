#pragma once
class Entity
{
public:
	void getPositionState(double& x, double& y, double& z) const;
	void setPositionState(double x, double y, double z);
	void getVelocityState(double& vx, double& vy, double& vz) const;
	void setVelocityState(double vx, double vy, double vz);
	int getID() const;
	Entity(int id);
	virtual ~Entity();
private:
	double x, y, z;
	double yaw, pitch, roll;
	double vx, vy, vz;
	int id;
};

