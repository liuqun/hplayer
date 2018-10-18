#pragma once

#include <QWidget>
#include "myserialportsettingswidget.h"

class MySidePanel : public QWidget
{
    Q_OBJECT

public:
    explicit MySidePanel(QWidget *parent = nullptr);
    ~MySidePanel();

private:
    MySerialPortSettingsWidget *m_settingsWidget;
};
