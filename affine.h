#ifndef AFFINE_H
#define AFFINE_H

#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QTextEdit>
namespace Ui {
class affine;
}

class affine : public QDialog
{
    Q_OBJECT

public:
    QString post_text;
    QTextEdit *a;
    QTextEdit *b;
    QLabel *label;
    QTableWidget* tableWidget;
    QMap<QChar, float> decomposed_text;
    explicit affine(QWidget *parent = nullptr);
    ~affine();
    QString pre_text;

private slots:
    void save_pushed();
    void chose_pushed();
    void encrypt_pushed();
private:
    Ui::affine *ui;
};

#endif // AFFINE_H
