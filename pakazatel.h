#ifndef MEASURE_H
#define MEASURE_H

#include "entity.h"

#include <QString>



class Pakazatel: public Entity
{
public:
    Pakazatel();
    ~Pakazatel();
    QString unit() const;
    void setUnit(const QString &unit);

    double nomValue() const;
    void setNomValue(double nomValue);

    double calcValue() const;
    void setCalcValue(double calcValue);

    double factValue() const;
    void setFactValue(double factValue);    

    Pakazatel *parent() const;
    void setParent(Pakazatel *parent);

    QString name() const;
    void setName(const QString &name);


    static QString Name;
    static QString EntityName;
    static QString Unit;
    static QString NomValue;
    static QString CalcValue;
    static QString FactValue;
    static QString ParentID;

private :

    QString m_name;
    QString m_unit;
    double m_nomValue;
    double m_calcValue;
    double m_factValue;

    Pakazatel *m_parent;
};

#endif // MEASURE_H
