#ifndef de_affine_H
#define de_affine_H

#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QTextEdit>
namespace Ui {
class de_affine;
}

class de_affine : public QDialog
{
    Q_OBJECT

public:
    QString post_text;
    QLabel *a;
    QLabel *b;
    QLabel *label;
    QTableWidget* tableWidget;
    QMap<QChar, double> decomposed_text;
    QMap<QChar, double> received_table;
    explicit de_affine(QWidget *parent = nullptr);
    ~de_affine();
    QString pre_text;


    int solveCongruences(int A[], int B[], int M[], int n);


private slots:
    void table_pushed();
    void chose_pushed();
    void decrypt_pushed();
    void save_pushed();
private:
    Ui::de_affine *ui;
};

#endif // de_affine_H
