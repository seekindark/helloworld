QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cantpappbase.cpp \
        main.cpp    \
        can_tp.cpp  \
        canctrl_cx.cpp \
        isotp.cpp   \
        canthread.cpp \
        testcantp.cpp \
        hrtimer.cpp \
        hrtimer_x.cpp
       # zlog.cpp

HEADERS += \
    can_tp.h \
    canctrl_cx.h \
    cantpappbase.h \
    isotp_config.h \
    isotp_defines.h \
    isotp.h \
    canthread.h \
    testcantp.h \
    version.h \
    hrtimer.h   \
    hrtimer_x.h
    #zlog.h

TRANSLATIONS += \
    qybmsclient-console_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += $$PWD/lib-zlg
LIBS +=-L$$PWD/lib-zlg/ -lControlCAN -lWinmm


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

