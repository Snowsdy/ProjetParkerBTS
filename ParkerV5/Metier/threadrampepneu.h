#ifndef THREADRAMPEPNEU_H
#define THREADRAMPEPNEU_H

#include <QThread>
#include "physiquefactory.h"
#include "vannesproportionnelle.h"

class ThreadRampePneu : public QThread
{
    Q_OBJECT

private:
    QString message;
    int erreur;
    double pression;
    double valMax;
    double valMini;
    double valEnCours;
    double pas;
    bool stopped;
    bool operation;         // ENCLENCHEMENT ou DECLENCHEMENT
    bool typeDistributeur;  // MONOSTABLE ou BISTABLE
    bool typeRampe;

    VannesProportionnelle *vannes;
    Config *conf;
    Param *param;

public:
    ThreadRampePneu(int choix);
    void stop();

    QString getMessage(void);
    int getErreur(void);

    void setPression(double);
    void setValEnCours(double); // Pour accelerer la fin d'une rampe
    void setOperation(bool);
    void setTypeDistributeur(bool);
    void setTypeRampe(bool type); //true normale sino personnalisée

    void setVannes(VannesProportionnelle*);

protected:
    void run();

signals:
    void majData(double);
};

#endif // THREADRAMPEPNEU_H
