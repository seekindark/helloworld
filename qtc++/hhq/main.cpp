#include <QApplication>
#include <QDesktopWidget>
#include "mainwindow.h"
#include "loginwindow.h"
#include "mylog.h"
#include "hsecurity.h"
#include "haudioproc.h"
#include "hudpproc.h"
#include "hsettings.h"

HAudioProc *g_audioProc = new HAudioProc();
HUdpProc *g_udpProc = new HUdpProc();
HSettings *g_settings = new HSettings();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    HSecurity *sec = HSecurity::get_instance();
    MyLog(">>> Sytem is starting...");

    QDesktopWidget* desktop = a.desktop();
    w.move((desktop->width() - w.width())/2, (desktop->height() - w.height())/2);
    w.show();
    w.show_login_window();

    MyLog("Sytem is started successfully");

    int ret = a.exec();

    delete sec;

    return ret;
}
