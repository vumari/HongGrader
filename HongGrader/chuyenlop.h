#ifndef CHUYENLOP_H
#define CHUYENLOP_H

#include <QMainWindow>

namespace Ui {
class chuyenlop;
}

class chuyenlop : public QMainWindow
{
    Q_OBJECT

public:
    explicit chuyenlop(QWidget *parent = nullptr);
    ~chuyenlop();

private:
    Ui::chuyenlop *ui;
};

#endif // CHUYENLOP_H
