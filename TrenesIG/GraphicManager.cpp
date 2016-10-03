#include "GraphicManager.h"
#include "UpdateTransformCallback.h"
#include "Skybox.h"
#include "Sky.h"
#include <osg\Geode>
#include <osg\ShapeDrawable>
#include <osg\MatrixTransform>
#include <osg\Switch>
#include <osg\TexGen>
#include <osgDB\ReadFile>
#include <osgGA\TrackballManipulator>
#include <osgViewer\ViewerEventHandlers>
#include <osgViewer\config\SingleWindow>
#include <iostream>
#include <chrono>

GraphicManager::GraphicManager(World* data, World* ghost) : _data(data), _ghost(ghost), /*netMgr(data),*/ _path(nullptr), _sceneRoot(nullptr), _cameraCtrl(nullptr)
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

osg::ref_ptr<osg::Geode> GraphicManager::createFloor()
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(-1000.0f, 0.3f, -1000.0f));
	vertices->push_back(osg::Vec3(1000.0f, 0.3f, -1000.0f));
	vertices->push_back(osg::Vec3(1000.0f, 0.3f, 1000.0f));
	vertices->push_back(osg::Vec3(-1000.0f, 0.3f, 1000.0f));

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	texcoords->push_back(osg::Vec2(0.0f, 0.0f));
	texcoords->push_back(osg::Vec2(0.0f, 100.0f));
	texcoords->push_back(osg::Vec2(100.0f, 100.0f));
	texcoords->push_back(osg::Vec2(100.0f, 0.0f));

	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setVertexArray(vertices.get());
	quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
	quad->setTexCoordArray(0, texcoords.get());
	quad->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("C:\\ObjetosVarios\\grass-tile.jpg");
	texture->setImage(image.get());
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	osg::ref_ptr<osg::Geode> floor = new osg::Geode;
	floor->addDrawable(quad.get());
	floor->getOrCreateStateSet()->setTextureAttributeAndModes(
		0, texture.get());
	return floor.release();
}

osg::Node* GraphicManager::createLightSource(unsigned int num,
	const osg::Vec3& trans,
	const osg::Vec4& color)
{
	osg::ref_ptr<osg::Light> light = new osg::Light;
	light->setLightNum(num);
	light->setDiffuse(color);
	light->setPosition(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	osg::ref_ptr<osg::LightSource> lightSource = new
		osg::LightSource;
	lightSource->setLight(light);
	osg::ref_ptr<osg::MatrixTransform> sourceTrans =
		new osg::MatrixTransform;
	sourceTrans->setMatrix(osg::Matrix::translate(trans));
	sourceTrans->addChild(lightSource.get());
	return sourceTrans.release();
}

osg::Node* GraphicManager::createSky()
{
	osg::ref_ptr<osg::Node> sky = osgDB::readNodeFile("skydome.osgt.90,0,0.rot");
	osg::ref_ptr<osg::MatrixTransform> transfSky = new osg::MatrixTransform;
	transfSky->setMatrix(osg::Matrix::scale(osg::Vec3f(0.035f, 0.035f, 0.035f)) * osg::Matrix::translate(osg::Vec3f(-950.0f, 50.0f, -1300.0f)));
	transfSky->addChild(sky.get());
	osg::ref_ptr<Sky> skydome = new Sky;
	skydome->addChild(transfSky);
	return skydome.release();
}

void GraphicManager::createScene(){
	osg::ref_ptr<osg::Group> scene = new osg::Group;

	osg::ref_ptr<osg::Node> light = createLightSource(0, osg::Vec3(10.0f, -10.0f, -20.0f), osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\vagon.flt.90,270,0.rot");
	osg::ref_ptr<osg::Node> rail = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\tramo_vias2.flt.90,90,0.rot");
	osg::ref_ptr<osg::Node> anden = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\escenario_anden.flt.90,90,0.rot");
	
	osg::ref_ptr<osg::Geode> floor = createFloor();
	osg::ref_ptr<osg::Node> skydome = createSky();

	osg::ref_ptr<osg::MatrixTransform> transf = new osg::MatrixTransform;
	transf->setMatrix(osg::Matrix::translate(osg::Vec3f()));
	transf->addChild(model.get());
	transf->setUpdateCallback(new UpdateTransformCallback(_data, _ghost));

	osg::ref_ptr<osg::MatrixTransform> transfAnden1 = new osg::MatrixTransform;
	transfAnden1->setMatrix(osg::Matrix::translate(osg::Vec3f(-5.0f, 0.0f, 0.0f)));
	transfAnden1->addChild(anden);

	/*osg::ref_ptr<osg::MatrixTransform> scaleModel = new osg::MatrixTransform;
	scaleModel->setMatrix(osg::Matrix::scale(osg::Vec3f(0.1f, 0.1f, 0.1f)));
	scaleModel->addChild(transf);

	osg::ref_ptr<osg::MatrixTransform> scaleRail = new osg::MatrixTransform;
	scaleRail->setMatrix(osg::Matrix::scale(osg::Vec3f(0.1f, 0.1f, 0.1f)));
	scaleRail->addChild(transf);*/

	for (int i = 0; i < 10; i++)
	{
		osg::ref_ptr<osg::MatrixTransform> railTransf = new osg::MatrixTransform;
		railTransf->setMatrix(osg::Matrix::translate(osg::Vec3f(26.0f * i, 0.0f, 0.0f)));
		railTransf->addChild(rail);
		scene->addChild(railTransf);
		//scaleRail->addChild(railTransf);
	}

	scene->addChild(light);
	scene->addChild(transf);
	scene->addChild(rail);
	scene->addChild(transfAnden1);
	/*scene->addChild(scaleRail);
	scene->addChild(scaleModel);*/	
	scene->addChild(floor);
	scene->addChild(skydome);
	_sceneRoot = scene;
}

int GraphicManager::runViewer(){
	osgViewer::Viewer viewer;
	//viewer.addEventHandler(_cameraCtrl.get());
	viewer.setSceneData(_sceneRoot.get());
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.apply(new osgViewer::SingleWindow(10, 10, 800, 600));
	//netMgr.startThread();
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
