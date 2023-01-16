//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)	: QMainWindow(parent), ui(new Ui::MainWindowClass)
{
	ui->setupUi(this);
	ui->glwidget->setFocusPolicy(Qt::StrongFocus);
    QObject::connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setNum(int)));
    QObject::connect(ui->pushButton, &QPushButton::released, ui->glwidget, &GLWidget::handleButton);
}

MainWindow::~MainWindow()
{
	delete ui;
}

