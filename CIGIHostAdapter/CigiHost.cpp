//#include <windows.h>
#include <iostream>
#include <future>
#include <chrono>
#include "CigiHost.h"
#define BUFFER_SIZE 32768

CigiHost::CigiHost(World* data, World* ghost, Semaphore* sem, std::queue<DataPacket>* rawData) : io_service(), socket(io_service)
{
	this->data = data;
	this->ghost = ghost;
	this->s = sem;
	this->rawData = rawData;
	cigi = std::make_unique<CigiPacker>();
	dr = std::make_unique<DeadReckoning>(data, ghost);
	clock = std::make_unique<RealTimeClock>();
	dataFile.open("dataHost.csv");
	log.open("logHost.txt");
	prevSimulationTime = 0;
	last = false;
	usingDR = true;
	quadratic = true;
	latency = 300;
	//compensationTime = 0;
}

void CigiHost::setupNetwork(const std::string& ip, const std::string& port)
{
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), ip, port);
	receiver_endpoint = *resolver.resolve(query);
	socket.open(udp::v4());
}

void CigiHost::changeThreshold(float thresh)
{
	dr->setThreshold(thresh);
}

bool CigiHost::updateModelFromNetwork()
{
	s->wait();
	prevSimulationTime = simulationTime;
	if (!rawData->empty())
	{
		auto lastData = rawData->front();
		data->updateEntityPosition(lastData.getID(), osg::Vec3f(lastData.getX(), lastData.getY(), lastData.getZ()));
		data->updateEntityVelocity(lastData.getID(), osg::Vec3f(lastData.getVx(), lastData.getVy(), lastData.getVz()));
		data->updateEntityOrientation(lastData.getID(), osg::Vec3f(lastData.getAlpha(), 0.0f, 0.0f));
		data->updateEntityAngularVelocity(lastData.getID(), osg::Vec3f(lastData.getAlphaV(), 0.0f, 0.0f));
		data->updateEntityAcceleration(lastData.getID(), osg::Vec3f(lastData.getAx(), lastData.getAy(), lastData.getAz()));
		last = lastData.getLast();
		simulationTime = lastData.getTime();
		rawData->pop();
		return true;
	}
	return false;
}

void CigiHost::sendCigiPacket()
{

}

void CigiHost::changeLatency(int latency)
{
	if (latency >= 0)
		this->latency = latency;
}

bool CigiHost::toggleDR()
{
	usingDR = !usingDR;
	return usingDR;
}

bool CigiHost::togglePredictionMethod()
{
	quadratic = !quadratic;
	return quadratic;
}

void CigiHost::run(std::atomic_bool& quit)
{
	bool first = true;
	try
	{
		unsigned char* outBuffer;
		int outBufferSize = 0;
		setupNetwork("127.0.0.1", "8888");
		while (!quit)
		{
			if (updateModelFromNetwork())
			{
				if (first)
					clock->init();

				auto deltaSimTime = simulationTime - prevSimulationTime;

				auto entity = data->getEntity(1);
				auto p = entity.getPosition();

				auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
				auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
				dataFile << millis % 100000 << ";" << p.x() << "; " << p.z() << std::endl;

				if (quadratic)
					dr->secondOrderUpdateGhost(1, deltaSimTime);
				else
					dr->firstOrderUpdateGhost(1, deltaSimTime);

				bool sendUpdate = true;
				if (usingDR)
					sendUpdate = dr->isThresholdViolated(1) || first || last;

				if (first)
					first = false;

				if (last)
					first = true;

				if (sendUpdate){
					clock->sync(simulationTime);
					auto v = entity.getVelocity();
					auto a = entity.getAcceleration();
					auto pg = ghost->getEntity(1).getPosition();
					log << "Correcting Time = " << simulationTime <<
						"; Real Time = " << millis <<
						"; ghost = " << "(" << pg.x() << "; " << pg.y() << "; " << pg.z() << ") " <<
						"; model = P=(" << p.x() << "; " << p.y() << +"; " << p.z() << ") " <<
						"V=(" << v.x() << "; " << v.y() << +"; " << v.z() << ") " <<
						"A=(" << a.x() << "; " << a.y() << +"; " << a.z() << ") " << std::endl;

					dr->correctGhost(1);
					//clock->sync(simulationTime);
					auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
					long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
					cigi->packData(entity, timestamp, &outBuffer, outBufferSize);

					boost::system::error_code ignored_error;
					socket.send_to(boost::asio::buffer(outBuffer, outBufferSize),
						receiver_endpoint,
						0,
						ignored_error);

					cigi->freePacket();
				}
				else
					clock->sync(simulationTime);
			}
		}
		socket.close();
		log.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

CigiHost::~CigiHost()
{
}
