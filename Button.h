#pragma once
#include <SFML/Graphics.hpp>

class Button {
public:
    Button(const std::string& label, const sf::Vector2f& position, const sf::Vector2f& size);

    void render(sf::RenderWindow& window) const;
    bool contains(float x, float y) const;
    const std::string& getLabel() const { return m_label; }

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    std::string m_label;
};