#ifndef THONGKEDIEM_H
#define THONGKEDIEM_H

#include <QMainWindow>

namespace Ui {
class thongkediem;
}

class thongkediem : public QMainWindow
{
    Q_OBJECT

public:
    explicit thongkediem(QWidget *parent = nullptr);
    ~thongkediem();

private:
    Ui::thongkediem *ui;
};

#endif // THONGKEDIEM_H
