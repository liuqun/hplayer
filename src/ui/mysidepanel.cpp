#include <QBoxLayout.h>
#include <QMessageBox>
#include <QPushButton>
#include "mysidepanel.h"
#include "myserialportsettingswidget.h"

MySidePanel::MySidePanel(QWidget *parent) :
    QWidget(parent), m_settingsWidget(new MySerialPortSettingsWidget)
    , m_toggleButton(new QPushButton(tr("Switch On/Off")))
    , m_plainTextEdit(new MyPlainTextEdit(this))
    , m_serial(new QSerialPort(this))
{
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_settingsWidget, 0, Qt::AlignTop);
    vbox->addWidget(m_toggleButton, 0, Qt::AlignCenter);
    vbox->addWidget(m_plainTextEdit);
    setLayout(vbox);
    m_toggleButton->setCheckable(true);
    m_toggleButton->setChecked(false);
    m_toggleButton->setFixedSize(150,28);
    m_toggleButton->setStyleSheet("QPushButton{background-color:#550000}"
                                 "QPushButton:checked{background-color:green}");
    connect(m_toggleButton,
            &QPushButton::toggled,this,&MySidePanel::togggleSerialPort);
    connect(m_serial, &QSerialPort::readyRead, this, &MySidePanel::onSeiralPortDataReceived);
}

MySidePanel::~MySidePanel()
{
    delete m_settingsWidget;
    delete m_toggleButton;
    delete m_plainTextEdit;
    delete m_serial;
}

void MySidePanel::togggleSerialPort(bool open)
{
    if (open) {
        openSerialPort();
        return;
    }
    closeSerialPort();
}

void MySidePanel::openSerialPort()
{
    const MySerialPortSettingsWidget::Settings p = m_settingsWidget->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (!m_serial->open(QIODevice::ReadWrite)) {
        m_toggleButton->setChecked(false);
        m_settingsWidget->setEnabled(true);
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
    }
    m_settingsWidget->setEnabled(false);
    //showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
    //    .arg(p.name)
    //    .arg(p.stringBaudRate)
    //    .arg(p.stringDataBits)
    //    .arg(p.stringParity)
    //    .arg(p.stringStopBits)
    //    .arg(p.stringFlowControl)
    //);
}

void MySidePanel::closeSerialPort()
{
    if (m_serial->isOpen()) {
        m_serial->close();
    }
    m_toggleButton->setChecked(false);
    m_settingsWidget->setEnabled(true);
}

void MySidePanel::onSeiralPortDataReceived()
{
    const QByteArray data = m_serial->readAll();
    m_plainTextEdit->clear();
    m_plainTextEdit->putGBK(data);
}
