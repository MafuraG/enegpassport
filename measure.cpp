#include "measure.h"

Measure::Measure()
{

}

Measure::~Measure()
{

}
QString Measure::unit() const
{
    return m_unit;
}

void Measure::setUnit(const QString &unit)
{
    m_unit = unit;
}
double Measure::nomValue() const
{
    return m_nomValue;
}

void Measure::setNomValue(double nomValue)
{
    m_nomValue = nomValue;
}
double Measure::calcValue() const
{
    return m_calcValue;
}

void Measure::setCalcValue(double calcValue)
{
    m_calcValue = calcValue;
}
double Measure::factValue() const
{
    return m_factValue;
}

void Measure::setFactValue(double factValue)
{
    m_factValue = factValue;
}





