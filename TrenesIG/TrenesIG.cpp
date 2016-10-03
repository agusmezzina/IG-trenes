// SimulacionFerroviaria.cpp: define el punto de entrada de la aplicación de consola.
//

//#include "stdafx.h"
#include <iostream>
#include "NetworkManager.h"
#include "GraphicManager.h"
#include "DataPacket.h"
#include <vector>
#include "CigiNetworkManager.h"
#include "SceneData.h"
#include "World.h"
#include "DeadReckoning.h"

//using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
	//NetworkManager netmng;
	//SceneData data;
	World data;
	World ghost;
	//ghost.updateEntityVelocity(1, osg::Vec3f(0.0f, 1.0f, 0.0f));
	DeadReckoning dr(&data, &ghost);
	GraphicManager graphmng(&data, &ghost);

	//std::cout << "Esperando entrada de datos..." << std::endl;
	//auto data = netmng.readSimulationData("127.0.0.1", "8888", 500);
	//std::cout << "Datos recibidos." << std::endl;
	//for (auto const& value : data) {
	//	graphmng.addKeyFrame(value.getX(), value.getY(), value.getZ(), value.getT());
	//	//std::cout << "(" << value.getX() << "; " << value.getT() << ")" << std::endl;
	//}
	//return graphmng.runScene();

	CigiNetworkManager mng(&data);
	mng.startThread();
	graphmng.runScene();
	//mng.cancel();
	return 0;
}

