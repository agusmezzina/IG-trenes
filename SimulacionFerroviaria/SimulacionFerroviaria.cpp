// SimulacionFerroviaria.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include "NetworkManager.h"
#include "GraphicManager.h"
#include "DataPacket.h"
#include <vector>

//using boost::asio::ip::udp;

int _tmain(int argc, _TCHAR* argv[])
{
	NetworkManager netmng;
	GraphicManager graphmng;

	std::cout << "Esperando entrada de datos..." << std::endl;
	auto data = netmng.readSimulationData("127.0.0.1", "8888", 500);
	std::cout << "Datos recibidos." << std::endl;
	for (auto const& value : data) {
		graphmng.addKeyFrame(value.getX(), value.getY(), value.getZ(), value.getT());
		//std::cout << "(" << value.getX() << "; " << value.getT() << ")" << std::endl;
	}
	return graphmng.runScene();
	/*try
	{
		boost::asio::io_service io_service;
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "127.0.0.1", "8888");
		udp::endpoint sender_endpoint = *resolver.resolve(query);

		udp::socket socket(io_service);
		socket.open(udp::v4());
		socket.bind(sender_endpoint);

		boost::array<char, 128> recv_buf;
		int i = 0;
		std::cout << "Esperando datos" << std::endl;
		while (i < 500){
			size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
			std::cout.write(recv_buf.data(), len);
			i++;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}*/

	/*char q;
	std::cin >> q;
	return 0;*/
}

