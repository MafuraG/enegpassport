#ifndef DBCTX_H
#define DBCTX_H

#include "fragment.h"
#include "pakazatel.h"
#include "section.h"

#include <QHash>
#include <QList>
#include <QSqlQuery>
#include <QString>



class dbctx
{
public:
    dbctx();
    dbctx(const QString &dbtype, const QString &dbname);
    ~dbctx();
    void getSections(QList<Section> &sectionList, const QStringList filter);
    void getFragments(QList<Fragment> &fragmentList,const QStringList filter);
    void getPakazateli(QList<Pakazatel> &PakazatelList,const QStringList filter);

    Entity getEntity(QHash<int, Entity> cache, const int id);

private:
    QSqlDatabase db;
    QSqlQuery query;

    void buildSelectQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &filter);
    void buildString(QStringList &result, const QStringList &fragments, const QString &separator);
    void buildFilter(QString &q,  const QString &oper, const QStringList &params);
    void buildInsertQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &values);

    QHash<int,Section> sectCache;
    QHash<int,Fragment> fragCache;
    QHash<int,Pakazatel> pakCache;

    void refreshSectCache(QList<Section> &sectionList);
    void refreshFragCache(QList<Fragment> &fragmentList);
    void refreshPakCache(QList<Pakazatel> &pakazatelList);
    void refreshCache(QHash<int,Entity> cache, QList<Entity> &list);


};

#endif // DBCTX_H
