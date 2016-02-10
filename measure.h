#ifndef MEASURE_H
#define MEASURE_H

#include <QString>



class Measure
{
public:
    Measure();
    ~Measure();
    QString unit() const;
    void setUnit(const QString &unit);

    double nomValue() const;
    void setNomValue(double nomValue);

    double calcValue() const;
    void setCalcValue(double calcValue);

    double factValue() const;
    void setFactValue(double factValue);

private :
    QString m_unit;
    double m_nomValue;
    double m_calcValue;
    double m_factValue;
};

#endif // MEASURE_H
