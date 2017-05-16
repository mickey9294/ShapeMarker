/********************************************************************************
** Form generated from reading UI file 'ShapeMarker.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAPEMARKER_H
#define UI_SHAPEMARKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <myglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_ShapeMarkerClass
{
public:
    QWidget *centralWidget;
    MyGLWidget *displayGLWidget;
    QPushButton *generateButton;
    QPushButton *saveButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ShapeMarkerClass)
    {
        if (ShapeMarkerClass->objectName().isEmpty())
            ShapeMarkerClass->setObjectName(QStringLiteral("ShapeMarkerClass"));
        ShapeMarkerClass->resize(829, 714);
        centralWidget = new QWidget(ShapeMarkerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        displayGLWidget = new MyGLWidget(centralWidget);
        displayGLWidget->setObjectName(QStringLiteral("displayGLWidget"));
        displayGLWidget->setGeometry(QRect(10, 10, 801, 601));
        generateButton = new QPushButton(centralWidget);
        generateButton->setObjectName(QStringLiteral("generateButton"));
        generateButton->setGeometry(QRect(20, 620, 131, 31));
        saveButton = new QPushButton(centralWidget);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(170, 620, 121, 31));
        ShapeMarkerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ShapeMarkerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 829, 21));
        ShapeMarkerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ShapeMarkerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ShapeMarkerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ShapeMarkerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ShapeMarkerClass->setStatusBar(statusBar);

        retranslateUi(ShapeMarkerClass);

        QMetaObject::connectSlotsByName(ShapeMarkerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ShapeMarkerClass)
    {
        ShapeMarkerClass->setWindowTitle(QApplication::translate("ShapeMarkerClass", "ShapeMarker", Q_NULLPTR));
        generateButton->setText(QApplication::translate("ShapeMarkerClass", "Generate Markers", Q_NULLPTR));
        saveButton->setText(QApplication::translate("ShapeMarkerClass", "Save Environment", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ShapeMarkerClass: public Ui_ShapeMarkerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAPEMARKER_H
