#include "pakazatel.h"

Pakazatel::Pakazatel()
{

}

Pakazatel::~Pakazatel()
{

}
QString Pakazatel::unit() const
{
    return m_unit;
}

void Pakazatel::setUnit(const QString &unit)
{
    m_unit = unit;
}
double Pakazatel::nomValue() const
{
    return m_nomValue;
}

void Pakazatel::setNomValue(double nomValue)
{
    m_nomValue = nomValue;
}
double Pakazatel::calcValue() const
{
    return m_calcValue;
}

void Pakazatel::setCalcValue(double calcValue)
{
    m_calcValue = calcValue;
}
double Pakazatel::factValue() const
{
    return m_factValue;
}

void Pakazatel::setFactValue(double factValue)
{
    m_factValue = factValue;
}

Pakazatel *Pakazatel::parent() const
{
    return m_parent;
}

void Pakazatel::setParent(Pakazatel *parent)
{
    m_parent = parent;
}
QString Pakazatel::name() const
{
    return m_name;
}

void Pakazatel::setName(const QString &name)
{
    m_name = name;
}


QString Pakazatel::Name = "name";
QString Pakazatel::EntityName = "pakazatel";
QString Pakazatel::Unit = "unit";
QString Pakazatel::NomValue = "normvalue";
QString Pakazatel::CalcValue = "calcvalue";
QString Pakazatel::FactValue = "factvalue";
QString Pakazatel::ParentID = "parentid";










