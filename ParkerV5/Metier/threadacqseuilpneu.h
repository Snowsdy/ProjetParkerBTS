#ifndef THREADACQSEUILPNEU_H
#define THREADACQSEUILPNEU_H

#include <QThread>
#include <QTime>
#include "physiquefactory.h"
#include "appareilGPIB.h"
#include "capteurspression.h"
#include "entreetor.h"

class ThreadAcqSeuilPneu : public QThread
{
    Q_OBJECT

private:
    int erreur;
    QString message;        // Message explicatif de la fin d'acquisition
    double data[8];         // tableau des valeurs d'acquisition
    double dataEnBars[8];   // tableau des valeurs converties
    unsigned char dataETOR[8];  // tableau des états des Entrées TOR
    double pression;        // Valeur pression d'alimentation
    bool flagETOR;          // indicateur changement d'état de l'entrée TOR
    bool changement_d_etat; // Précise un enclenchement ou déclenchement réalisé
    bool typeDistributeur;  // MONOSTABLE ou BISTABLE
    bool operation;         // ENCLENCHEMENT ou DECLENCHEMENT
    bool stopped;           // indicateur sortie de thread
    int pas;                // Delai d'échantillonnage
    float valSeuil;         // en bar
    float valSeuilCourant;  // en mA

    CapteursPression *capteurs;
    EntreeTOR *etor;
    AppareilGPIB *multimetre;

    Config *conf;
    Param *param;

public:
    ThreadAcqSeuilPneu(int choix);

    bool getChangement_d_etat(void);
    QString getMessage(void);
    int getErreur(void);

    void setPression(double);
    void setOperation(bool);
    void setTypeDistributeur(bool);

    void setCapteur(CapteursPression*);
    void setETOR(EntreeTOR*);
    void setMultimetre(AppareilGPIB*);
    void stop();

protected:
    void run();

signals:
    void majData(double*);
    void seuilAtteint(double, double);
};

#endif // THREADACQSEUILPNEU_H
