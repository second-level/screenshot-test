#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>
#include <QGuiApplication>
#include <QWindow>
#include <QPixmap>
#include <QDesktopWidget>
#include <QDBusInterface>
#include <QProcess>

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
//    QProcess process;
//    QString cmd = "/home/dmytro/Desktop/screenload-sg";
//    // declare a StringList
//    QStringList args;
//    // Append argument "-a" to stringList
//    args << "-f /home/dmytro/Desktop/test1.png";
//     // start process
//    process.start(cmd, args);
//    process.waitForFinished(2000);
    QProcess().execute("/home/dmytro/Desktop/screenload-sg -f /home/dmytro/Desktop/test1.png");

    //process.close();

//    QScreen *screen = QGuiApplication::primaryScreen();

//    if (const QWindow *window = windowHandle())
//        screen = window->screen();

//    if (!screen)
//        return;

//    QApplication::beep();

//    QPixmap originalPixmap = screen->grabWindow(QApplication::desktop()->internalWinId(), 0, 0, 200, 200);
//    ui->label->setPixmap(originalPixmap);

//    QDBusInterface screenshotInterface(
//      QStringLiteral("org.gnome.Shell.Screenshot"),
//      QStringLiteral("/org/gnome/Shell/Screenshot"),
//      QStringLiteral("org.gnome.Shell.Screenshot"));

//    screenshotInterface.call(QDBus::BlockWithGui, "Screenshot", "(bbs)", false,  true, "/home/dmytro/.cache/gnome-screenshot/scr-1936288449.png");
}

