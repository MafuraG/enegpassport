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

    bool calculated() const;
    void setCalculated(bool calculated);

    unsigned int numeration() const;
    void setNumeration(unsigned int numeration);

    static QString Name;
    static QString EntityName;
    static QString Unit;
    static QString NomValue;
    static QString CalcValue;
    static QString FactValue;
    static QString ParentID;
    static QString Calculated;
    static QString Numeration;

    static QString D_Name;
    static QString D_EntityName;
    static QString D_Unit;
    static QString D_NomValue;
    static QString D_CalcValue;
    static QString D_FactValue;
    static QString D_ParentID;
    static QString D_Calculated;
    static QString D_Numeration;




private :

    QString m_name;
    QString m_unit;
    double m_nomValue;
    double m_calcValue;
    double m_factValue;
    bool m_calculated;
    unsigned int m_numeration;

    Pakazatel *m_parent;
};

#endif // MEASURE_H
