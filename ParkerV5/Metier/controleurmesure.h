#ifndef CONTROLEURMESURE_H
#define CONTROLEURMESURE_H

#include <QObject>
#include <QString>

#include "relais.h"
#include "vannesproportionnelle.h"
#include "capteurspression.h"
#include "appareilGPIB.h"
#include "config.h"
#include "entreetor.h"

#include "threadacqseuilpneu.h"
#include "threadacqtpsreppneu.h"
#include "threadacqtpsrepelec.h"
#include "threadrampepneu.h"
#include "threadrampeelec.h"

#include "mesurestest.h"

class ControleurMesure : public QObject
{
    Q_OBJECT

private:
    MesuresTest *mtp;   // Résultats des tests pneu
    bool mesureEnCours;     // indicateur qu'une mesure est en cours
    double pressionAlim;
    double tensionAlim;
    bool operation;         // ENCLENCHEMENT ou DECLENCHEMENT
    bool mode;              // false:Pneu ; true:Elec => utile pour getAlim
    bool typeDistributeur;  // MONOSTABLE ou BISTABLE
    bool courant;           // CONTINU ou ALTERNATIF
    int compteurFinThread;  // Compteur de thread terminé
    bool etatMesure;        // false:Erreur; true:OK
    QString msgThread;
    QString message;        // pour formatage de chaine de caractères
    bool ELEC;

public:
    Relais *relais[3];
    CapteursPression *capteursP;
    VannesProportionnelle *vannesP;
    AppareilGPIB *alimCC;
    AppareilGPIB *alimAC;
    EntreeTOR *e_tor;
    Config *conf;

    ThreadAcqSeuilPneu *acqSeuilPneu;
    ThreadAcqTpsRepPneu *acqTpsRepPneu;
    ThreadAcqTpsRepElec *acqTpsRepElec;
    ThreadRampePneu *rampePneu;
    ThreadRampeElec *rampeElec;

public:
    ControleurMesure(int choix); // Parker (0) ou Lycée (1)
    double getTensionAlim(void);
    double getPressionAlim(void);
    bool getOperation(void);
    bool getTypeDistributeur(void);
    bool getMesureEnCours(void);
    bool getMode(void);
    QString getMsgThread(void);

    void setMsgThread(QString);
    void setTensionAlim(double);
    void setPressionAlim(double);
    void setOperation(bool);        // ENCLENCHEMENT ou DECLENCHEMENT
    void setMode(bool);             // mesure Elec ou Pneu
    void setMesureEnCours(bool);


    void setTypeDistributeur(bool);
    void setCourant(bool);
    void setMtp(MesuresTest*);

    void testPneuSeuil(bool type);
    void testElecSeuil(void);
    void testPneuTpsRep(void);
    void testElecTpsRep(void);
    int activationRelaisTestPneu(void);
    int desactivationRelaisTestPneu(void);
    int activationRelaisTestElec(void);
    int desactivationRelaisTestElec(void);

    void startGPIB();
    void stopGPIB();
    void stopBanc(void);
//    void raz(void);

public slots:
    void startBanc(void);
    void finAcqSeuilPneu(void);
    void finAcqTpsRepPneu(void);
    void finAcqTpsRepElec(void);
    void finRampePneu(void);
    void finRampeElec(void);
    void detectSeuilPneu(double, double);
    void detectTpsRep(int); // pneu et elec

signals:
    void topProgress(int);
    void finInit(void);
    void finArret(void);
    void infoMatos(QString);
    void finDeTest(bool);
};

#endif // CONTROLEURMESURE_H
