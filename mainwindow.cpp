#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ComP = new ComPort;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
                 ui->NamePort->addItem(info.portName());

    ui->baudRateBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));
    ui->baudRateBox->setCurrentIndex(5);

    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    //ui->dataBitsBox->setCurrentIndex(0);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);
    //ui->dataBitsBox->setCurrentIndex(0);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

    ui->statusBar->showMessage("Выберите COM порт и нажмите кнопку Подключить ");

    connect(ComP, SIGNAL(OutData(QByteArray*)), this, SLOT(ResiveData(QByteArray*)));
    connect(ComP->serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    connect(ComP, SIGNAL(MessageInGUI(QString)), this, SLOT(MessageComPort(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ResiveData(QByteArray* buf)
{
    ui->textBrowser->insertPlainText(*buf);

    QScrollBar *bar = ui->textBrowser->verticalScrollBar();
    bar->setValue(bar->maximum());

    delete buf;
}

void MainWindow::on_pushButton_clicked()
{
    ui->NamePort->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
                 ui->NamePort->addItem(info.portName());
}

void MainWindow::on_NamePort_currentIndexChanged(int index)
{
   QString str =  ui->NamePort->itemText(index)+" : ";
   QSerialPortInfo info = QSerialPortInfo(ui->NamePort->itemText(index));

   str = str + "  Vendor ID ="+ QString::number(info.vendorIdentifier());
   str = str + "  Product ID ="+ QString::number(info.productIdentifier());
  // str = str + "  Описание: " + info.description();
   str = str + "  Serial № " + info.serialNumber();
   str = str + "  Manufacturer ="+ info.manufacturer();

   ui->statusBar->showMessage(str);
   ui->label_4->setText("Описание порта: "+info.description()) ;
}


void MainWindow::on_ClearButton_clicked()
{
    ui->DataInPort->clear();
    ui->textBrowser->clear();
}

void MainWindow::on_SendButton_clicked()
{
    QString str = ui->DataInPort->toPlainText();
    ComP->serial->write(str.toUtf8());
}

void MainWindow::on_ConnectButton_clicked()
{
    ComP->Setting.name = ui->NamePort->currentText();
    ComP->Setting.baudRate = static_cast<QSerialPort::BaudRate>(ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    ComP->Setting.dataBits = static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    ComP->Setting.parity = static_cast<QSerialPort::Parity>(ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    ComP->Setting.stopBits = static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    ComP->Setting.flowControl = static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());

    if (!ComP->isRunning()) ComP->start(QThread::NormalPriority);
    if (ComP->isRunning())
    {
        ui->DisConnect->setEnabled(true);
        ui->ConnectButton->setEnabled(false);
        if (ComP->serial->isOpen()) ui->textBrowser->append("Начат приём:");
    }
}

void MainWindow::on_DisConnect_clicked()
{
    ComP->terminate();
    ui->DisConnect->setEnabled(false);
    ui->ConnectButton->setEnabled(true);

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Ошибка! Потеряна связь с устройством"), ComP->serial->errorString());
        ComP->serial->close();
    }
}

 void MainWindow::MessageComPort(QString str)
 {
    QMessageBox::information(0,"Сообщение от порта", str);
 }
