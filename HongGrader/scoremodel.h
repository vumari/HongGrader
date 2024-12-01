#ifndef SCOREMODEL_H
#define SCOREMODEL_H

#include <QSqlQueryModel>
#include <QSqlDatabase>

class ScoreModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ScoreModel(QObject *parent = nullptr);
    ~ScoreModel() override;

    QSqlDatabase database() const;
    void select();
    void appendRow(const int studentId, const QString &subjectId,
                   const int &termId, const QString &schoolYear);
    bool deleteRow(const int studentId, const QString &subjectId,
                   const int &termId, const QString &schoolYear,
                   bool refresh = true);
    bool submitAll();

    void applyHeaders();

    int rowCount(const QModelIndex &parent    = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

private:
    QSqlDatabase db;
    QMap<QPersistentModelIndex, QVariant> changedCells;
    QWidget *parentWidget = nullptr;

    bool updateItem(const int studentId, const QString &subjectId,
                    const int &termId, const QString &schoolYear,
                    const QPersistentModelIndex &index, const QVariant &value);
    void updateAvgScore(const QModelIndex &index);
};

#endif // SCOREMODEL_H
