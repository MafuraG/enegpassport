#include "entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}
int Entity::id() const
{
    return m_id;
}

void Entity::setId(int id)
{
    m_id = id;
}


QString Entity::ID = "id";
