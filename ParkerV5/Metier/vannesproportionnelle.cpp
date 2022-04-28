#include "vannesproportionnelle.h"
#include "Physique/physiquefactory.h"

VannesProportionnelle::VannesProportionnelle(int choix){

//    qWarning("Constructeur VannesProportionnelles");
    carteAO = PhysiqueFactory::getCarteVannes(choix);
    conf = PhysiqueFactory::getConfigEnCours(choix);
    for(int i=0; i<4; i++)
        tabVal[i]= 0.0;
}

int VannesProportionnelle::startCmdVannes(void){
    if((erreur = carteAO->initTacheAO())!=0)
        envoiInfo(erreur);
    return(erreur);
}

int VannesProportionnelle::cmdVannePression(double val){
    tabVal[conf->PILOTAGE_PRESSION]= val;
    if((erreur = carteAO->ecrireSortieAO(tabVal))!=0)
        envoiInfo(erreur);
    return(erreur);
}

int VannesProportionnelle::cmdVannePilotageG(double val){
    tabVal[conf->PILOTAGE_GAUCHE]= val;
    if((erreur = carteAO->ecrireSortieAO(tabVal))!=0)
        envoiInfo(erreur);
    return(erreur);
}

int VannesProportionnelle::cmdVannePilotageD(double val){
    tabVal[conf->PILOTAGE_DROIT]= val;
    if((erreur = carteAO->ecrireSortieAO(tabVal))!=0)
        envoiInfo(erreur);
    return(erreur);
}

int VannesProportionnelle::stopCmdVannes(void){
    for(int i=0; i<4; i++)
        tabVal[i]=0;
    carteAO->ecrireSortieAO(tabVal); // RAZ toutes les vannes
    if((erreur = carteAO->clearTacheAO())!=0)
        envoiInfo(erreur);
    return(erreur);
}

double VannesProportionnelle::getTabVal(int indice){
    if(indice > 3){
//        qWarning("Débordement num Vannes Proportionnelles");
        indice= 0;
    }
    return(tabVal[indice]);
}

void VannesProportionnelle::setModuleNI(QString module){
    moduleNI = module;
    carteAO->setDeviceAO(module.toLatin1().data());
}

void VannesProportionnelle::envoiInfo(int err){
    msgErr.sprintf("Erreur carteAO: %s", carteAO->getMsgErreur(err));
    emit infoMatos(msgErr);
}
