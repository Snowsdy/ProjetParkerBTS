#ifndef CARTENI_H
#define CARTENI_H

#include "defs.h"
#include <NIDAQmx.h>
#include <QString>

class CarteNI
{
private:
    char deviceAI[64];
    char deviceAO[64];
    char deviceDI[64];
    char deviceDO[64];

    char msgErreur[64];
    int error;

    TaskHandle taskHandleAI;    // tache acq : Capteurs
    TaskHandle taskHandleAO;    // tache pilotage : Commandes vannes proportionnelles
    TaskHandle taskHandleDO;    // tache mise en service : Relais
    TaskHandle taskHandleDI;    // tache mise en service : Entrée contact + AU

    int choice; // Parker (0) ou Lycée (1)

public:
    CarteNI(int choix);
    ~CarteNI();

    int initTacheAI();
    int initTacheAO();
    int initTacheDO();
    int initTacheDI();

    int clearTacheAI(void);
    int clearTacheAO(void);
    int clearTacheDO(void);
    int clearTacheDI(void);

    int ecrireSortieDO(unsigned char*);
    int ecrireSortieAO(double *);
    int lireEntreesAI(double*);
    int lireEntreesDI(unsigned char*);

    void setDeviceAI(char *);
    void setDeviceAO(char *);
    void setDeviceDI(char *);
    void setDeviceDO(char *);

    char* getMsgErreur(int error);
};

#endif // CARTENI_H
