#ifndef CAPTEURSPRESSION_H
#define CAPTEURSPRESSION_H

#include <QObject>
#include <QString>
#include "carteni.h"

class CapteursPression : public QObject
{
    Q_OBJECT

private:
    QString moduleNI;
    CarteNI *carteAI;
    int erreur;
    QString msgErr;

public:
    CapteursPression(int choix);
    int startAcqCapteurs();
    int stopAcqCapteurs();

    int acqCapteurs(double *);

    void setModuleNI(QString);
    void envoiInfo(int);

signals:
    void infoMatos(QString);
};

#endif // CAPTEURSPRESSION_H
