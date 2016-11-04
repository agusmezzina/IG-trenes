#include "stdafx.h"
#include "CommandController.h"
#include <iostream>

bool CommandController::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa){
	if (!env) return false;
	bool res = true;

	switch (ea.getEventType())
	{
	case (osgGA::GUIEventAdapter::KEYDOWN) :
		switch (ea.getKey())
		{
		case 'o':
			std::cout << "Smoothness = " << env->addSmoothness() << std::endl;
			break;
		case 'p':
			std::cout << "Smoothness = " << env->decreaseSmoothness() << std::endl;
			break;
		case 'u':
			res = env->toggleDR();
			if (res)
				std::cout << "Dead Reckoning = ON" << std::endl;
			else
				std::cout << "Dead Reckoning = OFF" << res << std::endl;
			break;
		case 'i':
			res = env->togglePredictionMethod();
			if (res)
				std::cout << "Prediction = QUADRATIC" << std::endl;
			else
				std::cout << "Prediction = LINEAR" << res << std::endl;
			break;
		default:
			break;
		}
	default:
		break;
	}
}

CommandController::CommandController(osgCigi::CigiSimulationEnvironment* environment)
{
	this->env = environment;
}


CommandController::~CommandController()
{
}
