#include "mainwindow.h"
#include "splashscreen.h"

#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Ajout des polices custom :
    QStringList *fonts = new QStringList();
    fonts->append(":/fonts/Fonts/Quicksand.ttf");
    fonts->append(":/fonts/Fonts/Quicksand-Bold.ttf");
    fonts->append(":/fonts/Fonts/Quicksand-Light.ttf");
    fonts->append(":/fonts/Fonts/Quicksand-Medium.ttf");
    fonts->append(":/fonts/Fonts/Quicksand-Regular.ttf");
    fonts->append(":/fonts/Fonts/Quicksand-SemiBold.ttf");

    for (QString font : *fonts) {
        QFontDatabase::addApplicationFont(font);
    }

    delete fonts;
    // Fin de l'ajout des polices custom
    SplashScreen s;
    MainWindow w;

    QObject::connect(&s, SIGNAL(launchingEmitted(int)), &w, SLOT(launchApp(int)));
    QObject::connect(&s, SIGNAL(lancement(void)), &w, SLOT(miseEnRoute(void)));
    QObject::connect(&w, SIGNAL(progress(int)), &s, SLOT(majProgressBar(int)));
    QObject::connect(&w, SIGNAL(info(QString)), &s, SLOT(majInfo(QString)));

    s.setWindowFlag(Qt::SplashScreen);
    s.setWindowTitle("Projet Parker");
    s.show();


    return a.exec();
}
