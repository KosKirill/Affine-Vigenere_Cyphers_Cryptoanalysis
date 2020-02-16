#include "affine.h"
#include "ui_affine.h"
#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextEdit>

affine::affine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::affine)
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
    a = new QTextEdit(this);
    a->resize(50, 25);
    a->move(100, 480);

    b = new QTextEdit(this);
    b->resize(50, 25);
    b->move(200, 480);
}

affine::~affine()
{
    delete ui;
}
void affine::chose_pushed()
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
            decomposed_text[ch] = 1;
    }

    tableWidget->clear();
    tableWidget->setHorizontalHeaderLabels({"char"});

    int ex = 0;
    for(auto &key: decomposed_text.keys())
    {
        decomposed_text[key] = decomposed_text[key] / pre_text.size();
        tableWidget->setItem(ex, 0, new QTableWidgetItem(QString(key)));
        tableWidget->insertRow( tableWidget->rowCount());
        ex++;
    }
}

void affine::save_pushed()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                         "C:/Users/MiPro/Documents/untitled.txt",
                                                         tr("Text files (*.txt)"));
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    QTextStream out(&f);
    //out.setCodec("UTF-8");

    out << post_text;
    f.close();
}

void affine::encrypt_pushed()
{

    int m = decomposed_text.size();

    for(auto &ch: pre_text)
    {
        for(int i = 0; i< m; i++)
            if(decomposed_text.keys()[i] == ch)
            {
                    qInfo() << (((i) * a->toPlainText().toInt()) + b->toPlainText().toInt()) % m;
                    post_text += decomposed_text.keys()[(((i) * a->toPlainText().toInt()) + b->toPlainText().toInt()) % m];
            }

    }

    label->setText(post_text);


}

