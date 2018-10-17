QT += widgets serialport
requires(qtConfig(combobox))

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    myplaintextedit.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    myplaintextedit.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    dialog.ui

RESOURCES += \
    terminal.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialport/terminal
INSTALLS += target
