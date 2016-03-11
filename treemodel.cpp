/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "treeitem.h"
#include "treemodel.h"
#include "QDebug"
#include <cmath>
#include <QLocale>
#include <algorithm>

TreeModel::TreeModel(const QStringList &headers, const QList<Entity*> &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new TreeItem(rootData);
    setupModelData(data, rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::ForegroundRole){
        TreeItem *item = getItem(index);
        if ( index.column() == 3 && item->data(5).toBool() == false)
        {
            return QVariant( QColor( Qt::red ) );
        }
        return QVariant( QColor( Qt::black ) );
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    //column 0 not editable
    if (index.column() == 0 )
        return QAbstractItemModel::flags(index);
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

void TreeModel::refreshCache(TreeItem * item)
{
    if (item != nullptr){
        QString key = item->data(0).toString();
        m_cache_name[key] = item;
        if (item->childCount() > 0 ){
            for (int i = 0 ; i < item->childCount(); i++){
                refreshCache(item->child(i));
            }
        }
    }
}

TreeItem *TreeModel::searchTree(TreeItem *tree, const unsigned int col, QVariant item){
    if (tree != nullptr || col >= tree->columnCount()){
        if (item == tree->data(col)) return tree;
        else{
            TreeItem *t = nullptr;
            for(int i = 0; i < tree->childCount();i++){
                t =  searchTree(tree->child(i),col,item);
                if (t != nullptr) return t;
            }
        }

    }
    return nullptr;
}

void TreeModel::mapTreeItemPakazatel(TreeItem *tree, Pakazatel *i)
{
    QVariant val;
    val = tree->data(0);
    i->setName(val.toString());
    val = tree->data(1);
    i->setUnit(val.toString());
    val = tree->data(2);
    i->setNomValue(val.toDouble());
    val = tree->data(3);
    i->setCalcValue(val.toDouble());
    val = tree->data(4);
    i->setFactValue(val.toDouble());    
    val = tree->data(5);
    i->setCalculated(val.toBool());
    val = tree->data(6);
    i->setNumeration(val.toUInt());
    val = tree->data(7);
    i->setId(val.toInt());


    if (tree->parent() != nullptr){
        Pakazatel *p = new Pakazatel();
        i->setParent(p);
        mapTreeItemPakazatel(tree->parent(),p);
    }
    else
        i->setParent(nullptr);
}

void TreeModel::mapPakazatelTreeItem(TreeItem *tree, Pakazatel *i)
{
    tree->setData(0,i->name());
    tree->setData(1,i->unit());
    tree->setData(2,i->nomValue());
    tree->setData(3,i->calcValue());
    tree->setData(4,i->factValue());
    tree->setData(5,i->calculated());
    tree->setData(6,i->numeration());
    tree->setData(7,i->id());

}

void TreeModel::mapChildren(QMultiHash<int, Entity *> &dict, TreeItem *parent, Pakazatel *p)
{
    QList<Entity*> children = dict.values(p->id());
    for (int i = 0; i < children.count();i++){
        Pakazatel* c = (Pakazatel*) children[i];
        parent->insertChildren(parent->childCount(),1,rootItem->columnCount());
        int r = parent->childCount() - 1;
        TreeItem *childItem = parent->child(r);
        mapPakazatelTreeItem(childItem,c);

        if (dict.contains(c->id())){
            mapChildren(dict,childItem,c);
        }
    }
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

Pakazatel* TreeModel::getIndicatorByName(const QString name)
{
    TreeItem *t = searchTree(rootItem,0,name);
    if (t != nullptr){
        Pakazatel *p = new Pakazatel();
        mapTreeItemPakazatel(t,p);
        return p;
    }

}

Pakazatel* TreeModel::getIndicatorByID(const unsigned id)
{
    //qDebug()<<"get indicator called id = "<<id;
    TreeItem *t = searchTree(rootItem,7,id);
    if (t != nullptr){
        Pakazatel *p = new Pakazatel();
        mapTreeItemPakazatel(t,p);
        //qDebug()<<"id = "<<id;
        return p;
    }
    return nullptr;

}

void TreeModel::setIndicatorByName(const QString name, Pakazatel *p)
{
    if (m_cache_name.contains(name)){
        TreeItem * item = m_cache_name.value(name);
        item->setData(3,p->calcValue());
        item->setData(5,p->calculated());
    }
}

void TreeModel::setIndicatorByID(Pakazatel *p)
{
    TreeItem * item = searchTree(rootItem,7,p->id());
    if (item != nullptr){
        item->setData(3,p->calcValue());
        //item->setData(5,p->calculated());
    }
}

void TreeModel::getIndicators(QList<TreeItem *> &items)
{
    items.clear();
    getIndicators(items,rootItem);
}

bool TreeModel::hasFraction(double number){
    double intpart;
    if (modf(number,&intpart) == 0){
        return false;
    }
    return true;
}

bool TreeModel::compareTreeItems(const TreeItem *t1, const TreeItem *t2)
{
    if (t1 == nullptr || t2 == nullptr) return false;
    unsigned int val1 = t1->data(6).toUInt();
    unsigned int val2 = t2->data(6).toUInt();

    return val1 < val2 ;

}

void TreeModel::getIndicators(QList<TreeItem *> &items, TreeItem *tree)
{
    //qDebug()<< "In getIndicators"<<"\n";
    //items.clear();
    if (tree != nullptr){
        items.append(tree);
        for(int i = 0; i < tree->childCount(); i++){
            getIndicators(items,tree->child(i));
        }
    }
}

void TreeModel::sortTree(TreeItem *t)
{
    if (t != nullptr){
        if (t->childCount() != 0)
        {
            std::sort(t->childBegin(),t->childEnd(),compareTreeItems);
            for(int i = 0 ; i < t->childCount(); i++){
                sortTree(t->child(i));
            }
        }
    }
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::setupModelData(const QList<Entity*> data, TreeItem *parent)
{
    QMultiHash<int,Entity*> parentChildren;

    //Create a multi hash of key  and children
    for (int i = 0 ; i < data.count(); i++){
        Pakazatel* p = (Pakazatel*)data[i];
        //search for children
        for (int j= 0; j < data.count(); j++){
            Pakazatel * c = (Pakazatel*) data[j];
            if (c->parent() != nullptr && p->id() == c->parent()->id())
            {
               parentChildren.insert(p->id(),c);
            }

        }
    }

    //find roots

    for (int i = 0 ; i < data.count(); i++){
        Pakazatel* p = (Pakazatel*)data[i];
        if (p->parent() == nullptr){            
            QVector<QVariant> d(rootItem->columnCount());            
            parent->insertChildren(parent->childCount(),1,rootItem->columnCount());
            TreeItem *t = parent->child(parent->childCount() - 1) ;
            mapPakazatelTreeItem(t,p);

            mapChildren(parentChildren,t,p);
        }
    }

    refreshCache(rootItem);
    sortTree(rootItem);
}
