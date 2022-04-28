#include "mesurestest.h"

MesuresTest::MesuresTest(){
}

void MesuresTest::razMesures(void){
    dateTest.setDate(0,0,0);
    ref.clear();
    seuilTensionPilotageG = 0.0;
    seuilTensionDepilotageGouPilotageD = 0.0;
    seuilCourantPilotageG = 0.0;
    seuilCourantDepilotageGouPilotageD = 0.0;
    tpsRepPilotageG = 0;
    tpsRepDepilotageGouPilotageD = 0;
}

/*** Getteur ***/
QDate MesuresTest::getDateTest(void){
    return dateTest;
}

QString MesuresTest::getRef(void){
    return ref;
}

QString MesuresTest::getDesignation(void){
    return designation;
}

bool MesuresTest::getIndicateurGommage(void){
    return indicateurGommage;
}

double MesuresTest::getSeuilCourantPilotageG(){
return seuilCourantPilotageG;
}

double MesuresTest::getSeuilCourantDepilotageGouPilotageD(){
    return seuilCourantDepilotageGouPilotageD;
}

double MesuresTest::getSeuilTensionPilotageG(void){
    return seuilTensionPilotageG;
}

double MesuresTest::getSeuilTensionDepilotageGouPilotageD(void){
    return seuilTensionDepilotageGouPilotageD;
}

double MesuresTest::getTensionAlimentation(void){
    return tensionAlimentation;
}

double MesuresTest::getPressionAlimentation(void){
    return pressionAlimtation;
}

int MesuresTest::getTpsRepPilotageG(void){
    return tpsRepPilotageG;
}

int MesuresTest::getTpsRepDepilotageGouPilotageD(void){
    return tpsRepDepilotageGouPilotageD;
}

/*** Setteur ***/
void MesuresTest::setDateTest(QDate d){
    dateTest = d;
}

void MesuresTest::setRef(QString reference){
    ref = reference;
}

void MesuresTest::setDesignation(QString design){
    designation = design;
}

void MesuresTest::setIndicateurGommage(bool flag){
    indicateurGommage = flag;
}

void MesuresTest::setSeuilCourantPilotageG(double val){
    seuilCourantPilotageG = val;
}

void MesuresTest::setSeuilCourantDepilotageGouPilotageD(double val){
    seuilCourantDepilotageGouPilotageD = val;
}

void MesuresTest::setSeuilTensionPilotageG(double val){
    seuilTensionPilotageG= val;
}

void MesuresTest::setSeuilTensionDepilotageGouPilotageD(double val){
    seuilTensionDepilotageGouPilotageD= val;
}

void MesuresTest::setTpsRepPilotageG(int val){
    tpsRepPilotageG= val;
}

void MesuresTest::setTpsRepDepilotageGouPilotageD(int val){
    tpsRepDepilotageGouPilotageD= val;
}

void MesuresTest::setTensionAlimentation(double t){
    tensionAlimentation = t;
}

void MesuresTest::setPressionAlimentation(double p){
    pressionAlimtation = p;
}
