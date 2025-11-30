#include "../include/MyInclude/Entitys/Entity.hpp"

Entity::Entity(const sf::Vector2f& size) {
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color::White); // Cor padrão
}

Entity::~Entity() { }

void Entity::draw(sf::RenderWindow& window) const { window.draw(m_shape); }

void Entity::takeDamage(float damage, float& iFrameTime, const float IFRAME, EntityType type) {
    if (iFrameTime > 0 && type == EntityType::player) return;
    
    m_health -= damage;
    if (m_health < 0) m_health = 0;
    if (m_health > 0 && type == EntityType::player) iFrameTime = IFRAME;
}
