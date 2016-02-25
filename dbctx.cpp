#include "dbctx.h"

#include <QSqlQuery>
#include <QStringList>

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

    QString q;
    Pakazatel *p;
    Entity *parent;
    buildSelectQuery(q,columns,Pakazatel::EntityName,filter);
    if (query.exec(q)){
        while (query.next()) {
            p = new Pakazatel();
            p->setId(query.value(Pakazatel::ID).toInt());
            int p_id = query.value(Pakazatel::ParentID).toInt();
            parent = getEntity(pakCache,p_id);
            p->setParent((Pakazatel*)parent);
            p->setName(query.value(Pakazatel::Name).toString());
            p->setUnit(query.value(Pakazatel::Unit).toString());
            pakazatelList.append(p);
        }
    }
    refreshCache(pakCache,pakazatelList);
}

void Dbctx::insertPakazatel(const Pakazatel *p)
{
    QStringList columns;

    columns.append(Pakazatel::Name);
    columns.append(Pakazatel::Unit);
    columns.append(Pakazatel::NomValue);
    columns.append(Pakazatel::CalcValue);
    columns.append(Pakazatel::FactValue);
    columns.append(Pakazatel::ParentID);

    QStringList values;

    values.append(QString("'%0'").arg(p->name()));
    values.append(QString("'%0'").arg(p->unit()));
    values.append(QString("%0").arg(p->nomValue()));
    values.append(QString("%0").arg(p->calcValue()));
    values.append(QString("%0").arg(p->factValue()));
    values.append(QString("%0").arg(p->parent()->id()));

    QString q;
    buildInsertQuery(q,columns,Pakazatel::EntityName,values);


    if (query.exec(q) == true)
    {
        //qDebug()<<"insert :"<<name<<" success!";
        return ;
    }
    else
    {
        //qDebug()<<"FAIL: insert : "<<e_n.getEmp_id()<< " "<<e_n.getTel_num_id()<<" FAIL reason: "<<db.lastError()<<" Query : "<<q;;
    }
}



Entity *Dbctx::getEntity(QHash<int,Entity*> cache,const int id)
{
    if (cache.contains(id)){
        return cache.value(id);
    }
    return NULL;
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

    q_str.append("INSERT  INTO ");
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



void Dbctx::refreshCache(QHash<int, Entity *> cache, QList<Entity*> &list)
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

