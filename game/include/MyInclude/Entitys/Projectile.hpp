#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "../include/MyInclude/Entitys/Entity.hpp"
#include "../include/MyInclude/GameEnums/Direction.hpp"

class Projectile : public Entity {
public:
    enum class ProjectileType {
        player, enemy
    };

    Projectile(sf::Vector2f startingPosition, sf::Vector2f size, Direction direction, ProjectileType type, float speed);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

private:
    Direction m_direction;
    float m_speed = 0.f;

    void projectileMovement(float dt);
};

#endif