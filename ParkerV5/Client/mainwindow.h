#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <analogwidgets/led.h>
#include <qwt_dial_needle.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <QCheckBox>
#include <QLCDNumber>
#include <QTimer>
#include <QPushButton>
#include <QCloseEvent>

#include <QSqlTableModel>
#include <QtSql>

#include "sqlservice.h"
#include "mesurestest.h"
#include "controleurmesure.h"
#include "param.h"
#include "config.h"

#define TEST_PNEU_SEUIL     0
#define TEST_PNEU_TPS_REP   1
#define TEST_ELEC_SEUIL     2
#define TEST_ELEC_TPS_REP   3

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void info(QString);
    void progress(int);

private slots:
    void on_pushButton_2_clicked();

    void majProgressBar(int);
    void majInfo(QString);

    void demarrageBanc(void);
    void arretBanc(void);
    void miseAJourAcq(double*);
    void miseAJourRampe(double);
    void gestionFinDeTest(bool);
    void lirePression(void);
    void lireEntreeTOR(void);

    void on_rBPilotagePneu_clicked();
    void on_rBDepilotagePneu_clicked();
    void on_rBPilotageElec_clicked();
    void on_rBDepilotageElec_clicked();
    void on_rBMonostableElec_clicked();
    void on_rBBistableElec_clicked();
    void on_rBMonostablePneu_clicked();
    void on_rBBistablePneu_clicked();
    void on_pB_TestAutoPneu_clicked();
    void on_pB_TestAutoElec_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_pB_ConfigPneu_clicked();
    void on_pBConfigElec_clicked();
    void on_pB_TestPneuSeuil_clicked();
    void on_pB_TestElecSeuil_clicked();
    void on_pB_TestPneuTpsRep_clicked();
    void on_pB_TestElecTpsRep_clicked();
    void on_pB_StopTestPneu_clicked();
    void on_pB_StopTestElec_clicked();
    void on_hSliderMod2ao0_valueChanged(int value);
    void on_hSliderMod2ao1_valueChanged(int value);
    void on_hSliderMod2ao2_valueChanged(int value);
    void on_cB_Mod4port0line0_clicked(bool checked);
    void on_cB_Mod4port0line1_clicked(bool checked);
    void on_cB_Mod4port0line2_clicked(bool checked);
    void on_cB_Mod4port0line3_clicked(bool checked);
    void on_cB_Mod4port0line4_clicked(bool checked);
    void on_cB_Mod4port0line5_clicked(bool checked);
    void on_cB_Mod4port0line6_clicked(bool checked);
    void on_cB_Mod4port0line7_clicked(bool checked);
    void on_cB_Mod5port0line0_clicked(bool checked);
    void on_cB_Mod5port0line1_clicked(bool checked);
    void on_cB_Mod5port0line2_clicked(bool checked);
    void on_cB_Mod5port0line3_clicked(bool checked);
    void on_cB_Mod5port0line4_clicked(bool checked);
    void on_cB_Mod5port0line5_clicked(bool checked);
    void on_cB_Mod5port0line6_clicked(bool checked);
    void on_cB_Mod5port0line7_clicked(bool checked);
    void on_cB_Mod6port0line0_clicked(bool checked);
    void on_cB_Mod6port0line1_clicked(bool checked);
    void on_cB_Mod6port0line2_clicked(bool checked);
    void on_cB_Mod6port0line3_clicked(bool checked);
    void on_cB_Mod6port0line4_clicked(bool checked);
    void on_cB_Mod6port0line5_clicked(bool checked);
    void on_cB_Mod6port0line6_clicked(bool checked);
    void on_cB_Mod6port0line7_clicked(bool checked);

    void on_pB_SauvegardePneu_clicked();
    void on_pB_SauvegardeElec_clicked();
    void on_tableViewData_doubleClicked(const QModelIndex &index);
    void on_pB_ExporteMesures_clicked();
    void on_pB_SuppTable_clicked();
    void on_pB_CreerTable_clicked();
    void on_pB_initAlimDC_clicked();
    void on_pB_initAlimAC_clicked();
    void on_pB_OK_DC_clicked();
    void on_pB_OK_AC_clicked();
    void on_pB_EnvoiGPIB_clicked();
    void on_rBContinuElec_clicked();
    void on_rBAlternatifElec_clicked();
    void on_pushButtonPurgePneu_clicked();
    void on_pushButtonPurgeElec_clicked();
    void on_comboBoxFreq_currentIndexChanged(const QString &arg1);

    void on_pB_RAZ_GPIB_clicked();

protected slots:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow *ui;
    Led *tabLedMod[3][8];
    QCheckBox *tabCbMod[3][8];
    QLCDNumber *tabLcdMod1[8];
    QPushButton *tabPushBPneu[6];
    QPushButton *tabPushBElec[6];
    SqlService ssql;
    QSqlTableModel *model;	// permet de travailler en r/w sur la table
    Param *param;
    Config *conf;
    ControleurMesure *m;
    MesuresTest mesuresTestPneu;
    MesuresTest mesuresTestElec;
    int progression;            // valeur de la barre de progression d'initialisation du banc
    ulong index;
    int indicateurDeTest;       //voir entete de ce fichier
    QTimer *update;              // maj valeur lue sur entrée analogiques (ai)
    double valAI[8];            // Entrées analogiques
    unsigned char valDI[8]; // Entrées TOR

    QString choix;              // Choix : PARKER ou LYCEE

    QwtPlotCurve courbeSortieG;
    QwtPlotCurve courbeSortieD;
    QwtPlotCurve courbeRampe;   // pilotage gauche ou droit

    double tabTemps[1000];      // axe du temps, 1 mesure / 100ms ou 1ms
    double tabAcqSortieG[1000]; // Acq Sortie gauche
    double tabAcqSortieD[1000]; // Acq Sortie droite
    double tabRampe[1000];      // Consigne Vanne ou Acq

    double valRampe;            // Consigne sur vanne proportionnelle ou Alim
    double valAcqPression;
    double valAcqCmdG;          // ~ = rampe
    double valAcqCmdD;          // ~ = rampe
    double valAcqSortieG;
    double valAcqSortieD;
    int compteurDeTest;         // pour le mode auto

    bool modeAuto;
    bool darkMode;
    QString monStyle;

    int choixMode;

private:
    void initMainWindow(void);
    void initTabWidget(void);
    void bloqCommandePneu(void);
    void bloqCommandeElec(void);
    void stopBloqCommandePneu(void);
    void stopBloqCommandeElec(void);
    void clearChampsPneu(void);
    void clearChampsElec(void);
    void initOngletPneu(double);
    void initOngletElec(double);
    void initOngletModeManuel(void);
    void debloqueOnglets(void);
    void bloqueOnglets(int);
    void initGraphe(void);
    bool sortieDejaActive(void);
    void gestionTestAutoPneu(void);
    void gestionTestAutoElec(void);
    void testSeuil(void);
    void purge(void);
    void securitePression(void);

public slots:
    void launchApp(int val);
    void miseEnRoute(void);

};
#endif // MAINWINDOW_H
