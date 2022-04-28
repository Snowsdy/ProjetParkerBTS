#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sqlService = new SqlQervice();
    sqlService->connexionBase();
    sqlService->creerTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::relancerConnexion()
{
    sqlService->connexionBase();
    ui->lEUsername->clear();
    ui->lEMdp->clear();
    ui->lEUsername->setFocus();
    this->show();
}


void MainWindow::on_lEMdp_returnPressed()
{
    this->ui->pB_Connection->clicked(true);
}

void MainWindow::on_pB_Connection_clicked()
{
    int r = 0;
    if ((r = sqlService->isUserValid(ui->lEUsername->text(), ui->lEMdp->text())) > 0) {
        sqlService->db.close();
        sqlService->flag = false;
        emit connexionSuccessful(r);
        this->hide();
    } else if (r == -5) {
        QMessageBox box(QMessageBox::Warning, "Attention", "Vous n'avez pas les permissions pour accéder à ce programme.\n"
                                                           "Veuillez prendre contact avec un administrateur.");
        box.addButton("D'accord", QMessageBox::YesRole);
        box.exec();
        QTimer::singleShot(1000, this, SLOT(labelMsgClear()));
    } else {
        ui->labelMessage->setText("Mot de passe et/ou login incorrect !");
        QTimer::singleShot(1000, this, SLOT(labelMsgClear()));
    }

}

void MainWindow::labelMsgClear()
{
    this->clearAndFocus();
}

void MainWindow::clearAndFocus()
{
    ui->labelMessage->clear();
    ui->lEUsername->clear();
    ui->lEMdp->clear();
    this->ui->lEUsername->setFocus();
}
