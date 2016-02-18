#ifndef SECTION_H
#define SECTION_H

#include "entity.h"

#include <QString>



class Section: public Entity
{
public:
    Section();
    ~Section();    

    QString getName() const;
    void setName(const QString &value);


    static QString Name;
    static QString EntityName;

private:    
    QString name;
};

#endif // SECTION_H
