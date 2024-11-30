#ifndef QUANLYNAMHOC_H
#define QUANLYNAMHOC_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSqlTableModel;
class QDataWidgetMapper;
QT_END_NAMESPACE

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

    QSqlTableModel *model     = nullptr;
    QDataWidgetMapper *mapper = nullptr;

    bool checkValidInputs();
    void onEditCurrentRow();
    void onAddSchoolYear();
    void onDeleteSchoolYear();
};

#endif // QUANLYNAMHOC_H
