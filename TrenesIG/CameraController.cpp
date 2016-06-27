#include "CameraController.h"


CameraController::CameraController(osg::Switch* base) : _base(base)
{
}

bool CameraController::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa){
	if (!_base) return false;

	switch (ea.getEventType())
	{
	case (osgGA::GUIEventAdapter::KEYDOWN):
		switch (ea.getKey())
		{
		case '1':
			_base->setValue(0, true);
			_base->setValue(1, false);
			_base->setValue(2, false);
			break;
		case '2':
			_base->setValue(0, false);
			_base->setValue(1, true);
			_base->setValue(2, false);
			break;
		case '3':
			_base->setValue(0, false);
			_base->setValue(1, false);
			_base->setValue(2, true);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

CameraController::~CameraController()
{
}
