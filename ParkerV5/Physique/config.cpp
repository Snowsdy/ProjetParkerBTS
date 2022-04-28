#include "config.h"

Config::Config(int choice)
{
    this->choix= choice;
    if(choix) { // lycée = 1
        // Vannes proportionnelles
        PILOTAGE_GAUCHE= 0;
        PILOTAGE_DROIT= 1;
        PILOTAGE_PRESSION= 2; // non dispo

        // Capteurs pressions
        SORTIE_DROITE= 0;
        CMD_PRESSION= 1;
        SORTIE_GAUCHE= 2;
        CMD_DROITE= 3;
        CMD_GAUCHE= 4;

        // Nombre de cartes Relais
        NB_MODULE_RELAIS= 1;
        MODULE4= 0;

        // Entrée TOR
        AU= 0;
        ETOR= 2;

        // Nom des devices NI
//        devCapteurP = "Dev1/ai0:7";
//        devVannesP= "Dev1/ao0:1";
//        devRelaisMod4 = "Dev1/port0/line0:7"; // Pilotage Droit
//        devETOR= "Dev1/port1/line0:3"; // Entrées TOR (AU et contact)

        devCapteurP = "Dev2/ai0:7";
        devVannesP= "Dev1/ao0:1";
        devRelaisMod4 = "Dev2/port0/line0:7"; // Pilotage Droit
        devETOR= "Dev2/port1/line0:3"; // Entrées TOR (AU et contact)

        // Commandes appareil GPIB
        listCmdGPIB.append("V1?");
        listCmdGPIB.append("I1?");
        listCmdGPIB.append("V2?");
        listCmdGPIB.append("I2?");
        listCmdGPIB.append("OP1 0");
        listCmdGPIB.append("OP1 1");
        listCmdGPIB.append("OP2 0");
        listCmdGPIB.append("OP2 1");
        listCmdGPIB.append("MASTER 0");
        listCmdGPIB.append("MASTER 1");

        // Adresse appareil GPIB
        listAdrGPIB.append("2");
    } else { // Parker = 0
        // Vannes proportionnelles
        PILOTAGE_GAUCHE= 2;
        PILOTAGE_DROIT= 0;
        PILOTAGE_PRESSION= 1;

        // Capteurs pressions
        SORTIE_DROITE= 3;
        CMD_PRESSION= 0;
        SORTIE_GAUCHE= 3;
        CMD_DROITE= 2;
        CMD_GAUCHE= 1;

        // Nombre de cartes Relais
        NB_MODULE_RELAIS= 3;
        MODULE4= 0;
        MODULE5= 1;
        MODULE6= 2;

        // Nom des devices NI
        devCapteurP = "cDAQ9188-1737C8DMod1/ai0:7";
        devVannesP= "cDAQ9188-1737C8DMod2/ao0:3";
        devRelaisMod4 = "cDAQ9188-1737C8DMod4/port0/line0:7"; // Pilotage Droit
        devRelaisMod5 = "cDAQ9188-1737C8DMod5/port0/line0:7"; // Pilotage Pression
        devRelaisMod6 = "cDAQ9188-1737C8DMod6/port0/line0:7"; // Pilotage Gauche
        devETOR= "A définir"; // Entrées TOR (AU et contact)

        // Commandes appareils GPIB
        listCmdGPIB.append("MEAS:VOLT?");
        listCmdGPIB.append("MEAS:CURR?");
        listCmdGPIB.append("MEAS:VOLT:AC?");
        listCmdGPIB.append("MEAS:CURR:AC?");
        listCmdGPIB.append("MEAS:POW:AC:APP?");
        listCmdGPIB.append("MEAS:POW:AC:REAC?");
        listCmdGPIB.append("MEAS:POW:AC:REAL?");
        listCmdGPIB.append("MEAS:POW:AC:PFAC?");
        listCmdGPIB.append("MEAS:RES?");
        listCmdGPIB.append("MEAS:TEMP?");
        listCmdGPIB.append("OUTP ON");
        listCmdGPIB.append("OUTP OFF");

        // Adresses appareils GPIB
        listAdrGPIB.append("5");
        listAdrGPIB.append("7");
//        listAdrGPIB.append("16");
//        listAdrGPIB.append("19");
    }
}

QList<QString> Config::getListCmdGPIB(){
    return listCmdGPIB;
}

QList<QString> Config::getListAdrGPIB(){
    return listAdrGPIB;
}

int Config::getChoix(){
    return choix;
}

void Config::setPILOTAGE_GAUCHE(const int &val)
{
    this->PILOTAGE_GAUCHE = val;
}

void Config::setPILOTAGE_DROIT(const int &val)
{
    this->PILOTAGE_DROIT = val;
}

void Config::setPILOTAGE_PRESSION(const int &val)
{
    this->PILOTAGE_PRESSION = val;
}

void Config::setSORTIE_DROITE(const int &val)
{
    this->SORTIE_DROITE = val;
}

void Config::setCMD_PRESSION(const int &val)
{
    this->CMD_PRESSION = val;
}

void Config::setSORTIE_GAUCHE(const int &val)
{
    this->SORTIE_GAUCHE = val;
}

void Config::setCMD_DROITE(const int &val)
{
    this->CMD_DROITE = val;
}

void Config::setCMD_GAUCHE(const int &val)
{
    this->CMD_GAUCHE = val;
}

void Config::setNB_MODULE_RELAIS(const int &val)
{
    this->NB_MODULE_RELAIS = val;
}

void Config::setAU(const int &val)
{
    this->AU = val;
}

void Config::setETOR(const int &val)
{
    this->ETOR = val;
}

void Config::setDevCapteurP(const QString &val)
{
    this->devCapteurP = val;
}

void Config::setDevVannesP(const QString &val)
{
    this->devVannesP = val;
}

void Config::setDevRelaisMod4(const QString &val)
{
    this->devRelaisMod4 = val;
}

void Config::setDevRelaisMod5(const QString &val)
{
    this->devRelaisMod5 = val;
}

void Config::setDevRelaisMod6(const QString &val)
{
    this->devRelaisMod6 = val;
}

void Config::setdevETOR(const QString &val)
{
    this->devETOR = val;
}

void Config::setELEC(const bool &val)
{
    this->ELEC = val;
}
