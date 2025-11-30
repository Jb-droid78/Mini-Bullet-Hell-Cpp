#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "../include/MyInclude/Entitys/Entity.hpp"
#include "../include/MyInclude/GameEnums/EnemyState.hpp"
#include "../include/MyInclude/GameEnums/Direction.hpp"

#include <SFML/Graphics.hpp>

class Aplication;

class Enemy : public Entity {
public:
    Enemy(Aplication& refAplication);
    void update(float dt) override; // Implementa a IA
    void draw(sf::RenderWindow& window) const override;

private:
    const float m_speed = 700.f;
    bool m_secondPhase = false;

    Direction m_projectileDirection;
    bool m_isMove = false;
    bool m_isAttaking = false;
    float m_attackTimer = 0;

    float m_count = 0;
    bool m_nextWallTop = true;
    float m_speedprojectile = 700.f; 

    float m_timeToAttack = 0.f;
    float m_timeToMove = 0.f;

    sf::Vector2f m_startProjectilePosition;
    sf::Vector2f m_projectileSize;

    en::EnemyState m_enemyState;
    en::EnemyState m_stateRepeated;
    en::EnemyAttack m_enemyAttack;
    en::EnemyAttack m_attackRepeated;
    Aplication& m_appRef;

    void updateIaLogic(float dt);
    void enemyMove(float dt);
    void enemyAttack(float dt);
    void handleAttack();

    void attack1();
    void attack2();
    void attack3(float dt);
    void attack4(float dt);
};

#endif // ENEMY_HPP