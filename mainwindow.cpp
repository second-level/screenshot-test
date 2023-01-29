#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>
#include <QGuiApplication>
#include <QWindow>
#include <QPixmap>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QScreen *screen = QGuiApplication::primaryScreen();

//    if (const QWindow *window = windowHandle())
//        screen = window->screen();

//    if (!screen)
//        return;

    QApplication::beep();

    QPixmap originalPixmap = screen->grabWindow(QApplication::desktop()->internalWinId(), 0, 0, 200, 200);
    ui->label->setPixmap(originalPixmap);
}

