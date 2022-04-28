#include "capteurspression.h"
#include "physiquefactory.h"

CapteursPression::CapteursPression(int choix){
//    qWarning("Constructeur CapteursPression");
    carteAI = PhysiqueFactory::getCarteCapteurs(choix);
}

int CapteursPression::startAcqCapteurs(){
    if((erreur = carteAI->initTacheAI())!=0)
        envoiInfo(erreur);
    return(erreur);
}

int CapteursPression::acqCapteurs(double *val){
    if((erreur = carteAI->lireEntreesAI(val))!=0)
        envoiInfo(erreur);
    return(erreur);
}

int CapteursPression::stopAcqCapteurs(){
    if((erreur = carteAI->clearTacheAI())!=0)
        envoiInfo(erreur);
    return(erreur);
}

void CapteursPression::setModuleNI(QString module){
    moduleNI = module;
    carteAI->setDeviceAI(module.toLatin1().data());
}

void CapteursPression::envoiInfo(int err){
    msgErr.sprintf("Erreur carteAI:%s", carteAI->getMsgErreur(err));
    emit infoMatos(msgErr);
}
