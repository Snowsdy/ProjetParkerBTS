QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

RC_ICONS = favicon.ico

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Metier/administrateur.cpp \
    Physique/carteni.cpp \
    main.cpp \
    Client/mainwindow.cpp \
    Client/pageaccueil.cpp \
    Metier/personne.cpp \
    Physique/sqlqervice.cpp \
    Metier/technicien.cpp

HEADERS += \
    Metier/administrateur.h \
    Physique/carteni.h \
    Client/mainwindow.h \
    Client/pageaccueil.h \
    Metier/personne.h \
    Physique/sqlqervice.h \
    Metier/technicien.h

FORMS += \
    Client/mainwindow.ui \
    Client/pageaccueil.ui

INCLUDEPATH += Client/ \
               Metier/ \
               Physique/

LIBS += -LC:\Qt\lib_NI -lNIDAQmx
LIBS += C:\Qt\lib_NI\ni4882.obj

INCLUDEPATH += "C:\Qt\lib_NI"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressorces.qrc
