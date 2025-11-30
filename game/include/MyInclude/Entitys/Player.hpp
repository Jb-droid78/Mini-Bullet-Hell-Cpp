#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../include/MyInclude/Entitys/Entity.hpp"
#include "../include/MyInclude/GameEnums/Direction.hpp"

#include <SFML/Graphics.hpp>

class Aplication;

class Player : public Entity {
public: 
    Player(Aplication& refAplication);
    void update(float dt) override; // Implementa o método abstrato
    void draw(sf::RenderWindow& window) const override;
    const float ATTACK_RATE = 0.3f;
    float attackCooldownTimer = 0.f;


private:
    Direction m_chosenAttack = Direction::null;
    float m_speed = 450.f;
    Aplication& m_appRef;

    // Método para processar a entrada do teclado
    void handleInput(float dt);
    void handleAttack();
    void defineAttack(Direction dir);
    void checkBounds();
};

#endif // PLAYER_HPP