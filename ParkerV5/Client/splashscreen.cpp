#include "splashscreen.h"
#include "ui_splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);

    progression = 0;
    this->ssl = new SqlService();

    if (ssl->connexionBase()) {
        configs = ssl->getConfig();
        if( configs.isEmpty() ) {
            QMessageBox::critical(this, "Corruption de la BdD !!!", ssl->getMsgInfo());
            this->close();
        } else {
            ui->cBChoixConfig->addItems(configs);
        }
    } else {
        ui->textEdit->append("Connexion à la BdD impossible !");
        this->close();
    }
}

SplashScreen::~SplashScreen()
{
    if (ssl != nullptr)
        delete ssl;
    delete ui;
}

void SplashScreen::on_pBQuitter_clicked()
{
    QMessageBox box;
    box.setWindowTitle("Confirmation");
    box.setIcon(QMessageBox::Question);
    box.setText("Vous êtes sûr de vouloir quitter cette application ?");
    QAbstractButton *btn1 = box.addButton("Oui", QMessageBox::YesRole);
    box.addButton("Non", QMessageBox::NoRole);
    box.exec();
    if (box.clickedButton() == btn1) {
        this->close();
    }
}

void SplashScreen::on_cBChoixConfig_currentIndexChanged(int index)
{
    index = 0;
    ui->pBLancerConfig->setEnabled(true);
}

void SplashScreen::on_pBLancerConfig_clicked()
{
    ui->pBLancerConfig->setEnabled(false);
    int indexRelais = 0;
    int valInt = 0;
    QStringList values;
    QStringList line;
    if (ssl->getFlag()) {
        values = ssl->getCarteNiByIdConfig(ui->cBChoixConfig->currentText().split('\t').at(0).toInt());
        /// DeviceName  Path    Info    Value
        /// At(0) : NB_MODULE_RELAIS    Value
        /// Là on ajoute le type (Lycée ou Parker) :
        if (ui->cBChoixConfig->currentText().split('\t').at(2).contains("Parker")) {
            valInt = 0;
            conf = PhysiqueFactory::getConfigEnCours(0); // Parker
        } else {
            valInt = 1;
            conf = PhysiqueFactory::getConfigEnCours(1); // Lycée
        }
        // On construit la Map :
        for (QString val : values) {
//            qWarning("%s", val.toLatin1().data());
            line = val.split('\t');
            if (line.length() == 2) {
                // PATH_CAPTEURS Ou NB_MODULE_RELAIS ou PATH_VANNES:
                if (line.at(0).contains("PATH_CAPTEURS")){
                    // PATH_CAPTEURS
                    conf->setDevCapteurP(line.at(1));
//                    qWarning("DevCapteurP : %s", conf->devCapteurP.toLatin1().data()); // OK
                } else if (line.at(0).contains("PATH_VANNES")){
                    // PATH_VANNES
                    conf->setDevVannesP(line.at(1));
//                    qWarning("DevVannesP : %s", conf->devVannesP.toLatin1().data()); // OK
                } else if (line.at(0).contains("PATH_ETOR")) {
                    conf->setdevETOR(line.at(1));
//                    qWarning("DevETOR : %s", conf->devETOR.toLatin1().data()); // OK
                } else {
                    // NB_MODULE_RELAIS
                    conf->setNB_MODULE_RELAIS(line.at(1).toInt());
//                    qWarning("NB_MODULE_RELAIS : %d", conf->NB_MODULE_RELAIS); // OK
                }
                continue;
            } else if (line.at(2).left(line.at(2).length() - 1).contains("Relais")){
                /// RelaisPath
                indexRelais++;
                if (indexRelais == 1){
                    conf->setDevRelaisMod4(line.at(1));
//                    qWarning("DevRelaisMod4 : %s", conf->devRelaisMod4.toLatin1().data());
                } else if (indexRelais == 2){
                    conf->setDevRelaisMod5(line.at(1));
//                    qWarning("DevRelaisMod5 : %s", conf->devRelaisMod5.toLatin1().data());
                } else if (indexRelais == 3){
                    conf->setDevRelaisMod6(line.at(1));
//                    qWarning("DevRelaisMod6 : %s", conf->devRelaisMod6.toLatin1().data());
                }
                continue;
            } else {
                // length = 4 ( de l'indice 0 à 3)
                if (line.at(2).contains("PILOTAGE_GAUCHE")){
                    conf->setPILOTAGE_GAUCHE(line.at(1).right(1).toInt());
//                    qWarning("PILOTAGE_GAUCHE : %d", conf->PILOTAGE_GAUCHE);
                } else if (line.at(2).contains("PILOTAGE_PRESSION")){
                    conf->setPILOTAGE_PRESSION(line.at(1).right(1).toInt());
//                    qWarning("PILOTAGE_PRESSION : %d", conf->PILOTAGE_PRESSION);
                } else if (line.at(2).contains("PILOTAGE_DROIT")){
                    conf->setPILOTAGE_DROIT(line.at(1).right(1).toInt());
//                    qWarning("PILOTAGE_DROIT : %d", conf->PILOTAGE_DROIT);
                } else if (line.at(2).contains("CMD_PRESSION")){
                    conf->setCMD_PRESSION(line.at(1).right(1).toInt());
//                    qWarning("CMD_PRESSION : %d", conf->CMD_PRESSION);
                } else if (line.at(2).contains("SORTIE_DROITE")){
                    conf->setSORTIE_DROITE(line.at(1).right(1).toInt());
//                    qWarning("SORTIE_DROITE : %d", conf->SORTIE_DROITE);
                } else if (line.at(2).contains("CMD_GAUCHE")){
                    conf->setCMD_GAUCHE(line.at(1).right(1).toInt());
//                    qWarning("CMD_GAUCHE : %d", conf->CMD_GAUCHE);
                } else if (line.at(2).contains("CMD_DROITE")){
                    conf->setCMD_DROITE(line.at(1).right(1).toInt());
//                    qWarning("CMD_DROITE : %d", conf->CMD_DROITE);
                } else if (line.at(2).contains("SORTIE_GAUCHE")){
                    conf->setSORTIE_GAUCHE(line.at(1).right(1).toInt());
//                    qWarning("SORTIE_GAUCHE : %d", conf->SORTIE_GAUCHE);
                } else if (line.at(2).contains("ARR_URG")){
                    conf->setAU(line.at(1).right(1).toInt());
//                    qWarning("ARR_URG : %d", conf->AU);
                } else if (line.at(2).contains("SEQ_TESTS")){
                    conf->setETOR(line.at(1).right(1).toInt());
//                    qWarning("SEQ_TESTS : %d", conf->ETOR);
                }
                continue;
            }
        }
    }
    conf->setELEC(ui->checkBoxELEC->isChecked());
    emit launchingEmitted(valInt);
}

void SplashScreen::majProgressBar(int decalage)
{
    progression += decalage;
    ui->progressBar->setValue(progression);
    if (progression == 100) {
        // On emit le signal
        emit lancement();
        this->close();
    }
}

void SplashScreen::majInfo(QString info)
{
    ui->textEdit->append(info);
}

void SplashScreen::on_checkBoxELEC_stateChanged(int arg1)
{
    if (arg1)
        ui->checkBoxELEC->setText("Avec GPIB");
    else
        ui->checkBoxELEC->setText("Sans GPIB");
}
