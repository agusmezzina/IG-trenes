#pragma once
#include <osg/AnimationPath>
#include <osgViewer/CompositeViewer>
#include "CameraController.h"
#include <osgViewer/Viewer>
#include "SceneData.h"

class GraphicManager
{
private:
	SceneData* _data;
	osg::ref_ptr<osg::Node> _sceneRoot;
	osg::ref_ptr<osg::AnimationPath> _path;
	osg::ref_ptr<CameraController> _cameraCtrl;
	osg::ref_ptr<osg::Geode> createBallNode(const osg::Vec3& center, float radius, const osg::Vec4& color);
	osg::ref_ptr<osg::Camera> createCamera(const osg::Vec3& eye, const osg::Vec3& center, const osg::Vec3& up, osg::Node* scene);
	void createScene();
	int runViewer();
public:
	void addKeyFrame(double x, double y, double z, double t);
	GraphicManager(SceneData* data);
	int runScene();
	~GraphicManager();
};

