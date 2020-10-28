#ifndef DIALOGDELETEDEVICE_H
#define DIALOGDELETEDEVICE_H

#include <QDialog>

namespace Ui {
class DialogDeleteDevice;
}

class DialogDeleteDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeleteDevice(QWidget *parent = nullptr);
    ~DialogDeleteDevice();

private:
    Ui::DialogDeleteDevice *ui;
};

#endif // DIALOGDELETEDEVICE_H
