#ifndef HOPTRACUUHOCSINH_H
#define HOPTRACUUHOCSINH_H

#include <QGroupBox>

QT_BEGIN_NAMESPACE
class QSqlQueryModel;
class QSqlTableModel;
QT_END_NAMESPACE

namespace Ui {
    class hoptracuuhocsinh;
}

class hoptracuuhocsinh : public QGroupBox
{
    Q_OBJECT

public:
    explicit hoptracuuhocsinh(QWidget *parent = nullptr);
    ~hoptracuuhocsinh();

signals:
    void lookupPerformed(const int resultId);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::hoptracuuhocsinh *ui;

    QSqlQueryModel *schoolYearModel = nullptr;
    QSqlTableModel *classModel      = nullptr;
    QSqlQueryModel *studentModel    = nullptr;

    void onSchoolYearChanged(const int &index);
    void onClassChanged(const int &index);
    bool checkValidInputs();
    void find();
};

#endif // HOPTRACUUHOCSINH_H
