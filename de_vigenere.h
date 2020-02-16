#ifndef de_vigenere_H
#define de_vigenere_H

#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QTextEdit>
namespace Ui {
class de_vigenere;
}

class de_vigenere : public QDialog
{
    Q_OBJECT

public:
    QString post_text;
    QTextEdit *a;
    QTextEdit *b;
    QLabel *label;
    QTableWidget* tableWidget;
    QMap<QChar, double> decomposed_text;
    QMap<QChar, double> decomposed_text_num;
    QMap<QChar, double> received_table;
    explicit de_vigenere(QWidget *parent = nullptr);
    ~de_vigenere();
    QString pre_text;
    QList<QString> findPermutations(QList<QChar> a);
    int solveCongruences(int A[], int B[], int M[], int n);
    int findGCD(QList<int> arr, int n);
    int gcd(int a, int b);
    int freq_analysis(QString str);

private slots:
    void table_pushed();
    void chose_pushed();
    void decrypt_pushed();
    void save_pushed();
private:
    Ui::de_vigenere *ui;
};

#endif // de_vigenere_H
