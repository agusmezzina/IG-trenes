#include "ModelUpdater.h"
#include "EntityState.h"
#include <boost\algorithm\string.hpp>
#include <boost\bind.hpp>
#include <iostream>

ModelUpdater::ModelUpdater(Semaphore* sem, std::queue<DataPacket>* data)
{
	this->data = data;
	this->s = sem;
	socket = std::make_unique<udp::socket>(io_service, udp::endpoint(udp::v4(), 8887));
	recv_packet();
}

DataPacket ModelUpdater::readData(std::string message)
{
	bool lastBit = false;
	int id{ 0 }, last{ 0 };
	float x{ 0 }, y{ 0 }, z{ 0 }, vx{ 0 }, vy{ 0 }, vz{ 0 }, ax{ 0 }, ay{ 0 }, az{ 0 }/*, alpha{ 0 }, alphaV{ 0 }*/, t{ 0 };

	if (message.back() == '\f'){
		message.pop_back();
		std::vector<std::string> fields;
		boost::split(fields, message, boost::is_any_of(";"));
		if (fields.size() == 12)
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
			//alpha = std::stod(fields[10]);
			//alphaV = std::stod(fields[11]);
			t = std::stod(fields[10]);
			last = std::stoi(fields[11]);
			if (last == 1)
				lastBit = true;
		}
	}
	return DataPacket(id, x, y, z, vx, vy, vz, ax, ay, az, 0, 0, t, lastBit);
}

void ModelUpdater::enqueueData(DataPacket p)
{
	data->push(p);
	s->notify();
}

void ModelUpdater::handle_receive(const boost::system::error_code& error, std::size_t size)
{
	bool last = false;

	if (error && error != boost::asio::error::message_size)
		throw boost::system::system_error(error);
	if (size != 0)
	{
		std::string msg(reinterpret_cast<char*>(recv_buf.c_array()), size);
		auto data = readData(msg);
		last = data.getLast();
		enqueueData(data);
	}
	recv_packet();
}

void ModelUpdater::recv_packet()
{
	socket->async_receive_from(boost::asio::buffer(recv_buf),
		remote_endpoint,
		boost::bind(&ModelUpdater::handle_receive,
		this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void ModelUpdater::run(std::atomic_bool& quit)
{
	//while (!quit)
	io_service.run();
	s->notify();
	socket->close();
}

void ModelUpdater::stop()
{
	io_service.stop();
}

ModelUpdater::~ModelUpdater()
{
}
