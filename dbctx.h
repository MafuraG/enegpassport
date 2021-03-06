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



class Dbctx
{
public:
    Dbctx();
    Dbctx(const QString &dbtype, const QString &dbname);
    ~Dbctx();

    void getPakazateli(QList<Entity *> &pakazatelList, const QStringList filter);

    void insertPakazatel(const Pakazatel *p);
    Pakazatel *getPakazatelByName(const QString name);
    Pakazatel *getPakazatelByID(const unsigned int id);

    QSqlRelationalTableModel *getPakazatelModel();
    void initPakazatelModel();
    void startTransaction();
    void endTransaction();

private:
    QSqlDatabase db;
    QSqlQuery query;

    void buildSelectQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &filter);
    void buildString(QStringList &result, const QStringList &fragments, const QString &separator);
    void buildFilter(QString &q,  const QString &oper, const QStringList &params);
    void buildInsertQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &values);

    Entity* getEntity(QHash<int, Entity *> cache, const int id);
    void loadCache();


    QHash<int,Entity*> pakCache;

    void refreshCache(QHash<int, Entity *> &cache, QList<Entity *> &list);

    QSqlRelationalTableModel *m_pakazatelModel;


};

#endif // DBCTX_H
