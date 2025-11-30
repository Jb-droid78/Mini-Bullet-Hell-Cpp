#include "../include/MyInclude/Aplication.hpp"
#include "../include/MyInclude/Entitys/Entity.hpp"
#include "../include/MyInclude/Entitys/Enemy.hpp"
#include "../include/MyInclude/Entitys/Player.hpp"
#include "../include/MyInclude/Entitys/Projectile.hpp"
#include "../include/MyInclude/utils/Formatting.hpp"

#include <SFML/Graphics.hpp>
// #include <iostream>
#include <memory>
#include <algorithm>
#include <vector>

Aplication::Aplication() : m_window(sf::RenderWindow(sf::VideoMode({800, 600}), "Mini Game in C++")) { 
    m_window.setFramerateLimit(60);
}

int Aplication::execute() {
    std::cout << "\nInitializing execute..." << std::endl;
    initialize();
    runLoop();
    return 0;
}

void Aplication::createProjectile(sf::Vector2f startingPosition, sf::Vector2f size, Direction direction, Projectile::ProjectileType type, float speed) {
    if (direction == Direction::null) return;
    m_projectile.push_back(
        std::make_unique<Projectile>(startingPosition, size, direction, type, speed)
    );
}

void Aplication::initialize() {
    std::cout << "Initializing application..." << std::endl;
                            
    std::string fontPath = "colocar o caminho de alguma font que tenha no pc!!";
    if (!m_font.openFromFile(fontPath)) {
        std::cout << "ERRO: Falha ao carregar a fonte! \ncoloque alguma font na variavel fontPath" << std::endl;
        shutdown();
    }
    screenW = m_window.getSize().x;
    screenH = m_window.getSize().y;

    m_player = std::make_unique<Player>(*this); // inicializar um unique_ptr<>, tipo um new
    m_enemy = std::make_unique<Enemy>(*this);

    m_textFPS = std::make_unique<sf::Text>(m_font, "FPS: ", 10);
    m_textFPS -> setPosition({5.f, 5.f});

    m_textGameOver = std::make_unique<sf::Text>(m_font, "GAME OVER", 50);
    m_textGameOver -> setFillColor(sf::Color::Red);
    m_textGameOver -> setPosition({ 
        400 - m_textGameOver -> getLocalBounds().size.x / 2,
        300 - m_textGameOver -> getLocalBounds().size.y / 2
    });

    m_textWin = std::make_unique<sf::Text>(m_font, "YOU WIN", 50);
    m_textWin -> setFillColor(sf::Color::Green);
    m_textWin -> setPosition({ 
        400 - m_textWin -> getLocalBounds().size.x / 2,
        300 - m_textWin -> getLocalBounds().size.y / 2
    });

    m_textLifePlayer = std::make_unique<sf::Text>(m_font, "Player HP: ", 14);
    m_textLifePlayer -> setFillColor(sf::Color::Green);
    m_textLifePlayer -> setPosition({
        m_textFPS -> getPosition().x,
        m_textFPS -> getPosition().y + 15
    });

    m_textLifeEnemy = std::make_unique<sf::Text>(m_font, "Enemy HP: ", 14);
    m_textLifeEnemy -> setFillColor(sf::Color::Red);
    m_textLifeEnemy -> setPosition({
        m_textFPS -> getPosition().x,
        m_textFPS -> getPosition().y + 30
    });
}

void Aplication::runLoop() {
    std::cout << "Initializing loop...\n" << std::endl;
    sf::Clock clock;

    while (m_window.isOpen()) {

        float dt = clock.restart().asSeconds();

        while(std::optional event = m_window.pollEvent())
            if (event -> is < sf::Event::Closed > ()) m_window.close(); 
                
        if (!m_playing) shutdown();

        processEvent();
        updateGame(dt); 
        checkCollisions(dt);
        // criar uma função para verificar a vida das entidades
        cleanupProjectiles();
        renderGame(dt);
    }
}

void Aplication::processEvent() {
    // sair do game
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) shutdown();
    // setar vida
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) m_player -> setHealth(5.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) m_player -> setHealth(0.f);

}

void Aplication::updateGame(float dt) {
    m_player -> update(dt);
    m_enemy -> update(dt);

    for (const auto& p : m_projectile) {
        p -> update(dt);
    }
}

void Aplication::checkCollisions(float dt) {
    if (m_iFrameTimer > 0) {
        m_iFrameTimer -= dt;
        if (m_iFrameTimer < 0) 
            m_iFrameTimer = 0;
    } 
    if (m_player -> attackCooldownTimer > 0) {
        m_player -> attackCooldownTimer -= dt;
        if (m_player -> attackCooldownTimer < 0) 
            m_player -> attackCooldownTimer = 0;
    }

    if (m_player -> getBounds().findIntersection(m_enemy -> getBounds())) {   
        m_player -> takeDamage(1.f, m_iFrameTimer, IFRAME, Entity::EntityType::player);
    }
    
    for (const auto& p : m_projectile) {
        if (m_player -> getBounds().findIntersection(p -> getBounds()) && p -> getColor() == sf::Color::Red) 
            m_player -> takeDamage(1.f, m_iFrameTimer, IFRAME, Entity::EntityType::player); 
        
        if (m_enemy -> getBounds().findIntersection(p -> getBounds()) && p -> getColor() == sf::Color::Blue) {
            m_enemy -> takeDamage(3.5f, m_iFrameTimer, IFRAME, Entity::EntityType::enemy); 
            p -> destroyProjectile = true; 
        }
    }
}

void Aplication::renderUI(float dt) {
    int fps = static_cast<int>(1.f / dt);
    m_textFPS -> setString("FPS: " + std::to_string(fps) +
        " IFrame: " + ut::floatFormatting(m_iFrameTimer) +
        " AttackCooldown: " + ut::floatFormatting(m_player -> attackCooldownTimer));

    m_textLifePlayer -> setString("Player HP: " + ut::floatFormatting(m_player -> getHealth()));
    m_textLifeEnemy -> setString("Enemy HP: " + ut::floatFormatting(m_enemy -> getHealth()));

    if (m_player -> getHealth() <= 0) {
        m_window.draw(*m_textGameOver);
    } else if (m_enemy -> getHealth() <= 0) {
        m_window.draw(*m_textWin);
    } else {
        m_window.draw(*m_textLifePlayer);
        m_window.draw(*m_textLifeEnemy);
    }
    m_window.draw(*m_textFPS);
}

void Aplication::renderWorld() {
    if (m_player -> getHealth() > 0 && m_enemy -> getHealth() > 0) {
        m_player -> draw(m_window); 
        m_enemy -> draw(m_window);

        for (const auto& p : m_projectile) {
            p -> draw(m_window);
        }
    }
}

void Aplication::renderGame(float dt) {
    m_window.clear(sf::Color::Black);
    renderWorld();
    renderUI(dt);
    m_window.display();
}

void Aplication::cleanupProjectiles() {
    m_projectile.erase(
        std::remove_if(
            m_projectile.begin(),
            m_projectile.end(),
            [&](const std::unique_ptr<Entity>& p) {
                bool outOfBounds =  (
                    p -> getPosition().x < -70 || p -> getPosition().x > screenW ||
                    p -> getPosition().y < -70 || p -> getPosition().y > screenH
                );
                return outOfBounds || p -> destroyProjectile;
            }
        ),
        m_projectile.end()
    );
}