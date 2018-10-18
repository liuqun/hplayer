#include <QComboBox>
#include <QSerialPortInfo>
#include "myserialportsettingswidget.h"
#include "ui_serialportsettingswidget.h"

static const char blankString[] = QT_TRANSLATE_NOOP("MySerialPortSettingsWidget", "N/A");

MySerialPortSettingsWidget::MySerialPortSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortSettingsWidget)
{
    ui->setupUi(this);
    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);
    fillPortsParameters();
    updateAllSettings();
    ui->dataBitsBox->setEnabled(false);  // FIXME: Editable serial port options is not finished yet
    ui->parityBox->setEnabled(false); // FIXME: Editable serial port options is not finished yet
    ui->stopBitsBox->setEnabled(false);  // FIXME: Editable serial port options is not finished yet
    ui->flowControlBox->setEnabled(false);  // FIXME: Editable serial port options is not finished yet

    connect(ui->serialPortInfoListBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MySerialPortSettingsWidget::updatePortName);
    connect(ui->baudRateBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MySerialPortSettingsWidget::updateBaudRateValue);
}

MySerialPortSettingsWidget::~MySerialPortSettingsWidget()
{
    delete ui;
}

MySerialPortSettingsWidget::Settings MySerialPortSettingsWidget::settings() const
{
    return m_currentSettings;
}

void MySerialPortSettingsWidget::fillPortsParameters()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }

    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    //ui->baudRateBox->addItem(tr("Custom"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void MySerialPortSettingsWidget::updatePortName()
{
    m_currentSettings.name = ui->serialPortInfoListBox->currentText();
}

void MySerialPortSettingsWidget::updateBaudRateValue()
{
    if (ui->baudRateBox->currentIndex() == 4) {
        m_currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        m_currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);
}

void MySerialPortSettingsWidget::updateAllSettings()
{
    updatePortName();
    updateBaudRateValue();

    m_currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    m_currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    m_currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    m_currentSettings.stringParity = ui->parityBox->currentText();

    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    m_currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    m_currentSettings.stringFlowControl = ui->flowControlBox->currentText();
}
