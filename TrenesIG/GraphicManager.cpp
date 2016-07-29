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

GraphicManager::GraphicManager(World* data, World* ghost) : _data(data), _ghost(ghost), netMgr(data), _path(nullptr), _sceneRoot(nullptr), _cameraCtrl(nullptr)
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
	osg::ref_ptr<osg::Group> scene = new osg::Group;

	osg::ref_ptr<osg::LightSource> light = new osg::LightSource;
	light->getLight()->setPosition(osg::Vec4(4.0, 4.0, 10.0,
		0.0));
	light->getLight()->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0)
		);
	light->getLight()->setDiffuse(osg::Vec4(0.8, 0.8, 0.8, 1.0)
		);


	osg::ref_ptr<osg::ShapeDrawable> floorShape = new osg::ShapeDrawable;
	floorShape->setShape(new osg::Box(osg::Vec3(0.0f, -0.1f, 0.0f), 10.0f, 0.01f, 10.0f));
	floorShape->setColor(osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f));
	osg::ref_ptr<osg::Geode> floor = new osg::Geode;
	floor->addDrawable(floorShape.get());

	osg::ref_ptr<osg::Geode> model = createBallNode(
		osg::Vec3(0.0f, 0.0f, 0.0f),
		0.1f,
		osg::Vec4(1.0f, 1.0f, 0.5f, 1.0f));
	osg::ref_ptr<osg::MatrixTransform> transf = new osg::MatrixTransform;
	transf->setMatrix(osg::Matrix::translate(osg::Vec3f(0.0f, 3.0f, 0.0f)));
	transf->addChild(model.get());
	transf->setUpdateCallback(new UpdateTransformCallback(_data, _ghost));

	scene->addChild(light);
	scene->addChild(transf);
	scene->addChild(floor);
	//scene->addChild(model);

	//osg::Vec3 center = osg::Vec3(38.0f, -91.0f, 500.0f);
	/*osg::Vec3 center = osg::Vec3(0.0f, 3.0f, 0.0f);
	double radius = model->getBound().radius();

	osg::ref_ptr<osg::Camera> mainCamera = createCamera(center - (-osg::Z_AXIS * radius * 75.0), center, osg::Y_AXIS, scene.get());
	osg::ref_ptr<osg::Camera> camera1 = createCamera(center - (osg::Y_AXIS * radius * 15.0), center, osg::Z_AXIS, scene.get());
	osg::ref_ptr<osg::Camera> camera2 = createCamera(center - (osg::X_AXIS * radius * 15.0), center, osg::Z_AXIS, scene.get());

	osg::ref_ptr<osg::Switch> cameras = new osg::Switch;
	cameras->addChild(mainCamera.get(), true);
	cameras->addChild(camera1.get(), false);
	cameras->addChild(camera2.get(), false);

	_cameraCtrl = new CameraController(cameras.get());*/
	//_sceneRoot = cameras;
	_sceneRoot = scene;
}

int GraphicManager::runViewer(){
	osgViewer::Viewer viewer;
	//viewer.addEventHandler(_cameraCtrl.get());
	viewer.setSceneData(_sceneRoot.get());
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.apply(new osgViewer::SingleWindow(10, 10, 800, 600));
	netMgr.startThread();
	//while (!viewer.done())
	//{

	//	//netMgr.sendSOF();
	//	viewer.frame();
	//}
	//netMgr.cancel();
	return viewer.run();
	//return 0;
}

int GraphicManager::runScene(){
	createScene();
	int result = runViewer();
	return result;
}

GraphicManager::~GraphicManager()
{
}
