#ifndef MEASURE_H
#define MEASURE_H

#include <QString>



class Indicator
{
public:
    Indicator();
    ~Indicator();
    QString unit() const;
    void setUnit(const QString &unit);

    double nomValue() const;
    void setNomValue(double nomValue);

    double calcValue() const;
    void setCalcValue(double calcValue);

    double factValue() const;
    void setFactValue(double factValue);

    bool calculated() const;
    void setCalculated(bool calculated);

private :
    QString m_unit;
    double m_nomValue;
    double m_calcValue;
    double m_factValue;
    bool m_calculated;
};

#endif // MEASURE_H
