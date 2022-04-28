#include "threadacqtpsreppneu.h"

ThreadAcqTpsRepPneu::ThreadAcqTpsRepPneu(int choix){
    stopped = false;
    operation = ENCLENCHEMENT;   // par defaut
    pression = 5.0;         // par defaut
    conf= PhysiqueFactory::getConfigEnCours(choix);
    param= PhysiqueFactory::getParamEnCours(choix);
    pas= param->getPasTpsRep();
}

void ThreadAcqTpsRepPneu::run(){
    erreur=0;
    stopped = false;
    message.clear();

    double limite;              // poucentage de la pression de sortie considérée sortie active
    compteur = 0;                 // compteur pour test
    changement_d_etat = false;  // indicateur changement d'etat
    tpsRep = 0;                  // init valeur du temps de réponse
    bool chrono = false;

    for(int i= 0; i < 8; i++)
        data[i]= 0.0;

    // Déterminer le sens de la rampe
    if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT)){
        limite = pression * param->getCoefLimitePressionDeclenchement();
        valCommande = 0.0;
    }
    else {
        limite = pression * param->getCoefLimitePressionEnclenchement();
        valCommande = pression;
    }
    // Déterminer le pilotage Gauche ou Droite
    if((typeDistributeur==BISTABLE) && (operation==DECLENCHEMENT))
        erreur= vannes->cmdVannePilotageD(valCommande);
    else
        erreur= vannes->cmdVannePilotageG(valCommande);
    if(erreur){
        message= "ERREUR cmdVannePilotage";
        stopped = true; // Erreur, on arrête !
    }
    //    qWarning("Start ThreadAcqTpsRepPneu => limite:%f operation=%d distrib:%d (1:Enclenchement 0:Monostable)", limite, operation, typeDistributeur);
    while (!stopped) {  // boucle d'acquisition
        erreur= capteurs->acqCapteurs(data);
        if(erreur){
            message= "ERREUR=>LireEntreesAI";
            stopped = true;
        }
        else {
            double a, b;
            for(int i = 0; i < 8; i++) {
                param->getAI(i, a, b);
                dataEnBars[i]= data[i]*a + b; // correctif type ax+b
            }
        }
//        qWarning("CG(14)=%0.2f CD(12)=%0.2f SG(4)=%0.2f SD(2)=%0.2f P(1)=%0.2f",\
        dataEnBars[conf->CMD_GAUCHE], dataEnBars[conf->CMD_DROITE], dataEnBars[conf->SORTIE_GAUCHE],\
        dataEnBars[conf->SORTIE_DROITE], dataEnBars[conf->CMD_PRESSION] );
        if((chrono==false)&&(((dataEnBars[conf->CMD_GAUCHE] >= valSeuilEnc) && (operation==ENCLENCHEMENT)) ||\
        ((dataEnBars[conf->CMD_GAUCHE] <= valSeuilDec) && (operation==DECLENCHEMENT) && (typeDistributeur==MONOSTABLE)) ||\
        (((dataEnBars[conf->CMD_GAUCHE] >= valSeuilDec) && (operation==DECLENCHEMENT)) && (typeDistributeur==BISTABLE)))){
            //   qWarning("Start Chrono...");
            t.start();  // démarrage chrono, l'arrêt se fait dans le thread acq
            chrono = true;
        }
//        compteur++; // pour Debug
        if(changement_d_etat == false){
            if(((dataEnBars[conf->SORTIE_GAUCHE]>limite)&&(operation==ENCLENCHEMENT))||\
            ((typeDistributeur==MONOSTABLE)&&(dataEnBars[conf->SORTIE_GAUCHE] < limite) && (operation==DECLENCHEMENT))||
            ((typeDistributeur==BISTABLE)&&(dataEnBars[conf->SORTIE_GAUCHE]<limite) && (operation==DECLENCHEMENT))){
            // idem ((typeDistributeur==BISTABLE)&&(dataEnBars[SORTIE_DROITE]>limite)&&(operation==DECLENCHEMENT))){
                tpsRep = t.elapsed();    // qWarning("tpsRep = %d ms et compteur=%d", tpsRep, compteur);
                changement_d_etat = true;   // indicateur tpsRep déterminé
                emit tpsRepTrouve(tpsRep);
                stopped = true;
            }
        }
        this->msleep(pas); // temps d'échantillonnage pour test de temps de réponse
        //            emit majData(dataEnBars);
    }
    if((typeDistributeur==BISTABLE) && (operation==DECLENCHEMENT))
        vannes->cmdVannePilotageD(0);
    else if((typeDistributeur==BISTABLE) && (operation==ENCLENCHEMENT))
        vannes->cmdVannePilotageG(0);
    //    qWarning("End ThreadAcqTpsRepPneu ");
}

bool ThreadAcqTpsRepPneu::getChangement_d_etat(void){
    return changement_d_etat;
}

QString ThreadAcqTpsRepPneu::getMessage(void){
    return message;
}

int ThreadAcqTpsRepPneu::getErreur(void){
    return erreur;
}

void ThreadAcqTpsRepPneu::setTypeDistributeur(bool type){
    typeDistributeur = type;
}

void ThreadAcqTpsRepPneu::setOperation(bool declen_enclen){
    operation = declen_enclen;
}

void ThreadAcqTpsRepPneu::setPression(double p){
    if(p > 10.0)    // Pression max= 10.0
        p= 10.0;
    pression = p;
}

void ThreadAcqTpsRepPneu::setValSeuilEnc(float seuil){
    valSeuilEnc = seuil;
}

void ThreadAcqTpsRepPneu::setValSeuilDec(float seuil){
    valSeuilDec = seuil;
}

void ThreadAcqTpsRepPneu::setCapteur(CapteursPression *capteur){
    capteurs = capteur;
}

void ThreadAcqTpsRepPneu::setVannes(VannesProportionnelle *v){
    vannes = v;
}

void ThreadAcqTpsRepPneu::stop(){
    stopped = true;
    message="Demande arrêt d'acquisition";
}
