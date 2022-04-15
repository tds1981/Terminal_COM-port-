#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "comport.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_NamePort_currentIndexChanged(int index);

    void ResiveData(QByteArray* buf);

    void on_ClearButton_clicked();

    void on_SendButton_clicked();

    void on_ConnectButton_clicked();

    void on_DisConnect_clicked();
    void handleError(QSerialPort::SerialPortError error);
    void MessageComPort(QString str);
private:
    Ui::MainWindow *ui;
    ComPort *ComP;
};

#endif // MAINWINDOW_H
