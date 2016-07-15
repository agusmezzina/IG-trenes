#pragma once
class Entity
{
public:
	void getPositionState(double& x, double& y, double& z) const;
	void setPositionState(double x, double y, double z);
	int getID() const;
	Entity(int id);
	virtual ~Entity();
private:
	double x, y, z;
	double yaw, pitch, roll;
	int id;
};

