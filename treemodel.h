#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>



class TreeModel : public QAbstractItemModel
{
public:
    explicit TreeModel(const QString &data, QObject *parent = 0);
    ~TreeModel();

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    void setupModelData(const QStringList &lines,TreeItem*parent);

    TreeItem *rootItem;
};

#endif // TREEMODEL_H
