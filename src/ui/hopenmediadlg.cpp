#include "hopenmediadlg.h"
#include "hdevice.h"

FileTab::FileTab(QWidget *parent) : QWidget(parent){
    QVBoxLayout* vbox = new QVBoxLayout;

    vbox->addStretch();
    vbox->addWidget(new QLabel(tr("File:")));

    QHBoxLayout* hbox = new QHBoxLayout;
    edit = new QLineEdit;
    hbox->addWidget(edit);
    btnBrowse = new QPushButton("...");
    connect(btnBrowse, &QPushButton::clicked, this, [=](){
        QString file = QFileDialog::getOpenFileName(this, tr("Open Meida File"), "",
            "Video Files(*.3gp *.amv *.asf *.avi *.flv *.m2v *.m4v *.mkv *.mp2 *.mp4 *.mpg *.swf *.ts *.rmvb *.wmv)\n"
            "All Files(*)");
        if (!file.isEmpty()){
            edit->setText(file);
        }
    });
    hbox->addWidget(btnBrowse);

    vbox->addLayout(hbox);
    vbox->addStretch();

    setLayout(vbox);
}

NetworkTab::NetworkTab(QWidget *parent) : QWidget(parent){
    QVBoxLayout* vbox = new QVBoxLayout;

    vbox->addStretch();
    vbox->addWidget(new QLabel(tr("URL:")));

    edit = new QLineEdit;

    vbox->addWidget(edit);
    vbox->addStretch();

    setLayout(vbox);
}

CaptureTab::CaptureTab(QWidget *parent) : QWidget(parent){
    QVBoxLayout* vbox = new QVBoxLayout;

    vbox->addStretch();
    vbox->addWidget(new QLabel(tr("Device:")));

    cmb = new QComboBox;
    QStringList items;
    vector<HDevice> devs = getVideoDevices();
    for (int i = 0; i < devs.size(); ++i){
        items << devs[i].name;
    }
    cmb->addItems(items);

    vbox->addWidget(cmb);
    vbox->addStretch();

    setLayout(vbox);
}

HOpenMediaDlg::HOpenMediaDlg(QWidget *parent) : QDialog(parent)
{
    initUI();
    initConnect();
}

void HOpenMediaDlg::initUI(){
    setWindowTitle(tr("Open media"));
    setFixedSize(800, 200);

    QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Cancel);
    connect(btns, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);

    tab = new QTabWidget;
	mNetworkTab = new NetworkTab;

if (0){// TODO: 代码清理, 调试过程中强制改写成员变量media的初始值
    media.type = MEDIA_TYPE_NETWORK;
    media.src.assign("rtsp://192.168.1.10/user=admin&password=&channel=1&stream=0.sdp?real_stream");
}
    mNetworkTab->edit->setText("192.168.1.10");

    tab->addTab(mNetworkTab, QIcon(":/image/network.png"), tr("Network"));
    tab->addTab(new FileTab, QIcon(":/image/file.png"), tr("File"));
    const QString &tabNameUSBCamera = tr("USB Camera"); // TODO: 代码清理, 移除USB摄像头问题代码
    //tab->addTab(new CaptureTab, QIcon(":/image/usb-camera.png"), tabNameUSBCamera);

    tab->setCurrentIndex(DEFAULT_MEDIA_TYPE);

    QVBoxLayout* vbox = genVBoxLayout();
    vbox->addWidget(tab);
    vbox->addWidget(btns);

    setLayout(vbox);
}

void HOpenMediaDlg::initConnect(){

}

void HOpenMediaDlg::accept(){
    switch (tab->currentIndex()) {
    case MEDIA_TYPE_FILE:{
        FileTab* filetab = qobject_cast<FileTab*>(tab->currentWidget());
        if (filetab){
            media.type = MEDIA_TYPE_FILE;
            media.src  = qPrintable(filetab->edit->text());
        }
    }
        break;
    case MEDIA_TYPE_NETWORK:{
        NetworkTab* nettab = qobject_cast<NetworkTab*>(tab->currentWidget());
        if (nettab){
            media.type = MEDIA_TYPE_NETWORK;
            media.src.clear();
            media.src = "rtsp://";
            media.src += qPrintable(nettab->edit->text());
            media.src += "/user=admin&password=&channel=1&stream=0.sdp?real_stream";
        }
    }
        break;
    case MEDIA_TYPE_CAPTURE:{
        CaptureTab* captab = qobject_cast<CaptureTab*>(tab->currentWidget());
        if (captab){
            media.type = MEDIA_TYPE_CAPTURE;
            media.src  = qPrintable(captab->cmb->currentText());
            media.index = captab->cmb->currentIndex();
        }
    }
        break;
    default:
        break;
    }

    QDialog::accept();
}

const QString &HOpenMediaDlg::changeNetworkCameraIPAddress(const QString &strIP){
    // 例如 "rtsp://192.168.1.10/user=admin&password=&channel=1&stream=0.sdp?real_stream";
    mNetworkTab->edit->setText(strIP);
	media.type = MEDIA_TYPE_NETWORK;
	media.src = "rtsp://";
    media.src += qPrintable(strIP);
    media.src += "/user=admin&password=&channel=1&stream=0.sdp?real_stream";
	return (strIP);
}