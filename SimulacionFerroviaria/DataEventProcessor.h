#pragma once

#include "CigiBaseEventProcessor.h"
#include "CigiEntityCtrlV3_3.h"
#include "SceneData.h"
#include <osg\MatrixTransform>

class DataEventProcessor : public CigiBaseEventProcessor
{
public:
	DataEventProcessor(SceneData* data);
	virtual ~DataEventProcessor();
	virtual void OnPacketReceived(CigiBasePacket *Packet);
	void setOriginPacket(CigiEntityCtrlV3_3 *TPcktIn) { TPckt = TPcktIn; }

protected:
	CigiEntityCtrlV3_3 *TPckt;
	SceneData* data;
};