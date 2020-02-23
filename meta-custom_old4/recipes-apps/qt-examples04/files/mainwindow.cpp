#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "secondwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    /*
    SecondWindow window;
    window.setModal(true);
    window.exec();
    */
    hide();
    window = new SecondWindow(this);
    window->show();
}
