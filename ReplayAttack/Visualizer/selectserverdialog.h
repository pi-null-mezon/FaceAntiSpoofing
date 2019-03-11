#ifndef SELECTSERVERDIALOG_H
#define SELECTSERVERDIALOG_H

#include <QDialog>

namespace Ui {
class SelectServerDialog;
}

class SelectServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectServerDialog(QWidget *parent = nullptr);
    ~SelectServerDialog();

    QString getHostAddr() const;
    void    setHostAddr(const QString &_name);
    int  getHostPort() const;
    void setHostPort(int _port);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SelectServerDialog *ui;
};

#endif // SELECTSERVERDIALOG_H
