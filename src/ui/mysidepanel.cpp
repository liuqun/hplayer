#include <QBoxLayout.h>
#include "mysidepanel.h"
#include "myserialportsettingswidget.h"

MySidePanel::MySidePanel(QWidget *parent) :
    QWidget(parent), m_settingsWidget(new MySerialPortSettingsWidget)
{
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_settingsWidget, 0, Qt::AlignTop);
    setLayout(vbox);
}

MySidePanel::~MySidePanel()
{
    delete m_settingsWidget;
}
