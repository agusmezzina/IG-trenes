#pragma once
#include <osg\Switch>
#include <osgGA/GUIEventHandler>

class CameraController : public osgGA::GUIEventHandler
{
private:
	osg::ref_ptr<osg::Switch> _base;
public:
	CameraController(osg::Switch* base);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	~CameraController();
};

