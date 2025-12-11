#pragma once

#include <SFML/Graphics.hpp>
#include "Card.h"
#include "AssetManager.h"
#include "GameLogic.h"

class Game {
public:
    Game();
    void run();

private:
    void setupUI();
    void processEvents();
    void update();
    void render();
    void handleClick(float x, float y);
    void handleButtonClick(const std::string& name);
    void setupCardVisuals();

    struct Button {
        sf::RectangleShape shape;
        sf::Text label;

        bool contains(float x, float y) {
            return shape.getGlobalBounds().contains(x, y);
        }
    };

private:
    sf::RenderWindow window;
    AssetManager assets; 
    GameLogic logic;

    std::vector<Button> buttons;

    sf::Clock gameClock;
    float elapsedSeconds = 0.f;

    sf::Text movesText;
    sf::Text pairsText;
    sf::Text timeText;

    bool hintMode = false;
    sf::Clock hintClock;
    float HintDuration = 2.0f;
};
