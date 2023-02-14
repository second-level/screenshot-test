#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>
#include <QGuiApplication>
#include <QWindow>
#include <QPixmap>
#include <QDesktopWidget>
#include <QDBusInterface>
#include <QProcess>
#include <QTextEdit>

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

class CommandResult
{
private:
    QString outString;
    QString errString;

public:
    explicit CommandResult(const QString &outString, const QString &errString)
    {
        this->outString = outString;
        this->errString = errString;
    }

    bool hasError()
    {
        return nullptr != errString && !errString.isEmpty();
    }

    QString getOutString()
    {
        return outString;
    }

    QString getErrString()
    {
        return errString;
    }
};

const CommandResult& shellCommand(const QString& commandText)
{
    QProcess process;
    process.start(commandText);
    process.waitForFinished(-1); // will wait forever until finished

    const QByteArray stdoutBytes = process.readAllStandardOutput();
    const QByteArray stderrBytes = process.readAllStandardError();

    QString outString = QString(stdoutBytes);
    QString errString = QString(stderrBytes);

    return CommandResult(outString, errString);
}

void MainWindow::on_pushButton_clicked()
{
    CommandResult commandResult = shellCommand("gsettings get org.gnome.settings-daemon.plugins.media-keys custom-keybindings");

    if (commandResult.hasError())
    {
        ui->label->setText("error: " + commandResult.getErrString());
        return;
    }

    ui->label->setText("result: " + commandResult.getOutString());



//    QTextEdit* te = ui->textEdit;
//    QTextCursor tc = te->textCursor();

//    tc.select(QTextCursor::SelectionType::Document);

//    QString commandText = tc.selectedText();

//    CommandResult commandResult = shellCommand(commandText);

//    ui->label->setText(commandText + " > " + commandResult.getOutString() + " | " + commandResult.getErrString());

//    QProcess process;
//    QString cmd = "/home/dmytro/Desktop/screenload-sg";
//    // declare a StringList
//    QStringList args;
//    // Append argument "-a" to stringList
//    args << "-f /home/dmytro/Desktop/test1.png";
//     // start process
//    process.start(cmd, args);
//    process.waitForFinished(2000);
   // QProcess().execute("/usr/bin/screenload-sg -f /home/dmytro/Desktop/test1.png");

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


void MainWindow::on_pushButton_2_clicked()
{

}

