#ifndef QUANLYGIAOVIEN_H
#define QUANLYGIAOVIEN_H

#include <QMainWindow>
#include <QSqlRecord>

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
    QSqlRecord recordToInsert;

    bool checkValidInputs(const bool adding);
    void onAddRow();
    void onEditCurrentRow();
    void onDeleteCurrentRow();
};

#endif // QUANLYGIAOVIEN_H
