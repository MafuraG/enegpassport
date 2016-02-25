#ifndef FRAGMENT_H
#define FRAGMENT_H

#include "entity.h"
#include "section.h"



class Fragment:public Entity
{
public:
    Fragment();
    ~Fragment();


    static QString EntityName;
    static QString SectionID;
    static QString Tnorm;
    static QString Area;
    static QString Resistance;

    static QString D_EntityName;
    static QString D_SectionID;
    static QString D_Tnorm;
    static QString D_Area;
    static QString D_Resistance;

    double tnorm() const;
    void setTnorm(double tnorm);

    double area() const;
    void setArea(double area);

    Section *section() const;
    void setSection(Section *section);

    double resistance() const;
    void setResistance(double resistance);

private:
    Section *m_section;
    double m_tnorm;
    double m_area;
    double m_resistance;
};

#endif // FRAGMENT_H
