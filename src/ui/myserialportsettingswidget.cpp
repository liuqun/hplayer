#include "myserialportsettingswidget.h"
#include "ui_serialportsettingswidget.h"

MySerialPortSettingsWidget::MySerialPortSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortSettingsWidget)
{
    ui->setupUi(this);
}

MySerialPortSettingsWidget::~MySerialPortSettingsWidget()
{
    delete ui;
}

MySerialPortSettingsWidget::Settings MySerialPortSettingsWidget::settings() const
{
    return m_currentSettings;
}
