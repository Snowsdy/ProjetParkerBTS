#include "controleurmesure.h"

ControleurMesure::ControleurMesure(int choix){ // choix => 0:Parker, 1:Lycée
    conf = PhysiqueFactory::getConfigEnCours(choix);
    ELEC = conf->ELEC; // pour test sans GPIB
    capteursP = new CapteursPression(choix);
    vannesP = new VannesProportionnelle(choix);
    e_tor = new EntreeTOR(choix);
    for (int i = 0; i < 3; i++)
        relais[i] = nullptr;  //relais[i] = relai;
    for (int i = 0; i < conf->NB_MODULE_RELAIS; i++)
        relais[i] = new Relais(choix);  //relais[i] = relai;
    if(choix) {
        alimCC = PhysiqueFactory::getAlimentationCC();
    } else {
        alimCC = PhysiqueFactory::getAlimentationCC();
        alimAC = PhysiqueFactory::getAlimentationAC();
    }

    capteursP->setModuleNI(conf->devCapteurP);
    vannesP->setModuleNI(conf->devVannesP);
    relais[conf->MODULE4]->setModuleNI(conf->devRelaisMod4);
    if(!choix) { // 3 modules relais chez Parker
        relais[conf->MODULE5]->setModuleNI(conf->devRelaisMod5);
        relais[conf->MODULE6]->setModuleNI(conf->devRelaisMod6);
    } else { // A mettre en place chezParker
        e_tor->setModuleNI(conf->devETOR);
    }

    acqSeuilPneu = new ThreadAcqSeuilPneu(choix);
    acqTpsRepPneu = new ThreadAcqTpsRepPneu(choix);
    acqTpsRepElec = new ThreadAcqTpsRepElec(choix);
//    qWarning("Fin création Thread Acquisition");

    rampePneu = new ThreadRampePneu(choix);
    rampeElec = new ThreadRampeElec(choix);
//    qWarning("Fin création Thread rampe");

    etatMesure= false;
    connect(rampePneu, SIGNAL(finished()), this, SLOT(finRampePneu()));
    connect(rampeElec, SIGNAL(finished()), this, SLOT(finRampeElec()));
    connect(acqSeuilPneu, SIGNAL(finished()), this, SLOT(finAcqSeuilPneu()));
    connect(acqSeuilPneu, SIGNAL(seuilAtteint(double, double)), this, SLOT(detectSeuilPneu(double, double)));

    connect(acqTpsRepPneu, SIGNAL(finished()), this, SLOT(finAcqTpsRepPneu()));
    connect(acqTpsRepPneu, SIGNAL(tpsRepTrouve(int)),this, SLOT(detectTpsRep(int)));
    connect(acqTpsRepElec, SIGNAL(finished()), this, SLOT(finAcqTpsRepElec()));
    connect(acqTpsRepElec, SIGNAL(tpsRepTrouve(int)),this, SLOT(detectTpsRep(int)));
}

void ControleurMesure::startBanc(void) {
    int err, delai = 500;

    if((err = capteursP->startAcqCapteurs()) == 0){
        message = "Module capteurs de pression OK !";
        emit infoMatos(message);
        emit topProgress(10); // 0+10 = 10
    }
    Sleep(delai);
    if((err = vannesP->startCmdVannes()) == 0){
        message = "Module vannes proportionnelles OK !";
        emit infoMatos(message);
        emit topProgress(10); // 10+10 = 20
    }
    Sleep(delai);
    for(int i = 0; i < conf->NB_MODULE_RELAIS; i++) {
        if((err= relais[i]->startCmdRelais())==0){
            message.sprintf("Module relais n°%d OK", i+4);
            emit infoMatos(message);
            emit topProgress(30/conf->NB_MODULE_RELAIS); // pour Parker 20+10+10+10= 50 ou 20+30= 50 pour Lycée
        }
        Sleep(delai);
    }
    if((err = e_tor->startLectureE_TOR()) == 0){
        message = "Module entrées TOR OK !";
        emit infoMatos(message);
        emit topProgress(20); // 50+20 = 70
    }
    if(ELEC)
        startGPIB();
    else {
        emit infoMatos("GPIB non utilisé");
        emit topProgress(30); // 70+30= 100 => Init complet (BANC OK)
    }
    emit finInit();
}

void ControleurMesure::startGPIB(){
    if(conf->getChoix()){ // Init alimentation CC lycée
        if(alimCC->initDevice(conf->getListAdrGPIB().at(0).toInt()) == 0)
            message.sprintf("Appareil GPIB adresse %d: OK", conf->getListAdrGPIB().at(0).toInt());
        else
            message.sprintf("Problème appareil GPIB adresse %d", conf->getListAdrGPIB().at(0).toInt());
        emit infoMatos(message);
        emit topProgress(30); // 70+30= 100 => Init complet (BANC OK)
    }
    else { // Init alimentation CC Parker
        if(alimCC->initDevice(conf->getListAdrGPIB().at(0).toInt()) == 0) {
            message.sprintf("Appareil GPIB adresse %d: OK", conf->getListAdrGPIB().at(0).toInt());
        } else {
            message.sprintf("Problème appareil GPIB adresse %d", conf->getListAdrGPIB().at(0).toInt());
        }
        emit infoMatos(message);
        emit topProgress(15); // 70+15= 85
        // Init alimentation AC Parker
        if(alimAC->initDevice(conf->getListAdrGPIB().at(1).toInt()) == 0) {
            message.sprintf("Appareil GPIB adresse %d: OK", conf->getListAdrGPIB().at(1).toInt());
        } else {
            message.sprintf("Problème appareil GPIB adresse %d", conf->getListAdrGPIB().at(1).toInt());
        }
        emit infoMatos(message);
        emit topProgress(15); // 85+15= 100 => Init complet (BANC OK)
    }
}

void ControleurMesure::stopBanc(void){
    int err, delai = 500;

    if((err = capteursP->stopAcqCapteurs()) == 0){
        message = "Arrêt module capteurs de pression OK !";
        emit infoMatos(message);
        emit topProgress(-10); // 100-10= 90
    }
    Sleep(delai);
    if((err = vannesP->stopCmdVannes())==0){
        message = "Arrêt module vannes proportionnelles OK !";
        emit infoMatos(message);
        emit topProgress(-10); // 90-10= 80
    }
    Sleep(delai);
    for(int i = 0; i < conf->NB_MODULE_RELAIS; i++){
        if((err = relais[i]->stopCmdRelais())==0){
            message.sprintf("Arrêt module relais n°%d OK", i+4);
            emit infoMatos(message);
            emit topProgress(-30/conf->NB_MODULE_RELAIS);
        } // 80 -30= 50
        Sleep(delai);
    }
    if((err = e_tor->stopLectureE_TOR()) == 0){
        message = "Arrêt Module entrées TOR OK !";
        emit infoMatos(message);
        emit topProgress(-20); // 50-20= 30
    }
    if(ELEC)
        stopGPIB();
    else {
        emit infoMatos("GPIB non utilisé");
        emit topProgress(-30); // 30-30= 0 => Arrêt complet (BANC OFF)
    }
    emit finArret();
}

void ControleurMesure::stopGPIB(){
    alimCC->closeDevice();
    message = "Arrêt de l'alimentation CC OK !";
    emit infoMatos(message);
    emit topProgress(-15);
    if(!conf->getChoix()){
        alimAC->closeDevice(); // si Parker close Alim AC
        message = "Arrêt de l'alimentation AC OK !";
        emit infoMatos(message);
    }
    emit topProgress(-15);
    Sleep(500);
}

/*** TEST DE SEUIL PNEU ***/
void ControleurMesure::testPneuSeuil(bool type){
    double valPilotageInitial;
    mode = false; // Pneu
    compteurFinThread= 0;

    acqSeuilPneu->setCapteur(capteursP);
    acqSeuilPneu->setETOR(e_tor);
    acqSeuilPneu->setPression(this->pressionAlim);
    acqSeuilPneu->setOperation(operation);
    acqSeuilPneu->setTypeDistributeur(typeDistributeur);

    rampePneu->setPression(pressionAlim);
    rampePneu->setOperation(operation);
    rampePneu->setTypeDistributeur(typeDistributeur);
    rampePneu->setVannes(vannesP);
    rampePneu->setTypeRampe(type);
    acqSeuilPneu->setMultimetre(nullptr);

    desactivationRelaisTestElec();
    if(activationRelaisTestPneu() == 0){
        if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT)){ // rampe montante
            valPilotageInitial= pressionAlim;
        } else {  // rampe montante
            valPilotageInitial= 0.0;
        }
        if((typeDistributeur==BISTABLE) && (operation==DECLENCHEMENT)){
//            qWarning("bistable depilotage: Rampe montante pilotageD");
//            qWarning("Valeur pilotage initial cmd Gauche:0 cmd Droite: 0");
            vannesP->cmdVannePilotageG(0);
            vannesP->cmdVannePilotageD(0);
        }
        else {
            vannesP->cmdVannePilotageD(0);
            vannesP->cmdVannePilotageG(valPilotageInitial);
//            qWarning("Valeur pilotage initial cmd gauche: %f cmd Droite:0", valPilotageInitial);
        }
        Sleep(500);
        acqSeuilPneu->start();
        Sleep(200);
        rampePneu->start();
        mesureEnCours = true;
    }
    else{
        msgThread = "Problème de lancement du TEST DE SEUIL PNEU";
        emit finDeTest(mesureEnCours);
    }
}

/*** TEST DE SEUIL ELEC ***/
void ControleurMesure::testElecSeuil(void){
    double valPilotageInitial;
    QString s;
    mode = true; // Elec
    compteurFinThread=0;

    acqSeuilPneu->setCapteur(capteursP);
    acqSeuilPneu->setPression(this->pressionAlim);
    acqSeuilPneu->setOperation(operation);
    acqSeuilPneu->setTypeDistributeur(typeDistributeur);

    if(courant == CONTINU){
        acqSeuilPneu->setMultimetre(alimCC);
        rampeElec->setAlim(alimCC);
    } else {
        acqSeuilPneu->setMultimetre(alimAC);
        rampeElec->setAlim(alimAC);
    }
    rampeElec->setTension(tensionAlim);
    rampeElec->setOperation(operation);
    rampeElec->setTypeDistributeur(typeDistributeur);

    desactivationRelaisTestPneu();
    if(activationRelaisTestElec() == 0){
        if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT)){ // rampe descendante
            valPilotageInitial= tensionAlim;
        }
        else{  // rampe montante
            valPilotageInitial= tensionAlim/2.0; // 0.0;
        }
        if((typeDistributeur==BISTABLE) && (operation==DECLENCHEMENT)){
            // enclenchement relais adequat pour cde Droite
            if(relais[conf->MODULE6] != nullptr) {
                relais[conf->MODULE6]->desactiveRelais(4);
                relais[conf->MODULE6]->activeRelais(5); //pilotageD
            }
//            qWarning("Valeur pilotage initial cmd gauche: désactivée et cmd Droite: %f", valPilotageInitial);
        }
        else {
            if(relais[conf->MODULE6] != nullptr) {
                relais[conf->MODULE6]->desactiveRelais(5);
                relais[conf->MODULE6]->activeRelais(4); //pilotageG
            }
//            qWarning("Valeur pilotage initial cmd gauche: %f et cmd Droite: désactivée", valPilotageInitial);
        }
        QString rep;
        if(conf->getChoix()) { // Lycée
            s.sprintf("V1 %.1f", valPilotageInitial);
            alimCC->sendCommande(s.toLatin1().data());
            Sleep(500);
            alimCC->sendCommande((char*)"I1R?");
            alimCC->readData();
        } else { // Parker
            s.sprintf("VOLT %.1f", valPilotageInitial);
            if(courant == CONTINU)
                alimCC->sendCommande(s.toLatin1().data());
            else
                alimAC->sendCommande(s.toLatin1().data());
            Sleep(500);
            if(courant == CONTINU) {// Continu ou alternatif
                alimCC->sendCommande((char*)"MEAS:CURR:?");
                alimCC->readData();
                rep = alimCC->getBufRead();
            }
            else {
                alimAC->sendCommande((char*)"MEAS:CURR:AC?");
                alimAC->readData();
                rep = alimCC->getBufRead();
            }
        }
//        qWarning("courant avant acq = %s", rep.toLatin1().data());
        acqSeuilPneu->start();
        Sleep(200);
        rampeElec->start();
        mesureEnCours = true;
    } else {
        msgThread = "Problème de lancement du TEST DE SEUIL ELEC";
        emit finDeTest(mesureEnCours);
    }
}

/*** TEST DE TEMPS DE REPONSE PNEU ***/
void ControleurMesure::testPneuTpsRep(void){
    double valPilotageInitial;
    mode = false; // pneu
    msgThread.clear();

    acqTpsRepPneu->setCapteur(capteursP);
    acqTpsRepPneu->setVannes(vannesP);
    acqTpsRepPneu->setPression(this->pressionAlim);
    acqTpsRepPneu->setOperation(operation);
    acqTpsRepPneu->setTypeDistributeur(typeDistributeur);

    /*    if(operation==DECLENCHEMENT){
        if(mtp->getSeuilTensionDepilotageGouPilotageD()==0.0)
            msgThread = "Pas de seuil pneu dépilotage défini";
        else
            acqTpsRepPneu.setValSeuilDec(mtp->getSeuilTensionDepilotageGouPilotageD());
    }
    else {
        if(mtp->getSeuilTensionPilotageG()==0.0)
            msgThread = "Pas de seuil pneu pilotage défini";
        else
            acqTpsRepPneu.setValSeuilEnc(mtp->getSeuilTensionPilotageG());
    }
    if(!msgThread.isEmpty()){
        qWarning("Pas de seuil défini: %s", msgThread.toLatin1().data() );
        emit finDeTest(mesureEnCours);
        return;
    }*/

    desactivationRelaisTestElec();
    if(activationRelaisTestPneu()==0){
        valPilotageInitial=0.0;
        if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT)){ // rampe montante
            valPilotageInitial= pressionAlim;
        }
        else{  // rampe montante
            valPilotageInitial= 0.0;
        }
        if((typeDistributeur==BISTABLE) && (operation==DECLENCHEMENT)){
//            qWarning("bistable depilotage: Rampe montante pilotageD");
            /* Necessaire ???
            relais[MODULE4].desactiveRelais(0);
            relais[MODULE4].desactiveRelais(1); */
            vannesP->cmdVannePilotageG(0);
            vannesP->cmdVannePilotageD(0);
//            qWarning("Valeur pilotage initial cmd gauche: 0 cmd Droite: 0");
        }
        else {
            /* Necessaire ???
            relais[MODULE6].desactiveRelais(0);
            relais[MODULE6].desactiveRelais(1); */
            vannesP->cmdVannePilotageD(0);
            vannesP->cmdVannePilotageG(valPilotageInitial);
//            qWarning("Valeur pilotage initial cmd gauche: %f cmd Droite: 0", valPilotageInitial);
        }
        Sleep(500);
        acqTpsRepPneu->start();
        mesureEnCours = true;
    }
    else{
        msgThread = "Problème au lancement du test tps rep pneu ";
        emit finDeTest(mesureEnCours);
    }
}

/*** TEST DE TEMPS DE REPONSE ELEC ***/
void ControleurMesure::testElecTpsRep(void){
    double valPilotageInitial;
    QString s;
    mode = true; // elec

    acqTpsRepElec->setCapteur(capteursP);
    acqTpsRepElec->setPression(pressionAlim);
    acqTpsRepElec->setTension(tensionAlim);

    if(courant == CONTINU){
        acqTpsRepElec->setAlim(alimCC);
    } else {
        acqTpsRepElec->setAlim(alimAC);
    }

    acqTpsRepElec->setOperation(operation);
    acqTpsRepElec->setTypeDistributeur(typeDistributeur);

    desactivationRelaisTestPneu();
    if(activationRelaisTestElec()==0){
        if((typeDistributeur==MONOSTABLE) && (operation==DECLENCHEMENT)){ // rampe descendante
            valPilotageInitial= tensionAlim;
        }
        else{  // rampe montante
            valPilotageInitial= 0.0;
        }
        if((typeDistributeur==BISTABLE) && (operation==DECLENCHEMENT)){
            // enclenchement relais adequat pour cde Droite
            relais[conf->MODULE6]->desactiveRelais(4);
            relais[conf->MODULE6]->activeRelais(5);
//            qWarning("Valeur pilotage initial cmd gauche: désactivée cmd Droite: 0");
        }
        else {
            relais[conf->MODULE6]->desactiveRelais(5);
            relais[conf->MODULE6]->activeRelais(4);
//            qWarning("Valeur pilotage initial cmd gauche: %f cmd Droite: désactivée", valPilotageInitial);
        }
        if(conf->getChoix()) { // Lycée
            s.sprintf("V1 %.1f", valPilotageInitial);
            alimCC->sendCommande(s.toLatin1().data());
        } else { // Parker
            s.sprintf("VOLT %.1f", valPilotageInitial);
            if(courant == CONTINU)
                alimCC->sendCommande(s.toLatin1().data());
            else
                alimAC->sendCommande(s.toLatin1().data());
        }
        acqTpsRepElec->start(QThread::TimeCriticalPriority);
        mesureEnCours = true;
    }
    else {
        msgThread = "Problème au lancement du test tps rep elec ";
        emit finDeTest(mesureEnCours);
    }
}

int ControleurMesure::activationRelaisTestPneu(void){
    int err = 0;

    err = relais[conf->MODULE4]->activeRelais(0);
    err = relais[conf->MODULE4]->activeRelais(1);
    if (!conf->getChoix()){ // Parker
        err = relais[conf->MODULE5]->activeRelais(0);
        err = relais[conf->MODULE5]->activeRelais(1);
        err = relais[conf->MODULE6]->activeRelais(0);
        err = relais[conf->MODULE6]->activeRelais(1);
    }
    Sleep(500);
    return err;
}

int ControleurMesure::desactivationRelaisTestPneu(void){
    int err = 0;
    err = relais[conf->MODULE4]->desactiveRelais(0);
    err = relais[conf->MODULE4]->desactiveRelais(1);
    if (!conf->getChoix()) {
        //    err = relais[MODULE5].desactiveRelais(0);
        //    err = relais[MODULE5].desactiveRelais(1);
        err = relais[conf->MODULE6]->desactiveRelais(0);
        err = relais[conf->MODULE6]->desactiveRelais(1);
    }
    return err;
}

int ControleurMesure::activationRelaisTestElec(void){
    int err = 0;

    if (!conf->getChoix()){
        if(courant == CONTINU)
            err=relais[conf->MODULE4]->activeRelais(4); // source DC
        else
            err=relais[conf->MODULE5]->activeRelais(4); // source AC
        err=relais[conf->MODULE5]->activeRelais(0); // Pression
        err=relais[conf->MODULE5]->activeRelais(1); // Pression
    } else {
        err=relais[conf->MODULE4]->activeRelais(0);
        err=relais[conf->MODULE4]->activeRelais(1);
    }
    Sleep(500);
    return err;
}

int ControleurMesure::desactivationRelaisTestElec(void){
    int err = 0;

    if (!conf->getChoix()) {
        err=relais[conf->MODULE4]->desactiveRelais(4); // source DC
        err=relais[conf->MODULE5]->desactiveRelais(4); // source AC
        //    err=relais[MODULE5].desactiveRelais(0); // Pression
        //    err=relais[MODULE5].desactiveRelais(1); // Pression
        err=relais[conf->MODULE6]->desactiveRelais(4); // pilotage G
        err=relais[conf->MODULE6]->desactiveRelais(5); // pilotage D
    } else {
        err=relais[conf->MODULE4]->desactiveRelais(0);
        err=relais[conf->MODULE4]->desactiveRelais(1);
    }
    return err;
}

void ControleurMesure::finAcqSeuilPneu(void){
//    qWarning("ControleurMesure detect fin acq seuil pneu");
    compteurFinThread++;
    if(compteurFinThread == 2){ // si les 2 thread sont arrêtés on va à finTest
        if(acqSeuilPneu->getErreur()==0)
            etatMesure=true;
        else
            etatMesure=false;
        emit finDeTest(etatMesure);
    }
    else {  // sinon on arrête l'autre thread qui fera le emit finDeTest
        msgThread = acqSeuilPneu->getMessage();  // memo premiere cause d'arrêt
        if(mode == PNEUMATIQUE){
            if(rampePneu->isRunning()==true)
                rampePneu->stop();
        }
        else {
            if(rampeElec->isRunning()==true)
                rampeElec->stop();
        }
    }
}

void ControleurMesure::finAcqTpsRepPneu(){
    msgThread = acqTpsRepPneu->getMessage();
//    qWarning("ControleurMesure detect fin acq tps rep pneu : %s", msgThread.toLatin1().data());
    if(acqTpsRepPneu->getErreur()==0)
        etatMesure=true;
    else
        etatMesure=false;
    emit finDeTest(etatMesure);
}

void ControleurMesure::finAcqTpsRepElec(){
    msgThread = acqTpsRepElec->getMessage();
//    qWarning("ControleurMesure detect fin acq tps rep elec : %s", msgThread.toLatin1().data());
    if(acqTpsRepElec->getErreur()==0)
        etatMesure=true;
    else
        etatMesure=false;
    emit finDeTest(etatMesure);
}

void ControleurMesure::finRampePneu(){
//    qWarning("ControleurMesure detect fin rampe pneu");
    compteurFinThread++;
    if(compteurFinThread == 2){
        if(rampePneu->getErreur()==0)
            etatMesure=true;
        else
            etatMesure=false;
        emit finDeTest(etatMesure);
    }
    else {
        msgThread = rampePneu->getMessage(); // memo premiere cause d'arrêt
        if(acqSeuilPneu->isRunning()==true)
            acqSeuilPneu->stop();
    }
}

void ControleurMesure::finRampeElec(){
//    qWarning("ControleurMesure detect fin rampe elec");
    compteurFinThread++;
    if(compteurFinThread == 2){
        if(rampeElec->getErreur()==0)
            etatMesure=true;
        else
            etatMesure=false;
        emit finDeTest(etatMesure);
    }
    else {
        msgThread = rampeElec->getMessage(); // memo premiere cause d'arrêt
        if(acqSeuilPneu->isRunning()==true)
            acqSeuilPneu->stop();
    }
}

void ControleurMesure::detectSeuilPneu(double seuil, double seuilCourant){
//    qWarning("ControleurMesure detect changement d'état: %f %f", seuil, seuilCourant); // seuil pneu n'est pas valide en elec
    if(mode == ELECTRIQUE)
        seuil = rampeElec->getValEnCours();
    if(operation==ENCLENCHEMENT){
        mtp->setSeuilTensionPilotageG(seuil);
        mtp->setSeuilCourantPilotageG(seuilCourant);
        if(mode == PNEUMATIQUE)
            rampePneu->setValEnCours(pressionAlim - 0.8);    // pour arreter la rampe + vite si seuil atteint
        else
            rampeElec->setValEnCours(tensionAlim - 1);
    } else {
        if(typeDistributeur==MONOSTABLE){
            mtp->setSeuilTensionDepilotageGouPilotageD(seuil);
            mtp->setSeuilCourantDepilotageGouPilotageD(seuilCourant);
            if(mode == PNEUMATIQUE)
                rampePneu->setValEnCours(0.8);
            else
                rampeElec->setValEnCours(1.0);
        } else {
            mtp->setSeuilTensionDepilotageGouPilotageD(seuil);
            mtp->setSeuilCourantDepilotageGouPilotageD(seuilCourant);
            if(mode == PNEUMATIQUE)
                rampePneu->setValEnCours(pressionAlim - 0.8);
            else
                rampeElec->setValEnCours(tensionAlim - 1);
        }
    }
}

void ControleurMesure::detectTpsRep(int delai){
//    qWarning("ControleurMesure detect tps rep pneu : %d", delai);
    if(operation==ENCLENCHEMENT)
        mtp->setTpsRepPilotageG(delai);
    else
        mtp->setTpsRepDepilotageGouPilotageD(delai);
}

/*** Getteur et Accesseur ***/
double ControleurMesure::getTensionAlim(void){
    return tensionAlim;
}

double ControleurMesure::getPressionAlim(void){
    return pressionAlim;
}

bool ControleurMesure::getOperation(void){
    return operation;
}

bool ControleurMesure::getMode(void){
    return mode;
}

bool ControleurMesure::getTypeDistributeur(void){
    return typeDistributeur;
}

QString ControleurMesure::getMsgThread(void){
    return msgThread;
}

bool ControleurMesure::getMesureEnCours(void){
    return mesureEnCours;
}

void ControleurMesure::setMsgThread(QString m){
    msgThread = m;
}

void ControleurMesure::setPressionAlim(double val){
    pressionAlim = val;
}

void ControleurMesure::setTensionAlim(double val){
    tensionAlim = val;
}

void ControleurMesure::setOperation(bool declen_enclen){
    operation = declen_enclen;
}

void ControleurMesure::setTypeDistributeur(bool type){
    typeDistributeur = type;
}

void ControleurMesure::setMode(bool pneu_elec){
    mode = pneu_elec;
}

void ControleurMesure::setMesureEnCours(bool etat){
    mesureEnCours = etat;
}

void ControleurMesure::setMtp(MesuresTest *m){
    mtp = m;
}

void ControleurMesure::setCourant(bool typeCourant){
    courant = typeCourant;
}

