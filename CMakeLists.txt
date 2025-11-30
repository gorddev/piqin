cmake_minimum_required(VERSION 3.13)
project(norton)

set(CMAKE_CXX_STANDARD 17)

# These flags get passed when using emcmake
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(norton
        # Main and engine
        src/engine/Engine.cpp
        src/main.cpp
        # Game Logic Below
        src/game/GameMaster.cpp
        # Blackjack
        # Player and hand
        src/game/blackjack/BlackjackPlayer.cpp
        src/game/blackjack/Hand.cpp
        # Cards
        src/game/blackjack/cards/Card.cpp
        src/game/blackjack/cards/CardLogic.cpp
        src/game/blackjack/cards/CardSelector.cpp
        src/game/blackjack/cards/CardDraw.cpp
        src/game/blackjack/cards/Deck.cpp
        # Logic
        src/game/blackjack/bj-logic/BJ-SelectionLogic.cpp
        src/game/blackjack/bj-logic/BJ-InputRouting.cpp
        src/game/blackjack/bj-logic/BJ-CardLogic.cpp

        # Game Engine Below

        # Input
        src/engine/input/InputManager.cpp

        # Objects
        src/engine/objects/Path.cpp
        src/engine/objects/Object.cpp
        src/engine/objects/Shake.cpp
        src/engine/objects/ObjectManager.cpp

        # Particles
        src/engine/particles/ParticleManager.cpp

        # Rendering
        src/engine/render/RenderManager.cpp
        src/engine/render/Dst-Rects.cpp
        src/engine/render/Background.cpp

        # Sprites
        src/engine/sprites/SpriteSheet.cpp
        src/engine/sprites/SheetManager.cpp

        # Utilities
        src/engine/utilities/BasisDecoder.cpp
        src/engine/utilities/Utilities.cpp
        src/engine/utilities/types/Vertex.cpp
        src/engine/utilities/basisu_transcoder/basisu_transcoder.cpp
        src/engine/utilities/zstd/zstd.c

)

target_include_directories(norton PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include/scene
    ${CMAKE_SOURCE_DIR}/src/scene
)

# Emscripten-specific compile + link flags
target_compile_options(norton PRIVATE 
    "-sUSE_SDL=2"
    )
target_link_options(norton PRIVATE 
    "-sUSE_SDL=2"
    "--shell-file=${CMAKE_SOURCE_DIR}/assets/game_shell.html"
    "--preload-file=assets"
    "--emrun"
    "-sASSERTIONS=3"
    "-sMAXIMUM_MEMORY=128MB"
    "-sALLOW_MEMORY_GROWTH"
    "-sUSE_SDL_TTF=2"
)

set(CMAKE_TOOLCHAIN_FILE "/Users/gordie/emsdk/upstream/emscripten/.../Emscripten.cmake")