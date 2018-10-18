#include "mainwindow.h"
#include "appdef.h"
#include "qtstyles.h"

IMPL_SINGLETON(MainWindow)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    status = NORMAL;
    initUI();
    initConnect();
}

MainWindow::~MainWindow()
{
    hlogd("~MainWindow");
}

struct MyItem {
	int id;
	int row;
	int col;
	const QString &label;
	const char *image;
};

void MainWindow::initMenu(){
    // Media
    QMenu *mediaMenu = menuBar()->addMenu(tr("&Media"));
    QToolBar *mediaToolbar = addToolBar(tr("&Media"));
    toolbars.push_back(mediaToolbar);

    QAction* actOpenNetwork = new QAction(QIcon(":/image/ip-camera.png"), tr("Open IP Camera..."));
    actOpenNetwork->setShortcut(QKeySequence("Ctrl+N"));
    connect(actOpenNetwork, &QAction::triggered, this, [=](){
        onOpenMedia(MEDIA_TYPE_NETWORK);
    });
    mediaMenu->addAction(actOpenNetwork);
    mediaToolbar->addAction(actOpenNetwork);

    QAction* actOpenFile = new QAction(QIcon(":/image/file.png"), tr("Open File..."));
    actOpenFile->setShortcut(QKeySequence("Ctrl+F"));
    connect(actOpenFile, &QAction::triggered, this, [=](){
        onOpenMedia(MEDIA_TYPE_FILE);
    });
    mediaMenu->addAction(actOpenFile);
    mediaToolbar->addAction(actOpenFile);

    QAction* actOpenCapture = new QAction(QIcon(":/image/usb-camera.png"), tr("Open USB Camera...")); // TODO: 清理代码, 移除USB摄像头菜单项和按钮
    // actOpenCapture->setShortcut(QKeySequence("Ctrl+C"));
    // connect(actOpenCapture, &QAction::triggered, this, [=](){
    //     onOpenMedia(MEDIA_TYPE_CAPTURE);
    // });
    // mediaMenu->addAction(actOpenCapture);
    // mediaToolbar->addAction(actOpenCapture);

    // View
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

#if WITH_MV_STYLE
    QToolBar *viewToolbar = addToolBar(tr("&View"));
    toolbars.push_back(viewToolbar);

    QAction *actMVS;
    QSignalMapper *smMVS = new QSignalMapper(this);

struct MyItem items[] = {
{MV_STYLE_1, 1, 1, *(new QString(tr("MV1"))), ":/image/style1.png"},
{MV_STYLE_2, 1, 2, *(new QString(tr("MV2"))), ":/image/style2.png"},
{MV_STYLE_4, 2, 2, *(new QString(tr("MV4"))), ":/image/style4.png"},
{MV_STYLE_9, 3, 3, *(new QString(tr("MV9"))), ":/image/style9.png"},
{MV_STYLE_16, 4, 4, *(new QString(tr("MV16"))), ":/image/style16.png"},
{MV_STYLE_25, 5, 5, *(new QString(tr("MV25"))), ":/image/style25.png"},
{MV_STYLE_36, 6, 6, *(new QString(tr("MV36"))), ":/image/style36.png"},
{MV_STYLE_49, 7, 7, *(new QString(tr("MV49"))), ":/image/style49.png"},
{MV_STYLE_64, 8, 8, *(new QString(tr("MV64"))), ":/image/style64.png"},
};
const int N = 4;//(const int) (sizeof(items)/sizeof(items[0]));


    for (MyItem *i = items; i < &(items[N]); i++) {
		actMVS = new QAction(QIcon(i->image), i->label, this);
		actMVS->setToolTip(i->label);
		smMVS->setMapping(actMVS, i->id);
		connect( actMVS, SIGNAL(triggered(bool)), smMVS, SLOT(map()) );
		viewMenu->addAction(actMVS);
		if (i->row * i->col <= 16) {
			viewToolbar->addAction(actMVS);
		}
	}


    connect( smMVS, SIGNAL(mapped(int)), this, SLOT(onMVStyleSelected(int)) );
#endif

    viewMenu->addSeparator();

    //actMvFullscreen = new QAction(QIcon(":/image/fullscreen.png"), tr("Fullscreen F11"), this);
    //actMvFullscreen->setCheckable(true);
    //actMvFullscreen->setChecked(false);
    //connect( actMvFullscreen, &QAction::triggered, this, &MainWindow::mv_fullscreen );
    //viewMenu->addAction(actMvFullscreen);
    //viewToolbar->addAction(actMvFullscreen);

    actFullscreen = new QAction(QIcon(":/image/fullscreen.png"), tr("Fullscreen F11"));
    actFullscreen->setCheckable(true);
    actFullscreen->setChecked(false);
    connect( actFullscreen, &QAction::triggered, this, &MainWindow::fullscreen );
    viewMenu->addAction(actFullscreen);
    viewToolbar->addAction(actFullscreen);

    actMenubar = new QAction(tr("Menubar F10"));
    actMenubar->setCheckable(true);
    actMenubar->setChecked(true);
    connect( actMenubar, &QAction::triggered, [=](bool check){
        menuBar()->setVisible(check);
    });
    viewMenu->addAction(actMenubar);

    QAction *actToolbar = new QAction(tr("Toolbar"));
    actToolbar->setCheckable(true);
    actToolbar->setChecked(true);
    connect( actToolbar, &QAction::triggered, [=](bool check){
        foreach(auto toolbar, toolbars)
            toolbar->setVisible(check);
    });
    viewMenu->addAction(actToolbar);

    QAction *actStatusbar = new QAction(tr("Statusbar"));
    actStatusbar->setCheckable(true);
    actStatusbar->setChecked(true);
    connect( actStatusbar, &QAction::triggered, [=](bool check){
        statusBar()->setVisible(check);
    });
    viewMenu->addAction(actStatusbar);

    // Help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, SLOT(about()));
}

void MainWindow::initUI(){
    setWindowIcon(QIcon(":/image/icon.png"));
    setBaseSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    centerWidget(this);

    initMenu();

    center = new CentralWidget;
    setCentralWidget(center);

    statusBar()->showMessage(tr("No Message!"));
}

void MainWindow::initConnect(){
    connect(this, SIGNAL(reqPlay(HMedia&)), center->mv, SLOT(play(HMedia&)));
}

void MainWindow::fullscreen(){
    static QRect rcOld;
    if (isFullScreen()){
        menuBar()->setVisible(true);
        showNormal();
        setGeometry(rcOld);
        status = NORMAL;
    }else{
        rcOld = geometry();
        menuBar()->setVisible(false);
        showFullScreen();
        status = FULLSCREEN;
    }
    actFullscreen->setChecked(isFullScreen());
    actMenubar->setChecked(menuBar()->isVisible());
}

void MainWindow::mv_fullscreen(){
//    HMultiView* mv = center->mv;
//    if (mv->windowType() & Qt::Window){
//        mv->setWindowFlags(Qt::SubWindow);
//        this->show();
//        status = NORMAL;
//    }else{
//        mv->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//        this->hide();
//        mv->showFullScreen();
//        mv->raise();
//        status = MV_FULLSCREEN;
//    }
//    this->grabKeyboard();
//
//    actMvFullscreen->setChecked(status == MV_FULLSCREEN);
}

void MainWindow::keyPressEvent(QKeyEvent* e){
    switch (e->key()) {
    case Qt::Key_F10:
        toggle(menuBar());
        actMenubar->setChecked(menuBar()->isVisible());
        return;
    case Qt::Key_Escape:
        if (status != MV_FULLSCREEN) {
            return;
        }
        /* Fallthough */
    case Qt::Key_F11:
        fullscreen();
        return;
    default:
        QMainWindow::keyPressEvent(e);
        return;
    }
}

void MainWindow::about(){
    QString strAbout = APP_NAME " " APP_VERSION "\n\n";

    strAbout += tr("Build date");
    strAbout += QString::asprintf(" %s %s\n\n", __DATE__, __TIME__);

    strAbout += QString::asprintf("Copyright 2018 %s.\n", COMPANY_NAME);
    strAbout += "All rights reserved.\n";

    QMessageBox::information(this, tr("About Application"), strAbout);
}

void MainWindow::onMVStyleSelected(int id){
    int r,c;
    switch (id) {
    case MV_STYLE_2:
        r = 1;
        c = 2;
        break;
    case MV_STYLE_4:
        r = c = 2;
        break;
    case MV_STYLE_9:
        r = c = 3;
        break;
    case MV_STYLE_16:
        r = c = 4;
        break;
    case MV_STYLE_25:
        r = c = 5;
        break;
    case MV_STYLE_36:
        r = c = 6;
        break;
    case MV_STYLE_49:
        r = c = 7;
        break;
    case MV_STYLE_64:
        r = c = 8;
        break;
    case MV_STYLE_1:
    default:
        r = c = 1;
        break;
    }
    center->mv->setLayout(r,c);
}

#include "hopenmediadlg.h"
void MainWindow::onOpenMedia(int index){
    HOpenMediaDlg dlg(this);
    dlg.tab->setCurrentIndex(index);
    if (dlg.exec() == QDialog::Accepted){
        emit reqPlay(dlg.media);
    }
}
