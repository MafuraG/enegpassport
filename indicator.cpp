#include "indicator.h"

Indicator::Indicator()
{

}

Indicator::~Indicator()
{

}
QString Indicator::unit() const
{
    return m_unit;
}

void Indicator::setUnit(const QString &unit)
{
    m_unit = unit;
}
double Indicator::nomValue() const
{
    return m_nomValue;
}

void Indicator::setNomValue(double nomValue)
{
    m_nomValue = nomValue;
}
double Indicator::calcValue() const
{
    return m_calcValue;
}

void Indicator::setCalcValue(double calcValue)
{
    m_calcValue = calcValue;
}
double Indicator::factValue() const
{
    return m_factValue;
}

void Indicator::setFactValue(double factValue)
{
    m_factValue = factValue;
}
bool Indicator::calculated() const
{
    return m_calculated;
}

void Indicator::setCalculated(bool calculated)
{
    m_calculated = calculated;
}






