#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
        void analyse_pushed();
        void encrypt_affine();
        void encrypt_vigenere();
        void decrypt_affine();
        void decrypt_vigenere();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
