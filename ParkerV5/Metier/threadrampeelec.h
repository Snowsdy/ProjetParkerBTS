#ifndef THREADRAMPEELEC_H
#define THREADRAMPEELEC_H

#include "defs.h"
#include <QThread>
#include "appareilGPIB.h"
#include "relais.h"

class ThreadRampeElec : public QThread
{
    Q_OBJECT

private:
    AppareilGPIB *alim;
    QString device;
    QString message;
    int erreur;
    int adAlim;
    double tension;
    double valMax;
    double valMini;
    double valEnCours;
    double pas;
    int delaiEchantillonage;
    volatile bool stopped;
    bool operation;         // ENCLENCHEMENT ou DECLENCHEMENT
    bool typeDistributeur;  // MONOSTABLE ou BISTABLE

public:
    ThreadRampeElec(int choix);
    void stop();

    QString getMessage(void);
    int getErreur();
    double getValEnCours(void); // Pour obtenier la valeur de seuil elec

    void setDevice(QString);
    void setTension(double);
    void setValEnCours(double); // Pour accelerer la fin d'une rampe
    void setOperation(bool);
    void setTypeDistributeur(bool);
    void setAlim(AppareilGPIB*);

protected:
    void run();

signals:
    void majData(double);
    void topProgress();

public slots:

};

#endif // THREADRAMPEELEC_H
