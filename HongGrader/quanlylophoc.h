#ifndef QUANLYLOPHOC_H
#define QUANLYLOPHOC_H

#include <QMainWindow>

namespace Ui {
class quanlylophoc;
}

class quanlylophoc : public QMainWindow
{
    Q_OBJECT

public:
    explicit quanlylophoc(QWidget *parent = nullptr);
    ~quanlylophoc();

private:
    Ui::quanlylophoc *ui;
};

#endif // QUANLYLOPHOC_H
