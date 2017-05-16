#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShapeMarker.h"

class ShapeMarker : public QMainWindow
{
	Q_OBJECT

public:
	ShapeMarker(QWidget *parent = Q_NULLPTR);

	public slots:
	void generate_matched_pixels();

private:
	Ui::ShapeMarkerClass ui;
};
