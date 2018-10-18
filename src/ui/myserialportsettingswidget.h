#pragma once

#include <QWidget>
#include <QSerialPort>

namespace Ui {
class SerialPortSettingsWidget;
}

class MySerialPortSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MySerialPortSettingsWidget(QWidget *parent = 0);
    ~MySerialPortSettingsWidget();
public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
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
    Settings settings() const;

private:
    Ui::SerialPortSettingsWidget *ui;
    Settings m_currentSettings;
};
