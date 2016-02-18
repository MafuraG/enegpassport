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

    Section section() const;
    void setSection(const Section &section);

    double tnorm() const;
    void setTnorm(double tnorm);

    double area() const;
    void setArea(double area);

private:    
    Section m_section;
    double m_tnorm;
    double m_area;
};

#endif // FRAGMENT_H
