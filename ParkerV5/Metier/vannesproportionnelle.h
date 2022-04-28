#ifndef VANNESPROPORTIONNELLE_H
#define VANNESPROPORTIONNELLE_H

#include <QObject>
#include "carteni.h"
#include "physiquefactory.h"
#include <QString>

class VannesProportionnelle : public QObject
{
    Q_OBJECT

private:
    QString moduleNI;
    CarteNI *carteAO;
    Config *conf;
    double tabVal[4];
    int erreur;
    QString msgErr;

public:
    VannesProportionnelle(int choix);
    int startCmdVannes();
    int stopCmdVannes(void);

    int cmdVannePression(double val);
    int cmdVannePilotageG(double val);
    int cmdVannePilotageD(double val);

    double getTabVal(int);
    void setModuleNI(QString);
    void envoiInfo(int);

signals:
    void infoMatos(QString);
};

#endif // VANNESPROPORTIONNELLE_H
