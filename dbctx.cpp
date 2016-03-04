#include "dbctx.h"

#include <QSqlQuery>
#include <QStringList>
#include <QDebug>
#include <QSqlDatabase>

Dbctx::Dbctx()
{

}

Dbctx::Dbctx(const QString &dbtype, const QString &dbname)
{
    db = QSqlDatabase::addDatabase(dbtype);
    db.setDatabaseName(dbname);

    if( db.open() )
    {
        QSqlQuery temp1;
        query = temp1;

        loadCache();
    }
    else
    {
        //qDebug()<< "Error :"<<db.lastError();
    }
}

Dbctx::~Dbctx()
{
    db.close();
}

void Dbctx::getSections(QList<Entity*> &sectionList,const QStringList filter)
{
    sectionList.clear();
    QStringList columns;
    columns.append(Section::ID);
    columns.append(Section::Name);

    QString q;
    Section *s;
    buildSelectQuery(q,columns,Section::EntityName,filter);

    if(query.exec(q))
    {
        while (query.next()){
            s = new Section();
            s->setId(query.value(Section::ID).toInt());
            s->setName(query.value(Section::Name).toString());
            sectionList.append(s);
        }
    }
    //after getting inf from database we cache it
    refreshCache(sectCache,sectionList);
}

void Dbctx::getFragments(QList<Entity *> &fragmentList, const QStringList filter)
{
    fragmentList.clear();
    QStringList columns;
    columns.append(Fragment::ID);
    columns.append(Fragment::Area);
    columns.append(Fragment::SectionID);
    columns.append(Fragment::Tnorm);
    columns.append(Fragment::Resistance);

    QString q;
    Fragment *f ;
    Section *s;
    buildSelectQuery(q,columns,Fragment::EntityName,filter);

    if (query.exec(q))
    {
        while (query.next()){
            f = new Fragment();
            f->setId(query.value(Fragment::ID).toInt());
            int s_id = query.value(Fragment::SectionID).toInt();
            s = (Section*)getEntity(sectCache,s_id);
            f->setSection(s);
            f->setArea(query.value(Fragment::Area).toDouble());
            f->setTnorm(query.value(Fragment::Tnorm).toDouble());
            f->setResistance(query.value(Fragment::Resistance).toDouble());
            fragmentList.append(f);
        }
    }

    refreshCache(fragCache,fragmentList);
}

void Dbctx::getPakazateli(QList<Entity *> &pakazatelList, const QStringList filter)
{
    pakazatelList.clear();
    QStringList columns;
    columns.append(Pakazatel::ID);
    columns.append(Pakazatel::Name);
    columns.append(Pakazatel::Unit);
    columns.append(Pakazatel::ParentID);
    columns.append(Pakazatel::Calculated);
    columns.append(Pakazatel::Numeration);

    QString q;
    Pakazatel *p;
    Entity *parent;
    buildSelectQuery(q,columns,Pakazatel::EntityName,filter);
    if (query.exec(q)){
        while (query.next()) {
            p = new Pakazatel();
            QVariant val;
            val = query.value(Pakazatel::ID);
            if (!val.isNull()){
                p->setId(query.value(Pakazatel::ID).toInt());
                int p_id = query.value(Pakazatel::ParentID).toInt();
                parent = new Pakazatel();
                parent->setId(p_id);
                p->setParent((Pakazatel*)parent);
            }else{
                p->setParent(NULL);
            }
            p->setName(query.value(Pakazatel::Name).toString());
            p->setUnit(query.value(Pakazatel::Unit).toString());
            p->setCalculated(query.value(Pakazatel::Calculated).toBool());
            p->setNumeration(query.value(Pakazatel::Numeration).toUInt());
            pakazatelList.append(p);
        }
    }
    refreshCache(pakCache,pakazatelList);

    //fill parents of pakazatel
    for (int i = 0 ; i < pakazatelList.count(); i++){
        //qDebug()<<i;
        Pakazatel *c = (Pakazatel*)pakazatelList[i];
        if (c->parent() != nullptr){
            Pakazatel *p = (Pakazatel*)getEntity(pakCache,c->parent()->id());
            c->setParent(p);
        }
    }
}

void Dbctx::insertPakazatel(const Pakazatel *p)
{
    QStringList columns;

    columns.append(Pakazatel::ID);
    columns.append(Pakazatel::Name);
    columns.append(Pakazatel::Unit);
    columns.append(Pakazatel::NomValue);
    columns.append(Pakazatel::CalcValue);
    columns.append(Pakazatel::FactValue);
    if (p->parent() != nullptr ) columns.append(Pakazatel::ParentID);
    columns.append(Pakazatel::Calculated);
    columns.append(Pakazatel::Numeration);


    QStringList values;

    values.append(QString("%0").arg(p->id()));
    values.append(QString("'%0'").arg(p->name()));
    values.append(QString("'%0'").arg(p->unit()));
    values.append(QString("%0").arg(p->nomValue()));
    values.append(QString("%0").arg(p->calcValue()));
    values.append(QString("%0").arg(p->factValue()));
    if (p->parent() != nullptr ) values.append(QString("%0").arg(p->parent()->id()));
    values.append(QString("%0").arg(p->calculated()));
    values.append(QString("%0").arg(p->numeration()));

    QString q;
    buildInsertQuery(q,columns,Pakazatel::EntityName,values);

    if (p->id() == 53) qDebug()<<q<<"\n";
    if (query.exec(q) == true)
    {
        if (p->id() == 53) qDebug()<<"insert :"<<p->name()<<" success!";
        return ;
    }
    else
    {
        //qDebug()<<"FAIL: insert : "<<p->name()<<" Query : "<<q;
    }
}

Pakazatel *Dbctx::getPakazatelByName(const QString name)
{
    QHashIterator<int,Entity*> i(pakCache);
    while (i.hasNext()){
        i.next();
        Pakazatel *p = (Pakazatel*)i.value();
        if (p->name() == name)
            return p;
    }
    return nullptr;
}

Pakazatel *Dbctx::getPakazatelByID(const unsigned int id)
{
    QHashIterator<int,Entity*> i(pakCache);
    while (i.hasNext()){
        i.next();
        Pakazatel *p = (Pakazatel*)i.value();
        if (p->id() == id)
            return p;
    }
    return nullptr;
}

Entity *Dbctx::getEntity(QHash<int,Entity*> cache,const int id)
{
    if (cache.contains(id)){
        return cache.value(id);
    }
    return NULL;
}

void Dbctx::loadCache()
{
    QList<Entity*> list;
    QStringList f;
    getFragments(list,f);
    getSections(list,f);
    getPakazateli(list,f);
}

void Dbctx::buildSelectQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &filter)
{
    QStringList q_str;
    q_str.append("SELECT ");
    if (columns.size() == 0)
    {
        q_str.append(" * ");
    }
    else
    {
        buildString(q_str,columns,QString(","));
    }

    q_str.append("FROM ");

    q_str.append(QString("%0 ").arg(table));

    if (filter.size() > 0 )
    {
        q_str.append("WHERE ");

        buildString(q_str,filter,QString(" AND "));
    }

    QString str;

    for(QString s: q_str)
    {
        str += s;
    }

    q = str;
}

void Dbctx::buildString(QStringList &result, const QStringList &fragments, const QString &separator)
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

void Dbctx::buildFilter(QString &q, const QString &oper,const QStringList &params)
{
    if (params.size() != 2) return;
    q = QString("%0 %1 %2").arg(params[0],oper,params[1]);
}

void Dbctx::buildInsertQuery(QString &q, const QStringList &columns, const QString &table, const QStringList &values)
{
    QStringList q_str;
    //"insert into genres(name) values(?)"

    if (columns.size() != values.size()) return;

    q_str.append("INSERT OR REPLACE  INTO ");
    q_str.append(QString("%0 ").arg(table));

    q_str.append("( ");
    buildString(q_str,columns,QString(","));
    q_str.append(" ) ");

    q_str.append("VALUES ");

    q_str.append("( ");
    buildString(q_str,values,QString(","));
    q_str.append(" ) ");

    QString str;

    for(QString s: q_str)
    {
        str += s;
    }

    q = str;
}



void Dbctx::refreshCache(QHash<int, Entity *> &cache, QList<Entity*> &list)
{
    cache.clear();
    for (int i = 0 ; i < list.count(); i++){
        cache[list[i]->id()] = list[i];
    }
}
QSqlRelationalTableModel *Dbctx::getPakazatelModel()
{    
    return m_pakazatelModel;
}

void Dbctx::initPakazatelModel()
{
    m_pakazatelModel = new QSqlRelationalTableModel();
    m_pakazatelModel->setTable(Pakazatel::EntityName);

    m_pakazatelModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pakazatelModel->setRelation(6,QSqlRelation(Pakazatel::EntityName,Pakazatel::ID,Pakazatel::Name));

    m_pakazatelModel->setHeaderData(1,Qt::Horizontal,Pakazatel::D_Name);
    m_pakazatelModel->setHeaderData(2,Qt::Horizontal,Pakazatel::D_Unit);
    m_pakazatelModel->setHeaderData(3,Qt::Horizontal,Pakazatel::D_NomValue);
    m_pakazatelModel->setHeaderData(4,Qt::Horizontal,Pakazatel::D_CalcValue);
    m_pakazatelModel->setHeaderData(5,Qt::Horizontal,Pakazatel::D_FactValue);
    m_pakazatelModel->setHeaderData(6,Qt::Horizontal,Pakazatel::D_ParentID);

    m_pakazatelModel->select();    

}


QSqlRelationalTableModel *Dbctx::getFragmentModel()
{
    return m_fragmentModel;
}

void Dbctx::initFragmentModel()
{
    m_fragmentModel = new QSqlRelationalTableModel();
    m_fragmentModel->setTable(Fragment::EntityName);

    m_fragmentModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_fragmentModel->setRelation(1,QSqlRelation(Section::EntityName,Section::ID,Section::Name));

    m_fragmentModel->setHeaderData(1,Qt::Horizontal,Fragment::D_SectionID);
    m_fragmentModel->setHeaderData(2,Qt::Horizontal,Fragment::D_Tnorm);
    m_fragmentModel->setHeaderData(3,Qt::Horizontal,Fragment::D_Area);
    m_fragmentModel->setHeaderData(4,Qt::Horizontal,Fragment::D_Resistance);

    m_fragmentModel->select();

}

