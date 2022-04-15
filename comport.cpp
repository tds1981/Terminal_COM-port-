#include "comport.h"

ComPort::ComPort()
{
    serial = new QSerialPort();
}

void ComPort::run()
{
    if (!OpenSerial()) emit MessageInGUI("Ошибка! Порт не открыт. Возможно не подкючён кабель.");
    QByteArray *Buf;
    while (serial->isOpen())
    {
       serial->waitForReadyRead(1);
       qint64 maxSize = serial->bytesAvailable();
       if (maxSize > 3)
       {
            Buf = new QByteArray;
            *Buf = serial->readAll();
            emit OutData(Buf);
       }
    }
}

bool ComPort::OpenSerial()
{
    if (serial->isOpen()) serial->close();
    serial->clear();
    serial->setPortName(Setting.name);
    serial->setBaudRate(Setting.baudRate);
    serial->setDataBits(Setting.dataBits);
    serial->setParity(Setting.parity);
    serial->setStopBits(Setting.stopBits);
    serial->setFlowControl(Setting.flowControl);
    serial->open(QSerialPort::ReadWrite); //ReadWrite
    serial->clear();
    return serial->isOpen();
}
