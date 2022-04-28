#ifndef PARAM_H
#define PARAM_H

#include <QVector>
class Param
{
private:
    double AI[8][2]; // n° d'entrée analogique et 0 pour a et 1 pour b (ax+b)
    int pasTpsRep;
    int pasSeuil;
    float coefLimitePressionDeclenchement;
    float coefLimitePressionEnclenchement;
    int choix;
    bool rampe;

public:
    QVector<int> nbPas; // tour de boucle dans une étape
    QVector<int> pasRampe_en_ms; // pas en ms dans une étape
    QVector<double> pasRampe_en_bar; // 0.1V
    QVector<double> pression;

public:
    Param(int choix);

    int getPasTpsRep(); // en ms
    void setPasTpsRep(int delai);
    int getPasSeuil(); // en ms
    void setPasSeuil(int delai);

    float getCoefLimitePressionDeclenchement();
    void setCoefLimitePressionDeclenchement(float coef);
    float getCoefLimitePressionEnclenchement();
    void setCoefLimitePressionEnclenchement(float coef);

    void getAI(int n, double &a, double &b );
    void setAI(int n, double a, double b);

    void initRampe(bool flag);
};

#endif // PARAM_H
