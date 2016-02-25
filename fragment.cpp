#include "fragment.h"

Fragment::Fragment()
{

}

Fragment::~Fragment()
{

}

double Fragment::tnorm() const
{
    return m_tnorm;
}

void Fragment::setTnorm(double tnorm)
{
    m_tnorm = tnorm;
}
double Fragment::area() const
{
    return m_area;
}

void Fragment::setArea(double area)
{
    m_area = area;
}

Section *Fragment::section() const
{
    return m_section;
}

void Fragment::setSection(Section *section)
{
    m_section = section;
}
double Fragment::resistance() const
{
    return m_resistance;
}

void Fragment::setResistance(double resistance)
{
    m_resistance = resistance;
}


QString Fragment::SectionID = "sectionid";
QString Fragment::Tnorm = "tnorm";
QString Fragment::Area = "area";
QString Fragment::EntityName = "fragment";
QString Fragment::Resistance = "resistance";

QString Fragment::D_SectionID = "Наименование фрагмента";
QString Fragment::D_Tnorm = "tпом";
QString Fragment::D_Area = "Aф,I, м2";
QString Fragment::D_EntityName = "fragment";
QString Fragment::D_Resistance = "Rпро,i, (м2*0С)/Вт";




