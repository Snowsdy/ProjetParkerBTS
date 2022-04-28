#include "carteni.h"
#include <Windows.h>
#include <QDebug>

CarteNI::CarteNI(int choix){
//    qWarning("Constructeur carte NI");
    choice = choix;
    this->msgErreur[0]= '\0';
}

CarteNI::~CarteNI() {
//    qWarning("Destructeur carte NI");
    if(taskHandleAO != nullptr) {
        DAQmxClearTask(taskHandleAO);
    }
    if(taskHandleAI != nullptr) {
        DAQmxClearTask(taskHandleAI);
    }
    if(taskHandleDO != nullptr) {
        DAQmxClearTask(taskHandleDO);
    }
}

int CarteNI::initTacheAI(){
    error= 0;
    taskHandleAI= nullptr;

    if((error = DAQmxCreateTask("", &taskHandleAI)) != 0) return error;
//    qWarning("Create taskHandleAI %s OK", deviceAI);

    if ( !choice ) {    // "Parker"
        if((error = DAQmxCreateAIVoltageChan(taskHandleAI, deviceAI, "", DAQmx_Val_RSE, 0.0, 10.0, DAQmx_Val_Volts, nullptr))!=0) return error;
    } else {            // Lycée
        if((error = DAQmxCreateAIVoltageChan(taskHandleAI, deviceAI, "", DAQmx_Val_RSE, 0.0, 5.0, DAQmx_Val_Volts, nullptr))!=0) return error;
    }
//    qWarning("Config taskHandleAI %s OK", deviceAI);

    if((error = DAQmxStartTask(taskHandleAI)) != 0) return error;
//    qWarning("Start taskHandleAI %s OK", deviceAI);
    return error;
}

int CarteNI::lireEntreesAI(double* valIn) {
    int32   read;   // nombre de mesures réellement lues
    error= DAQmxReadAnalogF64(taskHandleAI, 1, 10.0,DAQmx_Val_GroupByChannel, valIn, 8, &read, nullptr);
    return error;
}

int CarteNI::clearTacheAI(void){
    if(taskHandleAI != nullptr) {
        error= DAQmxStopTask(taskHandleAI);
        error= DAQmxClearTask(taskHandleAI);
    }
    else
        error= 0;
//    qWarning("Stop et Clear taskHandleAI %s OK", deviceAI);
    return error;
}

int CarteNI::initTacheAO(){
    error= 0;
    taskHandleAO= nullptr;

    if((error = DAQmxCreateTask("", &taskHandleAO)) != 0) return error;
//    qWarning("Création taskHandleAO %s OK", deviceAO);

    if ( ! choice ) { // "Parker"
        if((error = DAQmxCreateAOVoltageChan(taskHandleAO, deviceAO, "", 0.0, 10.0, DAQmx_Val_Volts, ""))!=0) return error;
    } else {
        if((error = DAQmxCreateAOVoltageChan(taskHandleAO, deviceAO, "", 0.0, 10.0, DAQmx_Val_Volts, ""))!=0) return error;
    }
//    qWarning("Config taskHandleAO %s OK", deviceAO);
    if((error = DAQmxStartTask(taskHandleAO)) != 0) return error;
//    qWarning("Start taskHandleAO %s OK", deviceAO);
    return error;
}

int CarteNI::ecrireSortieAO(double *valOut) {
    error= DAQmxWriteAnalogF64(taskHandleAO, 1, 1, 10.0, DAQmx_Val_GroupByChannel, valOut, nullptr, nullptr);
    return error;
}

int CarteNI::clearTacheAO(void){
    if(taskHandleAO != nullptr) {
        error = DAQmxStopTask(taskHandleAO);
        error = DAQmxClearTask(taskHandleAO);
    }
    else
        error = 0;
//    qWarning("Stop et Clear taskHandleAO %s OK", deviceAO);
    return error;
}

int CarteNI::initTacheDI() {
    error = 0;
    this->taskHandleDI = nullptr;

    if((error = DAQmxCreateTask("", &taskHandleDI)) != 0) return error;
//    qWarning("Création taskHandleDI %s OK", deviceDI);
    if((error = DAQmxCreateDIChan(taskHandleDI, deviceDI, "", DAQmx_Val_ChanForAllLines)) != 0) return error;
//    qWarning("Config taskHandleDI %s OK", deviceDI);
    if((error = DAQmxStartTask(taskHandleDI)) != 0) return error;
//    qWarning("Start taskHandleDI %s OK", deviceDI);
    return error;
}

int CarteNI::lireEntreesDI(unsigned char *dataLu) {
    error= DAQmxReadDigitalLines(this->taskHandleDI, 1, 10.0, DAQmx_Val_GroupByChannel, dataLu, 4, nullptr, nullptr, nullptr);
    return error;
}

int CarteNI::clearTacheDI() {
    if(taskHandleDI != nullptr) {
        error = DAQmxStopTask(taskHandleDI);
        error = DAQmxClearTask(taskHandleDI);
    }
    else
        error = 0;
//    qWarning("Stop et Clear taskHandleDI %s OK", deviceDI);
    return error;
}

int CarteNI::initTacheDO(){
    error = 0;
    this->taskHandleDO = nullptr;

    if((error = DAQmxCreateTask("", &taskHandleDO)) != 0) return error;
//    qWarning("Création taskHandleDO %s OK", deviceDO);
    if((error = DAQmxCreateDOChan(taskHandleDO, deviceDO, "", DAQmx_Val_ChanForAllLines)) != 0) return error;
//    qWarning("Config taskHandleDO %s OK", deviceDO);
    if((error = DAQmxStartTask(taskHandleDO)) != 0) return error;
//    qWarning("Start taskHandleDO %s OK", deviceDO);
    return error;
}

int CarteNI::ecrireSortieDO(unsigned char* dataEcrit) {
    error = DAQmxWriteDigitalLines(taskHandleDO, 1, 1, 24.0, DAQmx_Val_GroupByChannel, dataEcrit, nullptr, nullptr);
    return (error);
}

int CarteNI::clearTacheDO(void){
    if(taskHandleDO != nullptr) {
        error = DAQmxStopTask(taskHandleDO);
        error = DAQmxClearTask(taskHandleDO);
    }
    else
        error = 0;
//    qWarning("Stop et Clear taskHandleDO %s OK", deviceDO);
    return error;
}

void CarteNI::setDeviceAI(char *dev){
    strcpy_s(deviceAI, dev );
//    qWarning("deviceAI= %s", deviceAI);
}

void CarteNI::setDeviceAO(char *dev){
    strcpy_s(deviceAO, dev );
//    qWarning("deviceAO= %s", deviceAO);
}

void CarteNI::setDeviceDI(char *dev){
    strcpy_s(deviceDI, dev );
//    qWarning("deviceDI= %s", deviceDI);
}

void CarteNI::setDeviceDO(char *dev){
    strcpy_s(deviceDO, dev );
//    qWarning("deviceDO= %s", deviceDO);
}

char* CarteNI::getMsgErreur(int err){
    if( DAQmxFailed(err) )
        DAQmxGetExtendedErrorInfo(msgErreur, 2048);
    return msgErreur;
}
