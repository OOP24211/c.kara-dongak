#include "Game.h"
#include "constants.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Memory Game"), logic(ROWS, COLUMNS) {

    setupUI();
    setupCardVisuals();

    const auto& cardTextures = assets.getCardTextures();
    const sf::Texture& backTexture = assets.getBackTexture();

    auto& cards = logic.getCards();
    for (size_t i = 0; i < cards.size(); ++i) {
        int pairId = cards[i].pairId;
        cards[i].setTexture(cardTextures[pairId], backTexture);
    }
}

void Game::run() {
    gameClock.restart();
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::setupUI() {
    const sf::Font& font = assets.getFont();

    movesText.setFont(font);
    movesText.setCharacterSize(24);
    movesText.setFillColor(sf::Color::White);
    movesText.setPosition(80.f, 10.f);

    pairsText.setFont(font);
    pairsText.setCharacterSize(24);
    pairsText.setFillColor(sf::Color::White);
    pairsText.setPosition(400.f, 10.f);

    timeText.setFont(font);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(200.f, 10.f);

    auto makeButton = [&](const std::string& name, float x, float y) {
        Button b;
        b.shape.setSize({140.f, 40.f});
        b.shape.setFillColor(sf::Color(80,80,200));
        b.shape.setPosition(x, y);

        b.label.setFont(font);
        b.label.setString(name);
        b.label.setCharacterSize(20);
        b.label.setFillColor(sf::Color::White);
        b.label.setPosition(x + 10.f, y + 5.f);

        buttons.push_back(b);
    };

    float btnWidth  = 140.f;
    float btnHeight = 40.f;
    float btnMargin = 10.f;

    float xRight = WINDOW_WIDTH - btnWidth - 20.f;
    float yStart = 120.f;

    makeButton("Stop",   xRight, yStart + 0 * (btnHeight + btnMargin));
    makeButton("Resume", xRight, yStart + 1 * (btnHeight + btnMargin));
    makeButton("New",    xRight, yStart + 2 * (btnHeight + btnMargin));
    makeButton("Hint",   xRight, yStart + 3 * (btnHeight + btnMargin));
}

void Game::setupCardVisuals() {
    const auto& cardTextures = assets.getCardTextures();
    const sf::Texture& backTexture = assets.getBackTexture();

    auto& cards = logic.getCards();
    for (size_t i = 0; i < cards.size(); ++i) {
        int pairId = cards[i].pairId;
        if (pairId >= 0 && static_cast<size_t>(pairId) < cardTextures.size()) {
            cards[i].setTexture(cardTextures[pairId], backTexture);
        } else {
            std::cerr << "Error: Invalid pairId " << pairId << " during texture setup." << std::endl;
        }
    }
}

void Game::processEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            float mouseX = static_cast<float>(event.mouseButton.x);
            float mouseY = static_cast<float>(event.mouseButton.y);

            handleClick(mouseX, mouseY);
        }
    }
}

void Game::update() {
    logic.updateAnimation();

    if (logic.isWaitingForPairCheck()) {
        float seconds = logic.getPairClock().getElapsedTime().asSeconds();
        if (seconds >= PAIR_CHECK_DELAY_SECONDS) {
            logic.checkPairIfNeeded();
        }
    }

    if (hintMode) {
        if (hintClock.getElapsedTime().asSeconds() > HintDuration) {
            hintMode = false;
            logic.setAllUnmatchedCardsOpen(false);
        }
    }

    if (logic.getState() == GameState::Running) {
        elapsedSeconds += gameClock.restart().asSeconds();
    }
    else {
        gameClock.restart();
    }

    movesText.setString("Moves: " + std::to_string(logic.getMoves()));
    pairsText.setString("Pairs: " + std::to_string(logic.getMatchedPairs()));
    int total = static_cast<int>(elapsedSeconds);
    int minutes = total / 60;
    int seconds = total % 60;

    std::string timeStr = "Time: " + std::to_string(minutes) + ":";
    if (seconds < 10) timeStr += "0";
    timeStr += std::to_string(seconds);

    timeText.setString(timeStr);

    if (logic.getMatchedPairs() == 8 && logic.getState() == GameState::Running) {
        logic.setState(GameState::Paused);
    }
}

void Game::render() {
    window.clear(sf::Color(150, 100, 220));

    for (auto& card : logic.getCards()) {
        card.draw(window);
    }

    window.draw(movesText);
    window.draw(pairsText);
    window.draw(timeText);

    for (auto& b : buttons) {
        window.draw(b.shape);
        window.draw(b.label);
    }

    window.display();
}

void Game::handleButtonClick(const std::string& name) {
    if (name == "Stop")   logic.setState(GameState::Paused);
    if (name == "Resume") logic.setState(GameState::Running);
    if (name == "New") {
        logic.init();
        setupCardVisuals();
        logic.setState(GameState::Running);

        elapsedSeconds = 0.f;
        gameClock.restart();
    }

    if (name == "Hint" && logic.getState() == GameState::Running) {
        if (!hintMode){
            hintMode = true;
            hintClock.restart();
            logic.setAllUnmatchedCardsOpen(true);
        }
    }
}

void Game::handleClick(float x, float y) {
    for (auto& b : buttons) {
        if (b.contains(x, y)) {
            handleButtonClick(b.label.getString());
            return;
        }
    }

    if (hintMode) {
        return;
    }

    if (logic.isInitialReveal()) {
        return;
    }

    if (logic.isWaitingForPairCheck()) {
        return;
    }

    int clickedIndex = -1;
    auto& cards = logic.getCards();
    for (int i = 0; i < static_cast<int>(cards.size()); ++i) {
        if (cards[i].contains(x, y)) {
            clickedIndex = i;
            break;
        }
    }

    if (clickedIndex != -1) {
        logic.handleCardClick(clickedIndex);
    }
}
