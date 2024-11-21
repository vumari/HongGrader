#ifndef DANGNHAP_H
#define DANGNHAP_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class dangnhap;
}
QT_END_NAMESPACE

class dangnhap : public QDialog
{
    Q_OBJECT

public:
    dangnhap(QWidget *parent = nullptr);
    ~dangnhap();

    QString username;

private slots:
    void on_btdangnhap_clicked();

private:
    Ui::dangnhap *ui;
};
#endif // DANGNHAP_H
