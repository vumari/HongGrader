#ifndef QUANLYGIAOVIEN_H
#define QUANLYGIAOVIEN_H

#include <QMainWindow>

namespace Ui {
class quanlygiaovien;
}

class quanlygiaovien : public QMainWindow
{
    Q_OBJECT

public:
    explicit quanlygiaovien(QWidget *parent = nullptr);
    ~quanlygiaovien();

private:
    Ui::quanlygiaovien *ui;
};

#endif // QUANLYGIAOVIEN_H
