#include "threadrampeelec.h"

ThreadRampeElec::ThreadRampeElec(int choix){
    stopped = false;
    pas = 0.08;             // step; -0.1
    valMini = 0.0;
    operation = ENCLENCHEMENT;  // rampe montante par defaut
    tension = 24.0;    // par defaut
    delaiEchantillonage = 200; // rendre paramétrable
}

void ThreadRampeElec::run(){
    erreur=0;
    stopped = false;
    message.clear();

    char commande[80];
    valMax = tension;      // seuil;

    if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT)){
        valEnCours = (tension/2) - pas;   // origine rampe / tension - pas;
    } else {
        valEnCours = (tension/2) + pas;   // origine rampe (ne pas envoyer la valeur mini ou max)
    }
//    qWarning("Start ThreadRampe Elec => operation=%d distrib:%d valEnCours=%f (1:Enclenchement 0:Monostable)", operation, typeDistributeur, valEnCours);
    while (!stopped) {
        sprintf_s(commande,"V1 %.1f", valEnCours );
        alim->sendCommande(commande);
        emit majData(valEnCours);
//        if(typeDistributeur==BISTABLE)
        if(typeDistributeur==MONOSTABLE && operation==DECLENCHEMENT)
            valEnCours -= pas;
        else
            valEnCours += pas;
        if((valEnCours >= valMax) || (valEnCours <= valMini) ){
            message= "Fin de rampe";
            stopped= true;  // Valeur max atteinte, on arrête !
        }
        msleep(delaiEchantillonage);
    }
    if(typeDistributeur==BISTABLE){
        sprintf_s(commande,"V1 %.1f", 0.0 );
        alim->sendCommande(commande);
    }
//    qWarning("End ThreadRampeElec ");
}

void ThreadRampeElec::setTension(double t){
    if(t > 300.0)    // Pression max= 10.0
        t= 300.0;
    tension = t;
}

void ThreadRampeElec::setDevice(QString dev){
    device = dev;
}

void ThreadRampeElec::setTypeDistributeur(bool type){
    typeDistributeur = type;
}

void ThreadRampeElec::setOperation(bool declen_enclen){
    operation = declen_enclen;
}

void ThreadRampeElec::setValEnCours(double v){
    valEnCours = v;
}

QString ThreadRampeElec::getMessage(void){
    return message;
}

int ThreadRampeElec::getErreur(void){
    return erreur;
}

double ThreadRampeElec::getValEnCours(void){
    return valEnCours;
}

void ThreadRampeElec::setAlim(AppareilGPIB *power){
    this->alim = power;
}

void ThreadRampeElec::stop(){
  stopped = true;
  message="Demande de fin de rampe";
}
