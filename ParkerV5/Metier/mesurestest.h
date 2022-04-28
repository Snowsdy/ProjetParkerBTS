 #ifndef MESURESTEST_H
#define MESURESTEST_H

#include <QString>
#include <QDate>

class MesuresTest{
private:
    QDate dateTest;
    QString ref;
    QString designation;
    bool indicateurGommage;
    double seuilTensionPilotageG;
    double seuilTensionDepilotageGouPilotageD;
    double seuilCourantPilotageG;
    double seuilCourantDepilotageGouPilotageD;
    int tpsRepPilotageG;
    int tpsRepDepilotageGouPilotageD;
    double tensionAlimentation;
    double pressionAlimtation;
public:
    MesuresTest();
    void razMesures(void);

    QDate getDateTest(void);
    QString getRef(void);
    QString getDesignation(void);
    bool getIndicateurGommage(void);

    double getSeuilCourantPilotageG(void);
    double getSeuilCourantDepilotageGouPilotageD(void);

    double getSeuilTensionPilotageG(void);
    double getSeuilTensionDepilotageGouPilotageD(void);

    double getTensionAlimentation(void);
    double getPressionAlimentation(void);

    int getTpsRepPilotageG(void);
    int getTpsRepDepilotageGouPilotageD(void);

    void setDateTest(QDate);
    void setRef(QString);
    void setDesignation(QString);
    void setIndicateurGommage(bool);

    void setSeuilCourantPilotageG(double);
    void setSeuilCourantDepilotageGouPilotageD(double);

    void setSeuilTensionPilotageG(double);
    void setSeuilTensionDepilotageGouPilotageD(double);

    void setTensionAlimentation(double);
    void setPressionAlimentation(double);

    void setTpsRepPilotageG(int);
    void setTpsRepDepilotageGouPilotageD(int);
};

#endif // MESURESTEST_H
