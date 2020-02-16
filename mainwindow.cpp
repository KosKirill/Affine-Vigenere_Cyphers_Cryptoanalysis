#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include "analyse.h"
#include "affine.h"
#include "vigenere.h"
#include "de_affine.h"
#include "de_vigenere.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget * wdg = new QWidget(this);
    wdg->resize(1000, 1000);

    QPushButton *analyse_button = new QPushButton(wdg);
    analyse_button->resize(165, analyse_button->height());
    analyse_button->setText(tr("analyse"));
    analyse_button->move(300, 200);
    connect(analyse_button, SIGNAL (released()), this, SLOT (analyse_pushed()));

    QPushButton *encrypt_button = new QPushButton(wdg);
    encrypt_button->resize(165, encrypt_button->height());
    encrypt_button->setText(tr("encrypt affine"));
    encrypt_button->move(300, 250);
    connect(encrypt_button , SIGNAL (released()), this, SLOT (encrypt_affine()));

    QPushButton *decrypt_button = new QPushButton(wdg);
    decrypt_button->resize(165, decrypt_button->height());
    decrypt_button->setText(tr("encrypt vigenere"));
    decrypt_button->move(300, 300);
    connect(decrypt_button, SIGNAL (released()), this, SLOT (encrypt_vigenere()));

    QPushButton *decrypt1_button = new QPushButton(wdg);
    decrypt1_button->resize(165, encrypt_button->height());
    decrypt1_button->setText(tr("decrypt affine"));
    decrypt1_button->move(300, 350);
    connect(decrypt1_button , SIGNAL (released()), this, SLOT (decrypt_affine()));

    QPushButton *decrypt2_button = new QPushButton(wdg);
    decrypt2_button->resize(165, decrypt_button->height());
    decrypt2_button->setText(tr("decrypt vigenere"));
    decrypt2_button->move(300, 400);
    connect(decrypt2_button, SIGNAL (released()), this, SLOT (decrypt_vigenere()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::analyse_pushed()
{
   analyse analyse_dialog;
   analyse_dialog.setModal(true);
   analyse_dialog.exec();

}

void MainWindow::encrypt_affine()
{
    affine affine_dialog;
    affine_dialog.setModal(true);
    affine_dialog.exec();
}

void MainWindow::encrypt_vigenere()
{
    vigenere vigenere_dialog;
    vigenere_dialog.setModal(true);
    vigenere_dialog.exec();
}

void MainWindow::decrypt_affine()
{
    de_affine deaffine_dialog;
    deaffine_dialog.setModal(true);
    deaffine_dialog.exec();
}

void MainWindow::decrypt_vigenere()
{
    de_vigenere devigenere_dialog;
    devigenere_dialog.setModal(true);
    devigenere_dialog.exec();
}
