#ifndef SQLQERVICE_H
#define SQLQERVICE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlError>

#include "personne.h"

/**
 * @brief The SqlQervice class
 * @author Snowsdy
 */
class SqlQervice
{

    /// Attributs publiques :
public:
    bool flag;          // etat connexion à la base
    bool r;             // valeur de retour des fonctions bool
    QSqlDatabase db;	// référence sur base
    QSqlQuery *q;		// objet de requete sql

    QString msgInfo;
    QString table;
    QString bdd;
    QStringList tables;

    /// Méthodes publiques :
public:
    SqlQervice();
    ~SqlQervice();

    bool connexionBase();
    bool creerTable();
    QString getMsgInfo(void);
    QString getTable(void);
    QString getTable(QString name);
    QStringList getListeTable(void);
    bool getFlag(void);
    void listDriverDispo(void);

    int isUserValid(QString login, QString mdp);

    QStringList getPermsById(int id);
    QStringList getUsers();
    QStringList getCarteNI();
    QStringList getConfig();

    int addEditUser(Personne p, bool modif);
    bool removeUser(int id);

    int addEditConfig(QString nomConfig, QString typeConfig);
    bool addCarteNI(QStringList vannes, QStringList capteurs, QStringList relais, QString arrUrg, QString seqTest, int idConfig,
                        int nbRelais, QString pathCapteurs, QString pathVannes, QString pathEtor);
    bool editCarteNI(QStringList vannes, QStringList capteurs, QStringList relais, QString arrUrg,
                     QString seqTest, QList<int> ids, QString pathCapteurs, QString pathVannes, QString pathEtor);
    QStringList getCarteNiByIdConfig(int idConfig);
    bool removeConfig(int idConfig);
};

#endif // SQLQERVICE_H
