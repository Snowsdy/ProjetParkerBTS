#include "physiquefactory.h"

CarteNI* PhysiqueFactory::carteRelais= nullptr;
CarteNI* PhysiqueFactory::carteETOR= nullptr;
CarteNI* PhysiqueFactory::carteVannes= nullptr;
CarteNI* PhysiqueFactory::carteCapteurs= nullptr;
AppareilGPIB* PhysiqueFactory::alimentationCC= nullptr;
AppareilGPIB* PhysiqueFactory::alimentationAC= nullptr;
Param* PhysiqueFactory::paramEnCours= nullptr;
Config* PhysiqueFactory::configEnCours= nullptr;

PhysiqueFactory::PhysiqueFactory(){
}

CarteNI* PhysiqueFactory::getCarteRelais(int choix){
  if((carteRelais == nullptr) && (carteETOR == nullptr) && (carteCapteurs == nullptr) ) { // config lycée
//    if((carteRelais == nullptr) && (carteETOR == nullptr) && (carteCapteurs == nullptr) && (carteVannes == nullptr)){ // pour test (1 carte)
        carteRelais = new CarteNI(choix);
        carteETOR= carteRelais;
        carteCapteurs= carteRelais;
//        carteVannes= carteRelais;
    }
    return carteRelais;
}

CarteNI* PhysiqueFactory::getCarteETOR(int choix){
    if((carteRelais == nullptr) && (carteETOR == nullptr) && (carteCapteurs == nullptr)) { // config lycée
//    if((carteRelais == nullptr) && (carteETOR == nullptr) && (carteCapteurs == nullptr) && (carteVannes == nullptr)){ // pour test (1 carte)
        carteETOR= new CarteNI(choix);
        carteRelais= carteETOR;
        carteCapteurs= carteETOR;
//        carteVannes= carteETOR;
    }
    return carteETOR;
}

CarteNI* PhysiqueFactory::getCarteCapteurs(int choix){
    if((carteRelais == nullptr) && (carteETOR == nullptr) && (carteCapteurs == nullptr)){ // config lycée
//    if((carteRelais == nullptr) && (carteETOR == nullptr) && (carteCapteurs == nullptr) && (carteVannes == nullptr)){ // pour test (1 carte)
        carteCapteurs= new CarteNI(choix);
        carteRelais= carteCapteurs;
        carteETOR= carteCapteurs;
//        carteVannes= carteCapteurs;
    }
    return carteCapteurs;
}

CarteNI* PhysiqueFactory::getCarteVannes(int choix){
    if(carteVannes == nullptr) { // config lycée
//    if((carteRelais == nullptr) && (carteETOR == nullptr) && (carteCapteurs == nullptr) && (carteVannes == nullptr)) { // pour test (1 carte)
           carteVannes= new CarteNI(choix);
//           carteRelais= carteVannes;
//           carteETOR= carteVannes;
           carteCapteurs= carteVannes;
    }
    return carteVannes;
}

AppareilGPIB *PhysiqueFactory::getAlimentationCC(){
    if(alimentationCC == nullptr)
        alimentationCC= new AppareilGPIB();
    return alimentationCC;
}

AppareilGPIB *PhysiqueFactory::getAlimentationAC(){
    if(alimentationAC == nullptr)
        alimentationAC= new AppareilGPIB();
    return alimentationAC; // déjà créé
}

Param* PhysiqueFactory::getParamEnCours(int choix){
    if(paramEnCours == nullptr)
        paramEnCours= new Param(choix);
    return paramEnCours;
}

Config* PhysiqueFactory::getConfigEnCours(int choix){
    if(configEnCours == nullptr)
        configEnCours= new Config(choix);
    return configEnCours;
}

