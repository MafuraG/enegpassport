#ifndef FRAGMENTTABLEMODEL_H
#define FRAGMENTTABLEMODEL_H

#include "fragment.h"

#include <QAbstractTableModel>



class FragmentTableModel : public QAbstractTableModel
{    
    Q_OBJECT

public:
    FragmentTableModel();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    Fragment* getFragment(const int row);

    QList<Fragment *> getFragmentList() const;

    void newFragment();
    void removeFragment(const int row);
private:
    Fragment *getItem(const QModelIndex &index) const;
    QList<Fragment*> m_fragmentList;
};

#endif // FRAGMENTTABLEMODEL_H
