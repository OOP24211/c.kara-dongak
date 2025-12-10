cmake_minimum_required(VERSION 3.16)
project(MemoryGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Укажи путь к папке SFML-2.6.2
set(SFML_DIR "/Users/coksumkara-dongak/SFML-2.6.2/lib/cmake/SFML")

# [cite_start]Ищем SFML 2.6 с компонентами (всё с маленькой буквы!) [cite: 1]
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(MemoryGame
    main.cpp
    Card.h
    constants.h
    Game.cpp          # Изменено имя
    Game.h            # Изменено имя
    AssetManager.h    # Новый файл
    GameLogic.h       # Новый файл
    GameLogic.cpp     # Новый файл
)

# Линкуемся со старыми именами библиотек
target_link_libraries(MemoryGame
    sfml-graphics
    sfml-window
    sfml-system
)