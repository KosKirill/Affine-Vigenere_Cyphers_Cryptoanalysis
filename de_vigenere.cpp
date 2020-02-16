#include "de_vigenere.h"
#include "ui_de_vigenere.h"
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

de_vigenere::de_vigenere(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::de_vigenere)
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
    label->resize(400, 530);
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

    QPushButton *save_file = new QPushButton(this);
    save_file->resize(303, 35);
    save_file->setText(tr("save file"));
    save_file->move(50, 535);
    connect(save_file, SIGNAL (released()), this, SLOT (save_pushed()));

    tableWidget = new QTableWidget(1, 3, this);
    tableWidget->move(0,35);
    tableWidget->resize(303, 265);
    tableWidget->setColumnWidth(0,100);
    tableWidget->setColumnWidth(1,100);
    tableWidget->setColumnWidth(2,100);
    //tableWidget->verticalHeader()->setVisible(false);

    QLabel *klabel = new QLabel(this);
    klabel->resize(50, 35);
    klabel->move(75, 480);
    klabel->setText("Key");

    a = new QTextEdit(this);
    a->resize(200, 35);
    a->move(100, 480);
}

de_vigenere::~de_vigenere()
{
    delete ui;
}
void de_vigenere::chose_pushed()
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

    QHash<QChar, unsigned int> new_map;

    for(QChar &ch: pre_text)
    {
        if(decomposed_text.contains(ch))
            decomposed_text[ch]++;
        else
            decomposed_text[ch] = 1;
    }
    decomposed_text_num = decomposed_text;

    for(auto &key: decomposed_text.keys())
    {
        decomposed_text[key] = float(decomposed_text[key]) / float(pre_text.size());
        decomposed_text[key] = QString::number(decomposed_text[key]).toDouble();
    }
    file.close();
}

void de_vigenere::table_pushed()
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
int de_vigenere::solveCongruences(int A[], int B[], int M[], int n)
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

QList<QString> de_vigenere::findPermutations(QList<QChar> list)
{
    QList<QString> permutations;
        for(auto i: list)
            for(auto j: list)
                for(auto k: list)
                {
                    permutations.append(QString(i) + j + k);
                }
    return permutations;
}
int de_vigenere::gcd(int a, int b)
{
    if (a == 0)
        return b;
    return gcd(b % a, a);
}

// Function to find gcd of array of
// numbers
int de_vigenere::findGCD(QList<int> arr, int n)
{
    int result = arr[0];
    for (int i = 1; i < n; i++)
        result = gcd(arr[i], result);

    return result;
}

void de_vigenere::decrypt_pushed()
{
    QList<QString> trigrams = findPermutations(decomposed_text.keys());
    QList<QPair<int, QString>> occurances;
    QList<int> indexes = {0};

    for(int i = 0; i < trigrams.size(); i++)
        occurances.append(QPair<int, QString>(pre_text.count(trigrams[i]), trigrams[i]));

    qStableSort(occurances);
    QString best_trigram = occurances[occurances.size() -1].second;

    qInfo() << occurances[occurances.size() -1];

//    for(int i = 0; i < occurances[occurances.size() -1].first; i++)
//    {
//        indexes.append(pre_text.indexOf(best_trigram));
//        pre_text[i]

//    }


    QString reg = "\\" + best_trigram[0] + "\\" + best_trigram[1] + "\\" + best_trigram[2];
    QRegExp rx(reg);
    int count = 0;
    int pos = 0;
    while ((pos = rx.indexIn(pre_text, pos)) != -1) {
        ++count;
        pos += rx.matchedLength();
        indexes.append(pos);
    }

//    int zbl = indexes[1];
//    for(auto &i: indexes)
//    {
//        i = i - zbl;
//    }

//    indexes.pop_front();
//    indexes.pop_front();

    int key_size = 0;
    //key_size = findGCD(indexes, indexes.size());
    qInfo() << "Kasiski verdict: key size is " << key_size;
    double delta = 0;
//------------------------------------------------------------------------------------------------
    QString first_fri;
    QString second_fri;
    QList<float> delts;
    for(int epoch = 1; epoch < 10; epoch++)
    {

        //qInfo() << "EPOCH " << epoch;
        int length = pre_text.size() / epoch;
        delta = 0;
        for(int interval = 0; interval < epoch; interval++)
        {
            QString new_pretext;
            QMap<QChar, unsigned int> new_map;

            for(int i = 0; i < pre_text.size(); i++)
                if(i % epoch == interval)
                    new_pretext+= pre_text[i];

            if(interval == 0)
                first_fri = new_pretext;
            if(interval == 3)
                second_fri = new_pretext;

            for(int i = 0; i < new_pretext.size(); i++)
            {
                if(new_map.contains(new_pretext[i]))
                    new_map[new_pretext[i]]++;
                else
                    new_map[new_pretext[i]] = 1;
            }



            for(auto &key: new_map.keys())
                delta += double(new_map[key] * (new_map[key] -1)) / (new_pretext.size() * (new_pretext.size() - 1));


        }

        delta/=epoch;
        delts.append(delta);

    }
    float maxdls = 0;
    int maxidx = 0;
    for(int i = 0; i < delts.size(); i++)
        if(delts[i] > maxdls)
        {
            maxdls = delts[i];
            maxidx = i;
        }

    key_size = maxidx + 1;

    QList<QString> mod_strings;
    for(int interval = 0; interval < key_size; interval++)
    {
        QString new_pretext;
        for(int i = 0; i < pre_text.size(); i++)
            if(i % key_size == interval)
                new_pretext+= pre_text[i];

        mod_strings.append(new_pretext);

    }


    qInfo() << "Fridman verdict: key size is " << key_size;

    QString base_str = pre_text;
    QList<int> key;
    key.push_back(freq_analysis(mod_strings[0]));

    qInfo() << "First key char is " << key[0];
//------------------------------------------------------------------------------------------------
    QList<int> offsets;
    offsets.append(0);
    int prev_offset = 0;
    for(auto & stringer: mod_strings)
    {
        QMap<QChar, unsigned int> fri_map;
        QList<QString> enc_strings;
        QList<float> deltas;
        delta = 0;
        int m = decomposed_text.size();
        for(int st = 1; st <= m; st++)
        {
            QString new_string;
            for(int j=0; j < stringer.size(); j++)
            {
                for(int i = 0; i< m; i++)
                    if(decomposed_text.keys()[i] == stringer[j])
                        new_string += decomposed_text.keys()[(i + st)  % m];
            }
            enc_strings.append(new_string);
        }

        for(int i = 0; i < first_fri.size(); i++)
        {
            if(fri_map.contains(first_fri[i]))
                fri_map[first_fri[i]]++;
            else
                fri_map[first_fri[i]] = 1;
        }

    //    for(QChar c:fri_map.keys())
    //        sum += fri_map[c];
    //        qInfo() << "Size " << fri_map.keys().size() << "Sum " << sum;



        int size = enc_strings.size();
        for(int st=0; st < size ; st++)
        {
            delta = 0;
            QMap<QChar, double> n_map = decomposed_text_num;
            for(QChar c:n_map.keys())
                n_map[c] = 0;
            int size_1 = enc_strings[st].size();

            for(int i = 0; i < size_1; i++)
            {
                if(n_map.contains(enc_strings[st][i]))
                    n_map[enc_strings[st][i]]++;
                else
                    n_map[enc_strings[st][i]] = 1;
            }

            for(auto &key: n_map.keys())
                delta += double(n_map[key] * (fri_map[key])) / double((enc_strings[st].size() * (first_fri.size())));

            deltas.append(delta);
        }

        float max_delta = 0;
        int max_delta_i = 0;;
        for(int i = 0; i < deltas.size(); i++)
            if(deltas[i] > max_delta)
            {
                max_delta = deltas[i];
                max_delta_i = i;
            }
        offsets.append(max_delta_i - prev_offset);
        prev_offset = max_delta_i;
        if(offsets.size() > 2)
            key.append(((key[key.size() - 1] - offsets[offsets.size() -1])
                    + deltas.size()) % deltas.size());
            qInfo() << "Offset " <<  key[key.size() - 1];


        qInfo() << deltas.size();
    }


    qInfo() << "Key is " << key;

    int m = decomposed_text.size();
    for(int j=0; j < pre_text.size(); j++)
    {
        for(int i = 0; i< m; i++)
            if(decomposed_text.keys()[i] == pre_text[j])
                post_text += decomposed_text.keys()[((i - (key[j % key.size()])) + m) % m];
    }

    label->setText(post_text);

    QStringList sl;
    foreach(int n, key) sl << QString::number(n);
    QString resultString = sl.join(",");

    a->setText(resultString);
       qInfo() << "decrypting finished";
}

int de_vigenere::freq_analysis(QString str)
{


    QMap<QChar, double> str_map;

    for(int i = 0; i < str.size(); i++)
    {
        if(str_map.contains(str[i]))
            str_map[str[i]]++;
        else
            str_map[str[i]] = 1;
    }

    int x1 = -1;
    int y1 = -1;

    QList<QPoint> most_similar;


    double min_diff = 2;
    int n = str_map.size();
    int m = received_table.size();

    QList<QPair<double, uint>> dec_max;
    QList<QPair<double, uint>> rec_max;

    for(int i = 0; i < n; i++)
    {
        dec_max.append(QPair<double, uint>(str_map[str_map.keys()[i]], i));
    }

    for(int i = 0; i < m; i++)
    {
        rec_max.append(QPair<double, uint>(received_table[received_table.keys()[i]], i));
    }

    qStableSort(dec_max.begin(), dec_max.end());
    qStableSort(rec_max.begin(), rec_max.end());


    QChar x1_char = str_map.keys()[dec_max[dec_max.size() - 1].second];
    QChar y1_char = received_table.keys()[rec_max[rec_max.size() - 1].second];

    if(str_map.contains(y1_char))
    {
        x1 = dec_max[dec_max.size() - 1].second;
        y1 = rec_max[rec_max.size() - 1].second;


        qInfo() << "first";

    }

    x1 = decomposed_text.keys().indexOf(x1_char);
    y1 = decomposed_text.keys().indexOf(y1_char);

    qInfo() << x1 << " " << y1;
    return (x1 - y1 + decomposed_text.size()) % decomposed_text.size();

}

void de_vigenere::save_pushed()
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
