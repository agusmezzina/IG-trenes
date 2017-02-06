#include "GraphicManager.h"
#include "Skybox.h"
#include "Sky.h"
#include <osg\Geode>
#include <osg\ShapeDrawable>
#include <osg\MatrixTransform>
#include <osg\PositionAttitudeTransform>
#include <osg\Switch>
#include <osg\TexGen>
#include <osgUtil/SmoothingVisitor>
#include <osgDB\ReadFile>
#include <osgGA\TrackballManipulator>
#include <osgViewer\ViewerEventHandlers>
#include <osgViewer\config\SingleWindow>
#include <iostream>
#include <chrono>
#include <vector>

GraphicManager::GraphicManager()
{
	env = std::make_unique<osgCigi::CigiSimulationEnvironment>();
	trackCurve = std::make_unique<osgCigi::CubicBezier>(
		osg::Vec3f(0.0f, 0.0f, 0.0f),
		osg::Vec3f(0.0f, 0.0f, 100.0f),
		osg::Vec3f(50.0f, 0.0f, 100.0f),
		osg::Vec3f(100.0f, 0.0f, 100.0f));
}

//osg::Camera* GraphicManager::createHUDCamera(double left, double right, double bottom, double top)
//{
//	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
//	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
//	camera->setRenderOrder(osg::Camera::POST_RENDER);
//	camera->setAllowEventFocus(false);
//	camera->setProjectionMatrix(
//		osg::Matrix::ortho2D(left, right, bottom, top));
//	camera->getOrCreateStateSet()->setMode(
//		GL_LIGHTING, osg::StateAttribute::OFF);
//	return camera.release();
//}

osg::Geode* GraphicManager::createBallNode(const osg::Vec3& center, float radius, const osg::Vec4& color){
	osg::ref_ptr<osg::ShapeDrawable> sphereShape = new osg::ShapeDrawable;
	sphereShape->setShape(new osg::Sphere(center, radius));
	sphereShape->setColor(color);
	osg::ref_ptr<osg::Geode> node = new osg::Geode;
	node->addDrawable(sphereShape.get());
	return node.release();
}

osg::Camera* GraphicManager::createCamera(const osg::Vec3& eye, const osg::Vec3& center, const osg::Vec3& up, osg::Node* scene){
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setViewMatrixAsLookAt(eye, center, up);
	camera->addChild(scene);
	return camera.release();
}

osg::Geode* GraphicManager::createFloor()
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

osg::Geode* GraphicManager::createProfile1()
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	/*osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));*/

	for (float t = 0; t <= 1; t += 0.01)
	{
		auto pos = trackCurve->getPositionByArcLength(t);
		auto rot = trackCurve->getOrientationByArcLength(t);
		double angle = 0, x = 0, y = 0, z = 0;
		rot.getRotate(angle, x, y, z);
		auto vert1 = osg::Vec3(pos.x() + /*pos.x() * */1.1 * cos(angle + osg::DegreesToRadians(90.0)), -0.15f, pos.z() + /*pos.z() * */1.1 * sin(angle + osg::DegreesToRadians(90.0)));
		auto vert2 = osg::Vec3(pos.x() + /*pos.x() * */0.9 * cos(angle + osg::DegreesToRadians(90.0)), -0.15f, pos.z() + /*pos.z() * */0.9 * sin(angle + osg::DegreesToRadians(90.0)));
		auto vert3 = osg::Vec3(pos.x() + /*pos.x() * */1.1 * cos(angle + osg::DegreesToRadians(90.0)), 0.3f, pos.z() + /*pos.z() * */1.1 * sin(angle + osg::DegreesToRadians(90.0)));
		auto vert4 = osg::Vec3(pos.x() + /*pos.x() * */0.9 * cos(angle + osg::DegreesToRadians(90.0)), 0.3f, pos.z() + /*pos.z() * */0.9 * sin(angle + osg::DegreesToRadians(90.0)));
		vertices->push_back(vert1);
		vertices->push_back(vert2);
		vertices->push_back(vert3);
		vertices->push_back(vert4);
		texcoords->push_back(osg::Vec2(0.0f, t * 100));
		texcoords->push_back(osg::Vec2(1.0f, t * 100));
		texcoords->push_back(osg::Vec2(1.0f, t * 100));
		texcoords->push_back(osg::Vec2(0.0f, t * 100));
	}

	osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_QUADS, 1584);
	for (int i = 0; i < 99; i++)
	{
		(*indices)[0 + 16 * i] = 0 + 4 * i; (*indices)[1 + 16 * i] = 4 + 4 * i; (*indices)[2 + 16 * i] = 5 + 4 * i; (*indices)[3 + 16 * i] = 1 + 4 * i;
		(*indices)[4 + 16 * i] = 1 + 4 * i; (*indices)[5 + 16 * i] = 5 + 4 * i; (*indices)[6 + 16 * i] = 7 + 4 * i; (*indices)[7 + 16 * i] = 3 + 4 * i;
		(*indices)[8 + 16 * i] = 2 + 4 * i; (*indices)[9 + 16 * i] = 6 + 4 * i; (*indices)[10 + 16 * i] = 7 + 4 * i; (*indices)[11 + 16 * i] = 3 + 4 * i;
		(*indices)[12 + 16 * i] = 0 + 4 * i; (*indices)[13 + 16 * i] = 4 + 4 * i; (*indices)[14 + 16 * i] = 6 + 4 * i; (*indices)[15 + 16 * i] = 2 + 4 * i;
	}

	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setVertexArray(vertices.get());
	quad->setTexCoordArray(0, texcoords.get());
	/*quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_OVERALL);*/
	//quad->setTexCoordArray(0, texcoords.get());
	//quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, 200));
	quad->addPrimitiveSet(indices.get());
	osgUtil::SmoothingVisitor::smooth(*quad);

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("C:\\ObjetosVarios\\textura-metal02.jpg");
	texture->setImage(image.get());
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	osg::ref_ptr<osg::Geode> profile = new osg::Geode;
	profile->addDrawable(quad.get());
	profile->getOrCreateStateSet()->setTextureAttributeAndModes(
		0, texture.get());
	return profile.release();
}

osg::Geode* GraphicManager::createProfile2()
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;

	for (float t = 0; t <= 1; t += 0.01)
	{
		auto pos = trackCurve->getPositionByArcLength(t);
		auto rot = trackCurve->getOrientationByArcLength(t);
		double angle = 0, x = 0, y = 0, z = 0;
		rot.getRotate(angle, x, y, z);
		auto vert1 = osg::Vec3(pos.x() + /*pos.x() * */1.1 * cos(angle - osg::DegreesToRadians(90.0)), -0.15f, pos.z() + /*pos.z() * */1.1 * sin(angle - osg::DegreesToRadians(90.0)));
		auto vert2 = osg::Vec3(pos.x() + /*pos.x() * */0.9 * cos(angle - osg::DegreesToRadians(90.0)), -0.15f, pos.z() + /*pos.z() * */0.9 * sin(angle - osg::DegreesToRadians(90.0)));
		auto vert3 = osg::Vec3(pos.x() + /*pos.x() * */1.1 * cos(angle - osg::DegreesToRadians(90.0)), 0.3f, pos.z() + /*pos.z() * */1.1 * sin(angle - osg::DegreesToRadians(90.0)));
		auto vert4 = osg::Vec3(pos.x() + /*pos.x() * */0.9 * cos(angle - osg::DegreesToRadians(90.0)), 0.3f, pos.z() + /*pos.z() * */0.9 * sin(angle - osg::DegreesToRadians(90.0)));
		vertices->push_back(vert1);
		vertices->push_back(vert2);
		vertices->push_back(vert3);
		vertices->push_back(vert4);
		texcoords->push_back(osg::Vec2(0.0f, t * 100));
		texcoords->push_back(osg::Vec2(1.0f, t * 100));
		texcoords->push_back(osg::Vec2(1.0f, t * 100));
		texcoords->push_back(osg::Vec2(0.0f, t * 100));
	}

	osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_QUADS, 1584);
	for (int i = 0; i < 99; i++)
	{
		(*indices)[0 + 16 * i] = 0 + 4 * i; (*indices)[1 + 16 * i] = 4 + 4 * i; (*indices)[2 + 16 * i] = 5 + 4 * i; (*indices)[3 + 16 * i] = 1 + 4 * i;
		(*indices)[4 + 16 * i] = 1 + 4 * i; (*indices)[5 + 16 * i] = 5 + 4 * i; (*indices)[6 + 16 * i] = 7 + 4 * i; (*indices)[7 + 16 * i] = 3 + 4 * i;
		(*indices)[8 + 16 * i] = 2 + 4 * i; (*indices)[9 + 16 * i] = 6 + 4 * i; (*indices)[10 + 16 * i] = 7 + 4 * i; (*indices)[11 + 16 * i] = 3 + 4 * i;
		(*indices)[12 + 16 * i] = 0 + 4 * i; (*indices)[13 + 16 * i] = 4 + 4 * i; (*indices)[14 + 16 * i] = 6 + 4 * i; (*indices)[15 + 16 * i] = 2 + 4 * i;
	}

	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setVertexArray(vertices.get());
	quad->setTexCoordArray(0, texcoords.get());
	quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
	//quad->setTexCoordArray(0, texcoords.get());
	//quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, 200));
	quad->addPrimitiveSet(indices.get());
	osgUtil::SmoothingVisitor::smooth(*quad);

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("C:\\ObjetosVarios\\textura-metal02.jpg");
	texture->setImage(image.get());
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	osg::ref_ptr<osg::Geode> profile = new osg::Geode;
	profile->addDrawable(quad.get());
	profile->getOrCreateStateSet()->setTextureAttributeAndModes(
		0, texture.get());
	return profile.release();
}

osg::Geode* GraphicManager::createPath()
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	/*vertices->push_back(osg::Vec3(-1000.0f, 0.299f, -2.0f));
	vertices->push_back(osg::Vec3(1000.0f, 0.299f, -2.0f));
	vertices->push_back(osg::Vec3(1000.0f, 0.299f, 2.0f));
	vertices->push_back(osg::Vec3(-1000.0f, 0.299f, 2.0f));*/

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	/*texcoords->push_back(osg::Vec2(0.0f, 0.0f));
	texcoords->push_back(osg::Vec2(500.0f, 0.0f));
	texcoords->push_back(osg::Vec2(500.0f, 1.0f));
	texcoords->push_back(osg::Vec2(0.0f, 1.0f));*/

	for (float t = 0; t <= 1; t += 0.01)
	{
		auto pos = trackCurve->getPositionByArcLength(t);
		auto rot = trackCurve->getOrientationByArcLength(t);
		double angle = 0, x = 0, y = 0, z = 0;
		rot.getRotate(angle, x, y, z);
		auto vert1 = osg::Vec3(pos.x() + /*pos.x() * */2*cos(angle + osg::DegreesToRadians(90.0)), 0.2f, pos.z() + /*pos.z() * */2*sin(angle + osg::DegreesToRadians(90.0)));
		auto vert2 = osg::Vec3(pos.x() + /*pos.x() * */2*cos(angle - osg::DegreesToRadians(90.0)), 0.2f, pos.z() + /*pos.z() * */2*sin(angle - osg::DegreesToRadians(90.0)));
		vertices->push_back(vert1);
		vertices->push_back(vert2);
		texcoords->push_back(osg::Vec2(0.0f, t * 100));
		texcoords->push_back(osg::Vec2(1.0f, t * 100));
	}

	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setVertexArray(vertices.get());
	quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
	quad->setTexCoordArray(0, texcoords.get());
	quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, 200));

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("C:\\ObjetosVarios\\road-with-gravel.jpg");
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
	light->setAmbient(osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f));
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

osg::Node* GraphicManager::createTrack()
{
	osg::ref_ptr<osg::Group> track = new osg::Group;
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\durmienteSolo.flt.90,0,0.rot.[0,0.3,0].trans");

	for (float t = 0; t <= 1; t += 0.01)
	{
		auto pos = trackCurve->getPositionByArcLength(t);
		auto rot = trackCurve->getOrientationByArcLength(t);
		osg::ref_ptr<osg::PositionAttitudeTransform> transf = new osg::PositionAttitudeTransform;
		transf->setPosition(pos);
		transf->setAttitude(rot);
		transf->addChild(model);
		track->addChild(transf);
	}
	return track.release();
	
}

void GraphicManager::createScene(){
	osg::ref_ptr<osg::Group> scene = new osg::Group;

	env->registerModel(/*1, */"C:\\ObjetosVarios\\EstacionDemo\\vagon.flt.90,270,0.rot.[0,-0.1,0].trans");
	osg::ref_ptr<osg::Group> sim = env->createSimulationScene();

	osg::ref_ptr<osg::Node> light = createLightSource(0, osg::Vec3(10.0f, -10.0f, -20.0f), osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	//osg::ref_ptr<osg::Node> rail = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\tramo_vias2.flt.90,90,0.rot");
	//osg::ref_ptr<osg::Node> anden = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\anden_con_techo\\anden_con_techo.flt.90,90,0.rot.[6,0.85,-3.5].trans");	
	osg::ref_ptr<osg::Geode> floor = createFloor();
	osg::ref_ptr<osg::Geode> path = createPath();
	osg::ref_ptr<osg::Node> skydome = createSky();
	osg::ref_ptr<osg::Node> track = createTrack();

	osg::ref_ptr<osg::Geode> profile1 = createProfile1();
	osg::ref_ptr<osg::Geode> profile2 = createProfile2();

	//osg::ref_ptr<osg::MatrixTransform> transfAnden1 = new osg::MatrixTransform;
	//transfAnden1->setMatrix(osg::Matrix::translate(osg::Vec3f(-5.0f, 0.0f, 0.0f)));
	//transfAnden1->addChild(anden);

	/*for (int i = -30; i < 30; i++)
	{
		osg::ref_ptr<osg::MatrixTransform> railTransf = new osg::MatrixTransform;
		railTransf->setMatrix(osg::Matrix::translate(osg::Vec3f(26.0f * i, 0.2f, 0.0f)));
		railTransf->addChild(rail);
		scene->addChild(railTransf);
	}*/
	
	scene->addChild(profile1);
	scene->addChild(profile2);

	scene->addChild(track);
	scene->addChild(light);
	//scene->addChild(transfAnden1);
	scene->addChild(floor);
	scene->addChild(path);
	scene->addChild(skydome);
	scene->addChild(sim);
	_sceneRoot = scene;
}

int GraphicManager::runViewer(){
	osgViewer::Viewer viewer;
	osg::ref_ptr<CommandController> commCrtl = new CommandController(env.get());
	viewer.setSceneData(_sceneRoot.get());
	viewer.addEventHandler(commCrtl.get());
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.apply(new osgViewer::SingleWindow(10, 10, 800, 600));
	env->start();
	return viewer.run();
}

int GraphicManager::runScene(){
	createScene();
	int result = runViewer();
	return result;
}

GraphicManager::~GraphicManager()
{
}
