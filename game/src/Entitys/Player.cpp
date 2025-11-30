#include "../include/MyInclude/Entitys/Entity.hpp"
#include "../include/MyInclude/Entitys/Player.hpp"
#include "../include/MyInclude/Aplication.hpp"

#include <SFML/Graphics.hpp>

Player::Player(Aplication& refAplication) : Entity({30.f, 30.f}), m_appRef(refAplication) {
    m_shape.setFillColor(sf::Color::Green);
    sf::FloatRect bounds = m_shape.getLocalBounds();
    m_shape.setPosition({
        600 - bounds.size.x / 2.f,
        300 - bounds.size.y / 2.f 
    });
    m_health = 5.f;
}

void Player::update(float dt) {
    handleInput(dt);
    checkBounds();
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}
 
void Player::handleInput(float dt) {
    // movimentação
    sf::Vector2f moviment(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) moviment.y -= m_speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) moviment.y += m_speed * dt;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) moviment.x -= m_speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) moviment.x += m_speed * dt;

    m_shape.move(moviment);

    if (attackCooldownTimer > 0) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) defineAttack(Direction::up);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) defineAttack(Direction::down);

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) defineAttack(Direction::left);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) defineAttack(Direction::right);
}

void Player::handleAttack() { 
    m_appRef.createProjectile({
            m_shape.getPosition().x + 10.f, 
            m_shape.getPosition().y + 10.f
        }, 
        {10.f, 10.f}, 
        m_chosenAttack, 
        Projectile::ProjectileType::player,
        700.f
    );
}

void Player::defineAttack(Direction dir) {
    m_chosenAttack = dir;
    attackCooldownTimer = ATTACK_RATE;
    handleAttack();
    m_chosenAttack = Direction::null;
}

void Player::checkBounds() {
    if (m_shape.getPosition().x > m_appRef.screenW - m_shape.getSize().x) 
        m_shape.setPosition({m_appRef.screenW - m_shape.getSize().x, m_shape.getPosition().y});
    
    if (m_shape.getPosition().x < 0) 
        m_shape.setPosition({0, m_shape.getPosition().y});

    if (m_shape.getPosition().y > m_appRef.screenH - m_shape.getSize().y) 
        m_shape.setPosition({m_shape.getPosition().x, m_appRef.screenH - m_shape.getSize().y});

    if (m_shape.getPosition().y < 0) 
        m_shape.setPosition({m_shape.getPosition().x, 0});
}