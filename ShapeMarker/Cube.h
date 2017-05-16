#pragma once
#include <QtOpenGL>

#include <Eigen\Core>
#include <Eigen\Dense>
#include <vector>
#include <utils_sampling.h>

#include "SamplePoint.h"

class Cube
{
public:
	Cube();
	~Cube();

	void draw();
	Eigen::Vector3f & operator[](int index);
	Eigen::Vector3f get_vertex(int index);
	SamplePoint & get_sample(int index);
	Eigen::Vector3i &get_face(int index);

	int num_vertices() { return vertices_.size(); }
	int num_samples() { return samples_.size(); }

private:
	std::vector<Eigen::Vector3f> vertices_;
	std::vector<Eigen::Vector3i> faces_;
	std::vector<SamplePoint> samples_;

	void sample_points();
};

