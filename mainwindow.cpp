#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->current->setValidator(new QIntValidator(0, 255));
    ui->voltage->setValidator(new QIntValidator(0, 255));
    ui->shim->setValidator(new QIntValidator(0, 255));
    ui->address->setValidator(new QIntValidator(0, 39));

    serialPort = new SerialPort;
    connect(serialPort, &SerialPort::message, this, &MainWindow::messageRecieve);

    ui->current->setText("16");
    ui->voltage->setText("24");
    ui->address->setText("10");
    ui->shim->setText("10");
    ui->portName->setText("COM1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::messageRecieve(QString input)
{
    ui->textBrowser->append(input);
}


void MainWindow::on_chargeButton_clicked()
{
    serialPort->chargeCommand(ui->address->text().toInt(), ui->voltage->text().toInt(), ui->current->text().toInt());
}


void MainWindow::on_dischargeButton_clicked()
{
    serialPort->dischargeCommand(ui->address->text().toInt(), ui->shim->text().toInt());
}


void MainWindow::on_connectButton_clicked()
{
    serialPort->connect(ui->portName->text());
}


void MainWindow::on_findButton_clicked()
{
    serialPort->findAvaliblePorts();
}


void MainWindow::on_readButton_clicked()
{
    serialPort->readCommand(ui->address->text().toInt());
}


void MainWindow::on_turnOffButton_clicked()
{
    serialPort->turnOffCommand(ui->address->text().toInt());
}

