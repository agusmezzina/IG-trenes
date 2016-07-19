#include "GraphicManager.h"
#include <osg\Geode>
#include <osg\ShapeDrawable>
#include <osg\MatrixTransform>
#include <osg\Switch>
#include <osgDB\ReadFile>
#include <osgGA\TrackballManipulator>
#include <osgViewer\ViewerEventHandlers>
#include <osgViewer\config\SingleWindow>
#include <iostream>
#include <chrono>
#include "UpdateTransformCallback.h"

GraphicManager::GraphicManager(World* data, World* ghost) : _data(data), _ghost(ghost), netMgr(ghost), _path(nullptr), _sceneRoot(nullptr), _cameraCtrl(nullptr)
{
	_path = new osg::AnimationPath();
	dr = std::make_unique<DeadReckoning>(data, ghost);
}

void GraphicManager::addKeyFrame(double x, double y, double z, double t){
	_path->insert(t, osg::AnimationPath::ControlPoint(osg::Vec3(x, y, z)));
}

osg::ref_ptr<osg::Geode> GraphicManager::createBallNode(const osg::Vec3& center, float radius, const osg::Vec4& color){
	osg::ref_ptr<osg::ShapeDrawable> sphereShape = new osg::ShapeDrawable;
	sphereShape->setShape(new osg::Sphere(center, radius));
	sphereShape->setColor(color);
	osg::ref_ptr<osg::Geode> node = new osg::Geode;
	node->addDrawable(sphereShape.get());
	return node.release();
}

osg::ref_ptr<osg::Camera> GraphicManager::createCamera(const osg::Vec3& eye, const osg::Vec3& center, const osg::Vec3& up, osg::Node* scene){
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setViewMatrixAsLookAt(eye, center, up);
	camera->addChild(scene);
	return camera.release();
}

void GraphicManager::createScene(){
	osg::ref_ptr<osg::Geode> model = createBallNode(
		osg::Vec3(0.0f, -0.5f, 0.0f),
		0.1f,
		osg::Vec4(1.0f, 1.0f, 0.5f, 1.0f));
	//osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::MatrixTransform> scene = new osg::MatrixTransform;
	scene->addChild(model.get());
	//osg::ref_ptr<osg::AnimationPathCallback> apcb = new	osg::AnimationPathCallback;
	//apcb->setAnimationPath(_path);
	//scene->setUpdateCallback(apcb.get());
	//osg::ref_ptr<UpdateTransformCallback> updcb = new UpdateTransformCallback(_data);
	scene->setUpdateCallback(new UpdateTransformCallback(_ghost));


	//osg::Vec3 center = osg::Vec3(38.0f, -91.0f, 500.0f);
	osg::Vec3 center = osg::Vec3(0.0f, 0.0f, 0.0f);
	double radius = model->getBound().radius();

	osg::ref_ptr<osg::Camera> camera1 = createCamera(center - (-osg::Z_AXIS * radius * 15.0), center, osg::Y_AXIS, scene.get());
	osg::ref_ptr<osg::Camera> mainCamera = createCamera(center - (osg::Y_AXIS * radius * 15.0), center, osg::Z_AXIS, scene.get());
	osg::ref_ptr<osg::Camera> camera2 = createCamera(center - (osg::X_AXIS * radius * 15.0), center, osg::Z_AXIS, scene.get());

	osg::ref_ptr<osg::Switch> cameras = new osg::Switch;
	cameras->addChild(mainCamera.get(), true);
	cameras->addChild(camera1.get(), false);
	cameras->addChild(camera2.get(), false);

	_cameraCtrl = new CameraController(cameras.get());
	_sceneRoot = cameras;
}

int GraphicManager::runViewer(){
	osgViewer::Viewer viewer;
	viewer.addEventHandler(_cameraCtrl.get());
	viewer.setSceneData(_sceneRoot.get());
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.apply(new osgViewer::SingleWindow(10, 10, 800, 600));
	netMgr.startThread();
	auto t1 = std::chrono::high_resolution_clock::now();
	while (!viewer.done())
	{
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> deltaT = t2 - t1;
		dr->updateGhost(deltaT.count());
		netMgr.sendSOF();
		viewer.frame();
	}
	netMgr.cancel();
	return 0;
}

int GraphicManager::runScene(){
	createScene();
	//netMgr.startThread();
	int result = runViewer();
	//netMgr.cancel();
	return result;
}

GraphicManager::~GraphicManager()
{
}
