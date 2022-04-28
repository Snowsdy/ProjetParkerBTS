#ifndef SQLSERVICE_H
#define SQLSERVICE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlError>
#include "Metier/mesurestest.h"

class SqlService
{
/**
  * Auteur:HH 
  */
public:
    bool flag;          // etat connexion à la base
    bool r;             // valeur de retour des fonctions bool
    QSqlDatabase db;	// référence sur base
    QSqlQuery *q;		// objet de requete sql

    QString msgInfo;
    QString table;
    QString bdd;
public:
    SqlService();
    ~SqlService();

	bool connexionBase();
    bool creerTable();
    bool enregistrerMesure(MesuresTest *);
    bool supprimerMesure(int);
    bool supprimerTable();
    bool exporterTableEnCVS(void);

    QString getMsgInfo(void);
    QString getTable(void);
    QStringList getListeTable(void);
    bool getFlag(void);
    void listDriverDispo(void);

    QStringList getConfig();
    /// Attention !!! Ce n'est pas la même fct que la 2ème appli
    /// Celle-ci permet simplement de remplir la Map !
    /// Donc, on a besoin que de Path / Info / Value !
    QStringList getCarteNiByIdConfig(int idConfig);
};

#endif
