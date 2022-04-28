#include "mainwindow.h"
#include "pageaccueil.h"

#include <QApplication>
#include <QFontDatabase>

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

    MainWindow w;
    PageAccueil p;

    QObject::connect(&w, SIGNAL(connexionSuccessful(int)), &p, SLOT(launchPG(int)));
    QObject::connect(&p, SIGNAL(deconnexion(void)), &w, SLOT(relancerConnexion(void)));

    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();
    return a.exec();
}
