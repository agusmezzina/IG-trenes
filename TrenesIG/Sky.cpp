#include "stdafx.h"
#include "Sky.h"
#include <osg/Depth>
#include <osgUtil/CullVisitor>

bool Sky::computeLocalToWorldMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const
{
	if (nv && nv->getVisitorType() ==
		osg::NodeVisitor::CULL_VISITOR)
	{
		osgUtil::CullVisitor* cv =
			static_cast<osgUtil::CullVisitor*>(nv);
		matrix.preMult(osg::Matrix::translate(cv->getEyeLocal()));
		return true;
	}
	else
		return osg::Transform::computeLocalToWorldMatrix(matrix, nv);
}

bool Sky::computeWorldToLocalMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const
{
	if (nv && nv->getVisitorType() ==
		osg::NodeVisitor::CULL_VISITOR)
	{
		osgUtil::CullVisitor* cv =
			static_cast<osgUtil::CullVisitor*>(nv);
		matrix.postMult(osg::Matrix::translate(
			-cv->getEyeLocal()));
		return true;
	}
	else
		return osg::Transform::computeWorldToLocalMatrix(matrix, nv);
}

Sky::Sky()
{
	setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	setCullingActive(false);
	osg::StateSet* ss = getOrCreateStateSet();
	ss->setAttributeAndModes(new osg::Depth(
		osg::Depth::LEQUAL, 1.0f, 1.0f));
	ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	ss->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
	ss->setRenderBinDetails(5, "RenderBin");
}


Sky::~Sky()
{
}
