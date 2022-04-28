#ifndef PHYSIQUEFACTORY_H
#define PHYSIQUEFACTORY_H

#include "carteni.h"
#include "appareilGPIB.h"
#include "param.h"
#include "config.h"

class PhysiqueFactory
{
public:
    PhysiqueFactory();
    // choix => Parker (0) ou Lycée (1)
    static CarteNI* getCarteRelais(int choix);
    static CarteNI* getCarteETOR(int choix);
    static CarteNI* getCarteVannes(int choix);
    static CarteNI* getCarteCapteurs(int choix);
    static AppareilGPIB* getAlimentationCC();
    static AppareilGPIB* getAlimentationAC();
    static Param* getParamEnCours(int choix);
    static Config* getConfigEnCours(int choix);

private:
    static CarteNI* carteRelais; // relais
    static CarteNI* carteETOR; // Entrée contact TOR et AU
    static CarteNI* carteVannes; // vannes proportionnelles
    static CarteNI* carteCapteurs; // capteurs pression
    static AppareilGPIB* alimentationCC; // alimentation courant Continu
    static AppareilGPIB* alimentationAC; // alimentation courant Alternatif
    static Param *paramEnCours;
    static Config *configEnCours;
};

#endif // PHYSIQUEFACTORY_H
