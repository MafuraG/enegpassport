#include "fragment.h"

Fragment::Fragment()
{

}

Fragment::~Fragment()
{

}

Section Fragment::section() const
{
    return m_section;
}

void Fragment::setSection(const Section &section)
{
    m_section = section;
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

QString Fragment::SectionID = "sectionid";
QString Fragment::Tnorm = "tnorm";
QString Fragment::Area = "area";
QString Fragment::EntityName = "fragment";




