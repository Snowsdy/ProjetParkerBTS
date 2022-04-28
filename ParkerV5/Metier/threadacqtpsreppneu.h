#ifndef THREADACQTPSREPPNEU_H
#define THREADACQTPSREPPNEU_H

#include <QThread>
#include <QTime>
#include "physiquefactory.h"
#include "capteurspression.h"
#include "vannesproportionnelle.h"

class ThreadAcqTpsRepPneu : public QThread
{
    Q_OBJECT

private:
    CapteursPression *capteurs;
    VannesProportionnelle *vannes;

    int erreur;
    QString message;        // Message explicatif de la fin d'acquisition
    double data[8];         // tableau des valeurs d'acquisition
    double dataEnBars[8];   // tableau des valeurs converties
    double pression;        // Valeur pression d'alimentation
    bool changement_d_etat; // Précise un enclenchement ou déclenchement réalisé
    bool typeDistributeur;  // MONOSTABLE ou BISTABLE
    bool operation;         // ENCLENCHEMENT ou DECLENCHEMENT
    bool stopped;           // indicateur sortie de thread
    int pas;                // Delai d'échantillonnage

    int tpsRep;             // en ms
    ulong compteur;         // Utiliser pour des tests de rapidité
    double valCommande;     // Valeur en bar à appliquer à CMD_GAUCHE lors d'un test tps de rep
    QTime t;                // Pour mesurer le tps de réponse
    float valSeuilEnc;
    float valSeuilDec;
    Config *conf;
    Param *param;

public:
    ThreadAcqTpsRepPneu(int choix);

    bool getChangement_d_etat(void);
    QString getMessage(void);
    int getErreur(void);

    void setPression(double);
    void setOperation(bool);
    void setTypeDistributeur(bool);
    void setValSeuilEnc(float);
    void setValSeuilDec(float);

    void setCapteur(CapteursPression*);
    void setVannes(VannesProportionnelle*);

    void stop();

protected:
    void run();

signals:
    void majData(double*);
    void tpsRepTrouve(int);
};

#endif // THREADACQTPSREPPNEU_H
