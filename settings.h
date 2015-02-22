#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDebug>

#include <QMainWindow>
#include <QSystemTrayIcon>

struct xcb_screen_t;
namespace Ui {
class Settings;
}

class QSystemTrayIcon;
#include <QSettings>
#include <QFile>
#include <QElapsedTimer>

class Settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    virtual ~Settings();

private:
    Ui::Settings    *ui;
    QSystemTrayIcon *tray;
    xcb_screen_t    *screen;
    QSettings s;
    QFile logFile;
    QElapsedTimer te;
    QString currentTask;
 signals:
    void logMessage(const QString&category, const QDateTime &timestamp);
public slots:
    virtual void exit();
    virtual void checkScreensaver();
    virtual void selectDirectory();
    virtual void saveSettings();
    virtual void loadSettings();
    virtual void trayActivated(QSystemTrayIcon::ActivationReason);
    virtual void logMessageDefault(const QString&message, const QDateTime &timestamp);
    virtual void startTask(const QString& category);
    virtual void stopTask(const QString& category);


};

#endif // SETTINGS_H
