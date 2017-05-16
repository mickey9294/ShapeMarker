#include "Cube.h"



Cube::Cube()
{
	vertices_.reserve(15);
	vertices_.push_back(Eigen::Vector3f(0.7, -1.0, 0.5));
	vertices_.push_back(Eigen::Vector3f(0.7, -1.0, -0.5));
	vertices_.push_back(Eigen::Vector3f(-0.7, -1.0, -0.5));
	vertices_.push_back(Eigen::Vector3f(-0.7, -1.0, 0.5));
	vertices_.push_back(Eigen::Vector3f(0.7, 1.0, 0.5));
	vertices_.push_back(Eigen::Vector3f(0.7, 1.0, -0.5));
	vertices_.push_back(Eigen::Vector3f(-0.7, 1.0, -0.5));
	vertices_.push_back(Eigen::Vector3f(-0.7, 1.0, 0.5));
	vertices_.push_back(Eigen::Vector3f(1.2, -1.0, 1.0));
	vertices_.push_back(Eigen::Vector3f(1.2, -1.0, -0.5));
	vertices_.push_back(Eigen::Vector3f(-0.7, -1.0, 1.0));
	vertices_.push_back(Eigen::Vector3f(1.2, -2.0, 1.0));
	vertices_.push_back(Eigen::Vector3f(1.2, -2.0, -0.5));
	vertices_.push_back(Eigen::Vector3f(-0.7, -2.0, -0.5));
	vertices_.push_back(Eigen::Vector3f(-0.7, -2.0, 1.0));
	faces_.reserve(24);
	faces_.push_back(Eigen::Vector3i(0, 3, 1));
	faces_.push_back(Eigen::Vector3i(1, 3, 2));
	faces_.push_back(Eigen::Vector3i(1, 5, 4));
	faces_.push_back(Eigen::Vector3i(1, 4, 0));
	faces_.push_back(Eigen::Vector3i(1, 5, 6));
	faces_.push_back(Eigen::Vector3i(1, 6, 2));
	faces_.push_back(Eigen::Vector3i(4, 5, 6));
	faces_.push_back(Eigen::Vector3i(4, 6, 7));
	faces_.push_back(Eigen::Vector3i(0, 4, 7));
	faces_.push_back(Eigen::Vector3i(0, 7, 3));
	faces_.push_back(Eigen::Vector3i(3, 7, 6));
	faces_.push_back(Eigen::Vector3i(3, 6, 2));

	faces_.push_back(Eigen::Vector3i(8,9,2));
	faces_.push_back(Eigen::Vector3i(8,2,10));
	faces_.push_back(Eigen::Vector3i(11,8,10));
	faces_.push_back(Eigen::Vector3i(11, 10, 14));
	faces_.push_back(Eigen::Vector3i(11,12,9));
	faces_.push_back(Eigen::Vector3i(11,9,8));
	faces_.push_back(Eigen::Vector3i(2,9,12));
	faces_.push_back(Eigen::Vector3i(2,12,13));
	faces_.push_back(Eigen::Vector3i(2,13,10));
	faces_.push_back(Eigen::Vector3i(13,14,10));
	faces_.push_back(Eigen::Vector3i(11,14,12));
	faces_.push_back(Eigen::Vector3i(14,13,12));

	sample_points();
}


Cube::~Cube()
{
}

void Cube::sample_points()
{
	using namespace Utils_sampling;
	std::vector<Utils_sampling::Vec3> verts(8);
	std::vector<Vec3> nors(8);
	std::vector<int> tris(3 * 6);

	int vert_idx = 0;
	for (int i = 0; i < 8; i++)
	{
		Vec3 vert(vertices_[i][0], vertices_[i][1], vertices_[i][2]);
		verts[vert_idx] = vert;
		nors[vert_idx++] = Vec3(0, 0, 0);
	}

	int tri_idx[] = { 0,4,7,0,7,3,0,1,5,0,5,4,4,5,6,4,6,7 };
	int face_idx = 0;
	for (int idx : tri_idx)
	{
		tris[face_idx++] = idx;
	}

	std::vector<Vec3> samples_pos;
	std::vector<Vec3> samples_nor;

	poisson_disk(0, 500, verts, nors, tris, samples_pos, samples_nor);

	samples_.resize(samples_pos.size());
	int samp_idx = 0;
	for (std::vector<Vec3>::iterator it = samples_pos.begin(); it != samples_pos.end(); ++it)
	{
		Vec3 point = *it;
		SamplePoint sample_point(samp_idx, point.x, point.y, point.z);
		samples_[samp_idx++] = sample_point;
	}
}

void Cube::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(2.0);

	glColor3f(1.0, 0.0, 0);
	glBegin(GL_TRIANGLES);

	for (std::vector<Eigen::Vector3i>::iterator face_it = faces_.begin(); face_it != faces_.end(); ++face_it)
	{
		Eigen::Vector3f v0 = vertices_[face_it->x()];
		Eigen::Vector3f v1 = vertices_[face_it->y()];
		Eigen::Vector3f v2 = vertices_[face_it->z()];

		Eigen::Vector3f vec1 = v1 - v0;
		Eigen::Vector3f vec2 = v2 - v0;
		Eigen::Vector3f normal = vec1.cross(vec2);

		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3f(v0[0], v0[1], v0[2]);
		glVertex3f(v1[0], v1[1], v1[2]);
		glVertex3f(v2[0], v2[1], v2[2]);
	}

	glEnd();

	//glPointSize(3.0);
	//glColor3f(0, 255, 0);
	//glBegin(GL_POINTS);
	//glVertex3f(-0.7, 0.0, 0.5);
	//glEnd();

	//glBegin(GL_LINES);
	//glVertex3f(-0.7, -1.0, -0.5);
	//glVertex3f(0.7, -1.0, 0.5);
	//glVertex3f(0.7, -1.0, -0.5);
	//glVertex3f(-0.7, -1.0, 0.5);
	//glEnd();

	glDisable(GL_BLEND);
}

Eigen::Vector3f & Cube::operator[](int index)
{
	return vertices_[index];
}

Eigen::Vector3f Cube::get_vertex(int index)
{
	return vertices_.at(index);
}

SamplePoint & Cube::get_sample(int index)
{
	return samples_[index];
}

Eigen::Vector3i & Cube::get_face(int index)
{
	return faces_[index];
}
