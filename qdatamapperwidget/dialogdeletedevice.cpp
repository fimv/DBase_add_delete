#include "dialogdeletedevice.h"
#include "ui_dialogdeletedevice.h"

DialogDeleteDevice::DialogDeleteDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDeleteDevice)
{
    ui->setupUi(this);
}

DialogDeleteDevice::~DialogDeleteDevice()
{
    delete ui;
}
