#ifndef APLICATION_HPP
#define APLICATION_HPP

#include "../include/MyInclude/Entitys/Entity.hpp"
#include "../include/MyInclude/Entitys/Enemy.hpp"
#include "../include/MyInclude/Entitys/Player.hpp"
#include "../include/MyInclude/Entitys/Projectile.hpp"
#include "../include/MyInclude/GameEnums/Direction.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

class Aplication {
public:
    Aplication();
    int execute();
    void createProjectile(sf::Vector2f startingPosition, sf::Vector2f size, Direction direction, Projectile::ProjectileType type, float speed); // fazer o resto dessa bomba amanhã

    float screenW;
    float screenH;

private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    bool m_playing = true;
    float m_iFrameTimer = 0.f;
    const float IFRAME = 1.f; 

    std::unique_ptr<sf::Text> m_textFPS;
    std::unique_ptr<sf::Text> m_textGameOver;
    std::unique_ptr<sf::Text> m_textWin;
    std::unique_ptr<sf::Text> m_textLifePlayer;
    std::unique_ptr<sf::Text> m_textLifeEnemy;

    std::unique_ptr<Player> m_player;
    std::unique_ptr<Enemy> m_enemy;

    std::vector<std::unique_ptr<Entity>> m_projectile;
 
    void initialize();
    void runLoop();
    void shutdown() { m_window.close(); }

    void processEvent(); // Processa eventos (mouse/teclado)
    void updateGame(float dt); // Lógica do jogo, física, IA
    void checkCollisions(float dt); // Lógica de colisão
    void renderGame(float dt); // Desenha tudo

    void renderUI(float dt);
    void renderWorld();

    void cleanupProjectiles();
};

#endif // APLICATION_HPP