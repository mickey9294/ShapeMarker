#include "ShapeMarker.h"

ShapeMarker::ShapeMarker(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.generateButton, SIGNAL(clicked()), this, SLOT(generate_matched_pixels()));
	connect(ui.saveButton, SIGNAL(clicked()), ui.displayGLWidget, SLOT(save_environment()));
}

void ShapeMarker::generate_matched_pixels()
{
	ui.displayGLWidget->generate_markers();
}