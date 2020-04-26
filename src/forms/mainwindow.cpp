#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    widgetLog = new QWidget();
    widgetLog->setLayout(new QVBoxLayout());
    widgetLog->layout()->setAlignment(Qt::AlignTop);
    ui->scrollAreaRunLog->setWidget(widgetLog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startBenchmark()
{
    QLabel *log = new QLabel("Start benchmark");
    widgetLog->layout()->addWidget(log);
}
