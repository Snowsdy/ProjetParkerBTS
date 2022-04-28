#include "threadacqtpsrepelec.h"

ThreadAcqTpsRepElec::ThreadAcqTpsRepElec(int choix){
    stopped = false;
    operation = ENCLENCHEMENT;  // par defaut
    pression = 5.0;             // par defaut
    conf= PhysiqueFactory::getConfigEnCours(choix);
    param= PhysiqueFactory::getParamEnCours(choix);
    pas= param->getPasTpsRep();
}

void ThreadAcqTpsRepElec::run(){
    //    qWarning("Start ThreadAcqTpsRepElec");
    erreur=0;
    stopped = false;
    message.clear();

    char commande[80];
    double limite;              // poucentage de la pression de sortie considérée sortie active
    compteur=0;                 // compteur pour test
    changement_d_etat = false;  // indicateur changement d'etat
    tpsRep= 0;                  // init valeur du temps de réponse

    for(int i= 0; i < 8; i++)
        data[i]= 0.0;

    // Déterminer le sens de la rampe
    if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT)){
        limite = pression * param->getCoefLimitePressionDeclenchement(); // limite = pression * 0.9;
        valCommande = 0.0;
//      qWarning("Seuil declenchement:%f", limite);
    }
    else {
        limite = pression * param->getCoefLimitePressionEnclenchement(); // limite = pression * 0.1;
        valCommande = tension;
//        qWarning("Seuil enclenchement:%f", limite);

    }
    if(conf->getChoix())
        sprintf_s(commande,"V1 %.1f", valCommande );
    else
        sprintf_s(commande,"VOLT %.1f", valCommande );
    //    qWarning("Envoi consigne et start Chrono..."); // A partir du seuilEnc ou du seuilDecl
    erreur= alim->sendCommande(commande);
    if(erreur){
        message= "ERREUR=>Envoi de l'ordre électrique";
        stopped = true;
    }
    t.start();  // démarrage chrono, l'arrêt se fait dans le thread acq
    //    qWarning("Start ThreadAcqTpsRepElec => limite:%f operation=%d distrib:%d (1:Enclenchement 0:Monostable)", limite, operation, typeDistributeur);
    while (!stopped) {  // boucle d'acquisition
        erreur= capteurs->acqCapteurs(data);
        if(erreur){
            message= "ERREUR=>LireEntreesAI";
            stopped = true;
        } else {
            double a, b;
            for(int i = 0; i < 8; i++) {
                param->getAI(i, a, b);
                dataEnBars[i]= data[i]*a + b; // correctif type ax+b
            }
        }
//        qWarning("CG(14)=%0.2f CD(12)=%0.2f SG(4)=%0.2f SD(2)=%0.2f P(1)=%0.2f",\
        dataEnBars[conf->CMD_GAUCHE], dataEnBars[conf->CMD_DROITE], dataEnBars[conf->SORTIE_GAUCHE],\
        dataEnBars[conf->SORTIE_DROITE], dataEnBars[conf->CMD_PRESSION] );
        compteur++; // pour Debug
        if(changement_d_etat == false){
                if(((dataEnBars[conf->SORTIE_GAUCHE]>limite)&&(operation==ENCLENCHEMENT))||\
                ((typeDistributeur==MONOSTABLE) && (dataEnBars[conf->SORTIE_GAUCHE] < limite) && (operation==DECLENCHEMENT))||\
                ((typeDistributeur==BISTABLE) && (dataEnBars[conf->SORTIE_GAUCHE] < limite) && (operation==DECLENCHEMENT))){
                // idem ((typeDistributeur==BISTABLE)&&(dataEnBars[SORTIE_DROITE]>limite)&&(operation==DECLENCHEMENT))){
                    tpsRep= t.elapsed();
//                    qWarning("tpsRep = %d ms et compteur=%d", tpsRep, compteur);
                    changement_d_etat = true;   // indicateur tpsRep déterminé
                    emit tpsRepTrouve(tpsRep);
                    stopped = true;
                }
            }
            this->msleep(param->getPasTpsRep()); // temps d'échantillonnage pour test de temps de réponse
        }
    if(typeDistributeur==BISTABLE){
        if(conf->getChoix())
            sprintf_s(commande,"V1 %.1f", 0.0 );
        else
            sprintf_s(commande,"VOLT %.1f", 0.0 );
        alim->sendCommande(commande);
    }
//    qWarning("End ThreadAcqTpsRepElec ");
}

bool ThreadAcqTpsRepElec::getChangement_d_etat(void){
    return changement_d_etat;
}

QString ThreadAcqTpsRepElec::getMessage(void){
    return message;
}

int ThreadAcqTpsRepElec::getErreur(void){
    return erreur;
}

void ThreadAcqTpsRepElec::setTypeDistributeur(bool type){
    typeDistributeur = type;
}

void ThreadAcqTpsRepElec::setOperation(bool declen_enclen){
    operation = declen_enclen;
}

void ThreadAcqTpsRepElec::setPression(double p){
    if(p > 10.0)    // Pression max= 10.0
        p= 10.0;
    pression = p;
}

void ThreadAcqTpsRepElec::setTension(double tem){
    if(tem > 300.0)    // Pression max= 10.0
        tem = 300.0;
    tension = tem;
}

void ThreadAcqTpsRepElec::setValSeuilEnc(float seuil){
    valSeuilEnc = seuil;
}

void ThreadAcqTpsRepElec::setValSeuilDec(float seuil){
    valSeuilDec = seuil;
}

void ThreadAcqTpsRepElec::setCapteur(CapteursPression *capteur){
    capteurs = capteur;
}

void ThreadAcqTpsRepElec::setAlim(AppareilGPIB *power){
    this->alim = power;
}

void ThreadAcqTpsRepElec::setAdAlim(int adresse){
    adAlim = adresse;
}

void ThreadAcqTpsRepElec::stop(){
    stopped = true;
    message="Demande arrêt d'acquisition";
}
