#include "threadrampepneu.h"

ThreadRampePneu::ThreadRampePneu(int choix){
    stopped = false;            // autorise l'exécution du thread
    valMini = 0.0;              // par defaut
    operation = ENCLENCHEMENT;  // rampe montante par defaut
    pression = 5;               // par defaut
    conf= PhysiqueFactory::getConfigEnCours(choix);
    param= PhysiqueFactory::getParamEnCours(choix);
}

void ThreadRampePneu::run(){
    erreur=0;
    int coef; // sens de la rampe
    int numEtape, nbEtape, i, j, delai;
    stopped = false;
    message.clear();
    valMax = pression;

    param->initRampe(typeRampe); // chargement des param rampe normale ou perso

    nbEtape= param->nbPas.size();
    pas= param->pasRampe_en_bar[0]; // 0.1 bar par défaut
    if(nbEtape == 0) {// rampe simple
        param->nbPas.append(pression/pas); //Ex: 5/0.1= 50 tours
    }
    // Déterminer le sens de la rampe
    if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT)) {
        valEnCours = pression - pas;   // origine rampe ( ne pas envoyer la valeur mini ou max )
        coef= -1;
    } else {
        valEnCours = 0.0 + pas;   // origine rampe
        coef= 1;
    }
//    qWarning("Start ThreadRampe Pneu => operation=%d distrib:%d valEnCours=%f (1:Enclenchement 0:Monostable)", operation, typeDistributeur, valEnCours);
//    qWarning("nbEtape= %d, pas= %f volt, nbpas= %d, typeRampe= %s", nbEtape, pas, param->nbPas[0], typeRampe?"normale":"perso" );
    numEtape= 0;
    while ((!stopped) && (numEtape <= nbEtape)) {
//        qWarning("passage while numEtape= %d", numEtape);
        j= param->nbPas[numEtape];
        for(i=0; (!stopped) && (!erreur) && (i < j); i++) {
            pas= param->pasRampe_en_bar[numEtape];
//            qWarning("passage for i=%d pas_en_bar= %f", i, pas);
            if(pas != 0) { // si rampe
                if((typeDistributeur==BISTABLE) && (operation==DECLENCHEMENT))
                    erreur= vannes->cmdVannePilotageD(valEnCours);
                else
                    erreur= vannes->cmdVannePilotageG(valEnCours);
                if(erreur){
                    message= "ERREUR cmdVannePilotage";
                    stopped = true; // Erreur, on arrête !
                }
                else {
                    emit majData(valEnCours); // qWarning("rampe: %0.2f", valEnCours);
                    valEnCours+= (param->pasRampe_en_bar[numEtape] * coef);
                    if((valEnCours >= valMax) || (valEnCours <= valMini) ){
                        message= "Fin de rampe";
                        stopped= true;  // Valeur max atteinte, on arrête !
                    }
                    else
                        this->msleep(param->pasRampe_en_ms[0]);
                }
            } else { // pas une rampe
                if(param->nbPas[numEtape]==1) { // nouveau niveau
                    valEnCours= param->pression[numEtape];
                    vannes->cmdVannePilotageG(valEnCours);
                    emit majData(valEnCours);
                } else {
                    emit majData(valEnCours);
                    this->msleep(param->pasRampe_en_ms[0]);
                }
            }
        }
        numEtape++;
        if(numEtape == nbEtape)
            stopped= true; // rampe perso terminée sans détection de changement
    }
    if((typeDistributeur==BISTABLE)&&(operation==DECLENCHEMENT))
        vannes->cmdVannePilotageD(0);
    else if((typeDistributeur==BISTABLE)&&(operation==ENCLENCHEMENT))
        vannes->cmdVannePilotageG(0);
//    qWarning("End ThreadRampe ");
}

void ThreadRampePneu::setPression(double p){
    if(p > 10.0)    // Pression max= 10.0
        p= 10.0;
    pression = p;
}

void ThreadRampePneu::setTypeDistributeur(bool type){
    typeDistributeur = type;
}

void ThreadRampePneu::setTypeRampe(bool type){
    typeRampe= type;
}

void ThreadRampePneu::setOperation(bool declen_enclen){
    operation = declen_enclen;
}

void ThreadRampePneu::setValEnCours(double v){
    valEnCours = v;
}

QString ThreadRampePneu::getMessage(void){
    return message;
}

int ThreadRampePneu::getErreur(void){
    return erreur;
}

void ThreadRampePneu::setVannes(VannesProportionnelle *v){
    vannes = v;
}

void ThreadRampePneu::stop(){
  stopped = true;
  message="Demande de fin de rampe";
}

