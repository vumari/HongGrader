#ifndef QUANLYGIAOVIEN_H
#define QUANLYGIAOVIEN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QDataWidgetMapper;
QT_END_NAMESPACE

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
    QSqlRelationalTableModel *model = nullptr;
    QDataWidgetMapper *mapper       = nullptr;

    bool checkValidInputs();
    void onAddRow();
    void onEditCurrentRow();
    void onDeleteCurrentRow();
};

#endif // QUANLYGIAOVIEN_H
