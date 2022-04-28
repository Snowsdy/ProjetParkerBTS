#include "param.h"

Param::Param(int choice){
    rampe= true;
    this->choix = choice;
    if( choix ) { // Lycée
#ifdef SIMU
        AI[0][0]= 0;
        AI[0][1]= 0;
        AI[1][0]= 0;
        AI[1][1]= 0;
        AI[2][0]= 0;
        AI[2][1]= 0;
        AI[3][0]= 0;
        AI[3][1]= 0;
        AI[4][0]= 0;
        AI[4][1]= 0;
#else
        AI[0][0]= 2.5; // 2.5x-2.5 SORTIE_D
        AI[0][1]= -2.5;
        AI[1][0]= 2.5; // 2.5x-2.5 P
        AI[1][1]= -2.5;
        AI[2][0]= 2.5; // 2.5x-2.5 SORTIE_G
        AI[2][1]= -2.5;
        AI[3][0]= 1.6036; // 1.6036x+0 CMD_D
        AI[3][1]= 0.0;
        AI[4][0]= 1.6025; // 1.6025x+0 CMD_G
        AI[4][1]= 0.0;
#endif
    } else { // Parker
        for (int i= 0; i<8; i++){
            AI[i][0]= 2; // 2x+0
            AI[i][1]= 0;
        }
    }
    pasTpsRep = 5; // ms
    pasSeuil = 50; //ms

    // rampe normale par défaut
    pasRampe_en_bar.append(0.1);
    pasRampe_en_ms.append(100);
    nbPas.clear(); // aucune étape => simple rampe
    pression.clear();

    coefLimitePressionDeclenchement= 0.5;
    coefLimitePressionEnclenchement= 0.5;
}

int Param::getPasTpsRep(){
    return pasTpsRep;
}

void Param::setPasTpsRep(int delai){
    pasTpsRep= delai;
}

int Param::getPasSeuil(){
    return pasSeuil;
}

void Param::setPasSeuil(int delai){
    pasSeuil= delai;
}
float Param::getCoefLimitePressionDeclenchement(){
    return coefLimitePressionDeclenchement;
}

void Param::setCoefLimitePressionDeclenchement(float coef){
    coefLimitePressionDeclenchement= coef;
}

float Param::getCoefLimitePressionEnclenchement(){
    return coefLimitePressionEnclenchement;
}

void Param::setCoefLimitePressionEnclenchement(float coef){
    coefLimitePressionEnclenchement= coef;
}

void Param::getAI(int n, double &a, double &b){
    a= AI[n][0];
    b= AI[n][1];
}

void Param::setAI(int n, double a, double b){
    AI[n][0]= a;
    AI[n][1]= b;
}

void Param::initRampe(bool flag){
    pasRampe_en_bar.clear();
    nbPas.clear();
    if(flag) { // rampe normale
        qWarning("rampe normale");
        pasRampe_en_bar.append(0.1);
        nbPas.clear(); // aucune étape => simple rampe
        pression.clear();
    } else { // rampe personalisée
        qWarning("rampe perso");
        pasRampe_en_bar.append(0.0333); // On veut 1bar en 30 top (1/30)
        nbPas.append(30); // durée de l'étape 3s => nBpas= 3/0.1s
        pression.append(0); // non utilisé
        //Etape2
        pasRampe_en_bar.append(0); // même niveau alors pas= 0
        nbPas.append(20); // durée de l'étape 2s => nBpas= 2/0.1s
        pression.append(0); // non utilisé
        //Etape3
        pasRampe_en_bar.append(0); // nouveau niveau alors pas=0 et nb=1
        nbPas.append(1); // changement de niveau en 1 coup
        pression.append(4); // nouvelle pression
        //Etape4
        pasRampe_en_bar.append(0); // même niveau alors pas= 0
        nbPas.append(30); // durée de l'étape 3s => nBpas= 3/0.1s
        pression.append(0); // non utilisé
        //Etape5
        pasRampe_en_bar.append(-0.4); // On veut -4bar en 10 top (-4/10)
        nbPas.append(10); // durée de l'étape 1s => nBpas= 1/0.1s
        pression.append(0);
    }
}


