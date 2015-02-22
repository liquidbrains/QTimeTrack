#include "settings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("QTimeTrack");
    a.setApplicationDisplayName("Qt based time tracking client");
    a.setApplicationVersion("0.1");
    a.setOrganizationName("Wian Potgieter");
    a.setQuitOnLastWindowClosed(false);

    Settings w;
    //if()
    //w.show();

    return a.exec();
}
