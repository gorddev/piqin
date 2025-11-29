cmake_minimum_required(VERSION 3.13)
project(norton)

set(CMAKE_CXX_STANDARD 17)

# These flags get passed when using emcmake
set(CMAKE_EXECUTABLE_SUFFIX ".html")


add_executable(norton
        include/Engine.hpp
        src/main.cpp

        src/generics/Vertex.cpp
        src/generics/Path.cpp
        src/generics/Object.cpp
        src/generics/Shake.cpp

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

        src/input/InputManager.cpp

        src/scene/ObjectManager.cpp

        src/scene/render/RenderManager/RenderManager.cpp
        src/scene/render/RenderManager/dst-rects.cpp
        src/scene/render/RenderManager/Render-fv.cpp
        src/scene/render/RenderManager/canvas.cpp
        src/scene/render/RenderManager/Shadows.cpp
        src/scene/render/RenderManager/Render_Rect.cpp
        src/scene/render/Background.cpp

        src/scene/textures/SpriteSheet.cpp
        src/scene/textures/SheetManager.cpp

        src/utilities/BasisDecoder.cpp
        src/utilities/Utilities.cpp
        src/utilities/basisu_transcoder/basisu_transcoder.cpp
        src/utilities/zstd/zstd.c
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
)

set(CMAKE_TOOLCHAIN_FILE "/Users/gordie/emsdk/upstream/emscripten/.../Emscripten.cmake")