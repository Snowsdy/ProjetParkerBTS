#ifndef ENTREETOR_H
#define ENTREETOR_H

#include <QObject>
#include <QString>
#include "carteni.h"
#include "physiquefactory.h"

class EntreeTOR : public QObject
{
    Q_OBJECT

private:
    QString moduleNI;
    CarteNI *carteDI;
    int erreur;
    QString msgErr;

public:
    EntreeTOR(int choix);
    int startLectureE_TOR(void);
    int stopLectureE_TOR(void);

    int lireTous_E_TOR(unsigned char *tabValDI);

    void setModuleNI(QString);
    void envoiInfo(int);

signals:
    void infoMatos(QString);
};

#endif // ENTREETOR_H
