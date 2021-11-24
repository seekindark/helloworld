#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "loginwindow.h"
#include "widdashboard.h"
#include "widbroadcast.h"
#include "widrecoding.h"
#include "widedit.h"
#include "widchangepwd.h"
#include "widlogs.h"
#include "widsetting.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void show_login_window();

    void log(const QString &msg);
    void closeEvent( QCloseEvent * event );

    void audioProc_reset();

public slots:
    void on_login_closed();
    void on_login_OK(const QString &user_name);

    //system button menu:
    void on_menu_logout_clicked();
    void on_menu_pwd_clicked();
    void on_menu_setting_clicked();
    void on_menu_logs_clicked();
    void on_menu_exit_clicked();



    //tabWidget window
    void on_tabBarClicked(int index);
    void on_currentChanged(int index);
private:
    LoginWindow *m_loginWind;

    QString m_user_name;
    QLabel *m_lab_title;
    QPushButton *m_bt_system;
    QLabel *m_lab_user;


    QVBoxLayout *m_vLayout_main;
    QTabWidget *m_tab;
    WidDashboard *m_wid_dashboard;
    WidBroadCast *m_wid_livecall;
    WidRecoding *m_wid_newRecord;
    WidEdit *m_wid_editRecord;

    WidSetting *m_wid_setting;

    QMenu *m_sys_menu;


    void construct_main();
    void construct_system_menu();

    void show_main_window();
    void hide_main_window();



};
#endif // MAINWINDOW_H
