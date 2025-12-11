#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class AssetManager {
public:
    AssetManager() {
        loadAssets();
    }

    const sf::Texture& getBackTexture() const { return backTexture; }
    const sf::Font& getFont() const { return font; }
    const std::vector<sf::Texture>& getCardTextures() const { return cardTextures; }

private:
    void loadAssets() {
        if (!backTexture.loadFromFile("assets/back_card.png")) {
            std::cerr << "Failed to load assets/back_card.png" << std::endl;
        }

        if (!font.loadFromFile("assets/arial.ttf")) {
            std::cerr << "Failed to load font." << std::endl;
        }

        cardTextures.push_back(loadTexture("assets/red_card_1.png"));
        cardTextures.push_back(loadTexture("assets/yellow_card2.png"));
        cardTextures.push_back(loadTexture("assets/green_card_3.png"));
        cardTextures.push_back(loadTexture("assets/blue_card_4.png"));
        cardTextures.push_back(loadTexture("assets/black_plus4.png"));
        cardTextures.push_back(loadTexture("assets/black_allcolor.png"));
        cardTextures.push_back(loadTexture("assets/green_block.png"));
        cardTextures.push_back(loadTexture("assets/yellow_return.png"));
    }

    sf::Texture loadTexture(const std::string& path) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            std::cerr << "Failed to load " << path << std::endl;
        }
        return tex;
    }

private:
    sf::Texture backTexture;
    sf::Font font;
    std::vector<sf::Texture> cardTextures;
};
