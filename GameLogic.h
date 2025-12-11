#pragma once

#include "Card.h"
#include "constants.h"
#include <vector>
#include <random>

enum class GameState {
    Running, Paused,
};

class GameLogic {
public:
    GameLogic(int rows, int cols) : rows(rows), cols(cols), firstOpenIndex(FIRST_OPEN_INDEX), secondOpenIndex(SECOND_OPEN_INDEX), moves(MOVES), matchedPairs(MATCHED_PAIRS), waitingForPairCheck(WAITING_FOR_PAIR_CHECK) {
        init();
    }

    void init();
    void updateAnimation();
    void checkPairIfNeeded();
    void handleCardClick(int clickedIndex);
    void setAllUnmatchedCardsOpen(bool open);

    // Геттеры для "Инкапсуляции"
    int getMoves() const { return moves; }
    int getMatchedPairs() const { return matchedPairs; }
    GameState getState() const { return state; }
    std::vector<GameCard>& getCards() { return cards; }
    void setState(GameState newState) { state = newState; }

    bool isInitialReveal() const { return initialReveal; }
    bool isWaitingForPairCheck() const { return waitingForPairCheck; }
    const sf::Clock getPairClock() const { return pairClock; }

private:
    std::vector<GameCard> cards;
    GameState state = GameState::Running;

    int rows;
    int cols;

    int firstOpenIndex;
    int secondOpenIndex;

    bool initialReveal = true;
    bool waitingForPairCheck = false;

    sf::Clock initialClock;
    sf::Clock pairClock;

    int moves;
    int matchedPairs;

};
