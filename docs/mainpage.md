# Gordie's Game Engine
\mainpage G-Engine

Welcome to my game engine's landing page! We'll shorthand "game engine " with **geng**. Let's start.

#### Quick Reference
- [Engine Hierarchy](EngineLayout.md)
- [MUST-KNOW Types](Types.md) 
- [The Engine Object](Engine.md)
- [Starter Program](beginner.md)
- [Layers](Layers.md)
- [Textures](Textures.md)

### Design Principles

**Geng** is a lightweight engine targeted at a web environment with *Emscripten* and *SDL2* running under the hood. It is completely `typed`, meaning that there is no (immediately visible) user-interface. It is meant to be an abstracted tool for those familiar with **object-oriented design**.

For an example of the object-centric nature of the engine, consider the program which initializes and runs the engine:

```c++
#include <engine/geng.hpp>

int main() {
    // Creates the engine object
    geng::Engine my_engine;
    // Initializes the engine object
    my_engine.init();
    // This is your game loop
    auto game_loop = [&]() {
        return true;
    }
    // Calls the game loop with your engine.
    GENG_START_LOOP(my_engine, game_loop);
    return 0;
}
```
The `Engine` object is the central character in everything you do, and you must speak in it's language. If you want to add renderable objects, you must make subclasses of the `Engine`'s objects, and if you want specialty behavior, you also define subclasses. 

This gets at the core of how **Geng** operates: **it's the engine's way or no way at all.** This doesn't mean that **geng** isn't flexible and highly customizable, but it does mean that you dont get to pick and choose a unique design strategy: those choices have already been made, whether you like it or not. 

Take, for example, `strings`. **Geng** does not use the `std::string` library. Instead it defines two custom string types `fstring<C>` and `hstring`. 
- `fstring<C>` (**fixed string**) is a string of fixed compile-time capacity stored on the stack, used for text objects and debugging to prevent [heap fragmentation](https://cpp4arduino.com/2018/11/06/what-is-heap-fragmentation.html).
- `hstring` (**heap string**) is a string of variable capacity stored on the heap that is used for path management & unknown file lengths.

Both of these strings are remarkably easy to use, such that piping information into them is trivially easy:

```c++
fstring<90> example = "Hello World!\n";
example << "Flexy: " << 1 << 2 << 3.3 << nullptr;
// now stored: "Hello World\nFlexy: 123.3nullptr"
```
However, if you want to interface with the engine's routines, these strings will pop up *everywhere*. Wanna make a piece of renderable `geng::Text`? Oops there's an `fstring<C>` in `Text` so  you gotta limit the size of your text out of the gate:
```c++
geng::Text<30>("my text", my_font);
```
Oh, you want to make a really long output into the `geng::dev` console? Haha nice try. Outputs are limited to 256 characters. Choose your characters carefully.  

Hey, you want to combine several textures together into a single banner? *Nope*. Not allowed. that is a design disaster for batching render calls, and slows down **geng** (and most engines) considerably. [Atlas](https://en.wikipedia.org/wiki/Texture_atlas) your textures beforehand if you want to do that. 

However, these restrictions come with some benefits to usability.
- **Speed**. This engine is fast, and can boot a window in less than two hundreths of a second under good conditions.
- **Communication**. Information abundance is a cornerstone. This means that if you want a piece of your code to know when something happens, *it will know.* Oh a mouse clicked on that object? Let's:
  - Notify the object via a callback
  - Notify each layer's `InputHandler` via a callback
  - Notify any existing`InputRouters` via a callback
  - Notify the `Console` via a callback
  - Notify the `Engine` via a callback
  - Source the click from the `InputDistributor`  
  
  This applies to pretty much every component of the Engine. Especially in a `typed` setting, having easy access to information is huge
- **Safety**. **geng** is crash-safe. No, really. It's crash-safe. If you crash your program in the game loop, your window doesn't shut down. **geng** keeps running and lets you observe the system state via the console and report any errors.
- **Debugging**. For a lightweight engine, the debugging tools are quite good. Via the `Console` in debug mode, you have access to many tools at your disposal. Furthermore, although logging is limited to 256 characters per log, here's the blessing in disguise: *no amount of log-spam will slow down the engine as long as you use `geng::note`, `geng::warn`, `geng::err`, or `geng::dev`*. 
- **Out of Box Usability**. As seen above, initializing **geng** after dev environment setup is not complicated. Just by creating the engine object, you get:
  - Window support with variable resolution
  - Mouse Support for all game objects
  - Full keyboard support (pressing and holding)
  - Debug Console and logging  

    Furthermore, once you create your first `Layer` you get full access to:
  - Texture initialization
  - Tilesets
  - Physics Engine
  - Z-Indexing
  - and much much more

I'm proud of the design work I've done so far making trade-offs and the like, and even if you may not understanding the workings of the engine, I hope you can appreciate the overall effort.

Now with that interlude ouf the way, let's move onto the hierarchy of the engine.

### Engine Hierarchy
The Engine has three primary goals:
- Make it easy to render what you want to the screen.
- Provide game-making utilities that greatly shorten the development process
- Encourage good syntax and thoughtful design through intentional design restriction.

Let's talk about "**rendering what you want**" first. The Engine is capable of rendering three primary object types, which are all subclasses of the **Gear** class: 
1. [`Sprites`](sprite.md) › An animated object that is lifetime-bound to a [`FrameTable`](frametable.md) (a table containing animation information)
2. [`ParticleGroups`](particlegroup.md) › A particle effect typically containing many particles that can optionally be attached to other **Gears**.
3. [`Banners`](banner.md) › A text-centric object that combines various [`Widgets`](widget.md) together to form a cohesive UI object.

Yet the Engine alone cannot render these types *raw*. It requires them to be wrapped in a `Layer`. Now I've mentioned `Layers` before but let's get in to what they actually are. The engine is structed as such:
```commandline
Engine object
 ├─ Console/Debug/Logging
 ├─ Timer
 ├─ Renderer
 ├─ TextureRegister
 ├─ SoundMixer
 ├─ LayerManager
 │  ├─ 'Layer 1'
 │  │  ├─ SpriteManager
 │  │  │  └─ FrameTables
 │  │  ├─ BannerManager
 │  │  │  ├─ Text
 │  │  │  ├─ Fonts
 │  │  │  └─ SyntaxMap
 │  │  ├─ ParticleManager
 │  │  ├─ Routes
 │  │  ├─ Morphs
 │  │  ├─ GameWorld
 │  │  │  ├─ Tilesets
 │  │  │  └─ Physics Engine
 │  │  ├─ EventManager
 │  │  │  ├─ Relative Queue
 │  │  │  ├─ Absolute Queue
 │  │  │  └─ Epoch Queue
 │  │  └─ Input Handler
 │  ├─ 'Layer 2'
 │  │
```


Each of these objects can optionally be tied to a **Texture** via the `Initializer`, and thus user-created artwork can be projected through these object types. 

- Fixed-size strings and buffers (`fstring<C>`) to prevent implicit allocations.
- Syntax-aware text rendering with capacity limits.
- Explicit, predictable resource management.

## Getting Started

- [Text System Overview](text_system.md)
- [SyntaxMap Overview](syntax_map.md)
- [Rendering API](class_geng_1_1_renderer.html