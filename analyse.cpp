#include "analyse.h"
#include "ui_analyse.h"
#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include<QTableWidget>
#include <QHeaderView>
#include <QHash>
#include <string>

analyse::analyse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::analyse)
{
    ui->setupUi(this);
    this->resize(800, 600);

    QPushButton *chose_file = new QPushButton(this);
    chose_file->resize(800,35);
    chose_file->setText(tr("Chose file"));
    //chose_file->move(300, 200);
    connect(chose_file, SIGNAL (released()), this, SLOT (chose_pushed()));

    label = new QLabel(this);
    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label->resize(400, 565);
    label->move(400,35);

    tableWidget = new QTableWidget(1, 3, this);
    tableWidget->move(0,35);
    tableWidget->resize(303, 400);
    tableWidget->setColumnWidth(0,100);
    tableWidget->setColumnWidth(1,100);
    tableWidget->setColumnWidth(2,100);
    //tableWidget->verticalHeader()->setVisible(false);

    QPushButton *save_table = new QPushButton(this);
    save_table->resize(303, 35);
    save_table->setText(tr("save table"));
    save_table->move(0, 435);
    connect(save_table, SIGNAL (released()), this, SLOT (save_pushed()));
}

analyse::~analyse()
{
    delete ui;
}

void analyse::chose_pushed()
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
    tableWidget->setHorizontalHeaderLabels({"char", "occurances", "frequency"});

    int ex = 0;
    for(auto &key: decomposed_text.keys())
    {

        tableWidget->setItem(ex, 0, new QTableWidgetItem(QString(key)));
        tableWidget->setItem(ex, 1, new QTableWidgetItem(QString(QString("%1").arg(decomposed_text[key]))));

        decomposed_text[key] = decomposed_text[key] / double(pre_text.size());

        tableWidget->setItem(ex, 2, new QTableWidgetItem(QString::fromStdString(std::to_string(decomposed_text[key]))));
        tableWidget->insertRow( tableWidget->rowCount());
        ex++;
    }

}

void analyse::save_pushed()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                         "C:/Users/MiPro/Documents/untitled.txt",
                                                         tr("Text files (*.txt)"));
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    QTextStream out(&f);
    //out.setCodec("UTF-8");

    int ex = 0;
    for(auto &key: decomposed_text.keys())
    {
        out << QString("%1\t %2\t %3\n").arg(ex + 1).arg(key).arg(QString::number(decomposed_text[key]));
        ex++;
    }
    f.close();
}
