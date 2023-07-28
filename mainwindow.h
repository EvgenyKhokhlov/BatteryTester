#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SerialPort *serialPort;

private slots:
    void messageRecieve(QString input);
    void on_chargeButton_clicked();
    void on_dischargeButton_clicked();
    void on_connectButton_clicked();
    void on_findButton_clicked();
    void on_readButton_clicked();
    void on_turnOffButton_clicked();
};
#endif // MAINWINDOW_H
