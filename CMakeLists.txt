cmake_minimum_required(VERSION 3.13)
project(norton)

set(CMAKE_CXX_STANDARD 17)

# These flags get passed when using emcmake
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(norton
        include/engine/Engine.hpp
        src/main.cpp

        src/engine/utilities/types/Vertex.cpp
        src/engine/objects/Path.cpp
        src/engine/objects/Object.cpp
        src/engine/objects/Shake.cpp

        src/game/GameMaster.cpp
        src/game/blackjack/BlackjackPlayer.cpp
        src/game/blackjack/bj-logic/BJ-SelectionLogic.cpp
        src/game/blackjack/bj-logic/BJ-InputRouting.cpp
        src/game/blackjack/bj-logic/BJ-CardLogic.cpp
        src/game/blackjack/Hand.cpp
        src/game/blackjack/cards/Card.cpp
        src/game/blackjack/cards/CardLogic.cpp
        src/game/blackjack/cards/CardSelector.cpp
        src/game/blackjack/CardDraw.cpp

        src/engine/input/InputManager.cpp

        src/engine/objects/ObjectManager.cpp

        src/engine/render/RenderManager.cpp
        src/engine/render/Dst-Rects.cpp
        src/engine/render/RenderManager/Render-fv.cpp
        src/engine/render/RenderManager/canvas.cpp
        src/engine/render/RenderManager/Shadows.cpp
        src/engine/render/RenderManager/Render_Rect.cpp
        src/engine/render/Background.cpp

        src/engine/sprites/SpriteSheet.cpp
        src/engine/sprites/SheetManager.cpp

        src/engine/utilities/BasisDecoder.cpp
        src/engine/utilities/Utilities.cpp
        src/engine/utilities/basisu_transcoder/basisu_transcoder.cpp
        src/engine/utilities/zstd/zstd.c
        src/game/blackjack/cards/Deck.cpp
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
    "-sASSERTIONS=2"
    "-sMAXIMUM_MEMORY=128MB"
    "-sALLOW_MEMORY_GROWTH"
    "-sUSE_SDL_TTF=2"
)

set(CMAKE_TOOLCHAIN_FILE "/Users/gordie/emsdk/upstream/emscripten/.../Emscripten.cmake")