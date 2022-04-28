#include "threadacqseuilpneu.h"

ThreadAcqSeuilPneu::ThreadAcqSeuilPneu(int choix){
    stopped = false;
    operation = ENCLENCHEMENT;  // par defaut
    pression = 5.0;             // par defaut
    capteurs = nullptr;
    multimetre = nullptr;
    valSeuilCourant = 0.0;
    valSeuil = 0.0;
    conf= PhysiqueFactory::getConfigEnCours(choix);
    param= PhysiqueFactory::getParamEnCours(choix);
    pas= param->getPasSeuil();
}

void ThreadAcqSeuilPneu::run(){
    erreur = 0;
    double limite;              // pourcentage de la pression de sortie considérée sortie active
    changement_d_etat = false;  // indicateur changement d'etat
    stopped = false;            // indicateur pour sortir du thread
    flagETOR = false;
    message.clear();
    int numE= conf->ETOR;

    for(int i= 0; i < 8; i++)
        data[i]= 0.0;

    if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT))
        limite = pression * param->getCoefLimitePressionDeclenchement(); // limite= pression*0.1; soit 10% pression alim
    else
        limite = pression * param->getCoefLimitePressionEnclenchement();  // limite = 90% pression alim

    qWarning("Start ThreadAcqSeuilPneu => limite:%f operation=%d distrib:%d (1:Enclenchement 0:Monostable)",\
             limite, operation, typeDistributeur);
    while (!stopped) {  // boucle d'acquisition
        erreur= capteurs->acqCapteurs(data); // lecture entrée analogique
        if(erreur){
            message = "ERREUR=>LireEntreesAI";
            stopped = true;
        } else {
            double a, b;
            for(int i = 0; i < 8; i++) {
                param->getAI(i, a, b);
                dataEnBars[i]= data[i]*a + b; // correctif type ax+b
            }
            erreur= etor->lireTous_E_TOR(dataETOR);
            if(erreur){
               message = "ERREUR=>LireEntreesTOR";
               stopped = true;
             } else {
                if(dataETOR[numE] == 1 ) // passe à 0 volt
                    flagETOR= true;
            }
        }
        if(!erreur) { // lecture AI et DI OK
/*            qWarning("CG(14)=%0.2f CD(12)=%0.2f SG(4)=%0.2f SD(2)=%0.2f P(1)=%0.2f eTOR= %d",\
            dataEnBars[conf->CMD_GAUCHE], dataEnBars[conf->CMD_DROITE], dataEnBars[conf->SORTIE_GAUCHE],\
            dataEnBars[conf->SORTIE_DROITE], dataEnBars[conf->CMD_PRESSION], flagETOR ); */
            if(changement_d_etat == false) {
                if(((dataEnBars[conf->SORTIE_GAUCHE] > limite) && (operation == ENCLENCHEMENT)) || flagETOR || \
                ((typeDistributeur==MONOSTABLE) && (dataEnBars[conf->SORTIE_GAUCHE] < limite) && (operation==DECLENCHEMENT))|| \
                ((typeDistributeur==BISTABLE) && (dataEnBars[conf->SORTIE_GAUCHE] < limite) && (operation==DECLENCHEMENT))) {
                    qWarning("changement d'état");
                    changement_d_etat = true;
                    if( flagETOR) {
                        qWarning("Changement d'état Entrée contact détectée");
                        valSeuil= (float) dataEnBars[conf->CMD_GAUCHE];
                    } else {
                        if((typeDistributeur == BISTABLE) && (operation == DECLENCHEMENT)){
                            valSeuil= (float) dataEnBars[conf->CMD_DROITE]; qWarning("Passage CMD_DROITE");
                        }
                        else{
                            valSeuil= (float) dataEnBars[conf->CMD_GAUCHE]; qWarning("Passage CMD_GAUCHE");
                        }
                    }
                    if( multimetre != nullptr ) { // Test Elec => mesure de courant
                        QString cmd, rep;
                        int lg;
                        if(conf->getChoix()) { // Lycée
                            cmd="I1R?";
                        } else { // Parker
                            if(multimetre->getPrimaryAddress() == conf->getListAdrGPIB().at(0))// Continu
                                cmd="MEAS:CURR?";
                            else    // ou alternatif
                                cmd="MEAS:CURR:AC?";
                        }
                        //  qWarning("Envoi commande : %s", cmd.toLatin1().data());
                        multimetre->sendCommande(cmd.toLatin1().data());
                        multimetre->readData();
                        rep = multimetre->getBufRead();
                        lg= rep.length()-2;
                        rep= rep.rightJustified(lg, 'x', true);
                        qWarning("COURANT = %s, %d", rep.toLatin1().data(), lg);
                        valSeuilCourant = rep.toFloat()*1000;
                    }
                    qWarning("Seuil atteint: %f %f", valSeuil, valSeuilCourant );
                    emit seuilAtteint(valSeuil, valSeuilCourant);
                }
            }
            this->msleep(pas); // temps d'échantillonnage pour test de seuil
            emit majData(dataEnBars);
        }
    }
    multimetre = nullptr;
    valSeuilCourant = 0.0;
    //    qWarning("End ThreadAcqSeuilPneu");
}

bool ThreadAcqSeuilPneu::getChangement_d_etat(void){
    return changement_d_etat;
}

QString ThreadAcqSeuilPneu::getMessage(void){
    return message;
}

int ThreadAcqSeuilPneu::getErreur(void){
    return erreur;
}

void ThreadAcqSeuilPneu::setTypeDistributeur(bool type){
    typeDistributeur = type;
}

void ThreadAcqSeuilPneu::setOperation(bool declen_enclen){
    operation = declen_enclen;
}

void ThreadAcqSeuilPneu::setPression(double p){
    if(p > 10.0)    // Pression max= 10.0
        p= 10.0;
    pression = p;
}

void ThreadAcqSeuilPneu::setCapteur(CapteursPression *capteur){
    this->capteurs = capteur;
}

void ThreadAcqSeuilPneu::setETOR(EntreeTOR* e){
    this->etor= e;
}

void ThreadAcqSeuilPneu::stop(){
    stopped = true;
    message="Demande arrêt d'acquisition";
}

void ThreadAcqSeuilPneu::setMultimetre(AppareilGPIB *multim){
    this->multimetre = multim;
}
