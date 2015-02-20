#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>

namespace Ui {
class Settings;
}

class QSystemTrayIcon;

class Settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    QSystemTrayIcon *tray;
    Ui::Settings *ui;
public slots:
    virtual void exit();
};

#endif // SETTINGS_H
