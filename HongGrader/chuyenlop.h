#ifndef CHUYENLOP_H
#define CHUYENLOP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QComboBox;
class QSqlQueryModel;
QT_END_NAMESPACE

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

    QSqlQueryModel *schoolYearModel = nullptr;
    QSqlQueryModel *fromClassModel  = nullptr;
    QSqlQueryModel *toClassModel    = nullptr;
    QSqlQueryModel *fromModel       = nullptr;
    QSqlQueryModel *toModel         = nullptr;

    void onOldSchoolYearChanged();
    void onNewSchoolYearChanged();
    void onSchoolYearChanged(QComboBox *comboBox, QSqlQueryModel *model);
    void loadOldClassTable();
    void loadNewClassTable();
    void loadClassTable(QSqlQueryModel *model,
                        QComboBox *schoolYearCombo,
                        QComboBox *classCombo);
    void copySelectedStudents();
};

#endif // CHUYENLOP_H
