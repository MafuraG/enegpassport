#ifndef ENTITY_H
#define ENTITY_H

#include <QString>



class Entity
{
public:
    Entity();
    ~Entity();

    static QString ID;

    int id() const;
    void setId(int id);

private :
    int m_id;
};

#endif // ENTITY_H
