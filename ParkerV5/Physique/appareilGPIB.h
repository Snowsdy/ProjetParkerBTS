#ifndef APPAREILGPIB_H
#define APPAREILGPIB_H

#include <QObject>
#include <QString>
#include <ni4882.h>
#include <string.h>
#include <stdio.h>

#define GPIB0   0                 // Board handle

class AppareilGPIB: public QObject
{
    Q_OBJECT

private:
    QString msgErreur;
    QString msgStatus;
    int alimErreur;         /* error return from all functions */
    int psu[20];            /* tableau descriptor handle                  */
    int primaryAddress;     /* Primary address of the device           */
    int secondaryAddress;   /* Secondary address of the device         */
    int boardIndex;         /* Board Index (GPIB0 = 0, GPIB1 = 1, ...) */
//    int id;               /* n° identifiant appareil : 3222 */
    int Num_Listeners;      // Number of listeners on GPIB
    int loop;               // Loop counter
    Addr4882_t Instruments[32],     // Array of primary addresses
               Result[31];          // Array of listen addresses
    char bufWrite[100];
    char bufRead[2000];
    char output_buf[2000];
    char reqGetTension[64];
    char reqGetCourant[64];
    char reqSetTension[64];

public:
    AppareilGPIB();

    /* functions */
private:
    void writeData(char *buf);

public:
    int readData();
    int initDevice(int);
    int getPrimaryAddress();
    int scan(void);
    int sendCommande(char *);
    int gpibError();
    void closeDevice();
    char *getBufRead(void);
//    int getPsu(void);

signals:
    void infoMatos(QString);
};

#endif // alimDC_H
