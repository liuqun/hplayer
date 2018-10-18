#pragma once

#include <QWidget>
#include "myserialportsettingswidget.h"

class MySidePanel : public QWidget
{
    Q_OBJECT

//signals:
//    void getData(const QByteArray &data);

public:
    explicit MySidePanel(QWidget *parent = nullptr);
    ~MySidePanel();

//protected:
//    void keyPressEvent(QKeyEvent *e) override;
//    void mousePressEvent(QMouseEvent *e) override;
//    void mouseDoubleClickEvent(QMouseEvent *e) override;
//    void contextMenuEvent(QContextMenuEvent *e) override;

private:
    MySerialPortSettingsWidget *m_settingsWidget;
};
