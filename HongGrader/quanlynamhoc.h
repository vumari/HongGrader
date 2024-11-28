#ifndef QUANLYNAMHOC_H
#define QUANLYNAMHOC_H

#include <QMainWindow>

namespace Ui {
class quanlynamhoc;
}

class quanlynamhoc : public QMainWindow
{
    Q_OBJECT

public:
    explicit quanlynamhoc(QWidget *parent = nullptr);
    ~quanlynamhoc();

private:
    Ui::quanlynamhoc *ui;
};

#endif // QUANLYNAMHOC_H
