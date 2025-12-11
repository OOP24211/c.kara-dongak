cmake_minimum_required(VERSION 3.16)
project(MemoryGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "/Users/coksumkara-dongak/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(MemoryGame
    main.cpp
    Card.h
    constants.h
    Game.cpp          
    Game.h            
    AssetManager.h   
    GameLogic.h       
    GameLogic.cpp     
)

target_link_libraries(MemoryGame
    sfml-graphics
    sfml-window
    sfml-system
)
