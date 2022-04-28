#ifndef CARTENI_H
#define CARTENI_H

#include <NIDAQmx.h>
#include <stdint.h>
#include <QString>
#include <QStringList>

class CarteNI
{
private:
    QStringList device;

public:
    CarteNI();

    QString getDeviceList();
    QStringList getDeviceNames();
    QStringList getProductType();
    QString getIO(QString dev, int option);

};

#endif // CARTENI_H
