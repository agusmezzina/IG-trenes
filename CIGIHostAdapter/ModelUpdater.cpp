#include "ModelUpdater.h"
#include "EntityState.h"
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <boost\algorithm\string.hpp>
#include <iostream>

using boost::asio::ip::udp;

ModelUpdater::ModelUpdater(Semaphore* sem, std::queue<DataPacket>* data)
{
	this->data = data;
	this->s = sem;
}

void ModelUpdater::run()
{
	boost::asio::io_service io_service;
	udp::socket socket(io_service, udp::endpoint(udp::v4(), 8887));

	while (true)
	{
		boost::array<unsigned char, 32768> recv_buf;
		udp::endpoint remote_endpoint;
		boost::system::error_code error;

		std::size_t len = socket.receive_from(boost::asio::buffer(recv_buf),
			remote_endpoint,
			0,
			error);
		if (error && error != boost::asio::error::message_size)
			throw boost::system::system_error(error);

		int id{ 0 };
		float x{ 0 }, y{ 0 }, z{ 0 }, vx{ 0 }, vy{ 0 }, vz{ 0 }, ax{ 0 }, ay{ 0 }, az{ 0 }, t{ 0 };

		std::string msg(reinterpret_cast<char*>(recv_buf.c_array()), len);
		if (msg.back() == '\f'){
			msg.pop_back();
			std::vector<std::string> fields;
			boost::split(fields, msg, boost::is_any_of(";"));
			if (fields.size() == 11)
			{
				id = std::stoi(fields[0]);
				x = std::stod(fields[1]);
				y = std::stod(fields[2]);
				z = std::stod(fields[3]);
				vx = std::stod(fields[4]);
				vy = std::stod(fields[5]);
				vz = std::stod(fields[6]);
				ax = std::stod(fields[7]);
				ay = std::stod(fields[8]);
				az = std::stod(fields[9]);
				t = std::stod(fields[10]);
			}
			DataPacket state(id, x, y, z, vx, vy, vz, ax, ay, az, t);
			data->push(state);
			s->notify();
		}
		//std::cout << x << std::endl;
		//std::cout << y << std::endl;
		//std::cout << z << std::endl;
	}
	socket.close();
}

ModelUpdater::~ModelUpdater()
{
}
