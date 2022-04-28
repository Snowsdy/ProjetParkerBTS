#include "appareilGPIB.h"

AppareilGPIB::AppareilGPIB(){
//    qWarning("Creation appareil GPIB");
    alimErreur = 0;
    boardIndex = GPIB0;
    secondaryAddress = 0;
    for(int i=0; i<20; i++)
        psu[i]= 0;
}

void AppareilGPIB::writeData (char *buf){
//    qWarning("%x %s %d", psu[primaryAddress], buf, strlen(buf));
    ibwrt (psu[primaryAddress], buf, strlen(buf));
}

int AppareilGPIB::readData(){
    alimErreur = 0;
    ibrd (psu[primaryAddress], bufRead, sizeof(bufRead));
    if (Ibsta() & ERR)
        alimErreur= gpibError();   // Check for GPIB Error
    else
        bufRead[Ibcnt()]='\0';
    return(alimErreur);
}

int AppareilGPIB::scan(void){
    alimErreur = 0;
    SendIFC(GPIB0);
    if (Ibsta() & ERR) {
//        qWarning("Unable to open board");
        return -1;
    }

    for (loop = 0; loop < 30; loop++) {
        Instruments[loop] = loop + 1;
    }
    Instruments[30] = NOADDR;

//    qWarning("\nFinding all listeners on the bus...\n\n");
    FindLstn(GPIB0, Instruments, Result, 31);
    if (Ibsta() & ERR) {
//        qWarning("Unable to issue FindLstn call");
        return  -1;
    }

    Num_Listeners = Ibcnt();
//    qWarning("Number of Instruments found = %d\n\n", Num_Listeners);
    Result[Num_Listeners] = NOADDR;

    DevClearList(GPIB0, Result);
    if (Ibsta() & ERR) {
//        qWarning("Unable to clear devices");
        return  -1;
    }

    SendList(GPIB0, Result, "*IDN?", 5L, NLend);
    if (Ibsta() & ERR) {
//        qWarning("Unable to write to devices");
        return  -1;
    }

    for (loop = 0; loop < Num_Listeners; loop++) {
        Receive(GPIB0, Result[loop], bufRead, sizeof(bufRead), STOPend);
        if (Ibsta() & ERR) {
//            qWarning("Unable to read from a device");
            return  -1;
        }
        bufRead[Ibcnt()] = '\0';
//        qWarning("Returned string: %s", bufRead);
    } /* End of FOR loop */
    /* Take the board offline  */
    alimErreur= ibonl(GPIB0, 0);
//    qWarning("Take the board offline : %d", alimErreur);
    return(alimErreur);
}

int AppareilGPIB::initDevice(int ad){
    alimErreur = 0;
    if ((ad < 0) || (ad > 30))
        return -1;
    primaryAddress= ad;
//    qWarning("%d %d %d", boardIndex, primaryAddress, secondaryAddress);
    psu[primaryAddress] = ibdev( /* Create a unit descriptor handle         */
                 boardIndex,        /* Board Index (GPIB0 = 0, GPIB1 = 1, ...) */
                 primaryAddress,    /* Device primary address                  */
                 secondaryAddress,  /* Device secondary address                */
                 T10s,              /* Timeout setting (T10s = 10 seconds)     */
                 1,                 /* Assert EOI line at end of write         */
                 0);                /* EOS termination mode                    */
    if(psu[primaryAddress] == -1)
        return Ibsta();
//    qWarning("ibdev OK => psu[%d] = %x", primaryAddress, psu[primaryAddress]);

    // Clear the device
    ibclr (psu[primaryAddress]);
    if (Ibsta() & ERR) {
        alimErreur= gpibError();   // Check for GPIB Error
        return(alimErreur);
    }
//    qWarning("ibclr OK");
    // Identification
    if((alimErreur= sendCommande((char*)"*IDN?")) == -1) // get identifier string
        return alimErreur;
//    qWarning("*IDN? OK");

    if ((alimErreur=readData()) == -1)
        return alimErreur;
//    qWarning("Identification: %s", bufRead);
    switch(primaryAddress){
    case 2:
        alimErreur= sendCommande((char *)"I1 1");       // affecte un courant max (1 ampère)
        alimErreur= sendCommande((char *)"V1 0");       // Positionne l'alim à 0 volt
        alimErreur= sendCommande((char*)"MASTER 1");    // Autorise la tension sur le sortie 1
        alimErreur= sendCommande((char*)"OP1 1");       // active la sortie 1
        break;
    case 5:  break;
    case 7:
        alimErreur= sendCommande((char *)"CURR 1");     // affecte un courant max (1 ampère)
        alimErreur= sendCommande((char *)"VOLT 0");     // Positionne l'alim à 0 volt
        alimErreur= sendCommande((char*)"OUTP ON");     // Autorise la sortie en tension
    }
    return alimErreur;
}

int AppareilGPIB::getPrimaryAddress(){
    return primaryAddress;
}

int AppareilGPIB::sendCommande (char *cmd_buf){
    alimErreur = 0;
    sprintf_s (output_buf, "%s", cmd_buf);
    writeData(cmd_buf);
    if (Ibsta() & ERR)
        alimErreur=gpibError();
    return(alimErreur);
}

void AppareilGPIB::closeDevice(){
    if (psu[primaryAddress] > 0) {
        switch(primaryAddress){
        case 2:
            alimErreur= sendCommande((char*)"OP1 0");       // désactive la sortie 1
            alimErreur= sendCommande((char *)"V1 0");       // Positionne l'alim à 0 volt
            alimErreur= sendCommande((char*)"MASTER 0");    // Coupe la tension sur les sorties
            break;
        case 5:
        case 7:
            alimErreur= sendCommande((char*)"OUTP OFF");     // Coupe la tension sur les sorties
            break;
        }
        ibonl(psu[primaryAddress], 0);  /* Take the device offline */
        psu[primaryAddress] = 0;
    }
}

int AppareilGPIB::gpibError() { // retourne 0 si OK sinon -1
    alimErreur = 0;
    msgErreur.clear();
    msgStatus.clear();
    if (Ibsta() & ERR ){
        if (Iberr() == EDVR) msgErreur =" EDVR <Driver error>";
        if (Iberr() == ECIC) msgErreur =" ECIC <Not Controller-In-Charge>";
        if (Iberr() == ENOL) msgErreur =" ENOL <No Listener>";
        if (Iberr() == EADR) msgErreur =" EADR <Address error>";
        if (Iberr() == EARG) msgErreur =" EARG <Invalid argument>";
        if (Iberr() == ESAC) msgErreur =" ESAC <Not System Controller>";
        if (Iberr() == EABO) msgErreur =" EABO <Operation aborted>";
        if (Iberr() == ENEB) msgErreur =" ENEB <No GPIB board>";
        if (Iberr() == EOIP) msgErreur =" EOIP <Async I/O in progress>";
        if (Iberr() == ECAP) msgErreur =" ECAP <No capability>";
        if (Iberr() == EFSO) msgErreur =" EFSO <File system error>";
        if (Iberr() == EBUS) msgErreur =" EBUS <Command error>";
        if (Iberr() == ESTB) msgErreur =" ESTB <Status byte lost>";
        if (Iberr() == ESRQ) msgErreur =" ESRQ <SRQ stuck on>";
        if (Iberr() == ETAB) msgErreur =" ETAB <Table Overflow>";
        if (Iberr() == ELCK) msgErreur =" ELCK <Lock error>";
        if (Iberr() == EARM) msgErreur =" EARM <Ibnotify rearm error>";
        if (Iberr() == EHDL) msgErreur =" EHDL <Invalid Handle>";
        if (Iberr() == EWIP) msgErreur =" EWIP <Wait already in progress>";
        if (Iberr() == ERST) msgErreur =" ERST <Notification cancelled due to reset>";
        if (Iberr() == EPWR) msgErreur =" EPWR <Power error>";
    }
    if (Ibsta() & TIMO)  msgStatus =" TIMO";
    if (Ibsta() & END )  msgStatus =" END";
    if (Ibsta() & SRQI)  msgStatus =" SRQI";
    if (Ibsta() & RQS )  msgStatus =" RQS";
    if (Ibsta() & CMPL)  msgStatus =" CMPL";
    if (Ibsta() & LOK )  msgStatus =" LOK";
    if (Ibsta() & REM )  msgStatus =" REM";
    if (Ibsta() & CIC )  msgStatus =" CIC";
    if (Ibsta() & ATN )  msgStatus =" ATN";
    if (Ibsta() & TACS)  msgStatus =" TACS";
    if (Ibsta() & LACS)  msgStatus =" LACS";
    if (Ibsta() & DTAS)  msgStatus =" DTAS";
    if (Ibsta() & DCAS)  msgStatus =" DCAS";

//    if(!msgStatus.isEmpty())
//        qWarning("Status = %s", msgStatus.toLatin1().data());

    if(!msgErreur.isEmpty()){
        /* Call ibonl to take the device and interface offline */
//        qWarning("Erreur = %s", msgErreur.toLatin1().data());
        ibonl(psu[primaryAddress], 0);
        alimErreur = -1;
        emit infoMatos(msgErreur);
    }
    return alimErreur;
}

 char *AppareilGPIB::getBufRead(void){
     return bufRead;
 }
