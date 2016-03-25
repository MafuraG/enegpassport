#ifndef FRAGMENT_H
#define FRAGMENT_H

#include "entity.h"
#include "section.h"

#include <QVariant>



class Fragment:public Entity
{
public:
    Fragment();
    ~Fragment();


    static QString EntityName;
    static QString Section;
    static QString Tnorm;
    static QString Area;
    static QString Resistance;

    static QString D_EntityName;
    static QString D_Section;
    static QString D_Tnorm;
    static QString D_Area;
    static QString D_Resistance;
    static double ColumnCount;

    double tnorm() const;
    void setTnorm(double tnorm);

    double area() const;
    void setArea(double area);

    double resistance() const;
    void setResistance(double resistance);

    QString section() const;
    void setSection(const QString &section);

    QVariant data(const int col)const;
    bool setData(int col, QVariant val);

    static QVariant header(int col);
private:
    QString m_section;
    double m_tnorm;
    double m_area;
    double m_resistance;    
};

#endif // FRAGMENT_H
