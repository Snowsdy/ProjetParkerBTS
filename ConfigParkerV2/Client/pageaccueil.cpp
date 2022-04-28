#include "pageaccueil.h"
#include "ui_pageaccueil.h"

PageAccueil::PageAccueil(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PageAccueil)
{
    ui->setupUi(this);
    this->isEdit = false;
    this->isConfigEdit = false;
    this->initRealized = false;
    this->ssl = new SqlQervice();
    idUser = 0;
    nbRelais = 0;

    capteursPath = "";
    vannesPath = "";
    etorPath = "";
    deviceName = "";
    device = "";
    nomConfig = "";
    typeConfig = "";

    connect(this, SIGNAL(vannesConfigOK()), this, SLOT(desactivePbVanne()));
    connect(this, SIGNAL(capteursConfigOK()), this, SLOT(desactivePbCapteur()));
    connect(this, SIGNAL(seqTestConfigOK()), this, SLOT(desactivePbSeqTest()));
    connect(this, SIGNAL(arrUrgConfigOK()), this, SLOT(desactivePbArrUrg()));
    connect(this, SIGNAL(relaisConfigOK()), this, SLOT(desactivePbRelais()));
    connect(this, SIGNAL(configPrete()), this, SLOT(recapitulatif()));
    connect(this, SIGNAL(configSupprimee()), this, SLOT(suppressionConfigConfirme()));

    initConfigVar();

    ui->pBValidConfig->setVisible(false);
}

PageAccueil::~PageAccueil()
{
    delete ui;
}

void PageAccueil::launchPG(int id)
{
    initSqlService();
    if (ssl->getFlag())
        getTables();
    this->show();
    this->ui->stackedWidget->setCurrentIndex(0);
    ui->lEMsgInfo->setText("Bienvenue sur l'application de Gestion & d'Administration !");
    verifPerms(id);
}

void PageAccueil::desactivePbVanne()
{
    ui->pBVannesP->setEnabled(false);
}

void PageAccueil::desactivePbCapteur()
{
    ui->pBCapteurs->setEnabled(false);
}

void PageAccueil::desactivePbSeqTest()
{
    ui->pBSeqTests->setEnabled(false);
}

void PageAccueil::desactivePbArrUrg()
{
    ui->pBArrUrg->setEnabled(false);
}

void PageAccueil::desactivePbRelais()
{
    ui->pBRelais->setEnabled(false);
}

void PageAccueil::recapitulatif()
{
    // Passage à l'écran du récapitulatif :
    ui->pBValidConfig->setVisible(true);
}

void PageAccueil::ajoutConfigReussi()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lEMsgInfo->setText("Ajout / Modification de la Configuration réussi !");
    resetConfigVar();
    initConfigVar();
}

void PageAccueil::suppressionConfigConfirme()
{
    ui->checkBoxSuppConfig->setChecked(false);
}

void PageAccueil::on_pBBtn1_clicked()
{
    ui->lEMsgInfo->setText("Gestion des Utilisateurs !");
    ui->stackedWidget->setCurrentIndex(1);
}

void PageAccueil::on_pBBtn2_clicked()
{
    ui->lEMsgInfo->setText("Gestion de la Configuration !");
    ui->stackedWidget->setCurrentIndex(2);
}

void PageAccueil::on_pBBtn3_clicked()
{
    ui->lEMsgInfo->setText("Aperçu des tables !");
    ui->stackedWidget->setCurrentIndex(3);
}

void PageAccueil::on_pBBtnAccueil_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lEMsgInfo->clear();
}

void PageAccueil::getTables()
{
    ui->cBNomTable->clear();
    if (!ssl->getListeTable().isEmpty()) {
        ui->cBNomTable->addItems(ssl->getListeTable());
        ui->cBNomTable->removeItem(ui->cBNomTable->findText("sqlite_sequence"));
    } else {
        ui->lEMsgInfo->setText("Pas de table(s) dans la base !");
    }
}

bool PageAccueil::initSqlService()
{
    if (ssl->connexionBase()){
        ui->lEMsgInfo->setText("Connexion à la BdD réussie !");
        return ssl->getFlag();
    } else {
        ui->lEMsgInfo->setText("Erreur : " + ssl->getMsgInfo());
        return ssl->getFlag();
    }
}

void PageAccueil::setModif()
{
    if (isEdit){
        ui->gBModif->setEnabled(true);
        ui->labelUser->setEnabled(true);
        ui->cBEditUser->setEnabled(true);
        ui->pBAddEdit->setText("Modification");
    } else {
        ui->gBModif->setEnabled(false);
        ui->labelUser->setEnabled(false);
        ui->cBEditUser->setEnabled(false);
        ui->pBAddEdit->setText("Ajout");
    }
}

void PageAccueil::initCBUserEdit()
{
    initRealized = false;
    ui->cBEditUser->clear();
    if (!initRealized) {
        if (ssl->getFlag()){
            ui->cBEditUser->addItems(ssl->getUsers());
            this->initRealized = true;
        }
    }
}

void PageAccueil::verifPerms(int id)
{
    this->resetBtn();
    // On vérifie les perms pour savoir ce que l'on autorise à celui qui se connecte.
    QStringList permissions = ssl->getPermsById(id);
    if (!permissions.contains("CREATE") && !permissions.contains("DELETE")) {
        ui->pBBtn1->setEnabled(false);
    }
    if (!permissions.contains("MANAGE")) {
        ui->pBBtn2->setEnabled(false);
        ui->pBBtn3->setEnabled(false);
    }
}

void PageAccueil::clearAddEditForm()
{
    ui->lENom->clear();
    ui->lEPrenom->clear();
    ui->lELogin->clear();
    ui->lEMdp->clear();
}

void PageAccueil::desactiveChampModif()
{
    if (isEdit) {
        ui->lENom->setEnabled(false);
        ui->lEPrenom->setEnabled(false);
        ui->lELogin->setEnabled(false);
        ui->lEMdp->setEnabled(false);
        ui->cBStatus->setEnabled(false);
    } else {
        ui->lENom->setEnabled(true);
        ui->cBNom->setChecked(false);

        ui->lEPrenom->setEnabled(true);
        ui->cBPrenom->setChecked(false);

        ui->lELogin->setEnabled(true);
        ui->cBLogin->setChecked(false);

        ui->lEMdp->setEnabled(true);
        ui->cBMdp->setChecked(false);

        ui->cBStatus->setEnabled(true);
        ui->checkBoxStatus->setChecked(false);
    }
}

void PageAccueil::clearConfigForm()
{
    ui->lENomConfig->clear();
    ui->cBProductTypeConfig->clear();
}

void PageAccueil::resetConfigVar()
{
    idConfig = 0;
    nbRelais = 0;
    idsEdit.clear();
    nbRelaisStr.clear();
    capteursPath.clear();
    vannesPath.clear();
    etorPath.clear();

    nomConfig.clear();
    typeConfig.clear();
    device.clear();
    deviceName.clear();

    vannesProportionnelles.clear();
    capteurs.clear();
    relais.clear();
    arrUrg.clear();
    seqTest.clear();
}

void PageAccueil::resetBtn()
{
    ui->pBBtn1->setEnabled(true);
    ui->pBBtn2->setEnabled(true);
    ui->pBBtn3->setEnabled(true);
    ui->pBBtn4->setEnabled(true);
}

void PageAccueil::closeEvent(QCloseEvent *event)
{
    delete ssl;
    event->accept();
}

void PageAccueil::on_cBNomTable_currentIndexChanged(const QString &arg1)
{
    model = new QSqlTableModel();
    model->setTable(ssl->getTable(arg1));
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    ui->tVTablesSQLite->setModel(model);
    ui->tVTablesSQLite->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tVTablesSQLite->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->lEMsgInfo->setText("Table " + ui->cBNomTable->currentText() + " sélectionnée.");
}

void PageAccueil::on_cBAjout_stateChanged(int arg1)
{
    if (arg1) {
        ui->cBAjout->setText("Modification d'un utilisateur");
        ui->pBSupprimer->setEnabled(true);
        this->isEdit = true;
        initCBUserEdit();
        setModif();
        desactiveChampModif();
    } else {
        ui->cBAjout->setText("Ajout d'un utilisateur");
        ui->pBSupprimer->setEnabled(false);
        this->isEdit = false;
        // Clear les champs :
        clearAddEditForm();
        setModif();
        desactiveChampModif();
    }
}

void PageAccueil::on_pBAddEdit_clicked()
{
    int r = 0;

    if (isEdit) {
        QStringList user = ui->cBEditUser->currentText().split('\t');
        idUser = user.first().toInt();
    }

    if (ui->cBStatus->currentText().contains("Administrateur")) {
        Administrateur a;
        a.setNom(ui->lENom->text());
        a.setPrenom(ui->lEPrenom->text());
        a.setLogin(ui->lELogin->text());
        (!isEdit && !ui->cBMdp->isChecked()) || (isEdit && ui->cBMdp->isChecked()) ?
                    a.setMdp(ui->lEMdp->text()) : a.setEncodedMdp(ui->lEMdp->text());
        if (isEdit)
            a.setId(idUser);

        r = ssl->addEditUser(a, isEdit);
    } else if (ui->cBStatus->currentText().contains("Technicien")) {
        Technicien t;
        t.setNom(ui->lENom->text());
        t.setPrenom(ui->lEPrenom->text());
        t.setLogin(ui->lELogin->text());
        (!isEdit && !ui->cBMdp->isChecked()) || (isEdit && ui->cBMdp->isChecked()) ?
                    t.setMdp(ui->lEMdp->text()) : t.setEncodedMdp(ui->lEMdp->text());
        if (isEdit)
            t.setId(idUser);

        r = ssl->addEditUser(t, isEdit);
    } else {
        Personne p;
        p.setNom(ui->lENom->text());
        p.setPrenom(ui->lEPrenom->text());
        p.setLogin(ui->lELogin->text());
        (!isEdit && !ui->cBMdp->isChecked()) || (isEdit && ui->cBMdp->isChecked()) ?
                    p.setMdp(ui->lEMdp->text()) : p.setEncodedMdp(ui->lEMdp->text());
        if (isEdit)
            p.setId(idUser);

        r = ssl->addEditUser(p, isEdit);
    }

    // Faire la fonction dans sqlservice permettant d'ajouter/modifier un user :
    if (r && isEdit) {
        ui->lEMsgInfo->setText("Modification réussi avec succès !");
    } else if (r && !isEdit) {
        ui->lEMsgInfo->setText("Ajout réussi avec succès !");
    } else {
        ui->lEMsgInfo->setText("Une erreur s'est produite lors de l'ajout / modification.");
    }

    this->clearAddEditForm();

}

void PageAccueil::on_cBEditUser_currentIndexChanged(const QString &arg1)
{
    if (isEdit && (!arg1.isNull())){
        QStringList user = arg1.split('\t');
        ui->lENom->setText(user[1]);
        ui->lEPrenom->setText(user[2]);
        ui->lELogin->setText(user[3]);
        ui->lEMdp->setText(user[4]);
        if (user[5].contains('A')) {
            ui->cBStatus->setCurrentIndex(0);
        } else if (user[5].contains('T')) {
            ui->cBStatus->setCurrentIndex(1);
        } else if (user[5].contains('P')) {
            ui->cBStatus->setCurrentIndex(2);
        }
    }
}

void PageAccueil::on_cBNom_clicked(bool checked)
{
    ui->lENom->setEnabled(checked);
}

void PageAccueil::on_cBPrenom_clicked(bool checked)
{
    ui->lEPrenom->setEnabled(checked);
}

void PageAccueil::on_cBLogin_clicked(bool checked)
{
    ui->lELogin->setEnabled(checked);
}

void PageAccueil::on_cBMdp_clicked(bool checked)
{
    ui->lEMdp->setEnabled(checked);
}

void PageAccueil::on_checkBoxStatus_clicked(bool checked)
{
    ui->cBStatus->setEnabled(checked);
}

void PageAccueil::on_pBSupprimer_clicked()
{
    if (isEdit) {
        QStringList user = ui->cBEditUser->currentText().split('\t');
        idUser = user.first().toInt();
        if (ssl->removeUser(idUser)) {
            this->on_cBNomTable_currentIndexChanged("users");
            ui->lEMsgInfo->setText("Utilisateur supprimé avec succès");
        } else {
            ui->lEMsgInfo->setText("Une erreur s'est produite lors de la suppression.");
        }

        initCBUserEdit();
    }
}

void PageAccueil::on_pBBtn4_clicked()
{
    // on fait l'inverse : on envoie un signal pour cacher ce widget en ensuite relancer l'autre.
    emit deconnexion();
    ssl->db.close();
    ssl->flag = false;
    this->hide();
}

void PageAccueil::on_pBAddConfig_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void PageAccueil::on_checkBoxAddEditConfig_stateChanged(int arg1)
{
    if (arg1) {
        ui->checkBoxAddEditConfig->setText("Modification");
        ui->pBAddConfig->setEnabled(false);
        ui->pBEditConfig->setEnabled(true);
        ui->cBEditConfig->addItems(ssl->getConfig());
        ui->cBEditConfig->setEnabled(true);
        isConfigEdit = true;
    } else {
        ui->checkBoxAddEditConfig->setText("Ajout");
        ui->pBAddConfig->setEnabled(true);
        ui->pBEditConfig->setEnabled(false);
        ui->cBEditConfig->clear();
        ui->cBEditConfig->setEnabled(false);
        isConfigEdit = false;
    }
}

void PageAccueil::on_checkBoxSuppConfig_stateChanged(int arg1)
{
    if (arg1) {
        ui->cBSuppConfig->setEnabled(true);
        // on lui ajoute les perms actuelles.
        ui->cBSuppConfig->addItems(ssl->getConfig());
        ui->pBSuppConfig->setEnabled(true);
        // Et on désactive le reste du formulaire :
        ui->checkBoxAddEditConfig->setEnabled(false);
        ui->pBAddConfig->setEnabled(false);
        ui->pBEditConfig->setEnabled(false);
        ui->cBEditConfig->setEnabled(false);
    } else {
        ui->cBSuppConfig->setEnabled(false);
        // on lui retire les perms.
        ui->cBSuppConfig->clear();
        ui->pBSuppConfig->setEnabled(false);
        // Et on réactive le reste du formulaire :
        ui->checkBoxAddEditConfig->setEnabled(true);
        if (ui->checkBoxAddEditConfig->isChecked()){
            ui->pBEditConfig->setEnabled(true);
            ui->cBEditConfig->setEnabled(true);
        } else {
            ui->pBAddConfig->setEnabled(true);
        }
    }
}

void PageAccueil::on_pBEditConfig_clicked()
{
    int i;
    QStringList caps;
    isConfigEdit = true;
    // [0] : idConfig / [1] : NomConfig / [2] TypeConfig
    idConfig = ui->cBEditConfig->currentText().split('\t')[0].toInt();
    // Faire une fonction qui retourne la config en QStringList (nbRelais, vannes, capteurs, relais, arrurg, seqtest) --> ORDRE
    QStringList cartesNi = ssl->getCarteNiByIdConfig(idConfig);
    QString line;
    if (!cartesNi.isEmpty()) {
        for (i = 0; i < cartesNi.length(); i++) {
            line = cartesNi[i];

            if (i == 0) {
                // Nb Relais :
                nbRelaisStr += nbRelaisStr.asprintf("%s", line.toLatin1().data());
                nbRelais = nbRelaisStr.split('\t').at(2).toInt();
                continue;
            }
            if (i == 1) {
                // PATH_CAPTEURS :
                capteursPath = line; /// ID / INFO / VALUE
            }
            if (i == 2) {
                // PATH_VANNES :
                vannesPath = line; /// ID / INFO / VALUE
            }
            if (i == 3) {
                // PATH_ETOR :
                etorPath = line; /// ID / INFO / VALUE
            }
            if ((i == 4) || (i == 5) || (i == 6)) {
                // Vannes Proportionnelles :
                vannesProportionnelles.append(line);
            } else if ((i == 7) || (i == 8) || (i == 9) || (i == 10) || (i == 11)) {
                // Capteurs :
                capteurs.append(line);
            }
            // Relais :
            if (nbRelais == 1) {
                if (i == 12)
                    relais.append(line);
                if (i == 13)
                    // Arrêt d'Urgence :
                    arrUrg = line;
                if (i == 14)
                    // Séquence de Tests :
                    seqTest = line;
            } else if (nbRelais == 2) {
                if ((i == 12) || (i == 13))
                    relais.append(line);
                if (i == 14)
                    // Arrêt d'Urgence :
                    arrUrg = line;
                if (i == 15)
                    // Séquence de Tests :
                    seqTest = line;
            } else if (nbRelais == 3) {
                if ((i == 12) || (i == 13) || (i == 14))
                    relais.append(line);
                if (i == 15)
                    // Arrêt d'Urgence :
                    arrUrg = line;
                if (i == 16)
                    // Séquence de Tests :
                    seqTest = line;
            }

        }
    }
    ui->stackedWidget->setCurrentIndex(12);
}

void PageAccueil::on_pBAnnulerConfig_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    clearConfigForm();
    resetConfigVar();
}

void PageAccueil::on_pBContinuerConfig_clicked()
{
    if (ui->lENomConfig->text().isEmpty()) {
        QMessageBox box;
        box.setWindowTitle("Champs obligatoires");
        box.setIcon(QMessageBox::Warning);
        box.setText("Vous avez oublié un champ.");
        box.addButton("Ok", QMessageBox::YesRole);
        box.exec();
    } else {
        nomConfig = ui->lENomConfig->text().isEmpty() ? "" : ui->lENomConfig->text();
        typeConfig = ui->cBTypeConfig->currentText();
        ui->stackedWidget->setCurrentIndex(5);
    }
}

void PageAccueil::on_stackedWidget_currentChanged(int arg1)
{
    isConfigConfirme();
    if (arg1 == 5) {
        ui->lEMsgInfo->setText("Démarrage de la configuration...");
        // Demarrer les tâches CarteNI:
        clearConfigForm();
        ui->cBProductTypeConfig->addItems(carteNI.getProductType());
    } else if (arg1 == 7) {
        ui->lEMsgInfo->setText("Configuration des Vannes Proportionnelles...");
        // Récupérer le nombre de port Out Analog :
        ui->cBAOVanneD->addItems(outAnalog);
        ui->cBAOVanneP->addItems(outAnalog);
        ui->cBAOVanneG->addItems(outAnalog);
    } else if (arg1 == 8){
        ui->lEMsgInfo->setText("Configuration des Capteurs...");
        // Récupérer le nombre de port In Analog :
        ui->cBAICapteurSortieG->addItems(inAnalog);
        ui->cBAICapteurCmdP->addItems(inAnalog);
        ui->cBAICapteurSortieD->addItems(inAnalog);
        ui->cBAICapteurCmdG->addItems(inAnalog);
        ui->cBAICapteurCmdD->addItems(inAnalog);
    } else if (arg1 == 9){
        ui->lEMsgInfo->setText("Configuration de la séquence de tests...");
        // Récupérer le nombre de port DI Lines :
        ui->cBDILineSeqTest->addItems(nbPortIn);
    } else if (arg1 == 10){
        ui->lEMsgInfo->setText("Configuration de l'arrêt d'urgence...");
        // Récupérer le nombre de port DI Lines :
        ui->cBDILineArrUrg->addItems(nbPortIn);
    } else if (arg1 == 11) {
        ui->lEMsgInfo->setText("Configuration du ou des relais...");
        ui->cBDOLineRelais->addItems(outNum);
    } else if (arg1 == 12) {
        ui->lEMsgInfo->setText("Récapitulatif de la configuration");
        makeRecap();
    }
}

void PageAccueil::on_pBAnnulerConfig_2_clicked()
{
    on_pBAnnulerConfig_clicked();
}

void PageAccueil::on_pBContinuerConfig_2_clicked()
{
    QString temp;
    inAnalog.clear();

    deviceName = ui->cBProductTypeConfig->currentText().split(" ")[0];
    device = ui->cBProductTypeConfig->currentText().split(" ")[1];
    ui->stackedWidget->setCurrentIndex(6);

    if (!ui->pBCapteurs->isEnabled() && !capteursOK) {
        ui->pBCapteurs->setEnabled(true);
    }
    if (!ui->pBVannesP->isEnabled() && !vannesOK) {
        ui->pBVannesP->setEnabled(true);
    }
    if (!ui->pBSeqTests->isEnabled() && !seqTestOK) {
        ui->pBSeqTests->setEnabled(true);
    }
    if (!ui->pBArrUrg->isEnabled() && !arrUrgOK) {
        ui->pBArrUrg->setEnabled(true);
    }
    if (!ui->pBRelais->isEnabled() && !relaisOK) {
        ui->pBRelais->setEnabled(true);
    }

    /// Vérif AI :
    if ((temp = carteNI.getIO(deviceName, 1))!= "0"){
        if (temp.split(',').length() >= 5){
            inAnalog = temp.split(',');
        } else {
            ui->pBCapteurs->setEnabled(false);
        }
        temp.clear();
    } else {
        ui->pBCapteurs->setEnabled(false);
    }

    /// Vérif AO :
    if ((temp = carteNI.getIO(deviceName, 2)) != "0"){
        outAnalog = temp.split(',');
        temp.clear();
    } else {
        ui->pBVannesP->setEnabled(false);
    }

    /// Vérif DI :
    if ((temp = carteNI.getIO(deviceName, 3)) != "0"){
        inNum = temp.split(',');
        temp.clear();
        if ((temp = carteNI.getIO(deviceName, 5)) != "0"){
            nbPortIn = temp.split(',');
            temp.clear();
        }
    } else {
        ui->pBSeqTests->setEnabled(false);
        ui->pBArrUrg->setEnabled(false);
    }

    /// Vérif DO :
    if ((temp = carteNI.getIO(deviceName, 4)) != "0"){
        outNum = temp.split(',');
        temp.clear();
        if ((temp = carteNI.getIO(deviceName, 6)) != "0"){
            if (temp.split(',').length() >= 8) {
                nbPortOut = temp.split(',');
            } else {
                ui->pBRelais->setEnabled(false);
            }
            temp.clear();
        }
    } else {
        ui->pBRelais->setEnabled(false);
    }
}

void PageAccueil::on_pBVannesP_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void PageAccueil::on_pBCapteurs_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void PageAccueil::on_pBSeqTests_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void PageAccueil::on_pBArrUrg_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}

void PageAccueil::on_pBRelais_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void PageAccueil::on_lENomConfig_returnPressed()
{
    this->on_pBContinuerConfig_clicked();
}

void PageAccueil::initConfigVar()
{
    this->vannesOK = false;
    this->capteursOK = false;
    this->seqTestOK = false;
    this->arrUrgOK = false;
    this->relaisOK = false;
}

void PageAccueil::on_pBContinuerConfig_3_clicked()
{
    int maxRange = 0, oldValue = -1;
    int i, j, k;
    QStringList vannes;
    vannes.append(ui->cBAOVanneG->currentText());
    vannes.append(ui->cBAOVanneP->currentText());
    vannes.append(ui->cBAOVanneD->currentText());

    vannes.append("PILOTAGE_GAUCHE");
    vannes.append("PILOTAGE_PRESSION");
    vannes.append("PILOTAGE_DROIT");

    vannes.append(ui->lEValueVanneG->text().isNull() ? "" : ui->lEValueVanneG->text());
    vannes.append(ui->lEValueVanneP->text().isNull() ? "" : ui->lEValueVanneP->text());
    vannes.append(ui->lEValueVanneD->text().isNull() ? "" : ui->lEValueVanneD->text());

    if ((vannes[0] == vannes[1]) || (vannes[1] == vannes[2]) || (vannes[2] == vannes[0])){
        QMessageBox::warning(this, "Attention", "Vous ne pouvez pas rattacher plusieurs vannes avec une seule et même sortie analogique.");
        return;
    } else {
        this->vannesOK = true;
        for (i = 0, j = i + 3, k = j + 3; i < 3; i++, j++, k++){
            maxRange = vannes[i].right(1).toInt();
            if (oldValue > maxRange) {
                maxRange = oldValue;
            } else {
                oldValue = maxRange;
            }
            vannesProportionnelles.append(deviceName + "\t" + device + "\t" + vannes[i] + "\t" + vannes[j] + "\t" + vannes[k]);
        }
        if (maxRange > 0) {
            vannesPath += capteursPath.asprintf("%s:%d", vannes[0].toLatin1().data(), maxRange);
        }
        ui->stackedWidget->setCurrentIndex(5);
        clearVannesForm();
        emit vannesConfigOK();
    }
}

void PageAccueil::on_pBRetour_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    clearVannesForm();
}

void PageAccueil::clearVannesForm()
{
    ui->lEValueVanneD->clear();
    ui->lEValueVanneG->clear();
    ui->lEValueVanneP->clear();
    ui->cBAOVanneD->clear();
    ui->cBAOVanneG->clear();
    ui->cBAOVanneP->clear();
}

void PageAccueil::clearRecapForm()
{
    ui->lERecapArrUrg->clear();
    ui->lERecapRelais1->clear();
    ui->lERecapRelais2->clear();
    ui->lERecapRelais3->clear();
    ui->lERecapSeqTest->clear();
    ui->lERecapCapteurCmdD->clear();
    ui->lERecapCapteurCmdG->clear();
    ui->lERecapCapteurCmdP->clear();
    ui->lERecapCapteurSortieD->clear();
    ui->lERecapCapteurSortieG->clear();
    ui->lERecapVannePilotageD->clear();
    ui->lERecapVannePilotageG->clear();
    ui->lERecapVannePilotageP->clear();

    if (!ui->lERecapRelais2->isEnabled())
        ui->lERecapRelais2->setEnabled(true);
    if (!ui->lERecapRelais3->isEnabled())
        ui->lERecapRelais3->setEnabled(true);
}

void PageAccueil::on_pBRetour_3_clicked()
{
    clearCapteursForm();
    ui->stackedWidget->setCurrentIndex(6);
}

void PageAccueil::on_pBContinuerConfig_4_clicked()
{
    int maxRange = 0, oldValue = -1;
    int i, j, k;
    QStringList caps;
    caps.append(ui->cBAICapteurSortieG->currentText());
    caps.append(ui->cBAICapteurCmdP->currentText());
    caps.append(ui->cBAICapteurSortieD->currentText());
    caps.append(ui->cBAICapteurCmdG->currentText());
    caps.append(ui->cBAICapteurCmdD->currentText());

    caps.append("SORTIE_GAUCHE");
    caps.append("CMD_PRESSION");
    caps.append("SORTIE_DROITE");
    caps.append("CMD_GAUCHE");
    caps.append("CMD_DROITE");

    caps.append(ui->lECapteurSortieG->text().isNull() ? "" : ui->lECapteurSortieG->text());
    caps.append(ui->lECapteurCmdP->text().isNull() ? "" : ui->lECapteurCmdP->text());
    caps.append(ui->lECapteurSortieD->text().isNull() ? "" : ui->lECapteurSortieD->text());
    caps.append(ui->lECapteurCmdG->text().isNull() ? "" : ui->lECapteurCmdG->text());
    caps.append(ui->lECapteurCmdD->text().isNull() ? "" : ui->lECapteurCmdD->text());

    if ((caps[0] == caps[1]) || (caps[1] == caps[2]) || (caps[2] == caps[3]) ||
            (caps[3] == caps[4]) || (caps[4] == caps[0]) || (caps[4] == caps[1]) ||
            (caps[4] == caps[2]) || (caps[3] == caps[0]) || (caps[3] == caps[1]) ||
            (caps[2] == caps[0])) {
        QMessageBox::warning(this, "Attention", "Vous ne pouvez pas rattacher plusieurs capteurs avec une seule et même entrée analogique.");
        return;
    } else {
        this->capteursOK = true;
        for (i = 0, j = i + 5, k = j + 5; i < 5; i++, j++, k++){
            maxRange = caps[i].right(1).toInt();
            if (oldValue > maxRange) {
                maxRange = oldValue;
            } else {
                oldValue = maxRange;
            }
            capteurs.append(deviceName + "\t" + device + "\t" + caps[i] + "\t" + caps[j] + "\t" + caps[k]);
        }
        if (maxRange > 0) {
            capteursPath += capteursPath.asprintf("%s:%d", caps[0].toLatin1().data(), maxRange);
        }
        ui->stackedWidget->setCurrentIndex(5);
        clearCapteursForm();
        emit capteursConfigOK();
    }
}

void PageAccueil::clearCapteursForm()
{
    ui->cBAICapteurCmdG->clear();
    ui->cBAICapteurCmdP->clear();
    ui->cBAICapteurCmdD->clear();
    ui->cBAICapteurSortieD->clear();
    ui->cBAICapteurSortieG->clear();

    ui->lECapteurCmdG->clear();
    ui->lECapteurCmdP->clear();
    ui->lECapteurCmdD->clear();
    ui->lECapteurSortieD->clear();
    ui->lECapteurSortieG->clear();
}

void PageAccueil::on_pBRetour_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void PageAccueil::clearSeqTestForm()
{
    ui->cBDILineSeqTest->clear();
    ui->lEDILineSeqTest->clear();
}

void PageAccueil::on_pBRetour_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    clearSeqTestForm();
}

void PageAccueil::on_pBContinuerConfig_5_clicked()
{
    this->seqTestOK= true;
    seqTest.append(deviceName + "\t" + device + "\t" + ui->cBDILineSeqTest->currentText() + "\t" + "SEQ_TESTS" + "\t" +
                   (ui->lEDILineSeqTest->text().isNull() ? "" : ui->lEDILineSeqTest->text()));
    ui->stackedWidget->setCurrentIndex(5);
    clearSeqTestForm();
    emit seqTestConfigOK();
}

void PageAccueil::on_pBRetour_5_clicked()
{
    clearArrUrgForm();
    ui->stackedWidget->setCurrentIndex(6);
}

void PageAccueil::clearArrUrgForm()
{
    ui->cBDILineArrUrg->clear();
    ui->lEDILineArrUrg->clear();
}

void PageAccueil::on_pBContinuerConfig_6_clicked()
{
    arrUrgOK = true;
    arrUrg.append(deviceName + "\t" + device + "\t" + ui->cBDILineArrUrg->currentText() + "\t" + "ARR_URG" + "\t" +
                  (ui->lEDILineArrUrg->text().isNull() ? "" : ui->lEDILineArrUrg->text()));
    ui->stackedWidget->setCurrentIndex(5);
    clearArrUrgForm();
    emit arrUrgConfigOK();
}

void PageAccueil::isConfigConfirme()
{
    if (vannesOK && capteursOK && seqTestOK && arrUrgOK && relaisOK){
        emit configPrete();
        ui->stackedWidget->setCurrentIndex(5);
        initConfigVar();
    }
}

void PageAccueil::clearRelaisForm()
{
    ui->cBDOLineRelais->clear();
    ui->lEDOLineRelais->clear();
}

void PageAccueil::on_pBValidConfig_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void PageAccueil::on_pBRetour_6_clicked()
{
    clearRelaisForm();
    ui->stackedWidget->setCurrentIndex(6);
}

void PageAccueil::on_pBContinuerConfig_7_clicked()
{
    nbRelais++;
    relaisOK = true;

    if (nbRelais == 1){
        relais.append(deviceName + "\t" + device + "\t" + ui->cBDOLineRelais->currentText() + "/line0:7\t" + "Relais" + QString::number(nbRelais)
                      + "\t" + (ui->lEDOLineRelais->text().isNull() ? "" : ui->lEDOLineRelais->text()));
    } else if (nbRelais == 2){
        relais.append(deviceName + "\t" + device + "\t" + ui->cBDOLineRelais->currentText() + "/line0:7\t" + "Relais" + QString::number(nbRelais)
                      + "\t" + (ui->lEDOLineRelais->text().isNull() ? "" : ui->lEDOLineRelais->text()));
    } else if (nbRelais == 3) {
        relais.append(deviceName + "\t" + device + "\t" + ui->cBDOLineRelais->currentText() + "/line0:7\t" + "Relais" + QString::number(nbRelais)
                      + "\t" + (ui->lEDOLineRelais->text().isNull() ? "" : ui->lEDOLineRelais->text()));
        emit relaisConfigOK();
    }
    ui->stackedWidget->setCurrentIndex(5);
    clearRelaisForm();
}

void PageAccueil::makeRecap()
{
    int i;
    if (isConfigEdit) {
        QStringList modif;

        for (i = 0; i < vannesProportionnelles.length(); i++){
            modif = vannesProportionnelles.at(i).split('\t');
            idsEdit.append(modif.takeAt(0).toInt());
            vannesProportionnelles.replace(i, modif.join('\t'));
        }

        for (i = 0; i < capteurs.length(); i++){
            modif = capteurs.at(i).split('\t');
            idsEdit.append(modif.takeAt(0).toInt());
            capteurs.replace(i, modif.join('\t'));
        }

        for (i = 0; i < relais.length(); i++){
            modif = relais.at(i).split('\t');
            idsEdit.append(modif.takeAt(0).toInt());
            relais.replace(i, modif.join('\t'));
        }

        modif = arrUrg.split('\t');
        idsEdit.append(modif.takeAt(0).toInt()); /// INFO / VALUE
        arrUrg = modif.join('\t');

        modif = seqTest.split('\t');
        idsEdit.append(modif.takeAt(0).toInt()); /// INFO / VALUE
        seqTest = modif.join('\t');
    }

    ui->lERecapCapteurSortieG->setText((capteurs[0].isNull() ? "" : capteurs[0]));
    ui->lERecapCapteurCmdP->setText((capteurs[1].isNull() ? "" : capteurs[1]));
    ui->lERecapCapteurSortieD->setText((capteurs[2].isNull() ? "" : capteurs[2]));
    ui->lERecapCapteurCmdG->setText((capteurs[3].isNull() ? "" : capteurs[3]));
    ui->lERecapCapteurCmdD->setText((capteurs[4].isNull() ? "" : capteurs[4]));

    ui->lERecapVannePilotageD->setText((vannesProportionnelles[0].isNull() ? "" : vannesProportionnelles[0]));
    ui->lERecapVannePilotageP->setText((vannesProportionnelles[1].isNull() ? "" : vannesProportionnelles[1]));
    ui->lERecapVannePilotageG->setText((vannesProportionnelles[2].isNull() ? "" : vannesProportionnelles[2]));

    ui->lERecapArrUrg->setText((arrUrg.isNull() ? "" : arrUrg));

    ui->lERecapSeqTest->setText(seqTest.isNull() ? "" : seqTest);

    if (nbRelais == 3) {
        ui->lERecapRelais1->setText((relais[0].isNull() ? "" : relais[0]));
        ui->lERecapRelais2->setText((relais[1].isNull() ? "" : relais[1]));
        ui->lERecapRelais3->setText((relais[2].isNull() ? "" : relais[2]));
    } else if (nbRelais == 2) {
        ui->lERecapRelais3->setEnabled(false);
        ui->lERecapRelais1->setText((relais[0].isNull() ? "" : relais[0]));
        ui->lERecapRelais2->setText((relais[1].isNull() ? "" : relais[1]));
    } else if (nbRelais == 1) {
        ui->lERecapRelais3->setEnabled(false);
        ui->lERecapRelais2->setEnabled(false);
        ui->lERecapRelais1->setText((relais[0].isNull() ? "" : relais[0]));
    }
}

void PageAccueil::on_pBConfirmer_clicked()
{
    QString tempo, path;
    int maxRange = 0, oldValue = -1, seqTestMax = 0, arrUrgMax = 0, idEtor = 0;
    // Ajout / Modification de la configuration dans la BdD :
    // Dans un premier temps, ajouter la config (NomConfig / Type) :
    if (!isConfigEdit) {
        /// Prendre en compte pour ETOR_PATH SeqTests & ArrUrg !!!

        path = seqTest.split('\t').at(2);
        seqTestMax = path.right(1).toInt();

        path = arrUrg.split('\t').at(2);
        arrUrgMax = path.right(1).toInt();

        if (seqTestMax < arrUrgMax) {
            path = seqTest.split('\t').at(2);
            path.append(":" + QString::number(arrUrgMax));
            etorPath = path;
        } else {
            path = arrUrg.split('\t').at(2);
            path.append(":" + QString::number(seqTestMax));
            etorPath = path;
        }

        if ((idConfig = ssl->addEditConfig(nomConfig, typeConfig)) > 0) {
            // Config ajouté, on passe à la table carteNi :
            if (ssl->addCarteNI(vannesProportionnelles, capteurs, relais, arrUrg, seqTest, idConfig, nbRelais, capteursPath, vannesPath, etorPath)) {
                // Config ajouté, on nettoye le récapitulatif et on ramène à la page d'accueil :
                clearRecapForm();
                QTimer::singleShot(1000, this, SLOT(ajoutConfigReussi(void)));
            } else {
                ui->lEMsgInfo->setText("Erreur lors de l'ajout de la configuration !!!");
            }
        }
    } else {
        // on récupère les nouvelles valeurs :
        vannesProportionnelles.replace(0, ui->lERecapVannePilotageD->text());
        vannesProportionnelles.replace(1, ui->lERecapVannePilotageP->text());
        vannesProportionnelles.replace(2, ui->lERecapVannePilotageG->text());

        for (QString vanne : vannesProportionnelles){
            path = vanne.split('\t').at(2);
            maxRange = path.right(1).toInt();
            if (oldValue > maxRange) {
                maxRange = oldValue;
            } else {
                oldValue = maxRange;
            }
        }
        if (maxRange > 0) {
            int id = vannesPath.split('\t').at(0).toInt();
            tempo = vannesPath.split('\t').at(1);
            path = vannesProportionnelles.at(0).split('\t').at(2);
            path.append(":" + QString::number(maxRange));
            vannesPath = vannesPath.asprintf("%d\t%s\t%s", id, tempo.toLatin1().data(), path.toLatin1().data());
        }

        maxRange = 0;
        oldValue = -1;

        capteurs.replace(0, ui->lERecapCapteurSortieG->text());
        capteurs.replace(1, ui->lERecapCapteurCmdP->text());
        capteurs.replace(2, ui->lERecapCapteurSortieD->text());
        capteurs.replace(3, ui->lERecapCapteurCmdG->text());
        capteurs.replace(4, ui->lERecapCapteurCmdD->text());

        for (QString cap : capteurs) {
            path = cap.split('\t').at(2);
            maxRange = path.right(1).toInt();
            if (oldValue > maxRange) {
                maxRange = oldValue;
            } else {
                oldValue = maxRange;
            }
        }
        if (maxRange > 0) {
            int id = capteursPath.split('\t').at(0).toInt();
            tempo = capteursPath.split('\t').at(1);
            path = capteurs.at(0).split('\t').at(2);
            path.append(":" + QString::number(maxRange));
            capteursPath = capteursPath.asprintf("%d\t%s\t%s", id, tempo.toLatin1().data(), path.toLatin1().data());
        }

        switch (nbRelais) {
        default:
            ui->lEMsgInfo->setText("Erreur lors de la récupération du nombre de relais !");
            break;
        case 1:
            relais.replace(0, ui->lERecapRelais1->text());
            break;
        case 2:
            relais.replace(0, ui->lERecapRelais1->text());
            relais.replace(1, ui->lERecapRelais2->text());
            break;
        case 3:
            relais.replace(0, ui->lERecapRelais1->text());
            relais.replace(1, ui->lERecapRelais2->text());
            relais.replace(2, ui->lERecapRelais3->text());
            break;
        }

        arrUrg = ui->lERecapArrUrg->text();

        seqTest = ui->lERecapSeqTest->text();

        /// Prendre en compte pour ETOR_PATH SeqTests & ArrUrg !!!

        path = seqTest.split('\t').at(2);
        seqTestMax = path.right(1).toInt();

        path = arrUrg.split('\t').at(2);
        arrUrgMax = path.right(1).toInt();

        if (seqTestMax < arrUrgMax) {
            path = seqTest.split('\t').at(2);
            path.append(":" + QString::number(arrUrgMax));
            idEtor = etorPath.split('\t').at(0).toInt();
            tempo = etorPath.split('\t').at(1);
            etorPath = etorPath.asprintf("%d\t%s\t%s", idEtor, tempo.toLatin1().data(), path.toLatin1().data());
        } else {
            path = arrUrg.split('\t').at(2);
            path.append(":" + QString::number(seqTestMax));
            idEtor = etorPath.split('\t').at(0).toInt();
            tempo = etorPath.split('\t').at(1);
            etorPath = etorPath.asprintf("%d\t%s\t%s", idEtor, tempo.toLatin1().data(), path.toLatin1().data());
        }

        if (ssl->editCarteNI(vannesProportionnelles, capteurs, relais, arrUrg, seqTest, idsEdit, capteursPath, vannesPath, etorPath)) {
            // Config modifié, on nettoye le récapitulatif et on ramène à la page d'accueil :
            ui->checkBoxAddEditConfig->setChecked(false);
            clearRecapForm();
            QTimer::singleShot(1000, this, SLOT(ajoutConfigReussi(void)));
        } else {
            ui->lEMsgInfo->setText("Erreur lors de la modification de la configuration !!!");
        }
    }
}

void PageAccueil::on_pBSuppConfig_clicked()
{
    QAbstractButton *btn1;
    QMessageBox box;
    box.setWindowTitle("Confirmation Suppression");
    box.setIcon(QMessageBox::Warning);
    btn1 = box.addButton("Oui", QMessageBox::YesRole);
    box.addButton("Non", QMessageBox::NoRole);
    box.setText("Vous confirmez la suppression de cette configuration ?");
    box.exec();

    if (box.clickedButton() == btn1) {
        if (ssl->removeConfig(ui->cBSuppConfig->currentText().split('\t').at(0).toInt())) {
            ui->lEMsgInfo->setText("Suppression de la configuration réussie !");
            emit configSupprimee();
        } else {
            ui->lEMsgInfo->setText("Erreur lors de la suppression de la configuration !!!");
        }
    }
    delete btn1;
}

void PageAccueil::on_pBAnnuler_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lEMsgInfo->setText("Configuration annulée !");
    clearConfigForm();
    resetConfigVar();
    initConfigVar();
}
