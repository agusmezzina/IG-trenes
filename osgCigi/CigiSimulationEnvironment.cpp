#include "CigiSimulationEnvironment.h"
#include "CigiUpdateCallback.h"
#include <osgDB\ReadFile>

using namespace osgCigi;

void CigiSimulationEnvironment::registerModel(const int& entityID, const std::string& path)
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(path);
	Entity entity(entityID);
	world->addEntity(entity);
	ghost->addEntity(entity);
	osg::ref_ptr<osg::MatrixTransform> transf = new osg::MatrixTransform;
	transf->setMatrix(osg::Matrix::translate(osg::Vec3f()));
	
	osg::ref_ptr<CigiUpdateCallback> cb = new CigiUpdateCallback(world.get(), ghost.get());

	transf->setUpdateCallback(cb);

	transf->addChild(model);
	simSubtree->addChild(transf);
}

osg::Group* CigiSimulationEnvironment::createSimulationScene()
{
	return simSubtree.get();
}

void CigiSimulationEnvironment::start()
{
	network->startThread();
}

CigiSimulationEnvironment::CigiSimulationEnvironment()
{
	simSubtree = new osg::Group;
	world = std::make_unique<World>();
	ghost = std::make_unique<World>();
	network = std::make_unique<CigiIG>(world.get());
}


CigiSimulationEnvironment::~CigiSimulationEnvironment()
{
}
