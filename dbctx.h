#ifndef DBCTX_H
#define DBCTX_H

#include "fragment.h"
#include "pakazatel.h"
#include "section.h"

#include <QHash>
#include <QList>
#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <QSqlRelationalTableModel>



class dbctx
{
public:
    dbctx();
    dbctx(const QString &dbtype, const QString &dbname);
    ~dbctx();
    void getSections(QList<Entity *> &sectionList, const QStringList filter);
    void getFragments(QList<Entity *> &fragmentList, const QStringList filter);
    void getPakazateli(QList<Entity *> &pakazatelList, const QStringList filter);

    Entity* getEntity(QHash<int, Entity *> cache, const int id);

    QSqlRelationalTableModel *getFragmentModel() const;
    void setFragmentModel(QSqlRelationalTableModel *fragmentModel);

private:
    QSqlDatabase db;
    QSqlQuery query;

    void buildSelectQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &filter);
    void buildString(QStringList &result, const QStringList &fragments, const QString &separator);
    void buildFilter(QString &q,  const QString &oper, const QStringList &params);
    void buildInsertQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &values);

    QHash<int,Entity*> sectCache;
    QHash<int,Entity*> fragCache;
    QHash<int,Entity*> pakCache;

    void refreshCache(QHash<int,Entity*> cache, QList<Entity *> &list);
    QSqlRelationalTableModel *m_fragmentModel;


};

#endif // DBCTX_H
