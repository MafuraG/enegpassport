#ifndef ENTITY_H
#define ENTITY_H

#include <QString>



class Entity
{
public:
    Entity();
    ~Entity();

    static QString ID;

    unsigned int id() const;
    void setId(unsigned int id);

private :
    int m_id;
};

#endif // ENTITY_H
