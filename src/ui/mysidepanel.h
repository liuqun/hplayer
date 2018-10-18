#pragma once

#include <QPushButton>
#include <QWidget>
#include "myserialportsettingswidget.h"
#include "myplaintextedit.h"

class MySidePanel : public QWidget
{
    Q_OBJECT

public:
    explicit MySidePanel(QWidget *parent = nullptr);
    ~MySidePanel();

private slots:
    void togggleSerialPort(bool);
    void openSerialPort();
    void closeSerialPort();
    void onSeiralPortDataReceived();

private:
    MySerialPortSettingsWidget *m_settingsWidget;
    QPushButton *m_toggleButton;
    MyPlainTextEdit *m_plainTextEdit;
    QSerialPort *m_serial;
};
