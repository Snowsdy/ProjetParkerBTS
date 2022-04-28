#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QMessageBox>
#include <QStringList>
#include <QSqlDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    darkMode = false;
    progression = 0;
    choixMode = 0;
    modeAuto = false;
    compteurDeTest = 0; // pour test automatique
    m = nullptr;
    model = nullptr;
    conf = nullptr;
    param = nullptr;
    update = new QTimer();
    initTabWidget();
    initMainWindow();
}

MainWindow::~MainWindow()
{
    if (m != nullptr)
        delete m;
    if( conf != nullptr)
        delete conf;
    if(param != nullptr)
        delete param;
    if(model)
        delete model;
    delete ui;
}

void MainWindow::initMainWindow(void){
    for(int nbModule=0; nbModule < 3; nbModule++)
        for(int numLigne=0; numLigne < 8; numLigne++)
            tabLedMod[nbModule][numLigne]->setColor(Qt::black);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m != nullptr)
        m->stopBanc();
    event->accept();
}

void MainWindow::on_pushButton_2_clicked()
{
    monStyle = this->styleSheet();
    QFont font = this->font();
    QStyle *style = this->style();
    QFile f(":qdarkstyle/style.qss");

    if (!darkMode) {
        if (!f.exists())   {
            ui->lE_PneuStatus->setText("Feuille de Style non trouvé !");
        }
        else   {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
            darkMode = true;
            ui->pushButton_2->setText("Mode Clair");
            f.close();
            /* Pour les manomètres : */
            ui->manoPressionElec->setStyleSheet("color: black;");
            ui->manoPressionPneu->setStyleSheet("color: black;");
            /* Pour les boutons d'arrêt Pneu / Elec : */
            ui->pB_StopTestElec->setStyleSheet(ui->pB_StopTestElec->styleSheet() + "color: black;");
            ui->pB_StopTestPneu->setStyleSheet(ui->pB_StopTestPneu->styleSheet() + "color: black;");
            /* Pour les boutons Sueil, Tmp rèp, Auto pour Pneu / Elec : */
            ui->pB_TestPneuSeuil->setStyleSheet(ui->pB_TestPneuSeuil->styleSheet() + "color: black;");
            ui->pB_TestElecSeuil->setStyleSheet(ui->pB_TestElecSeuil->styleSheet() + "color: black;");

            ui->pB_TestAutoPneu->setStyleSheet(ui->pB_TestAutoPneu->styleSheet() + "color: black;");
            ui->pB_TestAutoElec->setStyleSheet(ui->pB_TestAutoElec->styleSheet() + "color: black;");

            ui->pB_TestPneuTpsRep->setStyleSheet(ui->pB_TestPneuTpsRep->styleSheet() + "color: black;");
            ui->pB_TestElecTpsRep->setStyleSheet(ui->pB_TestElecTpsRep->styleSheet() + "color: black;");
        }
    } else {
        darkMode = false;
        qApp->setStyle(style);
        qApp->setFont(font);
        qApp->setStyleSheet(monStyle);
        ui->pushButton_2->setText("Mode Sombre");
    }
}

void MainWindow::initTabWidget(void){
    tabLedMod[0][0]=ui->ledMod4port0line0;
    tabLedMod[0][1]=ui->ledMod4port0line1;
    tabLedMod[0][2]=ui->ledMod4port0line2;
    tabLedMod[0][3]=ui->ledMod4port0line3;
    tabLedMod[0][4]=ui->ledMod4port0line4;
    tabLedMod[0][5]=ui->ledMod4port0line5;
    tabLedMod[0][6]=ui->ledMod4port0line6;
    tabLedMod[0][7]=ui->ledMod4port0line7;

    tabLedMod[1][0]=ui->ledMod5port0line0;
    tabLedMod[1][1]=ui->ledMod5port0line1;
    tabLedMod[1][2]=ui->ledMod5port0line2;
    tabLedMod[1][3]=ui->ledMod5port0line3;
    tabLedMod[1][4]=ui->ledMod5port0line4;
    tabLedMod[1][5]=ui->ledMod5port0line5;
    tabLedMod[1][6]=ui->ledMod5port0line6;
    tabLedMod[1][7]=ui->ledMod5port0line7;

    tabLedMod[2][0]=ui->ledMod6port0line0;
    tabLedMod[2][1]=ui->ledMod6port0line1;
    tabLedMod[2][2]=ui->ledMod6port0line2;
    tabLedMod[2][3]=ui->ledMod6port0line3;
    tabLedMod[2][4]=ui->ledMod6port0line4;
    tabLedMod[2][5]=ui->ledMod6port0line5;
    tabLedMod[2][6]=ui->ledMod6port0line6;
    tabLedMod[2][7]=ui->ledMod6port0line7;

    tabCbMod[0][0]= ui->cB_Mod4port0line0;
    tabCbMod[0][1]= ui->cB_Mod4port0line1;
    tabCbMod[0][2]= ui->cB_Mod4port0line2;
    tabCbMod[0][3]= ui->cB_Mod4port0line3;
    tabCbMod[0][4]= ui->cB_Mod4port0line4;
    tabCbMod[0][5]= ui->cB_Mod4port0line5;
    tabCbMod[0][6]= ui->cB_Mod4port0line6;
    tabCbMod[0][7]= ui->cB_Mod4port0line7;

    tabCbMod[1][0]= ui->cB_Mod5port0line0;
    tabCbMod[1][1]= ui->cB_Mod5port0line1;
    tabCbMod[1][2]= ui->cB_Mod5port0line2;
    tabCbMod[1][3]= ui->cB_Mod5port0line3;
    tabCbMod[1][4]= ui->cB_Mod5port0line4;
    tabCbMod[1][5]= ui->cB_Mod5port0line5;
    tabCbMod[1][6]= ui->cB_Mod5port0line6;
    tabCbMod[1][7]= ui->cB_Mod5port0line7;

    tabCbMod[2][0]= ui->cB_Mod6port0line0;
    tabCbMod[2][1]= ui->cB_Mod6port0line1;
    tabCbMod[2][2]= ui->cB_Mod6port0line2;
    tabCbMod[2][3]= ui->cB_Mod6port0line3;
    tabCbMod[2][4]= ui->cB_Mod6port0line4;
    tabCbMod[2][5]= ui->cB_Mod6port0line5;
    tabCbMod[2][6]= ui->cB_Mod6port0line6;
    tabCbMod[2][7]= ui->cB_Mod6port0line7;

    tabLcdMod1[0]= ui->lcdMod1ai0;
    tabLcdMod1[1]= ui->lcdMod1ai1;
    tabLcdMod1[2]= ui->lcdMod1ai2;
    tabLcdMod1[3]= ui->lcdMod1ai3;
    tabLcdMod1[4]= ui->lcdMod1ai4;
    tabLcdMod1[5]= ui->lcdMod1ai5;
    tabLcdMod1[6]= ui->lcdMod1ai6;
    tabLcdMod1[7]= ui->lcdMod1ai7;

    tabPushBPneu[0]= ui->pB_ConfigPneu;
    tabPushBPneu[1]= ui->pB_SauvegardePneu;
    tabPushBPneu[2]= ui->pB_StopTestPneu;
    tabPushBPneu[3]= ui->pB_TestAutoPneu;
    tabPushBPneu[4]= ui->pB_TestPneuSeuil;
    tabPushBPneu[5]= ui->pB_TestPneuTpsRep;

    tabPushBElec[0]= ui->pBConfigElec;
    tabPushBElec[1]= ui->pB_SauvegardeElec;
    tabPushBElec[2]= ui->pB_StopTestElec;
    tabPushBElec[3]= ui->pB_TestAutoElec;
    tabPushBElec[4]= ui->pB_TestElecSeuil;
    tabPushBElec[5]= ui->pB_TestElecTpsRep;
}

void MainWindow::majProgressBar(int decalage){
    emit progress(decalage);
}

void MainWindow::demarrageBanc(void){
    if (progression != 100){
        emit info("ERREUR : Lire le message d'erreur et résoudre le problème...");
    } else {
        emit info("Banc opérationnel...");
    }
}

void MainWindow::debloqueOnglets(void){
    int nb=ui->tabWidget->count();
    for(int i=0; i < nb; i++)
        ui->tabWidget->setTabEnabled(i, true);
}

void MainWindow::bloqueOnglets(int sauf){
    int nb=ui->tabWidget->count();
    for(int i=0; i < nb ; i++)
        if(i != sauf)
            ui->tabWidget->setTabEnabled(i, false);
}

void MainWindow::initOngletModeManuel(void){
//    qWarning("mise à jour onglet mode manuel");
    connect(update, SIGNAL(timeout()), this, SLOT(lirePression()));
    connect(update, SIGNAL(timeout()), this, SLOT(lireEntreeTOR()));

    update->start(500);
    ui->hSliderMod2ao0->setValue(m->vannesP->getTabVal(conf->PILOTAGE_DROIT)*10);
    ui->hSliderMod2ao1->setValue(m->vannesP->getTabVal(conf->PILOTAGE_PRESSION)*10);
    ui->hSliderMod2ao2->setValue(m->vannesP->getTabVal(conf->PILOTAGE_GAUCHE)*10);
    for(int nbModule=0; nbModule < conf->NB_MODULE_RELAIS; nbModule++){
        for(int numLigne=0; numLigne<8; numLigne++){
            if(m->relais[nbModule]->getEtatRelais(numLigne)){
                tabLedMod[nbModule][numLigne]->setColor(Qt::red);
                tabCbMod[nbModule][numLigne]->setChecked(true);
            }
            else {
                tabLedMod[nbModule][numLigne]->setColor(Qt::green);
                tabCbMod[nbModule][numLigne]->setChecked(false);
            }
        }
    }
}

void MainWindow::initOngletPneu(double p){
    ui->manoPressionPneu->setValue(p);
    ui->lE_PneuStatus->setText("Configuration OK - test autorisé");
    mesuresTestPneu.razMesures();
    clearChampsPneu();
    initGraphe();
    stopBloqCommandePneu();
}

void MainWindow::initOngletElec(double p){
    ui->manoPressionElec->setValue(p);
    ui->lE_ElecStatus->setText("Configuration OK - test autorisé");
    mesuresTestElec.razMesures();
    clearChampsElec();
    initGraphe();
    stopBloqCommandeElec();
}

void MainWindow::clearChampsPneu(void){
    ui->lE_PneuSeuilDecl->clear();
    ui->lE_PneuSeuilEncl->clear();
    ui->lE_PneuTpsRepDecl->clear();
    ui->lE_PneuTpsRepEncl->clear();
}

void MainWindow::clearChampsElec(void){
    ui->lE_ElecSeuilEncl_2->clear();
    ui->lE_ElecSeuilDecl_2->clear();
    ui->lE_ElecTpsRepDecl_2->clear();
    ui->lE_ElecTpsRepEncl_2->clear();
    ui->lE_ElecSeuilCourantEncl->clear();
    ui->lE_ElecSeuilCourantDecl->clear();
}

void MainWindow::stopBloqCommandePneu(void){
    for(int i=0; i < 6; i++)
        tabPushBPneu[i]->setEnabled(true);
}

void MainWindow::stopBloqCommandeElec(void){
    for(int i=0; i < 6; i++)
        tabPushBElec[i]->setEnabled(true);
}

void MainWindow::bloqCommandePneu(void){
    for(int i=0; i < 6; i++){
        tabPushBPneu[i]->setEnabled(false);
        tabPushBPneu[i]->repaint();
    }
}

void MainWindow::bloqCommandeElec(void){
    for(int i=0; i < 6; i++){
        tabPushBElec[i]->setEnabled(false);
        tabPushBElec[i]->repaint();
    }
}

// Mise à jour courbes : appelée à chaque nouveau changement de la commande 'rampe' toutes les 100 ms.
void MainWindow::miseAJourRampe(double mesure){
    valRampe= mesure;
    if(valRampe < 0.1)
        valRampe= 0.0; // pour problème d'affichage en précision du lcdTension

    if(index >= 999){ // limite de 1000 acquisitions (ex: 24V, 240 acq avec un pas de 0.1)


        if(indicateurDeTest == TEST_PNEU_SEUIL){
            ui->lE_PneuStatus->setText("Sécurité=>Fin du test !"); // 100x1000ms=100s max
        }
        else{
            ui->lE_ElecStatus->setText("Sécurité=>Fin du test !"); // 100x1000ms=100s max
        }
        m->rampePneu->stop();
    }
    else {
        if(index == 50) { // && (index % 10) == 0)
            ui->graphePneu->setAxisAutoScale(2, true);
            ui->graphePneu->updateAxes();
        }
        tabTemps[index]=(double)index;
        if(m->getMode()==PNEUMATIQUE)
            tabRampe[index]= valRampe; //  ou utiliser valAcqCmdG
        else
            tabRampe[index]= valRampe; // ou valeur acq multimètre à faire
        tabAcqSortieG[index]= valAcqSortieG;
        tabAcqSortieD[index]= valAcqSortieD;

        courbeRampe.setRawSamples(tabTemps,  tabRampe, index);
        courbeSortieG.setRawSamples(tabTemps, tabAcqSortieG, index);
        courbeSortieD.setRawSamples(tabTemps, tabAcqSortieD, index);
        if(m->getMode()==PNEUMATIQUE)
            ui->graphePneu->replot();
        else
            ui->grapheElec->replot();
        index++;
    }
}

// appelée à chaque nouvelle acquisition
void MainWindow::miseAJourAcq(double *mes){
    valAcqSortieG = mes[conf->SORTIE_GAUCHE];
    valAcqSortieD = mes[conf->SORTIE_DROITE];
    valAcqPression = mes[conf->CMD_PRESSION];
    if((m->getTypeDistributeur()==BISTABLE) && (m->getOperation()==DECLENCHEMENT))
        valAcqCmdG = mes[conf->CMD_DROITE];
    else
        valAcqCmdG = mes[conf->CMD_GAUCHE];

    int valTemp = valAcqPression*100;
    valAcqPression = valTemp/100.0; // arrondi au dixième
    ui->manoPressionPneu->setValue(valAcqPression);

    if(m->getMode()==ELECTRIQUE)
        ui->lcdTension->display(valRampe);
}

// Appelée si problème matériel et pendant phase de mise en marche et d'arrêt du banc de test
void MainWindow::majInfo(QString msgInfo){
    emit info(msgInfo);
}

void MainWindow::initGraphe(void){
    courbeSortieG.setStyle(QwtPlotCurve::Lines ); // Lines, Dots, Sticks, Steps
    if (darkMode)
        courbeSortieG.setPen(QColor(Qt::green));
    else
        courbeSortieG.setPen(QColor(Qt::darkGreen));
    courbeSortieG.setCurveAttribute(QwtPlotCurve::Fitted ); // Fitted with Lines
    courbeSortieD.setStyle(QwtPlotCurve::Lines );
    if (darkMode)
        courbeSortieD.setPen(QColor(Qt::red));
    else
        courbeSortieD.setPen(QColor(Qt::darkRed));
    courbeSortieD.setCurveAttribute(QwtPlotCurve::Fitted );
    courbeRampe.setStyle(QwtPlotCurve::Lines );
    if (darkMode)
        courbeRampe.setPen(QColor(Qt::blue));
    else
        courbeRampe.setPen(QColor(Qt::darkBlue));
    courbeRampe.setCurveAttribute(QwtPlotCurve::Fitted ); // Inverted with Steps
    if(m->getMode()==PNEUMATIQUE){
        ui->graphePneu->setAxisScale(0, 0.0, m->getPressionAlim(), 0);
        ui->graphePneu->setAxisScale(2, 0.0, m->getPressionAlim()*10.0, 0);
    } else {
        ui->grapheElec->setAxisScale(0, 0.0, m->getTensionAlim(), 0);
        ui->grapheElec->setAxisScale(2, 0.0, m->getTensionAlim()*10.0, 0);
    }
    courbeSortieG.detach();
    courbeSortieD.detach();
    courbeRampe.detach();
    for(int i=0; i<1000; i++){
        tabTemps[i]=0.0;
        tabAcqSortieG[i]=0.0;
        tabAcqSortieD[i]=0.0;
        tabRampe[i]=0.0;
    }
    index= 0;
    if(m->getMode()==PNEUMATIQUE){
        courbeSortieG.attach(ui->graphePneu);
        courbeSortieD.attach(ui->graphePneu);
        courbeRampe.attach(ui->graphePneu);
        ui->graphePneu->replot();
    } else {
        courbeSortieG.attach(ui->grapheElec);
        courbeSortieD.attach(ui->grapheElec);
        courbeRampe.attach(ui->grapheElec);
        ui->grapheElec->replot();
    }
}

void MainWindow::arretBanc(void){
    Sleep(1000);
    this->close();
}

void MainWindow::testSeuil(void){
    if(indicateurDeTest == TEST_PNEU_SEUIL)
        bloqCommandePneu();
    else
        bloqCommandeElec();
    if(sortieDejaActive()){
        if(indicateurDeTest == TEST_PNEU_SEUIL)
            stopBloqCommandePneu();
        else
            stopBloqCommandeElec();
        gestionFinDeTest(false);
        return;
    }
    bloqueOnglets(ui->tabWidget->currentIndex());
    initGraphe();
    connect(m->acqSeuilPneu, SIGNAL(majData(double*)), this, SLOT(miseAJourAcq(double*)));
    if (indicateurDeTest == TEST_PNEU_SEUIL){
        ui->pB_StopTestPneu->setEnabled(true);
        connect(m->rampePneu, SIGNAL(majData(double)), this, SLOT(miseAJourRampe(double)));
        ui->lE_PneuStatus->setText("Mesure TEST PNEU SEUIL en cours...");
        m->testPneuSeuil(ui->cB_Rampe->isChecked());
    } else if (indicateurDeTest == TEST_ELEC_SEUIL){
        ui->pB_StopTestElec->setEnabled(true);
        connect(m->rampeElec, SIGNAL(majData(double)), this, SLOT(miseAJourRampe(double)));
        ui->lE_ElecStatus->setText("Mesure TEST ELEC SEUIL en cours...");
        m->testElecSeuil();
    }
}

bool MainWindow::sortieDejaActive(void){
    bool test=false;
    double limite= m->getPressionAlim()/2;
    m->activationRelaisTestPneu();
    Sleep(500);
    if(m->capteursP->acqCapteurs(valAI)==0){
        double a, b;
        for(int i = 0; i < 8; i++) {
            param->getAI(i, a, b);
            valAI[i]= valAI[i]*a + b; // correctif type ax+b
        }
//        qWarning("Actif ? : CG(14)=%0.2f CD(12)=%0.2f SG(4)=%0.2f SD(2)=%0.2f P(1)=%0.2f",\
//                 valAI[choice->value("CMD_GAUCHE")], valAI[choice->value("CMD_DROITE")], valAI[choice->value("SORTIE_GAUCHE")],
//                valAI[choice->value("SORTIE_DROITE")], valAI[choice->value("CMD_PRESSION")] );
        if(((valAI[conf->SORTIE_GAUCHE]>limite)&&(m->getOperation()==ENCLENCHEMENT))|| \
                ((m->getTypeDistributeur()==MONOSTABLE)&&(valAI[conf->SORTIE_GAUCHE]<limite)&&(m->getOperation()==DECLENCHEMENT))|| \
                ((m->getTypeDistributeur()==BISTABLE)&&(valAI[conf->SORTIE_DROITE]>limite)&&(m->getOperation()==DECLENCHEMENT))){
            m->setMsgThread("Sortie déjà active, changez le sens de la rampe !");
//            qWarning("Sortie déjà active, changez le sens de la rampe !");
            test = true;
        }
    }
    else {
        test = true;
        if(m->getMode()==PNEUMATIQUE)
            ui->lE_PneuStatus->setText("ERREUR - Voir onglet Initialisation");
        else
            ui->lE_ElecStatus->setText("ERREUR - Voir onglet Initialisation");
    }
    return test;
}

void MainWindow::gestionFinDeTest(bool testOK){
    QString s;
    //    qWarning("MainWindow detect fin de Test: etat mesure = %d", m.getMesureEnCours());
    if(m->getMesureEnCours()){
        switch(indicateurDeTest){
        case TEST_PNEU_SEUIL:
            disconnect(m->acqSeuilPneu, SIGNAL(majData(double*)), this, SLOT(miseAJourAcq(double*)));
            disconnect(m->rampePneu, SIGNAL(majData(double)), this, SLOT(miseAJourRampe(double)));
            if(testOK && m->acqSeuilPneu->getChangement_d_etat()){
                if(m->getOperation() == ENCLENCHEMENT){
                    s.sprintf("%.2f bars", mesuresTestPneu.getSeuilTensionPilotageG());
                    ui->lE_PneuSeuilEncl->setText(s);
                }
                else {
                    s.sprintf("%.2f bars", mesuresTestPneu.getSeuilTensionDepilotageGouPilotageD());
                    ui->lE_PneuSeuilDecl->setText(s);
                }
                ui->lE_PneuStatus->setText("Mesure effectuée");
            }
            else { // Pas d'erreur mais pas de détection de seuil
                ui->lE_PneuStatus->setText(m->getMsgThread());
            }
            qWarning("Fin TEST PNEU SEUIL\n");
            break;

        case TEST_PNEU_TPS_REP :
            if(testOK && m->acqTpsRepPneu->getChangement_d_etat()){
                if(m->getOperation()== ENCLENCHEMENT){
                    s.sprintf("%d ms", mesuresTestPneu.getTpsRepPilotageG());
                    ui->lE_PneuTpsRepEncl->setText(s);
                }
                else {
                    s.sprintf("%d ms", mesuresTestPneu.getTpsRepDepilotageGouPilotageD());
                    ui->lE_PneuTpsRepDecl->setText(s);
                }
                ui->lE_PneuStatus->setText("Mesure effectuée");
            }
            else
                ui->lE_PneuStatus->setText(m->getMsgThread());
            qWarning("Fin TEST PNEU TEMPS de REPONSE\n");
            break;

        case TEST_ELEC_SEUIL:
            disconnect(m->acqSeuilPneu, SIGNAL(majData(double*)), this, SLOT(miseAJourAcq(double*)));
            disconnect(m->rampeElec, SIGNAL(majData(double)), this, SLOT(miseAJourRampe(double)));
            qWarning("TestOk & changement d'etat : %d", (testOK && m->acqSeuilPneu->getChangement_d_etat()));
            if(testOK && m->acqSeuilPneu->getChangement_d_etat()){
                if(m->getOperation()==ENCLENCHEMENT){
                    s.sprintf("%.2f volts", mesuresTestElec.getSeuilTensionPilotageG());
                    ui->lE_ElecSeuilEncl_2->setText(s);
                    s.clear();
                    s.sprintf("%.2f mA", mesuresTestElec.getSeuilCourantPilotageG());
                    ui->lE_ElecSeuilCourantEncl->setText(s);
                } else { // On rentre pas dedans...
                    qWarning("NULL ? :\t%d", (mesuresTestElec.getSeuilTensionDepilotageGouPilotageD()));
                    s.sprintf("%.2f volts", mesuresTestElec.getSeuilTensionDepilotageGouPilotageD()); //mesuresTestElec.getSeuilTensionDepilotageGouPilotageD()
                    ui->lE_ElecSeuilDecl_2->setText(s);
                    s.clear();
                    s.sprintf("%.2f mA", mesuresTestElec.getSeuilCourantDepilotageGouPilotageD()); //mesuresTestElec.getSeuilCourantDepilotageGouPilotageD()
                    ui->lE_ElecSeuilCourantDecl->setText(s);
                }
                ui->lE_ElecStatus->setText("Mesure effectuée");
            }
            else {
                ui->lE_ElecStatus->setText(m->getMsgThread());
            }
            qWarning("Fin TEST ELEC SEUIL\n");
            break;

        case TEST_ELEC_TPS_REP:
            if(testOK && m->acqTpsRepElec->getChangement_d_etat()){
                if(m->getOperation()== ENCLENCHEMENT){
                    s.sprintf("%d ms", mesuresTestElec.getTpsRepPilotageG());
                    ui->lE_ElecTpsRepEncl_2->setText(s);
                }
                else {
                    s.sprintf("%d ms", mesuresTestElec.getTpsRepDepilotageGouPilotageD());
                    ui->lE_ElecTpsRepDecl_2->setText(s);
                }
                ui->lE_ElecStatus->setText("Mesure effectuée");
            }
            else
                ui->lE_ElecStatus->setText(m->getMsgThread());
            qWarning("Fin TEST ELEC TEMPS de REPONSE\n");
            break;
        }
        m->setMesureEnCours(false);
        Sleep(500);
    }
    else { // qWarning("Passage:%s", m.getMsgThread().toLatin1().data());
        if(m->getMode()==PNEUMATIQUE){
            ui->lE_PneuStatus->setText(m->getMsgThread());
            ui->lE_PneuStatus->repaint();
        }
        else{
            ui->lE_ElecStatus->setText(m->getMsgThread());
            ui->lE_ElecStatus->repaint();
        }
    }
    if(modeAuto){
        compteurDeTest++;
        Sleep(2000);
    }
    if(m->getMode()==PNEUMATIQUE)
        gestionTestAutoPneu();
    else  // Mode auto Elec
        gestionTestAutoElec();
}

void MainWindow::gestionTestAutoPneu(void){
    //    qWarning("Mode PNEUMATIQUE, compteur de test = %d", compteurDeTest);
    switch(compteurDeTest){
    case 1: ui->rBDepilotagePneu->setChecked(true);
        m->setOperation(DECLENCHEMENT);
        on_pB_TestPneuSeuil_clicked();
//        qWarning("compteur de test = %d", compteurDeTest);
        break;
    case 2: ui->rBPilotagePneu->setChecked(true);
        m->setOperation(ENCLENCHEMENT);
        on_pB_TestPneuTpsRep_clicked();
//        qWarning("compteur de test = %d", compteurDeTest);
        break;
    case 3: ui->rBDepilotagePneu->setChecked(true);
        m->setOperation(DECLENCHEMENT);
        on_pB_TestPneuTpsRep_clicked();
//        qWarning("compteur de test = %d", compteurDeTest);
        break;
    case 4: compteurDeTest=0;
        modeAuto=false;
        ui->lE_PneuStatus->setText("Fin du test auto");
//        qWarning("compteur de test = %d", compteurDeTest);
    case 0: debloqueOnglets();
        stopBloqCommandePneu();
//        qWarning("compteur de test = %d", compteurDeTest);
        break;
    }
}

void MainWindow::gestionTestAutoElec(void){
//    qWarning("Mode ELECTRIQUE, compteur de test = %d", compteurDeTest);
    switch(compteurDeTest){
    case 1: ui->rBDepilotageElec->setChecked(true);
        m->setOperation(DECLENCHEMENT);
        on_pB_TestElecSeuil_clicked();
        break;
    case 2: ui->rBPilotageElec->setChecked(true);
        m->setOperation(ENCLENCHEMENT);
        on_pB_TestElecTpsRep_clicked();
        break;
    case 3: ui->rBDepilotageElec->setChecked(true);
        m->setOperation(DECLENCHEMENT);
        on_pB_TestElecTpsRep_clicked();
        break;
    case 4: compteurDeTest=0;
        modeAuto=false;
        ui->lE_ElecStatus->setText("Fin du test auto");
    case 0: debloqueOnglets();
        stopBloqCommandeElec();
        break;
    }
}

/*** utilisé pour le mode manuel : Capteur pression ***/
void MainWindow::lirePression(void){
    if(m->capteursP->acqCapteurs(valAI)!=0){
        ui->lE_ManuelStatus->setText("ERREUR - Voir onglet Initialisation");
        update->stop();
    }
    else {
        for(int i=0; i<5; i++) {
            double a, b;
            param->getAI(i, a, b);
            valAI[i]= valAI[i]*a + b;
        }
    }
}

void MainWindow::launchApp(int val)
{
    // On récupère la config :
    conf = PhysiqueFactory::getConfigEnCours(val);

    ui->comboAdrGPIB->clear();
    ui->comboAdrGPIB->addItems(conf->getListAdrGPIB());
    ui->comboCmdGPIB->clear();
    ui->comboCmdGPIB->addItems(conf->getListCmdGPIB());

    m = new ControleurMesure(conf->getChoix());

    // Remontée d'info (bonnes ou mauvaises) de l'initialisation et de l'arrêt du système
    connect(m, SIGNAL(topProgress(int)), this, SLOT(majProgressBar(int)));
    connect(m, SIGNAL(infoMatos(QString)), this, SLOT(majInfo(QString)));
    connect(m, SIGNAL(finInit()), this, SLOT(demarrageBanc()));
    connect(m, SIGNAL(finArret()), this, SLOT(arretBanc()));

    // Signalement de la fin d'un test
    connect(m, SIGNAL(finDeTest(bool)), this, SLOT(gestionFinDeTest(bool)));


    // Signalement des erreurs matérielles : messages dans l'onglet initialisation
    for(int i=0; i < conf->NB_MODULE_RELAIS; i++)
        connect(m->relais[i], SIGNAL(infoMatos(QString)), this, SLOT(majInfo(QString)));
    connect(m->vannesP, SIGNAL(infoMatos(QString)), this, SLOT(majInfo(QString)));
    connect(m->capteursP, SIGNAL(infoMatos(QString)), this, SLOT(majInfo(QString)));
    connect(m->alimCC, SIGNAL(infoMatos(QString)), this, SLOT(majInfo(QString)));
    if(!conf->getChoix())
        connect(m->alimAC, SIGNAL(infoMatos(QString)), this, SLOT(majInfo(QString)));

    emit info("Initialisation du banc :");
    if(ssql.connexionBase() == true){
        emit info("Connexion base de données SQLITE OK !");
        QStringList sl = ssql.getListeTable();
        if(!sl.contains("mesures")){
            emit info("Pas de table(s) dans la base !");
            if(ssql.creerTable()){
                emit info("Table \"mesures\" créée !");
            }
            else {
                emit info("Problème de création de table !");
            }
        }
        else
            emit info("Table de données déjà existante !");
    }
    else {
        emit info(ssql.getMsgInfo());
    }

    if ( ssql.getFlag() == true) {
        model = new QSqlTableModel();
        QTimer::singleShot(1000, m, SLOT(startBanc()));
    }
}

void MainWindow::miseEnRoute()
{
    debloqueOnglets();
    bloqCommandePneu();
    tabPushBPneu[0]->setEnabled(true);
    this->show();
    on_tabWidget_currentChanged(0);
}

void MainWindow::purge(void){
    for(int i=0; i<3; i++){
        m->relais[i]->desactiveRelais(0);
        m->relais[i]->desactiveRelais(1);
        m->relais[i]->activeRelais(3);
    }
    m->relais[2]->activeRelais(7);
}

void MainWindow::securitePression(void){
    for(int i=0; i < conf->NB_MODULE_RELAIS; i++){
        m->relais[i]->desactiveRelais(3);
    }
    if (conf->NB_MODULE_RELAIS > 1)
        m->relais[2]->desactiveRelais(7);
    m->vannesP->cmdVannePilotageD(0.0);
    m->vannesP->cmdVannePilotageG(0.0);
}

void MainWindow::on_rBPilotagePneu_clicked()
{
    m->setOperation(ENCLENCHEMENT);
}

void MainWindow::on_rBDepilotagePneu_clicked()
{
    m->setOperation(DECLENCHEMENT);
}

void MainWindow::on_rBPilotageElec_clicked()
{
    m->setOperation(ENCLENCHEMENT);
}

void MainWindow::on_rBDepilotageElec_clicked()
{
    m->setOperation(DECLENCHEMENT);
}

void MainWindow::on_rBMonostableElec_clicked()
{
    m->setTypeDistributeur(MONOSTABLE);
}

void MainWindow::on_rBBistableElec_clicked()
{
    m->setTypeDistributeur(BISTABLE);
}

void MainWindow::on_rBMonostablePneu_clicked()
{
    m->setTypeDistributeur(MONOSTABLE);
}

void MainWindow::on_rBBistablePneu_clicked()
{
    m->setTypeDistributeur(BISTABLE);
}

void MainWindow::on_pB_TestAutoPneu_clicked()
{
    modeAuto = true;
    ui->rBPilotagePneu->setChecked(true);
    m->setOperation(ENCLENCHEMENT);
    on_pB_TestPneuSeuil_clicked();
}

void MainWindow::on_pB_TestAutoElec_clicked()
{
    modeAuto = true;
    ui->rBPilotageElec->setChecked(true);
    m->setOperation(ENCLENCHEMENT);
    on_pB_TestElecSeuil_clicked();
}

void MainWindow::on_tabWidget_currentChanged(int indice)
{
    if(indice != 2){ // si pas mode manuel
        disconnect(update, SIGNAL(timeout()), this, SLOT(lirePression()));
        update->stop();
    }
    switch(indice){

    case 0: // onglet Test Pneu
        m->setMtp(&mesuresTestPneu);
        m->setMode(PNEUMATIQUE);
        m->desactivationRelaisTestElec();
        bloqCommandePneu();                     // Bloque tout sauf
        ui->pB_ConfigPneu->setEnabled(true);    // bouton config
        ui->lE_PneuStatus->setText("Configurer le test et valider");
        break;

    case 1: // onglet Test Elec
        m->setMtp(&mesuresTestElec);
        m->setMode(ELECTRIQUE);
        m->desactivationRelaisTestPneu();
        bloqCommandeElec();
        ui->pBConfigElec->setEnabled(true);
        ui->lE_ElecStatus->setText("Configurer le test et valider");
        break;

    case 2: // onglet Mode Manuel Pneu
        initOngletModeManuel();
        ui->lE_ManuelStatus->setText("Vous pouvez commander les vannes proportionnelles et les relais");
        break;

    case 3: // onglet Exploitation des données
        if(ssql.getFlag()==false)
            ui->tabWidget->setCurrentIndex(0);
        else {
            model->setTable(ssql.getTable());
            model->setSort(0, Qt::AscendingOrder); // tri par n°id (colonne 0)
            /*            model->setHeaderData(0, Qt::Horizontal, "N°");	// renomme le titre des colonnes
            model->setHeaderData(0, Qt::Horizontal, "Date");
            model->setHeaderData(1, Qt::Horizontal, "Ref");
            model->setHeaderData(2, Qt::Horizontal, "Gommage");
            model->setHeaderData(3, Qt::Horizontal, "Seuil pilotage");
            model->setHeaderData(4, Qt::Horizontal, "Seuil dépilotage");
            model->setHeaderData(5, Qt::Horizontal, "Tps Rep pilotage");
            model->setHeaderData(6, Qt::Horizontal, "Tps Rep dépilotage");*/
            model->select(); // = select * from nomTable
            ui->tableViewData->setModel(model);	// ou qmodel
            ui->tableViewData->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableViewData->resizeColumnsToContents();
        }
        break;
    }
}

void MainWindow::on_pB_ConfigPneu_clicked()
{
    /* Passage obligé pour effectuer des mesures */
    double p;
    int err = 0;

    p = ui->dSBPressionPneu->value();
    securitePression();
    if( !conf->getChoix() )
        err = m->vannesP->cmdVannePression(p);
    if(err == 0){
        if(ui->rBPilotagePneu->isChecked() == true)
            m->setOperation(ENCLENCHEMENT);
        else
            m->setOperation(DECLENCHEMENT);
        if(ui->rBMonostablePneu->isChecked() == true)
            m->setTypeDistributeur(MONOSTABLE);
        else
            m->setTypeDistributeur(BISTABLE);
        m->setPressionAlim(p);
        initOngletPneu(p);
    } else {
        ui->lE_PneuStatus->setText("ERREUR - Voir onglet Initialisation");
    }
}

void MainWindow::on_pBConfigElec_clicked(){
    /* Passage obligé pour effectuer des mesures */
    double p, t;
    int err = 0;
    t = ui->dSBTensionElec->value();
    p = ui->dSBPressionElec->value();
    securitePression();
    err = m->vannesP->cmdVannePression(p);
    if(err == 0){
        if(ui->rBPilotageElec->isChecked() == true)
            m->setOperation(ENCLENCHEMENT);
        else
            m->setOperation(DECLENCHEMENT);
        if(ui->rBMonostableElec->isChecked() == true)
            m->setTypeDistributeur(MONOSTABLE);
        else
            m->setTypeDistributeur(BISTABLE);
        m->setPressionAlim(p);
        m->setTensionAlim(t);
        initOngletElec(p);
    }
    else
        ui->lE_ElecStatus->setText("ERREUR - Voir onglet Initialisation");
}

void MainWindow::on_pB_TestPneuSeuil_clicked()
{
    indicateurDeTest = TEST_PNEU_SEUIL;
    testSeuil();
}

void MainWindow::on_pB_TestElecSeuil_clicked()
{
    indicateurDeTest = TEST_ELEC_SEUIL;
    testSeuil();
}

void MainWindow::on_pB_TestPneuTpsRep_clicked()
{
//    qWarning("TEST PNEU TEMPS DE REPONSE");
    indicateurDeTest = TEST_PNEU_TPS_REP;
    bloqCommandePneu();
    if(sortieDejaActive()){
        stopBloqCommandePneu();
        gestionFinDeTest(false);
    }
    else {
//        qWarning("Mesure TEST PNEU TEMPS DE REPONSE en cours...");
        ui->pB_StopTestPneu->setEnabled(true);
        bloqueOnglets(ui->tabWidget->currentIndex());
        ui->lE_PneuStatus->setText("Mesure TEST PNEU TEMPS DE REPONSE en cours...");
        m->testPneuTpsRep();
    }
}

void MainWindow::on_pB_TestElecTpsRep_clicked()
{
    indicateurDeTest = TEST_ELEC_TPS_REP;
    bloqCommandeElec();
    if(sortieDejaActive()){
        stopBloqCommandeElec();
        gestionFinDeTest(false);
    }
    else {
        ui->pB_StopTestElec->setEnabled(true);
        bloqueOnglets(ui->tabWidget->currentIndex());
        ui->lE_ElecStatus->setText("Mesure TEST ELEC TEMPS DE REPONSE en cours...");
        m->testElecTpsRep();
    }
}

void MainWindow::on_pB_StopTestPneu_clicked()
{
    if(m->getMesureEnCours()== false)
        ui->lE_PneuStatus->setText("Aucune mesure en cours");
    else {
        switch(indicateurDeTest){
        case TEST_PNEU_SEUIL:
            if(m->rampePneu->isRunning())
                m->rampePneu->stop();
            else if(m->acqSeuilPneu->isRunning())
                m->acqSeuilPneu->stop();
            break;

        case TEST_PNEU_TPS_REP :
            if(m->acqTpsRepPneu->isRunning())
                m->acqTpsRepPneu->stop();
            break;
        }
        // Remise à 0 de la pression afin de pouvoir purger le produit
        securitePression();     //m.desactivationRelaisTestPneu();
        bloqCommandePneu();
    }
}

void MainWindow::on_pB_StopTestElec_clicked()
{
    if(m->getMesureEnCours()== false)
        ui->lE_ElecStatus->setText("Aucune mesure en cours");
    else {
        switch(indicateurDeTest){
        case TEST_ELEC_SEUIL:
            if(m->rampeElec->isRunning())
                m->rampeElec->stop();
            else if(m->acqSeuilPneu->isRunning())
                m->acqSeuilPneu->stop();
            break;

        case TEST_ELEC_TPS_REP :
            if(m->acqTpsRepElec->isRunning())
                m->acqTpsRepElec->stop();
            break;
        }
        // Remise à 0 de la pression afin de pouvoir purger le produit
        securitePression();     //m.desactivationRelaisTestPneu();
        bloqCommandeElec();
    }
}

/*** utilisé pour le mode manuel : Vannes proportionnelles ***/
void MainWindow::on_hSliderMod2ao0_valueChanged(int value)
{
    if(m->vannesP->cmdVannePilotageD((double)(value/5.0))!=0)
        ui->lE_ManuelStatus->setText("ERREUR - Voir onglet Initialisation");
}
void MainWindow::on_hSliderMod2ao1_valueChanged(int value)
{
    if((m->vannesP->cmdVannePression((double)(value/5.0))!=0) && (!conf->getChoix()))
        ui->lE_ManuelStatus->setText("ERREUR - Voir onglet Initialisation");
}
void MainWindow::on_hSliderMod2ao2_valueChanged(int value)
{
    if(m->vannesP->cmdVannePilotageG((double)(value/5.0))!=0)
        ui->lE_ManuelStatus->setText("ERREUR - Voir onglet Initialisation");
}

/*** utilisé pour le mode manuel : Maj Module4 */
void MainWindow::on_cB_Mod4port0line0_clicked(bool checked)
{
    if (ui->ledMod4port0line0->color() != Qt::black) {
        if(checked){
            ui->ledMod4port0line0->setColor(Qt::red);
            m->relais[conf->MODULE4]->activeRelais(0);
        } else {
            ui->ledMod4port0line0->setColor(Qt::green);
            m->relais[conf->MODULE4]->desactiveRelais(0);
        }
    } else {
        ui->cB_Mod4port0line0->setChecked(false);
    }
}
void MainWindow::on_cB_Mod4port0line1_clicked(bool checked)
{
    if (ui->ledMod4port0line1->color() != Qt::black) {
        if(checked){
            ui->ledMod4port0line1->setColor(Qt::red);
            m->relais[conf->MODULE4]->activeRelais(1);
        } else {
            ui->ledMod4port0line1->setColor(Qt::green);
            m->relais[conf->MODULE4]->desactiveRelais(1);
        }
    } else {
        ui->cB_Mod4port0line1->setChecked(false);
    }
}
void MainWindow::on_cB_Mod4port0line2_clicked(bool checked)
{
    if (ui->ledMod4port0line2->color() != Qt::black) {
        if(checked){
            ui->ledMod4port0line2->setColor(Qt::red);
            m->relais[conf->MODULE4]->activeRelais(2);
        } else {
            ui->ledMod4port0line2->setColor(Qt::green);
            m->relais[conf->MODULE4]->desactiveRelais(2);
        }
    } else {
        ui->cB_Mod4port0line2->setChecked(false);
    }
}
void MainWindow::on_cB_Mod4port0line3_clicked(bool checked)
{
    if (ui->ledMod4port0line3->color() != Qt::black) {
        if(checked){
            ui->ledMod4port0line3->setColor(Qt::red);
            m->relais[conf->MODULE4]->activeRelais(3);
        } else {
            ui->ledMod4port0line3->setColor(Qt::green);
            m->relais[conf->MODULE4]->desactiveRelais(3);
        }
    } else {
        ui->cB_Mod4port0line3->setChecked(false);
    }
}
void MainWindow::on_cB_Mod4port0line4_clicked(bool checked)
{
    if (ui->ledMod4port0line4->color() != Qt::black) {
        if(checked){
            ui->ledMod4port0line4->setColor(Qt::red);
            m->relais[conf->MODULE4]->activeRelais(4);
        } else {
            ui->ledMod4port0line4->setColor(Qt::green);
            m->relais[conf->MODULE4]->desactiveRelais(4);
        }
    } else {
        ui->cB_Mod4port0line4->setChecked(false);
    }
}
void MainWindow::on_cB_Mod4port0line5_clicked(bool checked)
{
    if (ui->ledMod4port0line5->color() != Qt::black) {
        if(checked){
            ui->ledMod4port0line5->setColor(Qt::red);
            m->relais[conf->MODULE4]->activeRelais(5);
        } else {
            ui->ledMod4port0line5->setColor(Qt::green);
            m->relais[conf->MODULE4]->desactiveRelais(5);
        }
    } else {
        ui->cB_Mod4port0line5->setChecked(false);
    }
}
void MainWindow::on_cB_Mod4port0line6_clicked(bool checked)
{
    if (ui->ledMod4port0line6->color() != Qt::black) {
        if(checked){
            ui->ledMod4port0line6->setColor(Qt::red);
            m->relais[conf->MODULE4]->activeRelais(6);
        } else {
            ui->ledMod4port0line6->setColor(Qt::green);
            m->relais[conf->MODULE4]->desactiveRelais(6);
        }
    } else {
        ui->cB_Mod4port0line6->setChecked(false);
    }
}
void MainWindow::on_cB_Mod4port0line7_clicked(bool checked)
{
    if (ui->ledMod4port0line7->color() != Qt::black) {
        if(checked){
            ui->ledMod4port0line7->setColor(Qt::red);
            m->relais[conf->MODULE4]->activeRelais(7);
        } else {
            ui->ledMod4port0line7->setColor(Qt::green);
            m->relais[conf->MODULE4]->desactiveRelais(7);
        }
    } else {
        ui->cB_Mod4port0line7->setChecked(false);
    }
}

/*** utilisé pour le mode manuel : Maj Module5 */
void MainWindow::on_cB_Mod5port0line0_clicked(bool checked)
{
    if (ui->ledMod5port0line0->color() != Qt::black) {
        if(checked){
            ui->ledMod5port0line0->setColor(Qt::red);
            m->relais[conf->MODULE5]->activeRelais(0);
        } else {
            ui->ledMod5port0line0->setColor(Qt::green);
            m->relais[conf->MODULE5]->desactiveRelais(0);
        }
    } else {
        ui->cB_Mod5port0line0->setChecked(false);
    }
}
void MainWindow::on_cB_Mod5port0line1_clicked(bool checked)
{
    if (ui->ledMod5port0line1->color() != Qt::black) {
        if(checked){
            ui->ledMod5port0line1->setColor(Qt::red);
            m->relais[conf->MODULE5]->activeRelais(1);
        } else {
            ui->ledMod5port0line1->setColor(Qt::green);
            m->relais[conf->MODULE5]->desactiveRelais(1);
        }
    } else {
        ui->cB_Mod5port0line1->setChecked(false);
    }
}
void MainWindow::on_cB_Mod5port0line2_clicked(bool checked)
{
    if (ui->ledMod5port0line2->color() != Qt::black) {
        if(checked){
            ui->ledMod5port0line2->setColor(Qt::red);
            m->relais[conf->MODULE5]->activeRelais(2);
        }
        else{
            ui->ledMod5port0line2->setColor(Qt::green);
            m->relais[conf->MODULE5]->desactiveRelais(2);
        }
    } else {
        ui->cB_Mod5port0line2->setChecked(false);
    }
}
void MainWindow::on_cB_Mod5port0line3_clicked(bool checked)
{
    if (ui->ledMod5port0line3->color() != Qt::black) {
        if(checked){
            ui->ledMod5port0line3->setColor(Qt::red);
            m->relais[conf->MODULE5]->activeRelais(3);
        } else {
            ui->ledMod5port0line3->setColor(Qt::green);
            m->relais[conf->MODULE5]->desactiveRelais(3);
        }
    } else {
        ui->cB_Mod5port0line3->setChecked(false);
    }
}
void MainWindow::on_cB_Mod5port0line4_clicked(bool checked)
{
    if (ui->ledMod5port0line4->color() != Qt::black) {
        if(checked){
            ui->ledMod5port0line4->setColor(Qt::red);
            m->relais[conf->MODULE5]->activeRelais(4);
        }
        else{
            ui->ledMod5port0line4->setColor(Qt::green);
            m->relais[conf->MODULE5]->desactiveRelais(4);
        }
    } else {
        ui->cB_Mod5port0line4->setChecked(false);
    }
}
void MainWindow::on_cB_Mod5port0line5_clicked(bool checked)
{
    if (ui->ledMod5port0line5->color() != Qt::black) {
        if(checked){
            ui->ledMod5port0line5->setColor(Qt::red);
            m->relais[conf->MODULE5]->activeRelais(5);
        } else {
            ui->ledMod5port0line5->setColor(Qt::green);
            m->relais[conf->MODULE5]->desactiveRelais(5);
        }
    } else {
        ui->cB_Mod5port0line5->setChecked(false);
    }
}
void MainWindow::on_cB_Mod5port0line6_clicked(bool checked)
{
    if (ui->ledMod5port0line6->color() != Qt::black) {
        if(checked){
            ui->ledMod5port0line6->setColor(Qt::red);
            m->relais[conf->MODULE5]->activeRelais(6);
        } else {
            ui->ledMod5port0line6->setColor(Qt::green);
            m->relais[conf->MODULE5]->desactiveRelais(6);
        }
    } else {
        ui->cB_Mod5port0line6->setChecked(false);
    }
}
void MainWindow::on_cB_Mod5port0line7_clicked(bool checked)
{
    if (ui->ledMod5port0line7->color() != Qt::black) {
        if(checked){
            ui->ledMod5port0line7->setColor(Qt::red);
            m->relais[conf->MODULE5]->activeRelais(7);
        } else {
            ui->ledMod5port0line7->setColor(Qt::green);
            m->relais[conf->MODULE5]->desactiveRelais(7);
        }
    } else {
        ui->cB_Mod5port0line7->setChecked(false);
    }
}

/*** utilisé pour le mode manuel : Maj Module6 */
void MainWindow::on_cB_Mod6port0line0_clicked(bool checked)
{
    if (ui->ledMod6port0line0->color() != Qt::black) {
        if(checked){
            ui->ledMod6port0line0->setColor(Qt::red);
            m->relais[conf->MODULE6]->activeRelais(0);
        } else {
            ui->ledMod6port0line0->setColor(Qt::green);
            m->relais[conf->MODULE6]->desactiveRelais(0);
        }
    } else {
        ui->cB_Mod6port0line0->setChecked(false);
    }
}
void MainWindow::on_cB_Mod6port0line1_clicked(bool checked)
{
    if (ui->ledMod6port0line1->color() != Qt::black) {
        if(checked){
            ui->ledMod6port0line1->setColor(Qt::red);
            m->relais[conf->MODULE6]->activeRelais(1);
        } else {
            ui->ledMod6port0line1->setColor(Qt::green);
            m->relais[conf->MODULE6]->desactiveRelais(1);
        }
    } else {
        ui->cB_Mod6port0line1->setChecked(false);
    }
}
void MainWindow::on_cB_Mod6port0line2_clicked(bool checked)
{
    if (ui->ledMod6port0line2->color() != Qt::black) {
        if(checked){
            ui->ledMod6port0line2->setColor(Qt::red);
            m->relais[conf->MODULE6]->activeRelais(2);
        } else {
            ui->ledMod6port0line2->setColor(Qt::green);
            m->relais[conf->MODULE6]->desactiveRelais(2);
        }
    } else {
        ui->cB_Mod6port0line2->setChecked(false);
    }
}
void MainWindow::on_cB_Mod6port0line3_clicked(bool checked)
{
    if (ui->ledMod6port0line3->color() != Qt::black) {
        if(checked){
            ui->ledMod6port0line3->setColor(Qt::red);
            m->relais[conf->MODULE6]->activeRelais(3);
        } else {
            ui->ledMod6port0line3->setColor(Qt::green);
            m->relais[conf->MODULE6]->desactiveRelais(3);
        }
    } else {
        ui->cB_Mod6port0line3->setChecked(false);
    }
}
void MainWindow::on_cB_Mod6port0line4_clicked(bool checked)
{
    if (ui->ledMod6port0line4->color() != Qt::black) {
        if(checked){
            ui->ledMod6port0line4->setColor(Qt::red);
            m->relais[conf->MODULE6]->activeRelais(4);
        } else {
            ui->ledMod6port0line4->setColor(Qt::green);
            m->relais[conf->MODULE6]->desactiveRelais(4);
        }
    } else {
        ui->cB_Mod6port0line4->setChecked(false);
    }
}
void MainWindow::on_cB_Mod6port0line5_clicked(bool checked)
{
    if (ui->ledMod6port0line5->color() != Qt::black) {
        if(checked){
            ui->ledMod6port0line5->setColor(Qt::red);
            m->relais[conf->MODULE6]->activeRelais(5);
        } else {
            ui->ledMod6port0line5->setColor(Qt::green);
            m->relais[conf->MODULE6]->desactiveRelais(5);
        }
    } else {
        ui->cB_Mod6port0line5->setChecked(false);
    }
}
void MainWindow::on_cB_Mod6port0line6_clicked(bool checked)
{
    if (ui->ledMod6port0line6->color() != Qt::black) {
        if(checked){
            ui->ledMod6port0line6->setColor(Qt::red);
            m->relais[conf->MODULE6]->activeRelais(6);
        } else {
            ui->ledMod6port0line6->setColor(Qt::green);
            m->relais[conf->MODULE6]->desactiveRelais(6);
        }
    } else {
        ui->cB_Mod6port0line6->setChecked(false);
    }
}
void MainWindow::on_cB_Mod6port0line7_clicked(bool checked)
{
    if (ui->ledMod6port0line7->color() != Qt::black) {
        if(checked){
            ui->ledMod6port0line7->setColor(Qt::red);
            m->relais[conf->MODULE6]->activeRelais(7);
        } else {
            ui->ledMod6port0line7->setColor(Qt::green);
            m->relais[conf->MODULE6]->desactiveRelais(7);
        }
    } else {
        ui->cB_Mod6port0line7->setChecked(false);
    }
}

void MainWindow::on_pB_SauvegardePneu_clicked()
{
    mesuresTestPneu.setDateTest(QDate::currentDate());
    mesuresTestPneu.setRef(ui->lineEditRefPneu->text());
    mesuresTestPneu.setIndicateurGommage(ui->cBGommagePneu->isChecked());
    mesuresTestPneu.setTensionAlimentation(0.0); // produit pneu
    mesuresTestPneu.setPressionAlimentation(ui->dSBPressionPneu->value());
    if(ssql.enregistrerMesure(&mesuresTestPneu)==false){
        ui->lE_PneuStatus->setText("Problème enregistrement");
        QMessageBox::critical(this, "Erreur", ssql.getMsgInfo());
    }
    else{
        ui->lE_PneuStatus->setText("Enregistrement effectuée");
        clearChampsPneu();
        mesuresTestPneu.razMesures();
    }
}

void MainWindow::on_pB_SauvegardeElec_clicked()
{
    mesuresTestElec.setDateTest(QDate::currentDate());
    //    mesuresTestElec.setRef(ui->comboBoxElec->currentText());
    mesuresTestElec.setRef(ui->lineEditRefElec->text());
    mesuresTestElec.setIndicateurGommage(ui->cBGommageElec->isChecked());
    mesuresTestElec.setTensionAlimentation(ui->dSBTensionElec->value());
    mesuresTestElec.setPressionAlimentation(ui->dSBPressionElec->value());
    if(ssql.enregistrerMesure(&mesuresTestElec)==false){
        ui->lE_ElecStatus->setText("Problème enregistrement");
        QMessageBox::critical(this, "Erreur", ssql.getMsgInfo());
    }
    else{
        ui->lE_ElecStatus->setText("Enregistrement effectuée");
        clearChampsElec();
        mesuresTestElec.razMesures();
    }
}

void MainWindow::on_tableViewData_doubleClicked(const QModelIndex &ind)
{
    QString valeur;
    int lig = ind.row();

    valeur= "Voulez-vous supprimer l'enregistrement n° " + model->index(lig, 0).data().toString();
    if( QMessageBox::information( this, "Application Qt :essai sqlite", valeur, "Oui", "Non", "Annuler", 0, 1 ) == 0) {
        model->removeRows(lig, 1);  // ou ssql.supprimerMesure(index.row()
        model->submitAll();
        //        ui->tableView->show();ui->tableViewData->repaint();
        QMessageBox::information(this, "Supprimer une mesure", "mesure supprimée avec succès !", QMessageBox::Ok);
    }
    else
        QMessageBox::critical(this, "Exporter table mesures", ssql.getMsgInfo(), QMessageBox::Ok);
}

void MainWindow::on_pB_ExporteMesures_clicked()
{
    if(ssql.exporterTableEnCVS() == true)
        QMessageBox::information(this, "Exporter table mesures", "mesures.csv exporté avec succès !", QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Exporter table mesures", ssql.getMsgInfo(), QMessageBox::Ok);
}

void MainWindow::on_pB_SuppTable_clicked()
{
    if(ssql.supprimerTable() == true)
        QMessageBox::information(this, "Supprimer table mesures", "Table mesures supprimée avec succès !", QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Supprimer table mesures", ssql.getMsgInfo(), QMessageBox::Ok);
}

void MainWindow::on_pB_CreerTable_clicked()
{
    if(ssql.creerTable() == true)
        QMessageBox::information(this, "Créer table mesures", "Table mesures créée avec succès !", QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Créer table mesures", ssql.getMsgInfo(), QMessageBox::Ok);
}

void MainWindow::on_pB_initAlimDC_clicked(){
    int ad= conf->getListAdrGPIB().at(0).toInt();
    if(m->alimCC->initDevice(ad) == 0)
        ui->lE_ManuelStatus->setText("ALIM CC Réinitialisée");
    else
        ui->lE_ManuelStatus->setText("Problème ALIM CC");
}

void MainWindow::on_pB_initAlimAC_clicked(){
    if(!conf->getChoix()) {
        int ad= conf->getListAdrGPIB().at(1).toInt();
        if(m->alimAC->initDevice(ad) == 0)
            ui->lE_ManuelStatus->setText("ALIM AC Réinitialisée");
        else
            ui->lE_ManuelStatus->setText("Problème ALIM AC");
    }
    else {
        ui->lE_ManuelStatus->setText("ALIM AC inexistante !");
    }
}

void MainWindow::on_pB_OK_DC_clicked(){
    bool ok;
    QString cmd;
    float tension;
    QString val = ui->lE_valTensionDC->text();
    tension=val.toFloat(&ok);
    if(ok == true){
        if(conf->getChoix())
            cmd.sprintf("V1 %.1f\n", tension );
        else
            cmd= "VOLT " + val; // cmd.sprintf("VOLT %.1f\n", tension );
        m->alimCC->sendCommande(cmd.toLatin1().data());
        ui->lcdMod1ai5->display(tension);
    }
    else
        ui->lE_ManuelStatus->setText("Problème GPIB");
}

void MainWindow::on_pB_OK_AC_clicked(){
    bool ok;
    QString cmd;
    float tension;
    QString val = ui->lE_valTensionAC->text();
    tension=val.toFloat(&ok);
    if(!conf->getChoix()) {
        if(ok == true){
            cmd= "VOLT " + val; // cmd.sprintf("VOLT %.1f\n", tension );
            m->alimAC->sendCommande(cmd.toLatin1().data());
            ui->lcdMod1ai6->display(tension);
        }
        else
            ui->lE_ManuelStatus->setText("Problème GPIB");
    } else {
        qWarning("ALIM AC inexistante !");
    }
}

void MainWindow::on_pB_EnvoiGPIB_clicked(){
    bool ok;
    int ad, ret;
    QString cmd, rep;
    //    int index= ui->comboCmdGPIB->currentIndex();
    cmd = ui->comboCmdGPIB->currentText();
    ad = ui->comboAdrGPIB->currentText().toInt(&ok);
    //    qWarning("%s %d index=%d", cmd.toLatin1().data(), ad, index );
    if(conf->getChoix()){
        m->alimCC->sendCommande(cmd.toLatin1().data());
        if((ret= m->alimCC->readData())==0)
            rep = m->alimCC->getBufRead();
    } else {
        if(ad == conf->getListAdrGPIB().at(0).toInt()) {
            m->alimCC->sendCommande(cmd.toLatin1().data());
            if((ret= m->alimCC->readData()) == 0)
                rep = m->alimCC->getBufRead();
        } else {
            m->alimAC->sendCommande(cmd.toLatin1().data());
            if((ret= m->alimAC->readData())==0)
                rep = m->alimAC->getBufRead();
        }
    }
    if(ret == 0) {
         ui->lineEditRepGPIB->setText(rep);
    }
}

void MainWindow::on_rBContinuElec_clicked()
{
    m->setCourant(CONTINU);
    ui->dSBTensionElec->setMaximum(120);
}

void MainWindow::on_rBAlternatifElec_clicked()
{
    m->setCourant(ALTERNATIF);
    ui->dSBTensionElec->setMaximum(300);
}

void MainWindow::on_pushButtonPurgePneu_clicked()
{
    purge();
    ui->manoPressionPneu->setValue(0.0);
}

void MainWindow::on_pushButtonPurgeElec_clicked()
{
    purge();
    ui->manoPressionElec->setValue(0.0);
}

void MainWindow::on_comboBoxFreq_currentIndexChanged(const QString &arg1)
{
    QString cmd;
//    qWarning("Passage1 :%s", arg1.toLatin1().data());
    if(arg1 == "50 hz")
        cmd="FREQ 50";
    else
        cmd="FREQ 60";
    if(!conf->getChoix())
        m->alimAC->sendCommande(cmd.toLatin1().data());
}

void MainWindow::on_pB_RAZ_GPIB_clicked(){
    m->stopGPIB();
    m->startGPIB();
}

void MainWindow::lireEntreeTOR() {
    int erreur;
    if((erreur= m->e_tor->lireTous_E_TOR(valDI)) !=0 ){
        update->stop();
        ui->lE_ManuelStatus->setText("ERREUR - Voir onglet Initialisation");
    }
    else {
        for(int i=0; i<8; i++){
            switch(i) {
            case 0: //A.U
                if( valDI[conf->AU] == 0 )
                    ui->Led_AU->setColor(Qt::green);
                else
                    ui->Led_AU->setColor(Qt::red);
                break;
            case 3: // Entrée contact
                if( valDI[conf->ETOR] == 0 )
                    ui->Led_EntreeTOR->setColor(Qt::green);
                else
                    ui->Led_EntreeTOR->setColor(Qt::red);
                break;
            }
        }
    }
}
