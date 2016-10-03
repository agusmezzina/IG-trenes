#pragma once
#include <osg/Transform>

class Sky : public osg::Transform
{
public:
	bool computeLocalToWorldMatrix(osg::Matrix& matrix,
		osg::NodeVisitor* nv) const override;
	bool computeWorldToLocalMatrix(osg::Matrix& matrix,
		osg::NodeVisitor* nv) const override;
	Sky();
	Sky(const Sky& copy, osg::CopyOp copyop = osg::CopyOp::SHALLOW_COPY)
		: osg::Transform(copy, copyop) {}
	META_Node(osg, Sky);
	virtual ~Sky();
};

