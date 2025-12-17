cmake_minimum_required(VERSION 3.13)
project(norton)

set(CMAKE_CXX_STANDARD 17)

# These flags get passed when using emcmake
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(norton
        src/main.cpp
        src/Card.cpp
        src/engine/Engine.cpp
        #[[
        # Main and engine


        # Game Logic Below
        src/game/GameMaster.cpp
        # Blackjack
        # Player and hand
        src/game/blackjack/Board.cpp
        src/game/blackjack/Round.cpp
        src/game/blackjack/bj-objects/Hand.cpp
        # Cards
        src/game/blackjack/cards/Card.cpp
        src/game/blackjack/bj-objects/CardSelector.cpp
        src/game/blackjack/bj-objects/Draw.cpp
        src/game/blackjack/bj-objects/Deck.cpp
        src/game/blackjack/bj-objects/Discard.cpp
        # Logic
        src/game/blackjack/bj-objects/Pather.cpp
        src/game/blackjack/cards/CardLogic.cpp
        src/game/blackjack/bj-logic/BJ-Actions.cpp
        src/game/blackjack/bj-logic/BJ-Input.cpp
        src/game/blackjack/bj-logic/BJ-CardLogic.cpp
        ]]

        # Game Engine Below
        #initialization
        src/engine/scene/initializer/Initializer.cpp

        #events
        src/engine/scene/events/EventManager.cpp

        # Input
        src/engine/input/InputSource.cpp
        src/engine/input/InputHandler.cpp

        # Layering
        src/engine/scene/Layer.cpp

        # Actors
        src/engine/scene/actors/ActorManager.cpp

        # Fonts
        src/engine/scene/font/Font.cpp

        # Effects
        src/engine/scene/morphs/MorphManager.cpp
        src/engine/scene/morphs/morph-types/Shake.cpp
        src/engine/scene/morphs/morph-types/Stretch.cpp

        # Particles
        src/engine/scene/particles/ParticleManager.cpp
        src/engine/scene/particles/particle-types/Rhombus.cpp
        src/engine/scene/particles/particle-types/Sparkle.cpp

        # Pathing
        src/engine/scene/routing/RouteManager.cpp
        src/engine/scene/routing/route-types/Balloon.cpp
        src/engine/scene/routing/route-types/Torpedo.cpp
        src/engine/scene/routing/route-types/Rebound.cpp
        src/engine/scene/routing/route-types/Linear.cpp
        src/engine/scene/routing/route-types/Sine.cpp

        # Rendering
        src/engine/render/Renderer.cpp
        src/engine/render/Background.cpp

        # Frame Management
        src/engine/scene/animation/FrameManager.cpp
        src/engine/scene/animation/FrameTable.cpp
        src/engine/scene/animation/Frames.cpp
        src/engine/scene/animation/AnimInfo.cpp
        src/engine/scene/initializer/TextureRegister.cpp

        # Utilities
        src/engine/utilities/IMGDecoder.cpp
        src/engine/utilities/Utilities.cpp

        # Types
        src/engine/types/Vertex.cpp
        src/engine/types/Quad.cpp

        # Globals
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
# regular run

target_link_options(norton PRIVATE 
    "-sUSE_SDL=2"
    "--shell-file=${CMAKE_SOURCE_DIR}/assets/game_shell.html"
    "--preload-file=assets"
    "--emrun"
    "-sMAXIMUM_MEMORY=32MB"
    "-sALLOW_MEMORY_GROWTH"
    "-sUSE_SDL_IMAGE=2"
    "-sSDL2_IMAGE_FORMATS=[\"png\"]"
)


# release build

#debug
#[[
target_link_options(norton PRIVATE
        "-sUSE_SDL=2"
        "--shell-file=${CMAKE_SOURCE_DIR}/assets/game_shell.html"
        "--preload-file=assets"
        "--emrun"
        "-sMAXIMUM_MEMORY=32MB"
        "-sALLOW_MEMORY_GROWTH"
        "-sASSERTIONS=1"
        "-sSTACK_OVERFLOW_CHECK=2"
        "-sSAFE_HEAP=1"
        "-g4"
        "-O0"
        "-gsource-map"
)
]]

set(CMAKE_TOOLCHAIN_FILE "/Users/gordie/emsdk/upstream/emscripten/.../Emscripten.cmake")
