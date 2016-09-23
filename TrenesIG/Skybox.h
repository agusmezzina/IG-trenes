#pragma once
#include <osg/TextureCubeMap>
#include <osg/Transform>

class Skybox : public osg::Transform
{
public:
	void setEnvironmentMap(unsigned int unit, osg::Image* posX,
		osg::Image* negX, osg::Image* posY, osg::Image* negY,
		osg::Image* posZ, osg::Image* negZ);
	bool computeLocalToWorldMatrix(osg::Matrix& matrix,
		osg::NodeVisitor* nv) const override;
	bool computeWorldToLocalMatrix(osg::Matrix& matrix,
		osg::NodeVisitor* nv) const override;
	Skybox();
	Skybox(const Skybox& copy, osg::CopyOp copyop = osg::CopyOp::SHALLOW_COPY)
		: osg::Transform(copy, copyop) {}
	META_Node(osg, Skybox);
protected:
	virtual ~Skybox();
};

