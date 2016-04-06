#include "fragmenttablemodel.h"
#include <QDebug>

FragmentTableModel::FragmentTableModel()
{   
    insertRow(rowCount());
}

int FragmentTableModel::rowCount(const QModelIndex &parent) const
{
    return m_fragmentList.count();
}

int FragmentTableModel::columnCount(const QModelIndex &parent) const
{
    return Fragment::ColumnCount;
}

QVariant FragmentTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    Fragment * item = getItem(index);

    return item->data(index.column());

}

bool FragmentTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    Fragment * item = getItem(index);
    bool result = item->setData(index.column(), value);

//    qDebug()<< "Setting data row:" << index.row()<<" col:"<<index.column()<<" val:"<<value
//            <<" result:"<<result<<" \n";
    if (result)
        emit dataChanged(index, index);

    return result;
}

QVariant FragmentTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return Fragment::header(section);

    return QVariant();
}

bool FragmentTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    bool success = true;

    beginInsertRows(parent, row, row + count - 1);
    for(int i = row; i < row + count; i++){
        Fragment *f = new Fragment();
        m_fragmentList.insert(i,f);
        //qDebug()<<"row inserted row:"<<i;
    }
    endInsertRows();

    return success;
}

bool FragmentTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool success = true;

    beginRemoveRows(parent, row, row + count - 1);
    for(int i = row; i < row + count; i++){
        delete m_fragmentList[row];
        m_fragmentList.removeAt(row);
    }
    endRemoveRows();

    return success;
}

Qt::ItemFlags FragmentTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void FragmentTableModel::newFragment()
{
    insertRows(rowCount(),1);
}

void FragmentTableModel::removeFragment(const int row)
{
    removeRows(row,1);
}


Fragment *FragmentTableModel::getFragment(const int row)
{
    if (row >=0 && row < rowCount())
        return m_fragmentList[row];

    return nullptr;
}

Fragment *FragmentTableModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        Fragment *item = m_fragmentList[index.row()];
        if (item)
            return item;
    }
    return new Fragment();
}

QList<Fragment *> FragmentTableModel::getFragmentList() const
{
    return m_fragmentList;
}
