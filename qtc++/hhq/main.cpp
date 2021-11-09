#include <QApplication>
#include <QDesktopWidget>
#include "mainwindow.h"
#include "loginwindow.h"
#include "mylog.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    MyLog("Sytem is starting...");

    QDesktopWidget* desktop = a.desktop();
    w.move((desktop->width() - w.width())/2, (desktop->height() - w.height())/2);
    w.show();
    w.show_login_window();

    MyLog("Sytem is started successfully");

    return a.exec();
}
