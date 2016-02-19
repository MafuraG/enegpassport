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

    double tnorm() const;
    void setTnorm(double tnorm);

    double area() const;
    void setArea(double area);

    Section *section() const;
    void setSection(Section *section);

private:
    Section *m_section;
    double m_tnorm;
    double m_area;
};

#endif // FRAGMENT_H
