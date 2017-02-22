#pragma once
#include <osg/AnimationPath>
#include <osgViewer/CompositeViewer>
#include "CameraController.h"
#include "CommandController.h"
#include <osgViewer/Viewer>
#include "CigiSimulationEnvironment.h"
#include "CubicBezier.h"

class GraphicManager
{
private:
	//SceneData* _data;
	//CigiNetworkManager netMgr;
	std::unique_ptr<osgCigi::CigiSimulationEnvironment> env;
	std::unique_ptr<osgCigi::CubicBezier> trackCurve;
	osg::ref_ptr<osg::Node> _sceneRoot;
	//osg::ref_ptr<CameraController> _cameraCtrl;
	//osg::Camera* createHUDCamera(double left, double right, double bottom, double top);
	osg::Geode* createBallNode(const osg::Vec3& center, float radius, const osg::Vec4& color);
	osg::Camera* createCamera(const osg::Vec3& eye, const osg::Vec3& center, const osg::Vec3& up, osg::Node* scene);
	osg::Geode* createFloor();
	osg::Geode* createPath();
	osg::Geode* createProfile1();
	osg::Geode* createProfile2();
	osg::Node* createLightSource(unsigned int num, const osg::Vec3& trans, const osg::Vec4& color);
	osg::Node* createSky();
	osg::Node* createTrack();
	osg::Node* createTrackPositiveExtension();
	osg::Node* createTrackNegativeExtension();
	osg::Node* createStation();
	void createScene();
	int runViewer();
public:
	GraphicManager();
	int runScene();
	~GraphicManager();
};

