#include "../include/MyInclude/Entitys/Entity.hpp"
#include "../include/MyInclude/Entitys/Projectile.hpp"

// #include <iostream>

Projectile::Projectile(sf::Vector2f startingPosition, sf::Vector2f size, Direction direction, ProjectileType type, float speed) : Entity(size) {
    m_direction = direction;
    m_shape.setPosition(startingPosition);
    m_speed = speed;
    if (type == ProjectileType::player) m_shape.setFillColor(sf::Color::Blue);
    else if (type == ProjectileType::enemy) m_shape.setFillColor(sf::Color::Red);
}

void Projectile::update(float dt) {
    projectileMovement(dt);
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

void Projectile::projectileMovement(float dt) {
    sf::Vector2f movement(0.f, 0.f);

    switch (m_direction)
    {
    case Direction::up:
        movement.y -= m_speed * dt;
        break;
    case Direction::down:
        movement.y += m_speed * dt;
        break;
    case Direction::left:
        movement.x -= m_speed * dt;
        break;
    case Direction::right:
        movement.x += m_speed * dt;
        break;
    }
    m_shape.move(movement);
}