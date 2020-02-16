#ifndef vigenere_H
#define vigenere_H

#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QTextEdit>
namespace Ui {
class vigenere;
}

class vigenere : public QDialog
{
    Q_OBJECT

public:
    QTextEdit *a;
    QTextEdit *b;
    QLabel *label;
    QTableWidget* tableWidget;
    QMap<QChar, unsigned int> decomposed_text;
    explicit vigenere(QWidget *parent = nullptr);
    ~vigenere();
    QString pre_text;
    QString post_text;
private slots:
    void save_pushed();
    void chose_pushed();
    void encrypt_pushed();
private:
    Ui::vigenere *ui;
};

#endif // vigenere_H
