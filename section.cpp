#include "section.h"

Section::Section()
{

}

Section::~Section()
{

}

QString Section::getName() const
{
    return name;
}

void Section::setName(const QString &value)
{
    name = value;
}

QString Section::Name = "name";
QString Section::EntityName = "section";
