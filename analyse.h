#ifndef ANALYSE_H
#define ANALYSE_H

#include <QDialog>
#include <QLabel>
#include <QTableWidget>
namespace Ui {
class analyse;
}

class analyse : public QDialog
{
    Q_OBJECT

public:
    QLabel *label;
    QTableWidget* tableWidget;
    QMap<QChar, double> decomposed_text;
    explicit analyse(QWidget *parent = nullptr);
    QString pre_text;
    ~analyse();

private slots:
    void save_pushed();
    void chose_pushed();


private:
    Ui::analyse *ui;
};

#endif // ANALYSE_H
