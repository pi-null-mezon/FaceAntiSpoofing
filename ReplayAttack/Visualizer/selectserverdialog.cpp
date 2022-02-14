#include "selectserverdialog.h"
#include "ui_selectserverdialog.h"

SelectServerDialog::SelectServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectServerDialog)
{
    if(parent) {
        setPalette(parent->palette());
        setFont(parent->font());
    }
    ui->setupUi(this);
    setWindowTitle(APP_NAME);    
}

SelectServerDialog::~SelectServerDialog()
{
    delete ui;
}

QString SelectServerDialog::getHostAddr() const
{
    return ui->addrLE->text();
}

void SelectServerDialog::setHostAddr(const QString &_name)
{
    ui->addrLE->setText(_name);
}

int SelectServerDialog::getHostPort() const
{
    return ui->portSB->value();
}

void SelectServerDialog::setHostPort(int _port)
{
    ui->portSB->setValue(_port);
}

void SelectServerDialog::on_pushButton_2_clicked()
{
    reject();
}

void SelectServerDialog::on_pushButton_clicked()
{
    accept();
}
