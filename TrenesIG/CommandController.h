#pragma once
#include <osgGA/GUIEventHandler>
#include "CigiSimulationEnvironment.h"

class CommandController : public osgGA::GUIEventHandler
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	CommandController(osgCigi::CigiSimulationEnvironment* environment);
	virtual ~CommandController();
private:
	osgCigi::CigiSimulationEnvironment* env;
};

