#include "entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}
unsigned int Entity::id() const
{
    return m_id;
}

void Entity::setId(unsigned int id)
{
    m_id = id;
}


QString Entity::ID = "id";
