#include "sqlservice.h"
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

SqlService::SqlService(){
    bdd = "parkerBdD";
    table = "mesures";
    flag = false; // non connecte
}

SqlService::~SqlService(){
//    qWarning("Fermeture et destruction du service SQL");
    if(flag){
		db.commit();
		db.close();
        delete q;
	}	
}

bool SqlService::connexionBase() {
	if(!flag) {			
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(bdd);
		if ( db.open() ) {
//            qWarning("Connexion DataBase OK");
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

bool SqlService::creerTable(){
    QString req = "";
    if(!flag){
        msgInfo = "Pas de connexion en cours à la base !";
        return false;
    }
    req.sprintf("CREATE TABLE %s "
    "(numero INTEGER PRIMARY KEY AUTOINCREMENT, "
    "date DATETIME NOT NULL, "
    "reference VARCHAR(64) NOT NULL, "
    "gommage BOOLEAN NOT NULL, "
    "seuil_U_Pil FLOAT, "
    "seuil_U_Depil FLOAT, "
    "seuil_I_Pil FLOAT, "
    "seuil_I_Depil FLOAT, "
    "tps_rep_Pil FLOAT, "
    "tps_rep_Depil FLOAT, "
    "Pression_Alim FLOAT, "
    "Tension_Alim FLOAT); ", table.toLatin1().data());
//    qWarning() << "Requête : " << req;
    r = q->exec(req);
    if(r == false){
        msgInfo = q->lastError().text();
	}
    else {
        msgInfo= "Table créée avec succès !";
	}
    return r;
}

bool SqlService::enregistrerMesure(MesuresTest *mt) {
    QString s, req;

    if(!flag){
        msgInfo = "Pas de connexion en cours à la base !";
        return false;
    }
    req = "INSERT INTO mesures"\
    "(date, reference, gommage, seuil_U_Pil, seuil_U_Depil, seuil_I_Pil, seuil_I_Depil, tps_rep_Pil, tps_rep_Depil, Pression_Alim, Tension_Alim)"\
    "VALUES ( '"\
    + mt->getDateTest().toString()              + "', '"\
    + mt->getRef()                              + "', "\
    + s.setNum(mt->getIndicateurGommage())      + ", "\
    + s.setNum(mt->getSeuilTensionPilotageG(),'f', 2)  + ", "\
    + s.setNum(mt->getSeuilTensionDepilotageGouPilotageD(),'f', 2)+ ", "\
    + s.setNum(mt->getSeuilCourantPilotageG(),'f', 2)  + ", "\
    + s.setNum(mt->getSeuilCourantDepilotageGouPilotageD(),'f', 2)+ ", "\
    + s.setNum(mt->getTpsRepPilotageG())        + ", "\
    + s.setNum(mt->getTpsRepDepilotageGouPilotageD()) + ", "\
    + s.setNum(mt->getPressionAlimentation()) + ", "\
    + s.setNum(mt->getTensionAlimentation()) + ");";
//    qWarning() << "Requête " << req.toLatin1().data();
    if ((r= q->exec(req)) == false){
        msgInfo = q->lastError().databaseText();
//        qWarning () << "Impossible d'écrire dans la table : " << msgInfo.toLatin1().data();
	}
	else {
//        qWarning ("Ecriture des données avec succès");
        db.commit();
	}
	return r;
} 

bool SqlService::supprimerMesure(int numMesure) { // non utilisé, on passe par le modele graphique
    QString req;

    if(!flag){
        msgInfo = "Pas de connexion en cours à la base !";
        return false;
    }
    req.sprintf("DELETE FROM mesures WHERE numero=%d", numMesure);
//    qWarning("Requete=%s", req.toLatin1().data());
    if ((r=q->exec(req)) == false){
        msgInfo = q->lastError().databaseText();
//        qWarning ("Impossible de supprimer la mesure: %s", msgInfo.toLatin1().data());
    }
//    else
//        qWarning("Mesure effacée");
    return r;
}

bool SqlService::supprimerTable(){
    if(!flag){
        msgInfo = "Pas de connexion en cours à la base !";
        return false;
    }
    if ((r=q->exec("DROP TABLE " + table)) == false)
        msgInfo= q->lastError().databaseText();
    else{
        msgInfo = "Table supprimée avec succès  .\n";
        db.commit();
    }
    return r;
}

void SqlService::listDriverDispo(void){
    QSqlDatabase dbase;
    QStringList l = dbase.drivers();
//    qWarning("count=%d", l.count());
//    for(int i=0; i<l.count(); i++)
//        qWarning("%s",l.at(i).toLocal8Bit().constData());
}

QStringList SqlService::getConfig()
{
    QString req;
    QStringList config;

    req.clear();
    q->clear();

    req = "SELECT * FROM config";

    r = q->exec(req);
    if (!r) {
//        qWarning("Error : %s", q->lastError().text().toLatin1().data());
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
//            qWarning("Error : %s", q->lastError().text().toLatin1().data());
            return QStringList();
        }
    }
}

QStringList SqlService::getCarteNiByIdConfig(int idConfig)
{
    q->clear();
    QStringList cartesNi;
    QString pathCapteurs;
    QString pathVannes;
    QString pathEtor;
    QString relais;
    int nbRelais = 0;
    q->prepare("SELECT * FROM cartesNi WHERE idConfig = ?");
    q->addBindValue(idConfig);
    if (q->exec()) {
        if (q->isSelect() && q->isActive()){
            while (q->next()) {
                int fieldDeviceName = q->record().indexOf("DeviceName");
                int fieldDevice = q->record().indexOf("Device");
                int fieldPath = q->record().indexOf("Path");
                int fieldInfo = q->record().indexOf("Info");
                int fieldValue = q->record().indexOf("Value");

                QString deviceName = q->record().value(fieldDeviceName).toString();
                QString device = q->record().value(fieldDevice).toString();
                QString path = q->record().value(fieldPath).toString();
                QString info = q->record().value(fieldInfo).toString();
                QString value = q->record().value(fieldValue).toString();

                // Maintenant, on vérifie qu'on a pas affaire à NB_MODULES_RELAIS ou PATH_VANNES ou PATH_CAPTEURS :
                if (deviceName.isNull() && device.isNull() && path.isNull()) {
                    // On a NB_MODULES_RELAIS  ou PATH_CAPTEURS:
                    if (info.contains("PATH_CAPTEURS")) {
                        // PATH_CAPTEURS :
                        pathCapteurs = info + '\t' + value;
                    } else if (info.contains("PATH_VANNES")){
                        // PATH_VANNES
                        pathVannes = info + '\t' + value;
                    } else if (info.contains("PATH_ETOR")){
                        // PATH_VANNES
                        pathEtor = info + '\t' + value;
                    } else {
                        nbRelais = value.toInt();
                        relais += relais.asprintf("%s\t%d",  info.toLatin1().data(), nbRelais);
                    }
                } else {
                    // Sinon, c'est la config :
                    cartesNi.append(deviceName + "\t" + path + "\t" + info + "\t" + value);
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

bool SqlService::exporterTableEnCVS(void){
    QString ligne, nomDuFichier, champs;
    int i, j;

    nomDuFichier = QDir::currentPath();
//    nomDuFichier = "A:/Engineering/Test terminés";
    nomDuFichier = nomDuFichier + "/mesures.csv";
    QFile f(nomDuFichier);
    if ( !f.open( QIODevice::WriteOnly | QIODevice::Append) ) {
        msgInfo = "Ne peut pas écrire dans" + nomDuFichier;
        return false;
    }
    QTextStream t( &f );
    if((r=q->exec("SELECT * FROM mesures")) == true){
//        qWarning("on a %d enregistrements", q->size());
        ligne="Numero;Date;Ref;Gommage;SeuilG pil;SeuilD depil;Seuil intG;Seuil intD;Tps rep pil;Tps rep depil;Pression Alim;Tension Alim\n";
        t << ligne.toLatin1().data();
        for(i=0; q->next(); i++) {  // 0 -> colonne "numero"
            ligne.clear();
            for(j=0; j<12; j++){
                champs = q->value(j).toString();
                if(j==4 || j==5 || j==6 || j==7 || j==10 || j==11){ // champs avec valeur de type float
                    champs.replace(".", ",");
                }
                ligne += champs;
                if(j!=11)
                    ligne+= ";";
            }
            ligne += "\n";
//            qWarning("%s",ligne.toLatin1().data());
            t << ligne.toLatin1().data();
        }
        f.close();
        msgInfo = "Fichier mesures.csv créé avec succès !";
    }
    else
        msgInfo= q->lastError().text();
    return(r);
}

QStringList SqlService::getListeTable(void) {
  return db.tables(QSql::Tables);
}

QString SqlService::getTable(void) {
  return table;
}

bool SqlService::getFlag(void){
    return flag;
}

QString SqlService::getMsgInfo(void){
    return msgInfo;
}
