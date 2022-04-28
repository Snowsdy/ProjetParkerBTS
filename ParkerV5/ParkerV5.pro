QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += qdarkstyle/style.qrc \
    ressources.qrc

SOURCES += \
    Client/splashscreen.cpp \
    Metier/entreetor.cpp \
    Physique/config.cpp \
    Physique/param.cpp \
    Physique/physiquefactory.cpp \
    main.cpp \
    Client/mainwindow.cpp \
    Physique/carteni.cpp \
    Metier/capteurspression.cpp \
    Metier/vannesproportionnelle.cpp \
    Metier/relais.cpp \
    Metier/threadrampepneu.cpp \
    Metier/threadacqseuilpneu.cpp \
    Metier/threadacqtpsreppneu.cpp \
    Metier/controleurmesure.cpp \
    Metier/mesurestest.cpp \
    Metier/threadrampeelec.cpp \
    Metier/threadacqtpsrepelec.cpp \
    Physique/sqlservice.cpp \
    Physique/appareilGPIB.cpp

HEADERS += \
    Client/splashscreen.h \
    Metier/defs.h \
    Client/mainwindow.h \
    Metier/capteurspression.h \
    Metier/entreetor.h \
    Metier/vannesproportionnelle.h \
    Metier/relais.h \
    Metier/threadrampepneu.h \
    Metier/threadacqseuilpneu.h \
    Metier/threadacqtpsreppneu.h \
    Metier/controleurmesure.h \
    Metier/mesurestest.h \
    Metier/threadrampeelec.h \
    Metier/threadacqtpsrepelec.h \
    Physique/config.h \
    Physique/param.h \
    Physique/physiquefactory.h \
    Physique/sqlservice.h \
    Physique/carteni.h \
    Physique/appareilGPIB.h

FORMS += \
    Client/mainwindow.ui \
    Client/splashscreen.ui

win32:RC_ICONS = favicon.ico

LIBS += -LC:\Qt\libs_Qt5.14\LIB_Qt5.14_Qwt-6.1.4\lib\MSVC -lqwt
LIBS += -LC:\Qt\libs_Qt5.14\LIB_Qt5.14_Analogwidgets-257\lib\MSVC -lanalogwidgets
LIBS += -LC:\Qt\lib_NI -lNIDAQmx
LIBS += C:\Qt\lib_NI\ni4882.obj

INCLUDEPATH += Metier/ \
               Physique/ \
               Client/

INCLUDEPATH += C:\Qt\libs_Qt5.14\LIB_Qt5.14_Qwt-6.1.4\include
INCLUDEPATH += C:\Qt\libs_Qt5.14\LIB_Qt5.14_Analogwidgets-257\include
INCLUDEPATH += "C:\Qt\lib_NI"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
