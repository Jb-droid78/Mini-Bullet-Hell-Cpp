#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity {
public:
    enum class EntityType {
        player, enemy
    };

    // Construtor Virtual - importante para a herança
    virtual ~Entity(); 

    // Métodos virtuais puros (abstratos): devem ser implementados pelas classes filhas
    virtual void update(float dt) = 0;

    // Métodos virtuais normais: podem ser sobrescritos pelas classes filhas
    virtual void draw(sf::RenderWindow& window) const;

    // Métodos para manipular a entidade
    void setColor(const sf::Color color) { m_shape.setFillColor(color); }
    sf::Color getColor() { return m_shape.getFillColor(); }

    // Metodos para manipular a saúde
    float getHealth() { return m_health; }
    void setHealth(float health) { m_health = health; }
    bool destroyProjectile = false;

    void takeDamage(float damage, float& iFrameTime, const float IFRAME, EntityType type);

    // Métodos de acesso
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }
    const sf::Vector2f getPosition() const { return m_shape.getPosition(); }

protected:
    sf::RectangleShape m_shape; 
    float m_health = 100.f;  
    float m_iFrame = 1.5f;

    Entity(const sf::Vector2f& size);
};

#endif // ENTITY_HPP