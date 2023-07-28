#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    SerialPort(QObject *parent = nullptr);

    void findAvaliblePorts();
    void connect(QString portName);
    void readCommand(int address);
    void turnOffCommand(int address);
    void chargeCommand(int address, int voltage, int current);
    void dischargeCommand(int address, int shim);

private:
    QSerialPort *port;
    const int timeoutSecs = 1;

    void sendRequest(QByteArray request);

private slots:
    void dataRecieve(int address);

signals:
    void message(QString message);
};

#endif // SERIALPORT_H
