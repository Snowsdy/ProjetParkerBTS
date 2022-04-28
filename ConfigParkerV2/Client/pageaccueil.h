#ifndef PAGEACCUEIL_H
#define PAGEACCUEIL_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QAbstractButton>
#include <QTimer>

#include "administrateur.h"
#include "technicien.h"
#include "personne.h"
#include "carteni.h"
#include "sqlqervice.h"

namespace Ui {
class PageAccueil;
}

class PageAccueil : public QMainWindow
{
    Q_OBJECT

public:
    explicit PageAccueil(QWidget *parent = nullptr);
    ~PageAccueil();

signals:
    void configPrete(void);
    void deconnexion(void);
    void vannesConfigOK(void);
    void capteursConfigOK(void);
    void seqTestConfigOK(void);
    void arrUrgConfigOK(void);
    void relaisConfigOK(void);
    void configSupprimee(void);

public slots:
    void launchPG(int);
    void desactivePbVanne(void);
    void desactivePbCapteur(void);
    void desactivePbSeqTest(void);
    void desactivePbArrUrg(void);
    void desactivePbRelais(void);
    void recapitulatif(void);
    void ajoutConfigReussi(void);
    void suppressionConfigConfirme(void);

private slots:
    void on_pBBtn1_clicked();
    void on_pBBtn2_clicked();
    void on_pBBtn3_clicked();
    void on_pBBtnAccueil_clicked();
    void on_cBNomTable_currentIndexChanged(const QString &arg1);
    void on_cBAjout_stateChanged(int arg1);
    void on_pBAddEdit_clicked();
    void on_cBEditUser_currentIndexChanged(const QString &arg1);
    void on_cBNom_clicked(bool checked);
    void on_cBPrenom_clicked(bool checked);
    void on_cBLogin_clicked(bool checked);
    void on_cBMdp_clicked(bool checked);
    void on_checkBoxStatus_clicked(bool checked);
    void on_pBSupprimer_clicked();
    void on_pBBtn4_clicked();
    void on_pBAddConfig_clicked();
    void on_checkBoxAddEditConfig_stateChanged(int arg1);
    void on_checkBoxSuppConfig_stateChanged(int arg1);
    void on_pBEditConfig_clicked();
    void on_pBAnnulerConfig_clicked();
    void on_pBContinuerConfig_clicked();
    void on_stackedWidget_currentChanged(int arg1);
    void on_pBAnnulerConfig_2_clicked();
    void on_pBContinuerConfig_2_clicked();
    void on_pBVannesP_clicked();
    void on_pBCapteurs_clicked();
    void on_pBSeqTests_clicked();
    void on_pBArrUrg_clicked();
    void on_pBRelais_clicked();
    void on_lENomConfig_returnPressed();
    void on_pBContinuerConfig_3_clicked();
    void on_pBRetour_2_clicked();
    void on_pBRetour_3_clicked();
    void on_pBContinuerConfig_4_clicked();
    void on_pBRetour_clicked();
    void on_pBRetour_4_clicked();
    void on_pBContinuerConfig_5_clicked();
    void on_pBRetour_5_clicked();
    void on_pBContinuerConfig_6_clicked();
    void on_pBValidConfig_clicked();
    void on_pBRetour_6_clicked();
    void on_pBContinuerConfig_7_clicked();
    void on_pBConfirmer_clicked();
    void on_pBSuppConfig_clicked();
    void on_pBAnnuler_clicked();

private:
    Ui::PageAccueil *ui;
    SqlQervice *ssl;
    QSqlTableModel *model;
    bool isEdit;
    bool isConfigEdit;
    bool initRealized;
    int idUser;
    // variables pour l'ajout d'une configuration :
    int idConfig;
    QString nomConfig;
    QString typeConfig;
    QString deviceName;
    QString device;
    CarteNI carteNI;
    QStringList inAnalog;
    QStringList outAnalog;
    QStringList inNum;
    QStringList outNum;
    QStringList nbPortIn;
    QStringList nbPortOut;
    // Booléens servant à vérifier le nombre nécessaire de composants au bon fonctionnement du banc de test :
    bool vannesOK;
    bool relaisOK;
    bool arrUrgOK;
    bool seqTestOK;
    bool capteursOK;

    int nbRelais;
    QString nbRelaisStr;

    QString capteursPath;
    QString vannesPath;
    QString etorPath;

    // Variables où sont stockés les id lors d'un edit :
    QList<int> idsEdit;

    QStringList vannesProportionnelles; // Où seront stockés les vannes (Selon la config de carteNI (BdD))
    QStringList capteurs;
    QString seqTest;
    QString arrUrg;
    QStringList relais;

private:
    void makeRecap();
    void isConfigConfirme();
    void clearRelaisForm();
    void clearArrUrgForm();
    void clearSeqTestForm();
    void clearCapteursForm();
    void clearVannesForm();
    void clearRecapForm();
    void initConfigVar();
    void getTables(void);
    bool initSqlService(void);
    void setModif(void);
    void initCBUserEdit(void);
    void verifPerms(int id);
    void clearAddEditForm();
    void desactiveChampModif();
    void clearConfigForm();
    void resetConfigVar();
    void resetBtn();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // PAGEACCUEIL_H
