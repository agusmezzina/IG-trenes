#include "GraphicManager.h"
#include "Skybox.h"
#include "Sky.h"
#include <osg\Geode>
#include <osg\ShapeDrawable>
#include <osg\MatrixTransform>
#include <osg\PositionAttitudeTransform>
#include <osg\Switch>
#include <osg\TexGen>
#include <osg\Camera>
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
	/*trackCurve = std::make_unique<osgCigi::CubicBezier>(
		osg::Vec3f(0.0f, 0.0f, 0.0f),
		osg::Vec3f(0.0f, 0.0f, 100.0f),
		osg::Vec3f(50.0f, 0.0f, 100.0f),
		osg::Vec3f(100.0f, 0.0f, 100.0f));*/
	trackCurve = std::make_unique<osgCigi::CubicBezier>("C:\\ObjetosVarios\\curve.txt");
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
	vertices->push_back(osg::Vec3(-2000.0f, 0.3f, -2000.0f));
	vertices->push_back(osg::Vec3(2000.0f, 0.3f, -2000.0f));
	vertices->push_back(osg::Vec3(2000.0f, 0.3f, 2000.0f));
	vertices->push_back(osg::Vec3(-2000.0f, 0.3f, 2000.0f));

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

	//Negative Extension
	double af = 0, xf = 0, yf = 0, zf = 0;
	auto rotFinal = trackCurve->getOrientation(0);
	rotFinal.getRotate(af, xf, yf, zf);
	int numberDurms = trackCurve->getTotalLength() / 2.0f;
	auto extPos = trackCurve->getNegativeExtension(numberDurms);
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */1.1 * cos(af + osg::DegreesToRadians(90.0)), -0.15f, extPos.z() + /*pos.z() * */1.1 * sin(af + osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */0.9 * cos(af + osg::DegreesToRadians(90.0)), -0.15f, extPos.z() + /*pos.z() * */0.9 * sin(af + osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */1.1 * cos(af + osg::DegreesToRadians(90.0)), 0.3f, extPos.z() + /*pos.z() * */1.1 * sin(af + osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */0.9 * cos(af + osg::DegreesToRadians(90.0)), 0.3f, extPos.z() + /*pos.z() * */0.9 * sin(af + osg::DegreesToRadians(90.0))));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));

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

	//Positive extension
	rotFinal = trackCurve->getOrientation(1);
	rotFinal.getRotate(af, xf, yf, zf);
	extPos = trackCurve->getPositiveExtension(numberDurms);
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */1.1 * cos(af + osg::DegreesToRadians(90.0)), -0.15f, extPos.z() + /*pos.z() * */1.1 * sin(af + osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */0.9 * cos(af + osg::DegreesToRadians(90.0)), -0.15f, extPos.z() + /*pos.z() * */0.9 * sin(af + osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */1.1 * cos(af + osg::DegreesToRadians(90.0)), 0.3f, extPos.z() + /*pos.z() * */1.1 * sin(af + osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */0.9 * cos(af + osg::DegreesToRadians(90.0)), 0.3f, extPos.z() + /*pos.z() * */0.9 * sin(af + osg::DegreesToRadians(90.0))));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));

	osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_QUADS, 1632);
	for (int i = 0; i < 102; i++)
	{
		(*indices)[0 + 16 * i] = 0 + 4 * i; (*indices)[1 + 16 * i] = 4 + 4 * i; (*indices)[2 + 16 * i] = 5 + 4 * i; (*indices)[3 + 16 * i] = 1 + 4 * i;
		(*indices)[4 + 16 * i] = 1 + 4 * i; (*indices)[5 + 16 * i] = 5 + 4 * i; (*indices)[6 + 16 * i] = 7 + 4 * i; (*indices)[7 + 16 * i] = 3 + 4 * i;
		(*indices)[8 + 16 * i] = 2 + 4 * i; (*indices)[9 + 16 * i] = 6 + 4 * i; (*indices)[10 + 16 * i] = 7 + 4 * i; (*indices)[11 + 16 * i] = 3 + 4 * i;
		(*indices)[12 + 16 * i] = 0 + 4 * i; (*indices)[13 + 16 * i] = 4 + 4 * i; (*indices)[14 + 16 * i] = 6 + 4 * i; (*indices)[15 + 16 * i] = 2 + 4 * i;
	}

	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setVertexArray(vertices.get());
	quad->setTexCoordArray(0, texcoords.get());
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

	//Negative Extension
	double af = 0, xf = 0, yf = 0, zf = 0;
	auto rotFinal = trackCurve->getOrientation(0);
	rotFinal.getRotate(af, xf, yf, zf);
	int numberDurms = trackCurve->getTotalLength() / 2.0f;
	auto extPos = trackCurve->getNegativeExtension(numberDurms);
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */1.1 * cos(af - osg::DegreesToRadians(90.0)), -0.15f, extPos.z() + /*pos.z() * */1.1 * sin(af - osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */0.9 * cos(af - osg::DegreesToRadians(90.0)), -0.15f, extPos.z() + /*pos.z() * */0.9 * sin(af - osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */1.1 * cos(af - osg::DegreesToRadians(90.0)), 0.3f, extPos.z() + /*pos.z() * */1.1 * sin(af - osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */0.9 * cos(af - osg::DegreesToRadians(90.0)), 0.3f, extPos.z() + /*pos.z() * */0.9 * sin(af - osg::DegreesToRadians(90.0))));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));

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

	//Positive extension
	rotFinal = trackCurve->getOrientation(1);
	rotFinal.getRotate(af, xf, yf, zf);
	extPos = trackCurve->getPositiveExtension(numberDurms);
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */1.1 * cos(af - osg::DegreesToRadians(90.0)), -0.15f, extPos.z() + /*pos.z() * */1.1 * sin(af - osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */0.9 * cos(af - osg::DegreesToRadians(90.0)), -0.15f, extPos.z() + /*pos.z() * */0.9 * sin(af - osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */1.1 * cos(af - osg::DegreesToRadians(90.0)), 0.3f, extPos.z() + /*pos.z() * */1.1 * sin(af - osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + /*pos.x() * */0.9 * cos(af - osg::DegreesToRadians(90.0)), 0.3f, extPos.z() + /*pos.z() * */0.9 * sin(af - osg::DegreesToRadians(90.0))));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));

	osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_QUADS, 1632);
	for (int i = 0; i < 102; i++)
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
	int vcount = 0;
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;

	//Negative extension path
	double af = 0, xf = 0, yf = 0, zf = 0;
	auto rotFinal = trackCurve->getOrientationByArcLength(0);
	rotFinal.getRotate(af, xf, yf, zf);
	int numberDurms = trackCurve->getTotalLength() / 2.0f;
	auto extPos = trackCurve->getNegativeExtension(numberDurms);
	vertices->push_back(osg::Vec3(extPos.x() + 2 * cos(af + osg::DegreesToRadians(90.0)), 0.2f, extPos.z() + 2 * sin(af + osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + 2 * cos(af - osg::DegreesToRadians(90.0)), 0.2f, extPos.z() + 2 * sin(af - osg::DegreesToRadians(90.0))));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	vcount += 2;
	

	//Curve path
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
		vcount += 2;
	}

	//Positive extension path
	rotFinal = trackCurve->getOrientationByArcLength(1);
	rotFinal.getRotate(af, xf, yf, zf);
	extPos = trackCurve->getPositiveExtension(numberDurms);
	vertices->push_back(osg::Vec3(extPos.x() + 2 * cos(af + osg::DegreesToRadians(90.0)), 0.2f, extPos.z() + 2 * sin(af + osg::DegreesToRadians(90.0))));
	vertices->push_back(osg::Vec3(extPos.x() + 2 * cos(af - osg::DegreesToRadians(90.0)), 0.2f, extPos.z() + 2 * sin(af - osg::DegreesToRadians(90.0))));
	texcoords->push_back(osg::Vec2(0.0f, 50.0f));
	texcoords->push_back(osg::Vec2(1.0f, 50.0f));
	vcount += 2;


	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setVertexArray(vertices.get());
	quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
	quad->setTexCoordArray(0, texcoords.get());
	quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, vcount));

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
	int numberDurms = trackCurve->getTotalLength() / 2.0f;
	float deltaT = 1.0f / numberDurms;

	for (float t = 0; t <= 1; t += deltaT)
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

osg::Node* GraphicManager::createTrackPositiveExtension()
{
	osg::ref_ptr<osg::Group> track = new osg::Group;
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\durmienteSolo.flt.90,0,0.rot.[0,0.3,0].trans");
	
	int numberDurms = trackCurve->getTotalLength() / 2.0f;
	for (float t = 0; t <= numberDurms; t += 2)
	{
		auto pos = trackCurve->getPositiveExtension(t);
		auto rot = trackCurve->getOrientation(1);
		osg::ref_ptr<osg::PositionAttitudeTransform> transf = new osg::PositionAttitudeTransform;
		transf->setPosition(pos);
		transf->setAttitude(rot);
		transf->addChild(model);
		track->addChild(transf);
	}
	return track.release();
}

osg::Node* GraphicManager::createTrackNegativeExtension()
{
	osg::ref_ptr<osg::Group> track = new osg::Group;
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\durmienteSolo.flt.90,0,0.rot.[0,0.3,0].trans");

	int numberDurms = trackCurve->getTotalLength() / 2.0f;
	for (float t = 0; t <= numberDurms; t += 2)
	{
		auto pos = trackCurve->getNegativeExtension(t);
		auto rot = trackCurve->getOrientation(0);
		osg::ref_ptr<osg::PositionAttitudeTransform> transf = new osg::PositionAttitudeTransform;
		transf->setPosition(pos);
		transf->setAttitude(rot);
		transf->addChild(model);
		track->addChild(transf);
	}
	return track.release();
}

osg::Node* GraphicManager::createStation()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Node> anden = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\anden_con_techo\\anden_con_techo.flt.90,-90,0.rot.[1,0.85,3.5].trans");
	osg::ref_ptr<osg::PositionAttitudeTransform> transfStart = new osg::PositionAttitudeTransform;
	osg::ref_ptr<osg::PositionAttitudeTransform> transfEnd = new osg::PositionAttitudeTransform;
	double angle = 0, x = 0, y = 0, z = 0;

	auto posStart = trackCurve->getPosition(0);
	auto rotStart = trackCurve->getOrientation(0);
	transfStart->setPosition(posStart);
	transfStart->setAttitude(rotStart);
	transfStart->addChild(anden);

	auto posEnd = trackCurve->getPosition(1);
	auto rotEnd = trackCurve->getOrientation(1);
	transfEnd->setPosition(posEnd);
	transfEnd->setAttitude(rotEnd);
	transfEnd->addChild(anden);

	root->addChild(transfStart);
	root->addChild(transfEnd);
	return root.release();
}

void GraphicManager::createScene(){
	osg::ref_ptr<osg::Group> scene = new osg::Group;

	env->registerModel(/*1, */"C:\\ObjetosVarios\\EstacionDemo\\vagon.flt.90,270,0.rot.[0,-0.1,0].trans");
	osg::ref_ptr<osg::Group> sim = env->createSimulationScene();

	osg::ref_ptr<osg::Node> light = createLightSource(0, osg::Vec3(10.0f, -10.0f, -20.0f), osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	//osg::ref_ptr<osg::Node> rail = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\tramo_vias2.flt.90,90,0.rot");
	//osg::ref_ptr<osg::Node> anden = osgDB::readNodeFile("C:\\ObjetosVarios\\EstacionDemo\\anden_con_techo\\anden_con_techo.flt.90,90,0.rot.[6,0.85,-3.5].trans");	
	osg::ref_ptr<osg::Node> station = createStation();
	osg::ref_ptr<osg::Geode> floor = createFloor();
	osg::ref_ptr<osg::Geode> path = createPath();
	osg::ref_ptr<osg::Node> skydome = createSky();
	osg::ref_ptr<osg::Node> track = createTrack();
	osg::ref_ptr<osg::Node> trackExtPos = createTrackPositiveExtension();
	osg::ref_ptr<osg::Node> trackExtNeg = createTrackNegativeExtension();
	osg::ref_ptr<osg::Geode> profile1 = createProfile1();
	osg::ref_ptr<osg::Geode> profile2 = createProfile2();
	
	scene->addChild(station);
	scene->addChild(profile1);
	scene->addChild(profile2);
	scene->addChild(trackExtPos);
	scene->addChild(trackExtNeg);
	scene->addChild(track);
	scene->addChild(light);
	scene->addChild(floor);
	scene->addChild(path);
	scene->addChild(skydome);
	scene->addChild(sim);
	_sceneRoot = scene;
}

int GraphicManager::runViewer(){
	osgViewer::Viewer viewer;
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	/*camera->setViewMatrixAsLookAt(
		osg::Vec3f(0.0f, 5.0f, 0.0f),
		osg::Vec3f(0.0f, 0.0f, 0.0f),
		osg::X_AXIS);*/

	/*camera->setViewMatrix(osg::Matrixd::lookAt(
		osg::Vec3d(0.0f, 25.0f, 0.0f),
		osg::Vec3d(),
		osg::X_AXIS));*/

	osg::Vec3d eye(200.0f, -200.0f, 0.0f);
	osg::Vec3d center;
	osg::Vec3d up(-1.0f, 0.0f, .0f);// = -osg::X_AXIS;

	osg::ref_ptr<CommandController> commCrtl = new CommandController(env.get());
	//viewer.setCamera(camera.get());
	viewer.setSceneData(_sceneRoot.get());
	viewer.addEventHandler(commCrtl.get());
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.apply(new osgViewer::SingleWindow(10, 10, 1200, 900));
	env->start();

	//viewer.getCamera()->setViewMatrixAsLookAt(eye, center, osg::X_AXIS);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.getCameraManipulator()->setHomePosition(eye, center, up);
	viewer.home();
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
