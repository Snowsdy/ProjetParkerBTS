#ifndef RELAIS_H
#define RELAIS_H

#include <QObject>
#include <QString>
#include "carteni.h"
#include "physiquefactory.h"

class Relais : public QObject
{
    Q_OBJECT

private:
    QString moduleNI;
    CarteNI *carteDO;
    unsigned char tabValDO[8];
    int erreur;
    QString msgErr;

public:
    Relais(int choix);
    int startCmdRelais(void);
    int stopCmdRelais(void);

    int activeRelais(int);
    int desactiveRelais(int);
    int desactiveTousRelais(void);

    void setModuleNI(QString);
    void envoiInfo(int);
    bool getEtatRelais(int);

signals:
    void infoMatos(QString);
};

#endif // RELAIS_H
