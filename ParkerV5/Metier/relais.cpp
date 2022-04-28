#include "relais.h"

Relais::Relais(int choix){
//    qWarning("Constructeur Relais");
    carteDO = PhysiqueFactory::getCarteRelais(choix);
    for(int i=0; i<8; i++)
        tabValDO[i]=0;
}

int Relais::startCmdRelais(void){
    if((erreur = carteDO->initTacheDO())!=0)
        envoiInfo(erreur);
    return(erreur);
}

int Relais::activeRelais(int numero){
    tabValDO[numero]=1;
    if((erreur =carteDO->ecrireSortieDO(tabValDO))!=0)
        envoiInfo(erreur);
    Sleep(100);
    return(erreur);
}

int Relais::desactiveRelais(int numero){
    tabValDO[numero]=0;
    if((erreur =carteDO->ecrireSortieDO(tabValDO))!=0)
        envoiInfo(erreur);
    Sleep(100);
    return(erreur);
}

int Relais::desactiveTousRelais(void){
    for(int i=0; i<8; i++)
        tabValDO[i]=0;
    if((erreur =carteDO->ecrireSortieDO(tabValDO))!=0)
        envoiInfo(erreur);
    Sleep(100);
    return(erreur);
}

int Relais::stopCmdRelais(void){
    desactiveTousRelais();
    if((erreur =carteDO->clearTacheDO())!=0)
        envoiInfo(erreur);
    return(erreur);
}

bool Relais::getEtatRelais(int numero){
    if(numero > 7){
//        qWarning("Débordement num Relais");
        numero=0;
    }
    if(tabValDO[numero])
        return true;
    else
        return false;
}

void Relais::setModuleNI(QString module){
    moduleNI = module;
    carteDO->setDeviceDO(module.toLatin1().data());
}

void Relais::envoiInfo(int err){
    msgErr.sprintf("Erreur carteDO: %s", carteDO->getMsgErreur(err));
    emit infoMatos(msgErr);
}
