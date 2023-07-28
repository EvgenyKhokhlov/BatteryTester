#include "serialport.h"

SerialPort::SerialPort(QObject *parent)
    : QObject(parent)
{
    // Создание и настройка последовательного порта
    port = new QSerialPort;
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
}

void SerialPort::connect(QString portName)
{
    if(port->isOpen())
        port->close();

    // Выбирем последовательный порт
    QSerialPortInfo selectedPort;
    for (const QSerialPortInfo &port : QSerialPortInfo::availablePorts()) {
        if (port.portName().contains(portName)) { // поиск порта по названию
            selectedPort = port;
            break;
        }
    }

    if (selectedPort.isNull()) {
        emit message("Устройство не найдено.");
        return;
    }

    // Задаем порт подключения
    port->setPort(selectedPort);

    // Попытка открыть порт
    if (!port->open(QIODevice::ReadWrite)) {
        emit message("Не удалось открыть порт.");
    }else{
        emit message("Порт " + port->portName() + " открыт.");
    }
}

void SerialPort::findAvaliblePorts()
{
    // Поиск доступных последовательных портов
    QVector<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    if (portList.isEmpty()) {
        emit message("Нет доступных портов.");
    }

    for(const QSerialPortInfo &port : portList){
        emit message("Имя порта: " + port.portName() + " Описание: " + port.description());
    }
}

void SerialPort::readCommand(int address)
{
    QByteArray message;
    message.append(address); // адрес
    message.append(0x49);    // команда чтения

    sendRequest(message);
}

void SerialPort::turnOffCommand(int address)
{
    QByteArray message;
    message.append(address); // адрес
    message.append(0x4F);    // команда выключения

    sendRequest(message);
}

void SerialPort::chargeCommand(int address, int voltage, int current)
{
    QByteArray message;
    message.append(address); // адрес
    message.append(0x43);    // команда зарядки
    message.append(voltage); // напряжение
    message.append(current); // ток

    sendRequest(message);
}

void SerialPort::dischargeCommand(int address, int shim)
{
    QByteArray message;
    message.append(address); // адрес
    message.append(0x4C);    // команда разрядки
    message.append(shim);    // ШИМ

    sendRequest(message);
}

void SerialPort::sendRequest(QByteArray request)
{
    port->write(request);
    emit message("Отправлен запрос: " + request.toHex() + " размером: " + QString::number(request.size()));
    dataRecieve(request.at(0));
}

void SerialPort::dataRecieve(int address)
{
    if(port->waitForReadyRead(timeoutSecs * 1000)){
        QByteArray data = port->readAll();

        if(data.size() != 5 || data.at(0) != address){ // Не верная контрольная сумма, или не соответствует адрес
            emit message("Не удалось прочитать ответ.");
            return;
        }
        // Считываем ответ в формате: 1 бит - адрес, 2, 3 - напряжение, 4, 5 - ток
        emit message("Батарея No: "  + QString::number(address) +
                     " напряжение: " + QString::number((quint8)data[1] << 8 | (quint8)data[2]) +
                     " ток: "        + QString::number((quint8)data[3] << 8 | (quint8)data[4]));
    }else{
        emit message("Превышено время ожидания ответа.");
    }
}
