#include "vigenere.h"
#include "ui_vigenere.h"
#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextEdit>

vigenere::vigenere(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vigenere)
{
    ui->setupUi(this);
    this->resize(800, 600);

    QPushButton *chose_file = new QPushButton(this);
    chose_file->resize(400,35);
    chose_file->setText(tr("Chose file"));
    chose_file->move(400, 0);
    connect(chose_file, SIGNAL (released()), this, SLOT (chose_pushed()));

    label = new QLabel(this);
    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label->resize(400, 565);
    label->move(400,35);

    QPushButton *encrypt  = new QPushButton(this);
    encrypt->resize(75, 75);
    encrypt->setText(tr("encrypt"));
    encrypt->move(165, 375);
    connect(encrypt, SIGNAL (released()), this, SLOT (encrypt_pushed()));

    QPushButton *save_table = new QPushButton(this);
    save_table->resize(303, 35);
    save_table->setText(tr("save file"));
    save_table->move(50, 535);
    connect(save_table, SIGNAL (released()), this, SLOT (save_pushed()));

    tableWidget = new QTableWidget(1, 3, this);
    tableWidget->resize(100, 300);
    tableWidget->setColumnWidth(0,100);
}
vigenere::~vigenere()
{
    delete ui;
}
void vigenere::chose_pushed()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Chose file", "C:/Users/MiPro/Documents");
    QFile file(file_name);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qInfo() << "File not opened";
    }

    QTextStream in(&file);
    //in.setCodec("UTF-8");
    pre_text = in.readAll();

    label->setText(pre_text);

    for(QChar &ch: pre_text)
    {
        if(decomposed_text.contains(ch))
            decomposed_text[ch]++;
        else
            decomposed_text[ch]=1;
    }


    tableWidget->clear();
    tableWidget->setHorizontalHeaderLabels({"char"});

    int ex = 0;
    for(auto &key: decomposed_text.keys())
    {
        tableWidget->setItem(ex, 0, new QTableWidgetItem(QString(key)));
        tableWidget->insertRow( tableWidget->rowCount());
        ex++;
    }
}

void vigenere::save_pushed()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                         "C:/Users/MiPro/Documents/untitled.txt",
                                                         tr("Text files (*.txt)"));
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    QTextStream out(&f);
//    /out.setCodec("UTF-8");

    out << post_text;
    f.close();
}

void vigenere::encrypt_pushed()
{
    QVector<int> cipher_key = {13, 23, 19, 17, 17};
    int m = decomposed_text.keys().size();

//    for(auto &c: cipher_key)
//        qInfo() << decomposed_text.keys()[c % m];




    QString keye;
    QString alphabet;

//    for(int a :cipher_key)
//        keye+= decomposed_text.keys()[a];

    for(auto a : decomposed_text.keys())
        alphabet+= a;

//    qInfo() << keye;
//    qInfo() << "-----------------";
    qInfo() << alphabet;

    for(int j=0; j < pre_text.size(); j++)
    {
        for(int i = 0; i< m; i++)
            if(decomposed_text.keys()[i] == pre_text[j])
                post_text += decomposed_text.keys()[(i + (cipher_key[j % cipher_key.size()])) % m];
    }

    label->setText(post_text);

}

