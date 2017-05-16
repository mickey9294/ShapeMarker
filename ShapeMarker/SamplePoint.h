#pragma once
#include <Eigen\Core>

class SamplePoint
{
public:
	SamplePoint();
	SamplePoint(const SamplePoint &another);
	SamplePoint(int id, Eigen::Vector3f loc);
	SamplePoint(int id, float x, float y, float z);
	~SamplePoint();

	int id() const;
	const Eigen::Vector3f & const_location() const;
	Eigen::Vector3f & location();

private:
	Eigen::Vector3f location_;
	int id_;
};

