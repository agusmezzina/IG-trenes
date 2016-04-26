#pragma once

#include "CigiBaseEventProcessor.h"
#include "CigiEntityCtrlV3_3.h"
#include <osg\MatrixTransform>

class DataEventProcessor : CigiBaseEventProcessor
{
public:
	DataEventProcessor();
	virtual ~DataEventProcessor();
	virtual void OnPacketReceived(CigiBasePacket *Packet);
	void setOriginPacket(CigiEntityCtrlV3_3 *TPcktIn) { TPckt = TPcktIn; }
	osg::MatrixTransform* getMatrixTransform() { return transform.get(); }

protected:
	CigiEntityCtrlV3_3 *TPckt;
	osg::ref_ptr<osg::MatrixTransform> transform;
};