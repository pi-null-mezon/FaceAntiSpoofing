#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QContextMenuEvent>

#include "qtcpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

private slots:
    void on_actionConnect_triggered();
    void decodeImage(const QByteArray &_encimage);

    void on_actionQuit_triggered();

protected:
    void contextMenuEvent(QContextMenuEvent *_event);

private slots:
    void __updateViewForSession();
    void __setDefaultView();

private:
    void __loadSettings();    
    Ui::MainWindow *ui;
    QTCPClient qtcpclient;
    QSettings *settings;
};

#endif // MAINWINDOW_H
