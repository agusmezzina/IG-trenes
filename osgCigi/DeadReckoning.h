#pragma once
#include "World.h"

namespace osgCigi
{
	class DeadReckoning
	{
	public:
		void firstOrderUpdateGhost(int entityID, float deltaT);
		void secondOrderUpdateGhost(int entityID, float deltaT);
		void correctGhost(int entityID, int step);
		void correctGhost(int entityID);
		void compensateAndCorrectGhost(int entityID);
		void setConvergencePoint(int entityID, float deltaT);
		bool isThresholdViolated(int entityID);
		int getSmoothness() const;
		void setSmoothness(int smoothness);
		float getThreshold() const;
		void setThreshold(float thresh);
		osg::Vec3f getConvergencePoint();
		DeadReckoning(World* model, World* ghost);
		virtual ~DeadReckoning();
	private:
		float rThreshold;
		int smoothness;
		World* model;
		World* ghost;
		osg::Vec3f startPoint;
		osg::Vec3f convergencePoint;
		osg::Vec3f convergenceVelocity;
	};
}

