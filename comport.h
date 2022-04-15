#ifndef COMPORT_H
#define COMPORT_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QObject>

struct Settings {
    QString name;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
    bool localEchoEnabled;
};

class ComPort : public QThread
{
     Q_OBJECT
public:
    ComPort();
    void run();
    bool OpenSerial();
    QSerialPort* serial;
    Settings Setting;
signals:
       void OutData(QByteArray* buf);
       void MessageInGUI(QString str);
};

#endif // COMPORT_H
