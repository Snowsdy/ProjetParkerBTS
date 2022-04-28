#ifndef THREADACQTPSREPELEC_H
#define THREADACQTPSREPELEC_H

#include <QThread>
#include <QTime>
#include "capteurspression.h"
#include "appareilGPIB.h"
#include "physiquefactory.h"

class ThreadAcqTpsRepElec : public QThread
{
    Q_OBJECT

private:
    CapteursPression *capteurs;
    AppareilGPIB *alim;

    int erreur;
    QString message;        // Message explicatif de la fin d'acquisition
    double data[8];         // tableau des valeurs d'acquisition
    double dataEnBars[8];   // tableau des valeurs converties
    double pression;        // Valeur pression d'alimentation
    double tension;
    bool changement_d_etat; // Précise un enclenchement ou déclenchement réalisé
    bool typeDistributeur;  // MONOSTABLE ou BISTABLE
    bool operation;         // ENCLENCHEMENT ou DECLENCHEMENT
    bool stopped;
    int pas;

    int adAlim;             // adresse GPIB Alim
    int tpsRep;             // en ms
    ulong compteur;         // Utiliser pour des tests de rapidité
    double valCommande;     // Valeur en bar à appliquer à CMD_GAUCHE lors d'un test tps de rep
    QTime t;                // Pour mesurer le tps de réponse
    float valSeuilEnc;
    float valSeuilDec;
    Config *conf;
    Param *param;

public:    
    ThreadAcqTpsRepElec(int choix);

    bool getChangement_d_etat(void);
    QString getMessage(void);
    int getErreur(void);

    void setPression(double);
    void setTension(double);
    void setOperation(bool);
    void setTypeDistributeur(bool);
    void setValSeuilEnc(float);
    void setValSeuilDec(float);
    void setAdAlim(int);
    void setCapteur(CapteursPression*);
    void setAlim(AppareilGPIB*);

    void stop();

protected:
    void run();

signals:
    void majData(double*);
    void tpsRepTrouve(int);
};

#endif // THREADACQTPSREPELEC_H
