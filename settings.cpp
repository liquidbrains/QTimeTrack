#include "settings.h"
#include "ui_settings.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QX11Info>
#include <X11/Xlib-xcb.h>

Settings::Settings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Settings),
    tray(new QSystemTrayIcon(this))
{
    ui->setupUi(this);
    tray->setIcon(QIcon(":/timer.png"));
    tray->show();
    connect(tray,SIGNAL(messageClicked()),this,SLOT(show()));

    setWindowIcon(QIcon(":icons/QBullet.png"));
    QApplication::setWindowIcon(QIcon(":icons/QBullet.png"));

    QMenu *menu = new QMenu(this);

    menu->addSeparator();
    menu->addAction("&Exit",this,SLOT(exit()));

    tray->setContextMenu(menu);
    Display *x = QX11Info::display();

    void *y = (void*)x;
}

Settings::~Settings()
{
    delete ui;
}


void Settings::exit()
{
    //qDebug() << "In " << QString(__FUNCTION__);
    //exitClicked = true;
    tray->hide();
    tray->setVisible(false);
    QApplication::exit();
}
