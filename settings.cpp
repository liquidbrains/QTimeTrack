#include "settings.h"
#include "ui_settings.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QX11Info>
#include <QFileDialog>
#include <X11/Xlib-xcb.h>
#include <xcb/screensaver.h>
#include <xcb/dpms.h>
#include <QMessageBox>
#include <QTimer>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QDateTime>

Settings::Settings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Settings),
    tray(new QSystemTrayIcon(this)),
    screen(xcb_setup_roots_iterator (xcb_get_setup (QX11Info::connection())).data),
    currentTask("nothing")
{
    ui->setupUi(this);
    tray->setIcon(QIcon(":/timer.png"));
    tray->show();
    connect(tray,SIGNAL(messageClicked()),this,SLOT(show()));
    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));

    setWindowIcon(QIcon(":/timer.png"));
    QApplication::setWindowIcon(QIcon(":/timer.png"));

    connect(ui->btnDirectorySelect,SIGNAL(clicked()),this,SLOT(selectDirectory()));
    connect(ui->btnCancel,SIGNAL(rejected()),this,SLOT(hide()));
    connect(ui->btnCancel,SIGNAL(accepted()),this,SLOT(saveSettings()));

    if(s.value("firstStart",true).toBool()){
        loadSettings();
        show();
    }

    QMenu *menu = new QMenu(this);

    menu->addSeparator();
    menu->addAction("&Exit",this,SLOT(exit()));

    tray->setContextMenu(menu);

    qDebug() <<QX11Info::isPlatformX11();

    if (QX11Info::isPlatformX11()== false){
        QMessageBox::warning(this,"Not running X?","I'm sorry, but I can only run in X.",QMessageBox::Ok,QMessageBox::NoButton);

        QApplication::exit(254);
    }

    screen = xcb_setup_roots_iterator (xcb_get_setup (QX11Info::connection())).data;

    QTimer *t = new QTimer(this);
    connect(t,SIGNAL(timeout()),this,SLOT(checkScreensaver()));

}

Settings::~Settings()
{
    delete ui;
    delete screen;
}


void Settings::exit()
{
    //qDebug() << "In " << QString(__FUNCTION__);
    //exitClicked = true;
    tray->hide();
    tray->setVisible(false);
    QApplication::exit();
}

void Settings::checkScreensaver(){
    static uint8_t oldState = -1;
    xcb_drawable_t  window   = screen->root;
    xcb_screensaver_query_info_cookie_t screensaver_cookie =
            xcb_screensaver_query_info_unchecked(QX11Info::connection(),window);

    xcb_screensaver_query_info_reply_t *r = xcb_screensaver_query_info_reply(QX11Info::connection(),screensaver_cookie,NULL);

    //qDebug() << "User idle time: " << r->ms_since_user_input << "ms.";

    QIdle

    delete r;
}

void Settings::selectDirectory(){
    QString directory(QFileDialog::getExistingDirectory(this,"Please select an existing directory to save time tracking data in.","~"));

    if (!QDir(directory).isReadable()){
        QMessageBox::warning(this,"Cannot read directory.","Directory "+directory+" is not readable.",QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    QFile file(directory+"/timetrack_"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+".csv");

    qDebug() << "file is : "+file.fileName();

    if(!file.isWritable()){
        QMessageBox::warning(this,"Cannot write to file.","File '"+file.fileName()+"' is not writable.  Please check your permissions.",QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    ui->txtDirectory->setText(directory);
}

void Settings::saveSettings(){
    s.setValue("directory", ui->txtDirectory->text());
    s.setValue("idleTimeout",ui->spnIdleTimeout->value());
    s.setValue("categories",ui->txtCategories->toPlainText());
    s.setValue("firstStart",false);
}

void Settings::loadSettings(){
    ui->txtCategories->setText(s.value("categories","").toString());
    ui->txtDirectory->setText(s.value("directory","~").toString());
    ui->spnIdleTimeout->setValue(s.value("idleTimeout",2).toInt());
}

void Settings::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug() << "In " << QString(__FUNCTION__);
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        show();
    }
}


void Settings::logMessageDefault(const QString&category, const QDateTime &timestamp){
    QFile file(s.value("directory","~").toString()+"/timetrack_"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+".csv");
    ///TODO Append?
    file.write(QString(category+" @ "+timestamp.toString(Qt::RFC2822Date)).toLocal8Bit());
    qDebug() << "Task "+category+" @ "+timestamp.toString(Qt::RFC2822Date);
}

void Settings::startTask(const QString& category){
    emit logMessage("Started timing "+category,QDateTime::currentDateTime());
}

void Settings::stopTask(const QString& category){
    emit logMessage("Stopped timing "+category,QDateTime::currentDateTime());
}
