#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "selectserverdialog.h"

#include <QStandardPaths>
#include <QTimer>
#include <QMenu>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
    setWindowTitle(APP_NAME);
    __loadSettings();

    connect(&qtcpclient,SIGNAL(imageRecieved(QByteArray)),this,SLOT(decodeImage(QByteArray)));
    connect(&qtcpclient,SIGNAL(startSessionCommand()),this,SLOT(__updateViewForSession()));
    connect(&qtcpclient,SIGNAL(endSessionCommand()),this,SLOT(__setDefaultView()));

    __setDefaultView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
    SelectServerDialog _dialog(this);
    _dialog.setHostAddr(settings->value("HostAddr","127.0.0.1").toString());
    _dialog.setHostPort(settings->value("HostPort",8000).toInt());
    if(_dialog.exec() == QDialog::Accepted) {
        qtcpclient.setHostAddr(QHostAddress(_dialog.getHostAddr()),_dialog.getHostPort());
        QTimer::singleShot(1000,&qtcpclient,SLOT(connectToHost()));
        settings->setValue("HostAddr",_dialog.getHostAddr());
        settings->setValue("HostPort",_dialog.getHostPort());
    }
}

void MainWindow::decodeImage(const QByteArray &_encimage)
{
    QImage qimg = QImage::fromData(_encimage);
    if(qimg.isNull()) {
        qtcpclient.notifyAboutInvalidImage();
        ui->centralWidget->updateInfo("Null image");
    } else {
        ui->centralWidget->updateImage(QImage::fromData(_encimage));
        QTimer::singleShot(300,&qtcpclient,SLOT(notifyAboutImageUpdate()));
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *_event)
{
    QMenu menu(this);
    menu.addAction(ui->actionConnect);
    menu.addAction(ui->actionQuit);
    menu.exec(_event->globalPos());
}

void MainWindow::__loadSettings()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/%1").arg(APP_NAME));
    if(!dir.exists())
        dir.mkpath(dir.absolutePath());
    settings = new QSettings(dir.absolutePath().append("/%1.ini").arg(APP_NAME),QSettings::IniFormat,this);
}

void MainWindow::__updateViewForSession()
{
    ui->toolBar->setVisible(false);
}

void MainWindow::__setDefaultView()
{
    ui->toolBar->setVisible(true);
    ui->centralWidget->updateImage(QImage());
    ui->centralWidget->updateInfo("Connect to\nserver\nfor start");
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
