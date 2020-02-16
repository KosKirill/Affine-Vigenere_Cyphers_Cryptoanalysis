#include "de_affine.h"
#include "ui_de_affine.h"
#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextEdit>
#include <QString>
#include <cmath>
#include <QtMath>
#include <QtAlgorithms>

de_affine::de_affine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::de_affine)
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
    encrypt->setText(tr("decrypt"));
    encrypt->move(165, 375);
    connect(encrypt, SIGNAL (released()), this, SLOT (decrypt_pushed()));

    QPushButton *save_table = new QPushButton(this);
    save_table->resize(400,35);
    save_table->setText(tr("chose table"));
    save_table->move(0, 0);
    connect(save_table, SIGNAL (released()), this, SLOT (table_pushed()));

    tableWidget = new QTableWidget(1, 3, this);
    tableWidget->move(0,35);
    tableWidget->resize(303, 265);
    tableWidget->setColumnWidth(0,100);
    tableWidget->setColumnWidth(1,100);
    tableWidget->setColumnWidth(2,100);

    QPushButton *save_file = new QPushButton(this);
    save_file->resize(303, 35);
    save_file->setText(tr("save file"));
    save_file->move(50, 535);
    connect(save_file, SIGNAL (released()), this, SLOT (save_pushed()));


    QLabel *al = new QLabel(this);
    al->resize(25, 25);
    al->move(80, 480);
    al->setText("a");

    QLabel *bl = new QLabel(this);
    bl->resize(25, 25);
    bl->move(180, 480);
    bl->setText("b");

    a = new QLabel(this);
    a->resize(50, 25);
    a->move(100, 480);

    b = new QLabel(this);
    b->resize(50, 25);
    b->move(200, 480);
}

de_affine::~de_affine()
{
    delete ui;
}
void de_affine::chose_pushed()
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


    for(auto &key: decomposed_text.keys())
    {
        decomposed_text[key] = float(decomposed_text[key]) / float(pre_text.size());
        decomposed_text[key] = QString::number(decomposed_text[key]).toDouble();
    }
    file.close();
}

void de_affine::table_pushed()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Chose file", "C:/Users/MiPro/Documents");
    QFile file(file_name);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qInfo() << "File not opened";
    }

    QTextStream in(&file);
    //in.setCodec("UTF-8");

    QString table = in.readAll();

    QRegularExpression rx("[0-9]+\\t (.|\n)\\t ([0-9]+.[0-9]*e*-*[0-9]*)");


    QRegularExpressionMatchIterator i = rx.globalMatch(table);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
             received_table[(match.captured(1)[0])] = match.captured(2).toDouble();
        }
    }
    //creceived_table;
    tableWidget->clear();
    tableWidget->setHorizontalHeaderLabels({"char", "occurances", "frequency"});

    int ex = 0;
    for(auto &key: received_table.keys())
    {
        tableWidget->setItem(ex, 0, new QTableWidgetItem(QString(key)));
        tableWidget->setItem(ex, 1, new QTableWidgetItem(QString::number(received_table[key])));
        tableWidget->insertRow( tableWidget->rowCount());
        ex++;
    }

    file.close();
}
int de_affine::solveCongruences(int A[], int B[], int M[], int n)
{
    int x = 0, mod = 1;

    for (int i = 0; i < n; i++)
     { int r1 = A[i]*mod, r2 = M[i], x1 = 1, x2 = 0, r = 1;
       while (r != 0)
        { int q = (int)floor((double)r1/r2), t = x1-q*x2;
          x1 = x2; x2 = t; r = r1-q*r2; r1 = r2; r2 = r;
        }
       int b = B[i]-A[i]*x;
       if (b % r1 != 0){ return 0; }
       x += mod * b * x1/r1;
       mod *= M[i]/r1;
     }

    if (x < 0 || x >= mod) x -= mod * (int)floor((double)x/mod);

    return x;
}

void de_affine::decrypt_pushed()
{
    int x1 = -1;
    int x2 = -1;

    int y1 = -1;
    int y2 = -1;
    QList<QPoint> most_similar;


    double min_diff = 2;
    int n = decomposed_text.size();
    int m = received_table.size();

    QList<QPair<double, uint>> dec_max;
    QList<QPair<double, uint>> rec_max;

    for(int i = 0; i < n; i++)
    {
        dec_max.append(QPair<double, uint>(decomposed_text[decomposed_text.keys()[i]], i));
    }

    for(int i = 0; i < m; i++)
    {
        rec_max.append(QPair<double, uint>(received_table[received_table.keys()[i]], i));
    }

    qStableSort(dec_max.begin(), dec_max.end());
    qStableSort(rec_max.begin(), rec_max.end());

    QChar x1_char = decomposed_text.keys()[dec_max[dec_max.size() - 1].second];
    QChar x2_char = decomposed_text.keys()[dec_max[dec_max.size() - 2].second];

    QChar y1_char = received_table.keys()[rec_max[rec_max.size() - 1].second];
    QChar y2_char = received_table.keys()[rec_max[rec_max.size() - 2].second];

    if(decomposed_text.contains(y1_char) && decomposed_text.contains(y2_char))
    {
        x1 = dec_max[dec_max.size() - 1].second;
        y1 = rec_max[rec_max.size() - 1].second;

        x2 = dec_max[dec_max.size() - 2].second;
        y2 = rec_max[rec_max.size() - 2].second;

        qInfo() << "first";

    }
    else
    {
        QList<QPair<qreal, QPair<uint, uint>>> diff_list;
        for(int i = 0; i < n; i++)
           for(int j = 0; j < m; j++)
            {
               min_diff = qFabs(decomposed_text[decomposed_text.keys()[i]] - received_table[received_table.keys()[j]]);
               //qInfo() << QPoint(i,j) << "diff " << min_diff << decomposed_text[decomposed_text.keys()[i]]<< "-" << received_table[received_table.keys()[j]];
               diff_list.append(QPair<qreal, QPair<uint, uint>>(min_diff, QPair<uint, uint>(i, j)));

             }

        qStableSort(diff_list);

        x1 =  diff_list[0].second.second;
        y1 =  diff_list[0].second.first;
        x2 =  diff_list[1].second.second;
        y2 =  diff_list[1].second.first;
    }



    y1 = decomposed_text.keys().indexOf(y1_char);
    y2 = decomposed_text.keys().indexOf(y2_char);


    int A[] = {y1 - y2}, B[] = {x1 - x2}, M[] = {decomposed_text.size()};
    n = 1;
    int a_coef = solveCongruences(A, B, M, n);


    int b_coef = (((x2 - a_coef * y2)) + decomposed_text.size() ) % decomposed_text.size();

    qInfo("And the oscar goes to...");
    qInfo() << a_coef << " " << b_coef;
    a->setText(QString::number(a_coef));
    b->setText(QString::number(b_coef));
int tr = 0;
    for(auto &ch: pre_text)
        for(int i = 0; i< decomposed_text.size(); i++)
            if(decomposed_text.keys()[i] == ch)
            {

                tr++;
                int A[] = {a_coef}, B[] = {i - b_coef}, M[] = {decomposed_text.size()}, n = 1;
                int source = solveCongruences(A, B, M, n);

                post_text.append(decomposed_text.keys()[source]);
            }

    label->setText(post_text);

}

void de_affine::save_pushed()
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
