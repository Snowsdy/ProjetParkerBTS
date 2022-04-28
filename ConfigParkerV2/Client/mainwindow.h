#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include "sqlqervice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void relancerConnexion(void);

private slots:
    void on_lEMdp_returnPressed();
    void on_pB_Connection_clicked();
    void labelMsgClear();

signals:
    void connexionSuccessful(int);

private:
    Ui::MainWindow *ui;
    SqlQervice *sqlService;

private:
    void clearAndFocus();
};
#endif // MAINWINDOW_H
