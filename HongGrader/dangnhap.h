#ifndef DANGNHAP_H
#define DANGNHAP_H

#include <QMainWindow>
#include "quanlydiem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class dangnhap;
}
QT_END_NAMESPACE

class dangnhap : public QMainWindow
{
    Q_OBJECT

public:
    dangnhap(QWidget *parent = nullptr);
    ~dangnhap();

private slots:
    void on_btdangnhap_clicked();

private:
    Ui::dangnhap *ui;
    quanlydiem *formdiem;
};
#endif // DANGNHAP_H
