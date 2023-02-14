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

    bool isEmpty()
    {
        return outString.isNull() || outString.isEmpty();
    }

    QString getOutString()
    {
        return outString;
    }

    QString getErrString()
    {
        return errString;
    }

    QString getCleanOutString()
    {
        QString temp = outString;
        temp = temp.trimmed();
        return temp;
    }

    QStringList getStringList()
    {
        QString temp = outString;

        temp = temp.trimmed();

        if (temp.startsWith("@as", Qt::CaseInsensitive))
            temp = temp.remove(0, 3);

        temp = temp.trimmed();

        if (temp.startsWith('['))
        {
            temp = temp.remove(0, 1);
            temp.chop(1);
        }

        QStringList parts = temp.split(',', QString::SkipEmptyParts);

        for (QStringList::iterator it = parts.begin(); it != parts.end(); ++it)
        {
            QString& part = *it;
            part = part.trimmed();
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

void setCustomKeybindingsIfNeeded()
{
    CommandResult getKeybindingsCommandResult = shellCommand("gsettings get org.gnome.settings-daemon.plugins.media-keys custom-keybindings");

    QStringList keybindings;

    if (!getKeybindingsCommandResult.hasError() && !getKeybindingsCommandResult.isEmpty())
        keybindings = getKeybindingsCommandResult.getStringList();

    const QString screenloadKeybinding = "'/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/'";

    if (!keybindings.isEmpty() && keybindings.contains(screenloadKeybinding, Qt::CaseInsensitive))
        return;

    keybindings.append(screenloadKeybinding);

    QString keybindingsLine = keybindings.join(',');

    QString setKeybindingsCommand = QString("gsettings set org.gnome.settings-daemon.plugins.media-keys custom-keybindings \"[%1]\"").arg(keybindingsLine);
    shellCommand(setKeybindingsCommand);
}

bool isCustomBindingExists()
{
    QString bindingCommand = QString("gsettings get org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ binding");
    CommandResult bindingCommandResult = shellCommand(bindingCommand);

    if (bindingCommandResult.hasError())
        return false;

    QString cleanOutString = bindingCommandResult.getCleanOutString();

    if (0 == cleanOutString.compare("''", Qt::CaseInsensitive))
        return false;

    return true;
}

void MainWindow::on_pushButton_clicked()
{
    QString res = isCustomBindingExists() ? "Yes" : "No";
    ui->label->setText("result: " + res);

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
    shellCommand("gsettings reset org.gnome.settings-daemon.plugins.media-keys screenshot");
    shellCommand("gsettings reset org.gnome.shell.keybindings show-screenshot-ui");

    if (!isCustomBindingExists())
    {
        CommandResult commandResult = shellCommand("gsettings get org.gnome.shell.keybindings show-screenshot-ui"); // Ubuntu 22

        if (commandResult.hasError()) // Ubuntu 20 or 18
        {
            commandResult = shellCommand("gsettings get org.gnome.settings-daemon.plugins.media-keys screenshot");

            QString outString = commandResult.getOutString();
            QString value = outString.replace("Print", "");

            QString setCommand = QString("gsettings set org.gnome.settings-daemon.plugins.media-keys screenshot \"%1\"").arg(value);
            shellCommand(setCommand);
        }
        else
            shellCommand("gsettings set org.gnome.shell.keybindings show-screenshot-ui \"['']\"");

        //shellCommand("gsettings set org.gnome.shell.keybindings show-screenshot-ui \"['']\"");
        //shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys screenshot \"['']\""); // Ubuntu 20
        //shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys screenshot \"''\""); // Ubuntu 18

        setCustomKeybindingsIfNeeded();

        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ name 'screenload'");
        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ command \"/usr/bin/screenload gui\"");
        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ binding 'Print'");
    }
    else
    {
        shellCommand("gsettings reset org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/screenload/ binding");

//        shellCommand("gsettings reset org.gnome.settings-daemon.plugins.media-keys screenshot");
//        shellCommand("gsettings reset org.gnome.shell.keybindings show-screenshot-ui");





//        shellCommand("gsettings set org.gnome.settings-daemon.plugins.media-keys screenshot \"['Print']\""); // Old OS.
//        shellCommand("gsettings set org.gnome.shell.keybindings show-screenshot-ui \"['Print']\"");
    }
}

