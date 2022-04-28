#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QFrame>
#include <QMessageBox>
#include <QAbstractButton>
#include "sqlservice.h"
#include "physiquefactory.h"
#include "config.h"

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QFrame
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

private slots:
    void on_pBQuitter_clicked();
    void on_cBChoixConfig_currentIndexChanged(int index);
    void on_pBLancerConfig_clicked();

    void on_checkBoxELEC_stateChanged(int arg1);

private:
    Ui::SplashScreen *ui;
    Config *conf;
    SqlService *ssl;
    QStringList configs;
    int progression;

signals:
    void launchingEmitted(int valInt);
    void lancement(void);

public slots:
    void majProgressBar(int);
    void majInfo(QString);

};

#endif // SPLASHSCREEN_H
