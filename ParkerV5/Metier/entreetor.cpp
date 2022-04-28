#include "entreetor.h"

EntreeTOR::EntreeTOR(int choix) {
//    qWarning("Constructeur EntreeTOR");
    carteDI = PhysiqueFactory::getCarteETOR(choix);
}

int EntreeTOR::startLectureE_TOR() {
    if((erreur = carteDI->initTacheDI())!=0)
        envoiInfo(erreur);
    return(erreur);
}

int EntreeTOR::stopLectureE_TOR(){
    if((erreur =carteDI->clearTacheDI())!=0)
        envoiInfo(erreur);
    return(erreur);
}

int EntreeTOR::lireTous_E_TOR(unsigned char *tabValDI){
    if((erreur = carteDI->lireEntreesDI(tabValDI)) !=0 )
        envoiInfo(erreur);
    return(erreur);
}

void EntreeTOR::setModuleNI(QString module){
    moduleNI = module;
    carteDI->setDeviceDI(module.toLatin1().data());
}

void EntreeTOR::envoiInfo(int err){
    msgErr.sprintf("Erreur carteDI: %s", carteDI->getMsgErreur(err));
    emit infoMatos(msgErr);
}
