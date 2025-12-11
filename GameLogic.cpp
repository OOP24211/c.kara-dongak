#include "GameLogic.h"

void GameLogic::init() {
    cards.clear();

    int totalCards = rows * cols;
    int totalPairs = totalCards / 2;

    std::vector<int> pairIds;
    pairIds.reserve(totalPairs);

    for (int id = 0; id < totalPairs; ++id) {
        pairIds.push_back(id);
        pairIds.push_back(id);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(pairIds.begin(), pairIds.end(), gen);

    int index = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            GameCard card;
            card.pairId = pairIds[index];
            card.isOpen = false;
            card.isMatched = false;

            card.shape.setSize(sf::Vector2f(CARDWIDTH, CARDHEIGHT));
            card.shape.setPosition(OFFSETX + c * (CARDWIDTH + MARGIN), OFFSETY + r * (CARDHEIGHT + MARGIN));

            card.shape.setFillColor(sf::Color(180, 180, 250));

            cards.push_back(card);
            ++index;
        }
    }

    firstOpenIndex = FIRST_OPEN_INDEX;
    secondOpenIndex = SECOND_OPEN_INDEX;
    moves = MOVES;
    matchedPairs = MATCHED_PAIRS;

    initialReveal = true;
    initialClock.restart();

    for (auto &card : cards) {
        card.isOpen = true;
        card.anim = 1.f;
    }
    initialClock.restart();
}

void GameLogic::updateAnimation() {
    if (state == GameState::Paused) return;

    if (initialReveal) {
        float seconds = initialClock.getElapsedTime().asSeconds();
        if (seconds > 3.f) {
            for (auto &card : cards) {
                if (!card.isMatched) {
                    card.isOpen = false;
                }
            }
            initialReveal = false;
        }
    }

    for (auto &card : cards) {
        float target = (card.isOpen || card.isMatched) ? 1.f : 0.f;

        if (card.anim < target) {
            card.anim += 0.1f;
            if (card.anim > target) {
                card.anim = target;
            }
        }
        else if (card.anim > target) {
            card.anim -= 0.1f;
            if (card.anim < target) {
                card.anim = target;
            }
        }
    }
}

void GameLogic::handleCardClick(int clickedIndex) {
    if (state == GameState::Paused) return;

    GameCard &card = cards[clickedIndex];

    if (card.isOpen || card.isMatched || waitingForPairCheck) {
        return;
    }

    if (firstOpenIndex == FIRST_OPEN_INDEX) {
        card.isOpen = true;
        firstOpenIndex = clickedIndex;
    } else if (secondOpenIndex == SECOND_OPEN_INDEX) {
        card.isOpen = true;
        secondOpenIndex = clickedIndex;

        moves++;

        waitingForPairCheck = true;
        pairClock.restart();

    }
}

void GameLogic::checkPairIfNeeded() {
    if (firstOpenIndex < 0 || secondOpenIndex < 0) {
        waitingForPairCheck = false;
        return;
    }

    GameCard &card1 = cards[firstOpenIndex];
    GameCard &card2 = cards[secondOpenIndex];

    if (card1.pairId == card2.pairId) {
        card1.isMatched = true;
        card2.isMatched = true;
        matchedPairs++;
    }
    else {
        card1.isOpen = false;
        card2.isOpen = false;
    }
    firstOpenIndex = FIRST_OPEN_INDEX;
    secondOpenIndex = SECOND_OPEN_INDEX;

    waitingForPairCheck = false;
}

void GameLogic::setAllUnmatchedCardsOpen(bool open) {
    for (auto& card : cards) {
        if (!card.isMatched) {
            card.isOpen = open;
        }
    }
}
