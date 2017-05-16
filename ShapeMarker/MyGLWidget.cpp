#include "MyGLWidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m = 7;
	m_model.reset(new Cube());
	m_xRot = 0;
	m_yRot = 0;
	m_zRot = 0;
	translation_.setZero();
	frame_no_ = 0;

	m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
	if (m_transparent)
		setAttribute(Qt::WA_TranslucentBackground);

	setFocusPolicy(Qt::StrongFocus);
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::setModel(std::shared_ptr<Cube> model)
{
	m_model = model;

	update();
}

void MyGLWidget::generate_markers()
{
	int num_pixels = m_model->num_samples() + 7;
	std::vector<Pixel> pixels(num_pixels);
	int pix_idx = 0;
	/* 7 corner vertices of the box */
	for (int i = 0; i < 8; i++)
	{
		if (i == 2)
			continue;

		Pixel pix = get_projected_pixel(m_model->operator[](i));
		pixels[pix_idx++] = pix;
	}

	for (int i = 0; i < m_model->num_samples(); i++)
	{
		Pixel pix = get_projected_pixel(m_model->get_sample(i).location());
		pixels[pix_idx++] = pix;
	}
	
	/* Save the marker pixels */
	std::string save_path = "../matched_pixels_" + std::to_string(frame_no_) + ".txt";
	std::ofstream out(save_path.c_str());

	if (out.is_open())
	{
		for (std::vector<Pixel>::iterator it = pixels.begin(); it != pixels.end(); it++)
		{
			out << it->x << " " << it->y << std::endl;
		}

		out.close();
	}

	/* Save the image */
	std::string img_save_path = "../image_" + std::to_string(frame_no_++);
	slotSnapshot(img_save_path);

	/* Save the focal length */
	float focal_length_in_pixels;
	float nRange = 3.0 * std::tan(25.0 * M_PI / 180.0);
	focal_length_in_pixels = 3.0 * height() / (2 * nRange);
	out.open("../focal_length.txt");
	if(out.is_open())
	{
		out << focal_length_in_pixels << std::endl;
		out.close();
	}
}

void MyGLWidget::save_environment()
{
	std::ofstream out("D:\\Projects\\reconstruct_from_2d\\ReconstructFrom2D\\state.txt");
	if (!out.is_open())
		return;

	out << 14 << " " << 21 << " " << 9 << " " << 3 << std::endl;

	/* postions of 8 corner vertices */
	std::vector<Pixel> pixels(8);
	int pix_idx = 0;

	for (int i = 0; i < 15; i++)
	{
		if (i == 2)
			continue;

		Pixel pix = get_projected_pixel(m_model->operator[](i));
		out << pix.x << " " << pix.y << std::endl;
	}

	/* 12 edges */
	std::vector<std::pair<int, int>> edges(21);
	edges[0] = std::pair<int, int>(0, 1);
	edges[1] = std::pair<int, int>(1, 4);
	edges[2] = std::pair<int, int>(3, 4);
	edges[3] = std::pair<int, int>(0, 3);
	edges[4] = std::pair<int, int>(0, 2);
	edges[5] = std::pair<int, int>(2, 6);
	edges[6] = std::pair<int, int>(3, 6);
	edges[7] = std::pair<int, int>(5, 6);
	edges[8] = std::pair<int, int>(4, 5);
	edges[9] = std::pair<int, int>(2, 9);
	edges[10] = std::pair<int, int>(7, 9);
	edges[11] = std::pair<int, int>(7, 8);
	edges[12] = std::pair<int, int>(1, 8);
	edges[13] = std::pair<int, int>(8, 11);
	edges[14] = std::pair<int, int>(7, 10);
	edges[15] = std::pair<int, int>(9, 13);
	edges[16] = std::pair<int, int>(5, 12);
	edges[17] = std::pair<int, int>(10, 13);
	edges[18] = std::pair<int, int>(10, 11);
	edges[19] = std::pair<int, int>(11, 12);
	edges[20] = std::pair<int, int>(12, 13);

	for (int i = 0; i < 21; i++)
		out << edges[i].first << " " << edges[i].second << std::endl;

	/* 6 vertices circuit sequences of 6 faces */
	out << 0 << " " << 1 << " " << 4 << " " << 3 << std::endl;
	out << 3 << " " << 4 << " " << 5 << " " << 6 << std::endl;
	out << 0 << " " << 3 << " " << 6 << " " << 2 << std::endl;
	out << 7 << " " << 10 << " " << 11 << " " << 8 << std::endl;
	out << 0 << " " << 2 << " " << 9 << " " << 7 << " " << 8 << " " << 1 << std::endl;
	out << 7 << " " << 9 << " " << 13 << " " << 10 << std::endl;
	out << 2 << " " << 6 << " " << 5 << " " << 12 << " " << 13 << " " << 9 << std::endl;
	out << 10 << " " << 13 << " " << 12 << " " << 11 << std::endl;
	out << 1 << " " << 8 << " " << 11 << " " << 12 << " " << 5 << " " << 4 << std::endl;
	
	/* 3 parallel groups */
	out << 0 << " " << 2 << " " << 7 << " " << 9 << " " << 11 << " " << 18 << " " << 20 << std::endl;
	out << 4 << " " << 6 << " " << 8 << " " << 12 << " " << 10 << " " << 17 << " " << 19 << std::endl;
	out << 3 << " " << 1 << " " << 16 << " " << 5 << " " << 15 << " " << 14 << " " << 13 << std::endl;

	out.close();

	out.open("D:\\Projects\\reconstruct_from_2d\\q_gt.csv");
	if (!out.is_open())
		return;
	Eigen::IOFormat CSVFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", "\n");

	float nRange = 3.0 * std::tan(25.0 * M_PI / 180.0);
	float scale = 601.0 / (2 * nRange);

	/* 6 normal vectors of the faces */
	Eigen::Vector3f x_axis = up_.cross(view_point_);
	x_axis.normalize();
	Eigen::Vector3f y_axis = up_.normalized();
	Eigen::Vector3f z_axis = view_point_.normalized();
	Eigen::Matrix3f P;
	P << x_axis, y_axis, z_axis;

	Eigen::MatrixXf normals(3, 9);
	normals.setZero();
	normals.block<3, 3>(0, 0) = Eigen::Matrix3f::Identity();
	normals.block<3, 3>(0, 3) = Eigen::Matrix3f::Identity();
	normals.block<3, 3>(0, 6) = -Eigen::Matrix3f::Identity();
	Eigen::Matrix3f P_inv = P.inverse();
	//Eigen::Matrix3f check = P_inv * P;
	//std::cout << check << std::endl;
	normals = P_inv * normals;

	Eigen::VectorXf q(27);
	for (int i = 0; i < 9; i++)
		q.segment(3 * i, 3) = normals.col(i);

	Eigen::Vector3f v0 = m_model->get_vertex(0);
	v0 -= view_point_;
	v0 = P_inv * v0;
	v0 *= scale;
	Eigen::Vector3f n0 = q.segment(0, 3);
	float minus_d = v0.dot(n0);
	n0 /= -minus_d;
	q.segment(0, 3) = n0;

	Eigen::Vector3f n4 = q.segment(12, 3);
	minus_d = v0.dot(n4);
	n4 /= -minus_d;
	q.segment(12, 3) = n4;

	Eigen::Vector3f n2 = q.segment(6, 3);
	minus_d = v0.dot(n2);
	n2 /= -minus_d;
	q.segment(6, 3) = n2;

	Eigen::Vector3f v6 = m_model->get_vertex(6);
	v6 -= view_point_;
	v6 = P_inv * v6;
	v6 *= scale;
	Eigen::Vector3f n1 = q.segment(3, 3);
	minus_d = v6.dot(n1);
	n1 /= -minus_d;
	q.segment(3, 3) = n1;

	Eigen::Vector3f n8 = q.segment(24, 3);
	minus_d = v6.dot(n8);
	n8 /= -minus_d;
	q.segment(24, 3) = n8;

	Eigen::Vector3f n6 = q.segment(18, 3);
	minus_d = v6.dot(n6);
	n6 /= -minus_d;
	q.segment(18, 3) = n6;

	Eigen::Vector3f v11 = m_model->get_vertex(11);
	v11 -= view_point_;
	v11 = P_inv * v11;
	v11 *= scale;
	Eigen::Vector3f n3 = q.segment(9, 3);
	minus_d = v11.dot(n3);
	n3 /= -minus_d;
	q.segment(9, 3) = n3;

	Eigen::Vector3f n5 = q.segment(15, 3);
	minus_d = v11.dot(n5);
	n5 /= -minus_d;
	q.segment(15, 3) = n5;

	Eigen::Vector3f n7 = q.segment(21, 3);
	minus_d = v11.dot(n7);
	n7 /= -minus_d;
	q.segment(21, 3) = n7;

	out << q.format(CSVFormat);
	out.close();

	out.open("D:\\Projects\\reconstruct_from_2d\\x_gt.csv");
	if (!out.is_open())
		return;

	
	for (int i = 0; i < m_model->num_vertices(); i++)
	{
		Eigen::Vector3f vert = m_model->get_vertex(i);
		vert -= view_point_;
		vert = P_inv * vert;
		vert *= scale;
		out << vert.x() << "," << vert.y() << "," << vert.z() << std::endl;
	}

	out.close();

	out.open("D:\\Projects\\reconstruct_from_2d\\faces_gt.csv");
	for (int i = 0; i < 24; i++)
	{
		Eigen::Vector3i &face = m_model->get_face(i);
		out << face.x() << "," << face.y() << "," << face.z() << std::endl;
	}
	out.close();
}

void MyGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(255, 255, 255, m_transparent ? 0 : 1);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //指定混合函数
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	init_light();
}

void MyGLWidget::init_light()
{
	GLfloat white_light[] = { 0.23, 0.23, 0.23, 1.0 };

	GLfloat light_position0[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_position1[] = { 1.0, 1.0, -1.0, 0.0 };
	GLfloat light_position2[] = { 1.0, -1.0, 1.0, 0.0 };
	GLfloat light_position3[] = { 1.0, -1.0, -1.0, 0.0 };
	GLfloat light_position4[] = { -1.0, 1.0, 1.0, 0.0 };
	GLfloat light_position5[] = { -1.0, -1.0, 1.0, 0.0 };
	GLfloat light_position6[] = { -1.0, 1.0, -1.0, 0.0 };
	GLfloat light_position7[] = { -1.0, -1.0, -1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0); glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1); glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light); glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2); glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light); glLightfv(GL_LIGHT2, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3); glLightfv(GL_LIGHT3, GL_DIFFUSE, white_light); glLightfv(GL_LIGHT3, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT4, GL_POSITION, light_position4); glLightfv(GL_LIGHT4, GL_DIFFUSE, white_light); glLightfv(GL_LIGHT4, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT5, GL_POSITION, light_position5); glLightfv(GL_LIGHT5, GL_DIFFUSE, white_light); glLightfv(GL_LIGHT5, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT6, GL_POSITION, light_position6); glLightfv(GL_LIGHT6, GL_DIFFUSE, white_light); glLightfv(GL_LIGHT6, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT7, GL_POSITION, light_position7); glLightfv(GL_LIGHT7, GL_DIFFUSE, white_light); glLightfv(GL_LIGHT7, GL_SPECULAR, white_light);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	//glEnable(GL_LIGHT5);
	//glEnable(GL_LIGHT6);
	//glEnable(GL_LIGHT7);
}

void MyGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	//glEnable(GL_DEPTH_TEST);
	//if (m_model != NULL)
	draw();
	glFlush();
	glFinish();
}

void MyGLWidget::draw()
{
	glPushMatrix();

	GLfloat no_mat[4] = { 0.0, 0.0, 0, 1 };
	GLfloat mat_diffuse[4] = { 0, 0, 0, 1 };		//r±íÊ¾´óÖµ£¬b±íÊ¾Ð¡Öµ
	GLfloat mat_specular[4] = { 0.5, 0.5, 0.5, 1 };
	float no_shininess[4] = { 1, 1, 1, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	Eigen::Matrix3f rotate_mat;
	rotate_mat = Eigen::AngleAxisf(deg2rad(m_xRot), Eigen::Vector3f::UnitX())
		* Eigen::AngleAxisf(deg2rad(m_yRot), Eigen::Vector3f::UnitY())
		* Eigen::AngleAxisf(deg2rad(m_zRot), Eigen::Vector3f::UnitZ());

	view_point_.setZero();
	view_point_[2] = m;
	view_point_ = rotate_mat *view_point_;
	up_ = view_point_.cross(Eigen::Vector3f::UnitY()).cross(view_point_);
	if (up_.dot(Eigen::Vector3f::UnitY()) < 0)
		up_ = -up_;
	gluLookAt(view_point_.x(), view_point_.y(), view_point_.z(), 0.0, 0.0, 0.0, up_[0], up_[1], up_[2]);

	//Eigen::Vector3d centroid;

	/*centroid.setZero();
	glTranslatef(centroid.x(), centroid.y(), centroid.z());
	glRotatef(m_xRot, 1.0, 0.0, 0.0);
	glRotatef(m_yRot, 0.0, 1.0, 0.0);
	glRotatef(m_zRot, 0.0, 0.0, 1.0);
	glTranslatef(-centroid.x(), -centroid.y(), -centroid.z());*/

	//glTranslatef(translation_.x(), translation_.y(), translation_.z());

	if (m_model != NULL)
		m_model->draw();

	glPopMatrix();
}

void MyGLWidget::resizeGL(int width, int height)
{
	GLfloat nRange = 3.0 * std::tan(25.0 * M_PI / 180.0);
	if (height == 0) {    // Prevent A Divide By Zero By  
		height = 1;    // Making Height Equal One  
	}
	glViewport(0, 0, width, height);    // Reset The Current Viewport  
	glMatrixMode(GL_PROJECTION);       // Select The Projection Matrix  
	glLoadIdentity();                  // Reset The Projection Matrix  

	if (width <= height)
		glFrustum(-nRange, nRange, -nRange*height / width, nRange*height / width, 3.0, 10.0);
	else
		glFrustum(-nRange*width / height, nRange*width / height, -nRange, nRange, 3.0, 10.0);
	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix  
	glLoadIdentity();
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
	m_lastPos = event->pos();
	clickEvent = true;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - m_lastPos.x();
	int dy = event->y() - m_lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(m_xRot + 3 * dy);
		setYRotation(m_yRot + 3 * dx);
	}
	else if (event->buttons() & Qt::RightButton) {
		setXRotation(m_xRot + 3 * dy);
		setZRotation(m_zRot + 3 * dx);
	}
	else if (event->buttons() & Qt::MiddleButton)
	{
		translation_[0] += 0.001 * dx;
		translation_[1] += 0.001 * dy;
		update();
	}
	m_lastPos = event->pos();
}

void MyGLWidget::wheelEvent(QWheelEvent *e)
{
	if (e->delta()>0)
		m -= 0.1f;
	if (e->delta()<0)
		m += 0.1f;
	if (m < 0.1)
		m = 0.1;
	update();
}

static void qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}

void MyGLWidget::setXRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_xRot) {
		m_xRot = angle;
		update();
	}
}

void MyGLWidget::setYRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_yRot) {
		m_yRot = angle;
		update();
	}
}

void MyGLWidget::setZRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_zRot) {
		m_zRot = angle;
		update();
	}
}

//void MyGLWidget::rotateModel(float angle, float x, float y, float z)
//{
//	if (m_model != NULL)
//	{
//		m_model->rotate(angle, x, y, z);
//		update();
//	}
//}

void MyGLWidget::slotSnapshot(const std::string _filename)
{
	QImage image;
	size_t w(width()), h(height());
	GLenum buffer(GL_BACK);

	try
	{
		image = QImage(w, h, QImage::Format_RGB32);

		std::vector<GLubyte> fbuffer(3 * w*h);

		qApp->processEvents();
		makeCurrent();
		update();
		glFinish();

		glReadBuffer(buffer);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		paintGL();
		glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, &fbuffer[0]);

		unsigned int x, y, offset;

		for (y = 0; y<h; ++y) {
			for (x = 0; x<w; ++x) {
				offset = 3 * (y*w + x);
				image.setPixel(x, h - y - 1, qRgb(fbuffer[offset],
					fbuffer[offset + 1],
					fbuffer[offset + 2]));
			}
		}


		//		QString name = "snapshot-";
		//#if defined(_MSC_VER)
		//		{
		//			std::stringstream s;
		//			QDateTime         dt = QDateTime::currentDateTime();
		//			s << dt.date().year()
		//				<< std::setw(2) << std::setfill('0') << dt.date().month()
		//				<< std::setw(2) << std::setfill('0') << dt.date().day()
		//				<< std::setw(2) << std::setfill('0') << dt.time().hour()
		//				<< std::setw(2) << std::setfill('0') << dt.time().minute()
		//				<< std::setw(2) << std::setfill('0') << dt.time().second();
		//			name += QString(s.str().c_str());
		//		}
		//#else
		//		name += QDateTime::currentDateTime().toString("yyMMddhhmmss");
		//#endif
		QString name = _filename.c_str();
		name += ".png";

		image.save(name, "PNG");
	}
	catch (std::bad_alloc&)
	{
		qWarning("Mem Alloc Error");
	}
}

float MyGLWidget::deg2rad(float angle)
{
	return angle / 180 * PI;;
}

Pixel MyGLWidget::get_projected_pixel(Eigen::Vector3f & vert)
{
	int width = this->width();
	int height = this->height();
	float nRange = 3.0 * std::tan(25.0 * M_PI / 180.0);
	float real_width = 2 * nRange*width / (float)height;
	float real_height = 2 * nRange;

	Eigen::Vector3f vec = vert - view_point_;

	float dist = std::abs(vec.dot(view_point_.normalized()));

	float y = vec.dot(up_.normalized());

	Eigen::Vector3f hori_norm = up_.cross(-view_point_);
	float x = vec.dot(hori_norm.normalized());

	x = 3.0 * x / dist;
	y = 3.0 * y / dist;

	float real_x = real_width / 2 - x;
	float real_y = real_height / 2 - y;

	float pixels_x = real_x / real_width * width;
	float pixels_y = real_y / real_height * height;

	return Pixel(pixels_x, pixels_y);
}
