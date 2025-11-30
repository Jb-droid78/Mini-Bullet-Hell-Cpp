#include "../include/MyInclude/Entitys/Enemy.hpp"
#include "../include/MyInclude/Entitys/Entity.hpp"
#include "../include/MyInclude/Aplication.hpp" 

#include <random>

// publicos
Enemy::Enemy(Aplication& refAplication) : Entity({70.f, 70.f}), m_appRef(refAplication) {
    m_shape.setFillColor(sf::Color::Red);
    sf::FloatRect bounds = m_shape.getLocalBounds();
    m_shape.setPosition({
        300 - bounds.size.x / 2.f,
        300 - bounds.size.y / 2.f 
    });
    m_health = 900.f;
}

void Enemy::update(float dt) {
    updateIaLogic(dt);
    enemyMove(dt);
    enemyAttack(dt);
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

// privados
void Enemy::updateIaLogic(float dt) {
    int phase;
    float timePhase;
    if (m_secondPhase) { timePhase = 6.5f; phase = 1; }
    else { timePhase = 3.f; phase = 3; }
    
    if (m_health <= 450.f) m_secondPhase = true;

    m_timeToAttack += dt;
    m_timeToMove += dt;

    if (m_timeToMove > 3.5f) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, static_cast<int>(en::EnemyState::null) - 1);

        en::EnemyState newState;

        do {
            newState = static_cast<en::EnemyState>(dist(rng));
        } while (newState == m_stateRepeated);

        m_enemyState = newState;
        m_stateRepeated = newState;
        m_isMove = true;
        m_timeToMove = 0.0f;
    }

    if (m_timeToAttack > timePhase) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, static_cast<int>(en::EnemyAttack::null) - phase);

        en::EnemyAttack newState;

        do {
            newState = static_cast<en::EnemyAttack>(dist(rng));
        } while (newState == m_attackRepeated);

        m_enemyAttack = newState;
        m_attackRepeated = newState;
        m_isAttaking = true;
        m_timeToAttack = 0.0f;
    }
    
}

void Enemy::enemyMove(float dt) {
    if (!m_isMove) return;
    sf::Vector2f movement({0.f, 0.f});

    switch (m_enemyState)
    {
    case en::EnemyState::moveUp:
        m_shape.setPosition({ m_shape.getPosition().x, 100.f });
        m_isMove = false;
        return;
    case en::EnemyState::moveDown:
        m_shape.setPosition({ m_shape.getPosition().x, 460.f });
        m_isMove = false;
        return;
    case en::EnemyState::moveLeft:
        movement.x -= m_speed * dt;
        break;
    case en::EnemyState::moveRight:
        movement.x += m_speed * dt;
        break;
    }

    if (m_shape.getPosition().x < 100) {
        m_shape.setPosition({100, m_shape.getPosition().y});
        m_isMove = false;
    } else if (m_shape.getPosition().x > 700 - m_shape.getLocalBounds().size.x) {
        m_shape.setPosition({700 - m_shape.getLocalBounds().size.x, m_shape.getPosition().y});
        m_isMove = false;
    }
    m_shape.move(movement);
}

void Enemy::enemyAttack(float dt) {
    if (!m_isAttaking) return;

    switch (m_enemyAttack)
    {
    case en::EnemyAttack::attack1: attack1(); break;
    case en::EnemyAttack::attack2: attack2(); break;
    case en::EnemyAttack::attack3: attack3(dt); break;
    case en::EnemyAttack::attack4: attack4(dt); break;
    }
}

void Enemy::attack1() {
    m_projectileSize.y = 7.f;
    m_projectileSize.x = 70.f;

    if (m_shape.getPosition().x < 400.5f) {
        m_projectileDirection = Direction::right;
        m_startProjectilePosition.x = -70.f; 
    } else {
        m_projectileDirection = Direction::left;
        m_startProjectilePosition.x = 800.f;
    }
    
    for (int i = 1; i <= 5; i++) {
        m_startProjectilePosition.y = 100.f * i;
        handleAttack();
    }

    m_isAttaking = false;
}

void Enemy::attack2() {
    m_projectileSize.x = 7.f;
    m_projectileSize.y = 70.f;

    if (m_shape.getPosition().y < 300.5f) {
        m_projectileDirection = Direction::down;
        m_startProjectilePosition.y = -70.f; 
    } else {
        m_projectileDirection = Direction::up;
        m_startProjectilePosition.y = 600.f;
    }

    for (int i = 1; i <= 7; i++) {
        m_startProjectilePosition.x = 100.f * i;
        handleAttack();
    }
    m_isAttaking = false;
}

void Enemy::attack3(float dt) {
    m_speedprojectile = 400.f;
    m_projectileSize.x = 70.f;
    m_projectileSize.y = 400.f;
    m_attackTimer += dt;

    if (m_shape.getPosition().x < 400.5f) {
        m_projectileDirection = Direction::right;
        m_startProjectilePosition.x = -70.f; 
    } else {
        m_projectileDirection = Direction::left;
        m_startProjectilePosition.x = 800.f;
    }
    
    if (m_attackTimer >= 1.f) {
        if (m_nextWallTop) m_startProjectilePosition.y = 0.f;
        else m_startProjectilePosition.y = 200.f;

        m_attackTimer = 0;
        m_nextWallTop = !m_nextWallTop;
        handleAttack();
    }

    if (m_count >= 5.f) {  
        m_count = 0.f; 
        m_speedprojectile = 700.f;
        m_isAttaking = false; 
    } 
    else { 
        m_count += dt;
    }
}

void Enemy::attack4(float dt) {
    m_projectileSize.x = 70.f;
    m_projectileSize.y = 300.f;
    m_startProjectilePosition.y = 150;

    if (m_shape.getPosition().x < 400.5f) {
        m_projectileDirection = Direction::right;
        m_startProjectilePosition.x = -70.f; 
    } else {
        m_projectileDirection = Direction::left;
        m_startProjectilePosition.x = 800.f;
    }

    handleAttack();

    if (m_count >= 2.f) { 
        m_count = 0.f;
        m_isAttaking = false;
    } else {
        m_count += dt;
    }
}

void Enemy::handleAttack() {
    m_appRef.createProjectile(
        m_startProjectilePosition, 
        m_projectileSize, 
        m_projectileDirection, 
        Projectile::ProjectileType::enemy,
        m_speedprojectile
    );
}