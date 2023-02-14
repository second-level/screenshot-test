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
#include <QRegularExpression>

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
        return !errString.isNull() && !errString.isEmpty();
    }

    QString getOutString()
    {
        return outString;
    }

    QString getErrString()
    {
        return errString;
    }

    QString getClearOutString()
    {
        QString temp = outString;

        temp = temp.trimmed();

        if (temp.startsWith('\''))
        {
            temp = temp.remove(0, 1);
            temp.chop(1);
        }

        return temp;
    }

    QStringList getStringList()
    {
        QString temp = outString;

        temp = temp.trimmed();

        if (temp.startsWith('['))
        {
            temp = temp.remove(0, 1);
            temp.chop(1);
        }

        QStringList parts = temp.split(',', Qt::SkipEmptyParts);

        for (QStringList::iterator it = parts.begin(); it != parts.end(); ++it)
        {
            QString& part = *it;

            part = part.trimmed();

            if (part.startsWith('\''))
            {
                part = part.remove(0, 1);
                part.chop(1);
            }
        }

        return parts;
    }
};

CommandResult shellCommand(const QString& commandText)
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

//QString getCustomKeyPath()
//{
//    CommandResult commandResult = shellCommand("gsettings get org.gnome.settings-daemon.plugins.media-keys custom-keybindings");

//    if (commandResult.hasError())
//        return QString::null;

//    QStringList keybindings = commandResult.getStringList();

//    for (QStringList::iterator it = keybindings.begin(); it != keybindings.end(); ++it)
//    {
//        QString& path = *it;

//        QString nameCommand = QString("gsettings get org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:%1 name").arg(path);

//        CommandResult nameCommandResult = shellCommand(nameCommand);

//        QString name = nameCommandResult.getClearOutString();

//        if (name.compare("screenload", Qt::CaseInsensitive) == 0)
//            return QString(path);
//    }

//    return QString::null;
//}

bool isCustomKeyExists()
{
    QString bindingCommand = QString("gsettings get org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ binding");
    CommandResult bindingCommandResult = shellCommand(bindingCommand);

    if (bindingCommandResult.hasError())
        return false;

    return !bindingCommandResult.getClearOutString().isEmpty();
}

void MainWindow::on_pushButton_clicked()
{
    ui->label->setText("result: " + isCustomKeyExists());

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
    if (isCustomKeyExists())
    {
        shellCommand("gsettings set org.gnome.shell.keybindings show-screenshot-ui '[]'");
        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys screenshot ''"); // Old OS.

        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys custom-keybindings \"['/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/']\"");
        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ name 'screenload'");
        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ command \"/usr/bin/screenload gui\"");
        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ binding ['Print']");
    }
    else
    {
        shellCommand("gsettings reset org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ binding");

        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys screenshot 'Print'"); // Old OS.
        shellCommand("gsettings set org.gnome.shell.keybindings show-screenshot-ui ['Print']");
    }
}

