#pragma once
#include <osg/AnimationPath>
#include <osgViewer/CompositeViewer>
#include "CameraController.h"
#include <osgViewer/Viewer>
#include "CigiSimulationEnvironment.h"

class GraphicManager
{
private:
	//SceneData* _data;
	//CigiNetworkManager netMgr;
	std::unique_ptr<CigiSimulationEnvironment> env;
	osg::ref_ptr<osg::Node> _sceneRoot;
	osg::ref_ptr<CameraController> _cameraCtrl;
	osg::ref_ptr<osg::Geode> createBallNode(const osg::Vec3& center, float radius, const osg::Vec4& color);
	osg::ref_ptr<osg::Camera> createCamera(const osg::Vec3& eye, const osg::Vec3& center, const osg::Vec3& up, osg::Node* scene);
	osg::ref_ptr<osg::Geode> createFloor();
	osg::Node* createLightSource(unsigned int num, const osg::Vec3& trans, const osg::Vec4& color);
	osg::Node* createSky();
	void createScene();
	int runViewer();
public:
	GraphicManager();
	int runScene();
	~GraphicManager();
};

