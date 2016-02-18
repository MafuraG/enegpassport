#include "dbctx.h"

#include <QSqlQuery>
#include <QStringList>

dbctx::dbctx()
{

}

dbctx::dbctx(const QString &dbtype, const QString &dbname)
{
    db = QSqlDatabase::addDatabase(dbtype);
    db.setDatabaseName(dbname);

    if( db.open() )
    {
        QSqlQuery temp1;
        query = temp1;
    }
    else
    {
        qDebug()<< "Error :"<<db.lastError();
    }
}

dbctx::~dbctx()
{

}

void dbctx::getSections(QList<Section> &sectionList,const QStringList filter)
{
    sectionList.clear();
    QStringList columns;
    columns.append(Section::ID);
    columns.append(Section::Name);

    QString q;
    Section s;
    buildSelectQuery(q,columns,Section::EntityName,filter);

    if(query.exec(q))
    {
        while (query.next()){
            s.setId(query.value(Section::ID).toInt());
            s.setName(query.value(Section::Name).toString());
            sectionList.append(s);
        }
    }
    //after getting inf from database we cache it
    refreshCache(sectCache,sectionList);
}

void dbctx::getFragments(QList<Fragment> &fragmentList,const QStringList filter)
{
    fragmentList.clear();
    QStringList columns;
    columns.append(Fragment::ID);
    columns.append(Fragment::Area);
    columns.append(Fragment::SectionID);
    columns.append(Fragment::Tnorm);

    QString q;
    Fragment f ;
    Section s;
    buildSelectQuery(q,columns,Fragment::EntityName,filter);

    if (query.exec(q))
    {
        while (query.next()){
            f.setId(query.value(Fragment::ID).toInt());
            int s_id = query.value(Fragment::SectionID).toInt();
            s = getEntity(sectCache,s_id);
            f.setSection(s);
            f.setArea(query.value(Fragment::Area).toDouble());
            f.setTnorm(query.value(Fragment::Tnorm).toDouble());
            fragmentList.append(s);
        }
    }

    refreshCache(fragCache,fragmentList);
}

void dbctx::getPakazateli(QList<Pakazatel> &PakazatelList, const QStringList filter)
{

}



Entity dbctx::getEntity(QHash<int,Entity> cache,const int id)
{
    if (cache.contains(id)){
        return cache.value(id);
    }
    return NULL;
}

void dbctx::buildSelectQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &filter)
{
    QStringList q_str;
    q_str.append("SELECT ");
    if (columns.size() == 0)
    {
        q_str.append(" * ");
    }
    else
    {
        build_string(q_str,columns,QString(","));
    }

    q_str.append("FROM ");

    q_str.append(QString("%0 ").arg(table));

    if (filter.size() > 0 )
    {
        q_str.append("WHERE ");

        build_string(q_str,filter,QString(" AND "));
    }

    QString str;

    for(QString s: q_str)
    {
        str += s;
    }

    q = str;
}

void dbctx::buildString(QStringList &result, const QStringList &fragments, const QString &separator)
{
    int i = 0;
    for(QString s:fragments)
    {
        if (i == 0)
        {
            result.append(QString("%0").arg(s));
            i++;
        }
        else
        {
            result.append(QString("%0 %1 ").arg(separator,s));
        }
    }
}

void dbctx::buildFilter(QString &q, const QString &oper,const QStringList &params)
{
    if (params.size() != 2) return;
    q = QString("%0 %1 %2").arg(params[0],oper,params[1]);
}

void dbctx::buildInsertQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &values)
{
    QStringList q_str;
    //"insert into genres(name) values(?)"

    if (columns.size() != values.size()) return;

    q_str.append("INSERT  INTO ");
    q_str.append(QString("%0 ").arg(table));

    q_str.append("( ");
    build_string(q_str,columns,QString(","));
    q_str.append(" ) ");

    q_str.append("VALUES ");

    q_str.append("( ");
    build_string(q_str,values,QString(","));
    q_str.append(" ) ");

    QString str;

    for(QString s: q_str)
    {
        str += s;
    }

    q = str;
}



void dbctx::refreshCache(QHash<int, Entity> cache, QList<Entity> &list)
{
    cache.clear();
    for (int i = 0 ; i < list.count(); i++){
        cache[list[i].id()] = list[i];
    }
}

