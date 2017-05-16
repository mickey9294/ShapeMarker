#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QtOpenGL>

#include <GL/glu.h>
#include <Eigen\Core>
#include <Eigen/Geometry>
#include <Windows.h>
#include "Cube.h"
#include "Pixel.h"
#include <memory>
#include <fstream>
#include <iostream>
#include <cmath>

#ifndef PI
#define PI 3.1415926536
#endif

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MyGLWidget(QWidget *parent);
	~MyGLWidget();

	int xRotation() const { return m_xRot; }
	int yRotation() const { return m_yRot; }
	int zRotation() const { return m_zRot; }

	//void resetView();
	std::shared_ptr<Cube> getModel() { return m_model; }

	public slots:
	void slotSnapshot(const std::string _filename);
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void setModel(std::shared_ptr<Cube> model);
	//void rotateModel(float angle, float x, float y, float z);
	void generate_markers();
	void save_environment();

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
	void addDebugText(QString text);

protected:
	void initializeGL();
	void paintGL();
	void init_light();
	void resizeGL(int w, int h);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	//void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *e);

	private slots:
	void draw();

private:
	GLfloat m_xRot;
	GLfloat m_yRot;
	GLfloat m_zRot;
	bool m_transparent;
	Eigen::Vector3f translation_;
	float m;
	std::shared_ptr<Cube> m_model;
	Eigen::Vector3f view_point_;
	Eigen::Vector3f up_;

	QPoint m_lastPos;
	bool clickEvent;

	int frame_no_;

	//void normalizeAngle(int &angle);
	float deg2rad(float angle);
	Pixel get_projected_pixel(Eigen::Vector3f & point);
};

#endif