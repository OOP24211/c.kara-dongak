#pragma once
#include <SFML/Graphics.hpp>

struct GameObject {
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool contains(float x, float y) = 0;
    virtual ~GameObject() = default;
};

struct BaseCard : public GameObject {
    int pairId = 0;
    bool isOpen = false;
    bool isMatched = false;
    sf::RectangleShape shape;
    float anim = 0.f;

    bool contains(float x, float y) override {
        return shape.getGlobalBounds().contains(x, y);
    }

    void draw(sf::RenderWindow& window) override {
    }
};

struct GameCard : public BaseCard {
    sf::Sprite faceSprite;
    sf::Sprite backSprite;

    void setTexture(const sf::Texture& faceTex, const sf::Texture& backTex) {
        faceSprite.setTexture(faceTex);
        backSprite.setTexture(backTex);

        sf::FloatRect bounds = faceSprite.getLocalBounds();
        faceSprite.setScale(
            shape.getSize().x / bounds.width,
            shape.getSize().y / bounds.height
        );
        backSprite.setScale(
            shape.getSize().x / bounds.width,
            shape.getSize().y / bounds.height
        );
    }

    void draw(sf::RenderWindow& window) override {
        sf::Sprite* currentSprite = (isOpen || isMatched) ? &faceSprite : &backSprite;
        currentSprite->setPosition(shape.getPosition());
        window.draw(*currentSprite);
    }
};
