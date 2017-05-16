#include "SamplePoint.h"



SamplePoint::SamplePoint()
{
	id_ = 0;
	location_.setZero();
}

SamplePoint::SamplePoint(const SamplePoint & another)
{
	id_ = another.id();
	location_ = another.const_location();
}

SamplePoint::SamplePoint(int id, Eigen::Vector3f loc)
{
	id_ = id;
	location_ = loc;
}

SamplePoint::SamplePoint(int id, float x, float y, float z)
{
	id_ = id;
	location_[0] = x;
	location_[1] = y;
	location_[2] = z;
}


SamplePoint::~SamplePoint()
{
}

int SamplePoint::id() const
{
	return id_;
}

const Eigen::Vector3f & SamplePoint::const_location() const
{
	return location_;
}

Eigen::Vector3f & SamplePoint::location()
{
	return location_;
}
