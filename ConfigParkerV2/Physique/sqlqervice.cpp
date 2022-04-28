#include "sqlqervice.h"

#include <QDebug>

SqlQervice::SqlQervice()
{
    this->bdd = "parkerBdD";
    this->tables.append("users");       /// Nom / Prénom / Mdp / Status (Admin, Technicien...)
    this->tables.append("perms");       /// IdPers / Perms (à définir avec Lucas...)
    this->tables.append("config");      /// Nom / Type (Parker ou Lycée) (Ma Partie)
    this->tables.append("cartesNi");    /// DevName / Device / Path / Info / Value / IdConfig (Ma Partie)
    this->flag = false; // Non connecté / Not connected
}

SqlQervice::~SqlQervice()
{
    qWarning("Fermeture et destruction du service SQL");
    if(flag){
        q->clear();
        delete q;
        db.commit();
        db.close();
    }
}

bool SqlQervice::connexionBase()
{
    if(!flag) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(bdd);
        if ( db.open() ) {
            qWarning("Connexion DataBase OK");
            q = new QSqlQuery(db);
            flag = true;
        }
        else
            msgInfo = db.lastError().text();
    }
    else
        msgInfo = "DataBase déjà ouverte !";
    return flag;
}

bool SqlQervice::creerTable()
{
    for (QString tab : tables) {
        QString req = "";
        if(!flag){
            msgInfo = "Pas de connexion en cours à la base !";
            return false;
        }
        switch (tables.indexOf(tab)) {
        case 0:
            /// Users
            this->table = tab;
            req.sprintf("CREATE TABLE %s"
                        "(Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "Nom VARCHAR(128) NOT NULL,"
                        "Prenom VARCHAR(128) NOT NULL,"
                        "Login VARCHAR(128) NOT NULL,"
                        "Mdp TEXT NOT NULL,"
                        "Status VARCHAR(1) NOT NULL);", tab.toLatin1().data());

            //            qWarning() << "Requête : " << req;
            r = q->exec(req);
            if (!r) {
                qWarning("Table '%s' déjà créée !", tab.toLatin1().data());
                msgInfo = q->lastError().text();
            } else {
                qWarning("Table '%s' créée avec succès !", tab.toLatin1().data());
                msgInfo= "Table créée avec succès !";
            }
            break;
        case 1:
            /// Perms
            this->table = tab;
            req.sprintf("CREATE TABLE %s"
                        "(Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "IdUser INTEGER NOT NULL,"
                        "Perms VARCHAR(128) NOT NULL,"
                        "FOREIGN KEY (IdUser) REFERENCES users (Id));", tab.toLatin1().data());

            //            qWarning() << "Requête : " << req;
            r = q->exec(req);
            if (!r) {
                qWarning("Table '%s' déjà créée !", tab.toLatin1().data());
                msgInfo = q->lastError().text();
            } else {
                qWarning("Table '%s' créée avec succès !", tab.toLatin1().data());
                msgInfo= "Table créée avec succès !";
            }
            break;
        case 2:
            /// Config  /// Nom / Type (Parker ou Lycée)
            this->table = tab;
            req.sprintf("CREATE TABLE %s"
                        "(Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "Nom VARCHAR(128) NOT NULL,"
                        "Type VARCHAR(128) NOT NULL);", tab.toLatin1().data());

            //            qWarning() << "Requête : " << req;
            r = q->exec(req);
            if (!r) {
                qWarning("Table '%s' déjà créée !", tab.toLatin1().data());
                msgInfo = q->lastError().text();
            } else {
                qWarning("Table '%s' créée avec succès !", tab.toLatin1().data());
                msgInfo= "Table créée avec succès !";
            }
            break;
        case 3:
            /// Cartes NI       /// DevName / Device / Path / Info / Value (Ma Partie)
            this->table = tab;
            req.sprintf("CREATE TABLE %s"
                        "(Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "DeviceName VARCHAR(128),"
                        "Device VARCHAR(128),"
                        "Path VARCHAR(128),"
                        "Info VARCHAR(128),"
                        "Value VARCHAR(128),"
                        "IdConfig INTEGER NOT NULL,"
                        "FOREIGN KEY (IdConfig) REFERENCES config (Id));", tab.toLatin1().data());

            //            qWarning() << "Requête : " << req;
            r = q->exec(req);
            if (!r) {
                qWarning("Table '%s' déjà créée !", tab.toLatin1().data());
                msgInfo = q->lastError().text();
            } else {
                qWarning("Table '%s' créée avec succès !", tab.toLatin1().data());
                msgInfo= "Table créée avec succès !";
            }
            break;
        default:
            break;
        }
    }
    return r;
}

QString SqlQervice::getMsgInfo()
{
    return this->msgInfo;
}

/**
 * @brief Retourne la dernière table sélectionnée.
 */
QString SqlQervice::getTable()
{
    return this->table;
}

QString SqlQervice::getTable(QString name)
{
    return name;
}

QStringList SqlQervice::getListeTable()
{
    return db.tables(QSql::Tables);
}

bool SqlQervice::getFlag()
{
    return this->flag;
}

void SqlQervice::listDriverDispo()
{
    QSqlDatabase dbase;
    QStringList l = dbase.drivers();
    qWarning("count=%d", l.count());
    for(int i=0; i<l.count(); i++)
        qWarning("%s",l.at(i).toLocal8Bit().constData());
}

int SqlQervice::isUserValid(QString login, QString mdp)
{
    /// Lors de la connexion, on vérifie les informations de l'utilisateur :

    mdp = Personne::md5(mdp);

    QString req;

    if (!flag) {
        return -1;
    } else {
        if (login.isEmpty() || mdp.isEmpty()) {
            return -2;
        } else {
            req += req.asprintf("SELECT id FROM users WHERE (login = '%s' AND mdp = '%s');", login.toLatin1().data(),
                                mdp.toLatin1().data());
            r = q->exec(req);
            if (!r) {
                return -3;
            } else {
                if (q->isActive() && q->isSelect()) {

                    r = q->next();

                    if (!r) {
                        qWarning("Cet utilisateur n'existe pas !");
                    } else {
                        int idField = q->record().indexOf("id");

                        int id = q->value(idField).toInt();
                        // Fonctionne : Mtn, on passe aux perms :
                        q->clear();
                        req.clear();
                        req += req.asprintf("SELECT perms FROM perms WHERE idUser = '%d'", id);

                        r = q->exec(req);
                        if (!r) {
                            qWarning("Error : %s", q->lastError().text().toLatin1().data());
                        } else {
                            if (q->isActive() && q->isSelect()) {
                                r = q->next();

                                if (!r) {
                                    return -4;
                                } else {
                                    int permsField = q->record().indexOf("perms");

                                    QStringList list = q->record().value(permsField).toString().split(',');
                                    if (!list.isEmpty() && list.contains("MANAGE"))
                                        return id;
                                    return -5;
                                }
                            } else {
                                return -6;
                            }
                        }
                    }
                }
                return -7;
            }
        }
    }

}

QStringList SqlQervice::getPermsById(int id)
{
    QString req;

    q->clear();

    req += req.asprintf("SELECT perms FROM perms WHERE idUser = '%d'", id);

    r = q->exec(req);
    if (!r) {
        qWarning("Error : %s", q->lastError().text().toLatin1().data());
        return QStringList();
    } else {
        if (q->isActive() && q->isSelect()) {
            q->next();

            int permsField = q->record().indexOf("perms");

            QStringList list = q->record().value(permsField).toString().split(',');

            return list;
        } else {
            return QStringList();
        }
    }
}

QStringList SqlQervice::getUsers()
{
    QString req;
    QStringList users;

    req.clear();
    q->clear();

    req = "SELECT * FROM users";

    r = q->exec(req);
    if (!r) {
        qWarning("Error : %s", q->lastError().text().toLatin1().data());
        return QStringList();
    } else {
        if (q->isActive() && q->isSelect()) {
            while (q->next()) {
                int idField = q->record().indexOf("id");
                int nomField = q->record().indexOf("nom");
                int prenomField = q->record().indexOf("prenom");
                int loginField = q->record().indexOf("login");
                int mdpField = q->record().indexOf("mdp");
                int statusField = q->record().indexOf("status");

                int id = q->record().value(idField).toInt();
                QString nom = q->record().value(nomField).toString();
                QString prenom = q->record().value(prenomField).toString();
                QString login = q->record().value(loginField).toString();
                QString mdp = q->record().value(mdpField).toString();
                QString status = q->record().value(statusField).toString();

                users.append(QString::number(id) + '\t'
                             + nom + '\t'
                             + prenom + '\t'
                             + login + '\t'
                             + mdp + '\t'
                             + status);

            }

            return users;
        } else {
            qWarning("Error : %s", q->lastError().text().toLatin1().data());
            return QStringList();
        }
    }
}

QStringList SqlQervice::getCarteNI()
{
    QString req;
    QStringList cartes;

    req.clear();
    q->clear();

    req = "SELECT * FROM cartesNi";

    r = q->exec(req);
    if (!r) {
        qWarning("Error : %s", q->lastError().text().toLatin1().data());
        return QStringList();
    } else {
        if (q->isActive() && q->isSelect()) {
            while (q->next()) {
                int idField = q->record().indexOf("Id");
                int deviceNameField = q->record().indexOf("DeviceName");
                int deviceField = q->record().indexOf("Device");
                int pathField = q->record().indexOf("Path");
                int infoField = q->record().indexOf("Info");
                int valueField = q->record().indexOf("Value");

                int id = q->record().value(idField).toInt();
                QString deviceName = q->record().value(deviceNameField).toString();
                QString device = q->record().value(deviceField).toString();
                QString path = q->record().value(pathField).toString();
                QString info = q->record().value(infoField).toString();
                QString value = q->record().value(valueField).toString();

                cartes.append(QString::number(id) + '\t'
                              + deviceName + '\t'
                              + device + '\t'
                              + path + '\t'
                              + info + '\t'
                              + value);

            }

            return cartes;
        } else {
            qWarning("Error : %s", q->lastError().text().toLatin1().data());
            return QStringList();
        }
    }
}

QStringList SqlQervice::getConfig()
{
    QString req;
    QStringList config;

    req.clear();
    q->clear();

    req = "SELECT * FROM config";

    r = q->exec(req);
    if (!r) {
        qWarning("Error : %s", q->lastError().text().toLatin1().data());
        return QStringList();
    } else {
        if (q->isActive() && q->isSelect()) {
            while (q->next()) {
                int idField = q->record().indexOf("Id");
                int nomField = q->record().indexOf("Nom");
                int typeField = q->record().indexOf("Type");

                int id = q->record().value(idField).toInt();
                QString nom = q->record().value(nomField).toString();
                QString type = q->record().value(typeField).toString();

                config.append(QString::number(id) + '\t'
                              + nom + '\t'
                              + type);

            }

            return config;
        } else {
            qWarning("Error : %s", q->lastError().text().toLatin1().data());
            return QStringList();
        }
    }
}

/**
 * @brief Ajoute ou Modifie la Personne passée en paramètre.
 * Retourne 1 si succès lors de l'Ajout/Modification.
 * Retourne -1 si échec.
 */
int SqlQervice::addEditUser(Personne p, bool modif)
{
    QString req;
    q->clear();

    if (!modif) {
        q->prepare("INSERT INTO users (nom, prenom, login, mdp, status) VALUES(?, ?, ?, ?, ?)");
        q->addBindValue(p.getNom().toLatin1().data());
        q->addBindValue(p.getPrenom().toLatin1().data());
        q->addBindValue(p.getLogin().toLatin1().data());
        q->addBindValue(p.getMdp().toLatin1().data());
        q->addBindValue(QString(p.getStatus()));
    } else {
        req += req.asprintf("UPDATE users SET "
                            "nom = '%s', "
                            "prenom = '%s', "
                            "login = '%s', "
                            "mdp = '%s', "
                            "status = '%c' "
                            "WHERE id = %d",
                            p.getNom().toLatin1().data(),
                            p.getPrenom().toLatin1().data(),
                            p.getLogin().toLatin1().data(),
                            p.getMdp().toLatin1().data(),
                            p.getStatus(),
                            p.getId());
        q->prepare(req);
    }

    if (q->exec()){
        req.clear();
        req += req.asprintf("SELECT id FROM users WHERE ("
                            "nom = '%s' AND "
                            "prenom = '%s' AND "
                            "login = '%s' AND "
                            "mdp = '%s')",
                            p.getNom().toLatin1().data(),
                            p.getPrenom().toLatin1().data(),
                            p.getLogin().toLatin1().data(),
                            p.getMdp().toLatin1().data());
        q->clear();
        q->prepare(req);
        if (q->exec()) {
            if (q->isSelect() && q->isActive()) {
                q->next();

                int idField = q->record().indexOf("id");
                int id = q->record().value(idField).toInt();

                // On vérifie mtn si on ajoute les perms ou bien si on les modifie :
                if (!modif) {
                    QString perms = p.getPermissions().join(',');

                    q->clear();

                    q->prepare("INSERT INTO perms (idUser, perms) VALUES(?, ?)");
                    q->addBindValue(id);
                    q->addBindValue(perms.toLatin1().data());

                    if (q->exec())
                        return 1;

                } else {
                    QString perms = p.getPermissions().join(',');

                    q->clear();
                    req.clear();

                    req += req.asprintf("UPDATE perms SET perms = '%s' WHERE idUser = %d", perms.toLatin1().data(), id);

                    q->prepare(req);

                    if (q->exec())
                        return 1;
                }
            }
        }
    }
    return 0;
}

bool SqlQervice::removeUser(int id)
{
    q->clear();
    q->prepare("DELETE FROM perms WHERE idUser = ?");
    q->addBindValue(id);

    if (q->exec()) {
        q->clear();
        q->prepare("DELETE FROM users WHERE id = ?");
        q->addBindValue(id);
        return q->exec();
    } else {
        return false;
    }
}

int SqlQervice::addEditConfig(QString nomConfig, QString typeConfig)
{
    QString req;
    q->clear();
    q->prepare("INSERT INTO config (Nom, Type) VALUES(?, ?)");
    q->addBindValue(nomConfig.toLatin1().data());
    q->addBindValue(typeConfig.toLatin1().data());
    if (q->exec()) {
        q->clear();
        q->prepare("SELECT seq FROM sqlite_sequence WHERE name = 'config'");
        if (q->exec()){
            if (q->isSelect() && q->isActive()){
                q->next();
                int seqField = q->record().indexOf("seq");
                int idConfig = q->record().value(seqField).toInt();
                return idConfig;
            } else {
                return -1;
            }
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

bool SqlQervice::addCarteNI(QStringList vannes, QStringList capteurs, QStringList relais, QString arrUrg, QString seqTest, int idConfig,
                            int nbRelais, QString pathCapteurs, QString pathVannes, QString pathEtor)
{
    int sucess = 0;
    QString req;
    QList<QStringList> infos;
    infos.append(vannes);
    infos.append(capteurs);
    infos.append(relais);
    infos.append(QStringList(arrUrg));
    infos.append(QStringList(seqTest));

    for (QStringList list : infos){
        for (QString val : list) {
            QStringList values = val.split('\t');
            q->clear();
            q->prepare("INSERT INTO cartesNi (DeviceName, Device, Path, Info, Value, IdConfig) "
                       "VALUES(?, ?, ?, ?, ?, ?)");
            q->addBindValue(values[0].toLatin1().data());
            q->addBindValue(values[1].toLatin1().data());
            q->addBindValue(values[2].toLatin1().data());
            q->addBindValue(values[3].toLatin1().data());
            q->addBindValue(values[4].toLatin1().data());
            q->addBindValue(idConfig);
            if (q->exec()) {
                sucess++;
            } else {
                sucess--;
            }
        }
    }
    if (sucess <= 0) {
        return false;
    } else {
        q->clear();
        q->prepare("INSERT INTO cartesNi (Info, Value, IdConfig) VALUES(?, ?, ?)");
        q->addBindValue("NB_MODULE_RELAIS");
        q->addBindValue(nbRelais);
        q->addBindValue(idConfig);
        if (q->exec()) {
            q->clear();
            q->prepare("INSERT INTO cartesNi (Info, Value, IdConfig) VALUES(?, ?, ?)");
            q->addBindValue("PATH_CAPTEURS");
            q->addBindValue(pathCapteurs.toLatin1().data());
            q->addBindValue(idConfig);
            if (q->exec()){
                q->clear();
                q->prepare("INSERT INTO cartesNi (Info, Value, IdConfig) VALUES(?, ?, ?)");
                q->addBindValue("PATH_VANNES");
                q->addBindValue(pathVannes.toLatin1().data());
                q->addBindValue(idConfig);
                if (q->exec()){
                    q->clear();
                    q->prepare("INSERT INTO cartesNi (Info, Value, IdConfig) VALUES(?, ?, ?)");
                    q->addBindValue("PATH_ETOR");
                    q->addBindValue(pathEtor.toLatin1().data());
                    q->addBindValue(idConfig);
                    if (q->exec()) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

bool SqlQervice::editCarteNI(QStringList vannes, QStringList capteurs, QStringList relais,
                             QString arrUrg, QString seqTest, QList<int> ids, QString pathCapteurs, QString pathVannes, QString pathEtor)
{
    int sucess = 0, i, j = 0;
    QString req;
    QList<QStringList> infos;
    infos.append(vannes);
    infos.append(capteurs);
    infos.append(relais);
    infos.append(QStringList(arrUrg));
    infos.append(QStringList(seqTest));

    for (QStringList list : infos) {
        for (i = 0; i < list.length(); i++) {
            QStringList values = list.at(i).split('\t');
            q->clear();
            req.clear();
            req += req.asprintf("UPDATE cartesNi SET "
                                "DeviceName = '%s',"
                                "Device = '%s',"
                                "Path = '%s',"
                                "Info = '%s',"
                                "Value = '%s' "
                                "WHERE Id = %d",
                                values[0].toLatin1().data(),
                    values[1].toLatin1().data(),
                    values[2].toLatin1().data(),
                    values[3].toLatin1().data(),
                    values[4].toLatin1().data(),
                    ids.at(j++));
            q->prepare(req);
            if (q->exec())
                sucess++;
            else
                sucess--;
        }
    }

    if (sucess <= 0) {
        msgInfo = q->lastError().text();
        return false;
    } else {
        q->clear();
        req = req.asprintf("UPDATE cartesNi SET "
                           "Value = '%s' "
                           "WHERE Id = %d", pathCapteurs.split('\t').at(2).toLatin1().data(), pathCapteurs.split('\t').at(0).toInt());
        q->prepare(req);
        if (q->exec()) {
            q->clear();
            req = req.asprintf("UPDATE cartesNi SET "
                               "Value = '%s' "
                               "WHERE Id = %d", pathVannes.split('\t').at(2).toLatin1().data(), pathVannes.split('\t').at(0).toInt());
            q->prepare(req);
            if (q->exec()) {
                q->clear();
                req = req.asprintf("UPDATE cartesNi SET "
                                   "Value = '%s' "
                                   "WHERE Id = %d", pathEtor.split('\t').at(2).toLatin1().data(), pathEtor.split('\t').at(0).toInt());
                q->prepare(req);
                if (q->exec()) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

QStringList SqlQervice::getCarteNiByIdConfig(int idConfig)
{
    q->clear();
    QStringList cartesNi;
    QString pathVannes;
    QString pathEtor;
    QString relais;
    QString pathCapteurs;
    int nbRelais = 0;
    q->prepare("SELECT * FROM cartesNi WHERE idConfig = ?");
    q->addBindValue(idConfig);
    if (q->exec()) {
        if (q->isSelect() && q->isActive()){
            while (q->next()) {
                int fieldId = q->record().indexOf("Id");
                int fieldDeviceName = q->record().indexOf("DeviceName");
                int fieldDevice = q->record().indexOf("Device");
                int fieldPath = q->record().indexOf("Path");
                int fieldInfo = q->record().indexOf("Info");
                int fieldValue = q->record().indexOf("Value");

                int id = q->record().value(fieldId).toInt();
                QString deviceName = q->record().value(fieldDeviceName).toString();
                QString device = q->record().value(fieldDevice).toString();
                QString path = q->record().value(fieldPath).toString();
                QString info = q->record().value(fieldInfo).toString();
                QString value = q->record().value(fieldValue).toString();

                // Maintenant, on vérifie qu'on a pas affaire à NB_MODULES_RELAIS ou PATH_CAPTEURS ou PATH_VANNES :
                if (deviceName.isNull() && device.isNull() && path.isNull()) {
                    if (info.contains("PATH_CAPTEURS")){
                        // PATH_CAPTEURS :
                        pathCapteurs = pathCapteurs.asprintf("%d\t%s\t%s", id, info.toLatin1().data(), value.toLatin1().data());
                    } else if (info.contains("PATH_VANNES")){
                        // PATH_VANNES
                        pathVannes = pathVannes.asprintf("%d\t%s\t%s", id, info.toLatin1().data(), value.toLatin1().data());
                    } else if (info.contains("PATH_ETOR")){
                        // PATH_ETOR
                        pathEtor = pathEtor.asprintf("%d\t%s\t%s", id, info.toLatin1().data(), value.toLatin1().data());
                    } else {
                        // On a NB_MODULES_RELAIS :
                        nbRelais = value.toInt();
                        relais += relais.asprintf("%d\t%s\t%d", id, info.toLatin1().data(), nbRelais);
                    }
                }else {
                    // Sinon, c'est la config :
                    cartesNi.append(QString::number(id) + "\t" +deviceName + "\t" + device + "\t" + path + "\t" + info + "\t" + value);
                }
            }
            cartesNi.push_front(pathEtor);
            cartesNi.push_front(pathVannes);
            cartesNi.push_front(pathCapteurs);
            cartesNi.push_front(relais);

            return cartesNi;
        } else {
            return QStringList();
        }
    } else {
        return QStringList();
    }
}

bool SqlQervice::removeConfig(int idConfig)
{
    q->clear();
    q->prepare("DELETE FROM cartesNi WHERE IdConfig = ?");
    q->addBindValue(idConfig);

    if (q->exec()) {
        q->clear();
        q->prepare("DELETE FROM config WHERE Id = ?");
        q->addBindValue(idConfig);
        return q->exec();
    } else {
        msgInfo = q->lastError().text();
        return false;
    }
}
