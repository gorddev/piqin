### Readme incomplete
Each of the directory names are pretty self-explanatory, but this is a game engine + a game I'm building on top of it.

Current updates on progress

- Working on engine <<< currently here
- Working on game
- Done with game

I am working on:
- Physics
- Tilesets
- Sound

---
## Documenting the Engine Structure
The Engine is slowly growing into a more complicated beast, so I thought it best to start documenting the actual structure of how things are layed out. Please note this engine is build off of the hard work done by the people making `SDL3`. 
### The Engine
The `engine` itself is just a `C++ object` that contains a couple key subobjects that do all the heavy lifting. The Engine contains the following member variables:

- `Layer` › This is the central unit the engine works in. One layer is essentially a sandboxed environment of `Sprites`, `Particles`, `Banners`, `Tilesets`, and `Textures` that operates entirely independently of the engine. If it had direct access to the window, it could even render itself. However, there are several key components layers cannot directly access:
  - `input` › Input is routed to the `Layer`, but does not have direct access to keyboard input
  - `renderer` › The Layer cannot render itself and must rely on the renderer to compose it's components
  - `debug logging` › The layer does not have access to the central logging system and must make requests to the `EngineContext` to contribute to the running system log. 
    - `time` › The layers recieve a centralized `dt` from the engine via the `EngineContext`. They do not make time themselves. 


- `EngineContext` › Contains all the important information that makes running the engine possible, like `time`, `scene width, height`, and direct access to the `debug log`.


- `Renderer` › Handles the rendering of everything in the scene. The Renderer passes a `RenderBuffer` to all layers, and they append to the buffer via the buffer's member functions while updating it with texture information. Then, the renderer attempts to batch as many draw calls as possible together at render time. The renderer also handles the connection between the program and the output window.


- `InputManager` › The inputManager primarily serves to route input to the appropriate channels. Without any modifications by the user, input is directly routed to the active `Layer`'s `InputHandler`. This `InputHandler` then does the following:
  - If the user specifies any `Sprite`, `Banner`, or `Particle` as a `keyboard_reciever` or `mouse_reciever` via one of the Layer's member functions, the Layer routes the input to their respective `on_hover`, `on_click`, `on_keyboard_press`, ect. functions. 
    - If this object is further specified as `draggable`, and it is the object with the highest `z-position` the mouse is hovering over, the `Mouse` object will drag the object across the screen and repeatedly call it's `on_drag(dx, dy)` function.  
  - However, if the user wants to send input to a place other than directly to a `Layer`, the user can create a subclass of the `InputRouter` class. The `InputManager` will always prioritze sending input to `Routers` before `Handlers`, and if `Routers` want to prevent input from propogating to routers and handlers down the chain, they return true on one of their callback functions, consuming the input. 

- `Debug` › Debug is super limited right now and is basically just printing logs and filtering based on severity, and then displaying hitboxes on the screen for `Sprites`, `Banners`, and `Particles`. 

### Layers
Now here's where the real meat and potatoes are. Layers basically do everything and nothing at all at the same time. They serve as sandboxed containers to handle game events and updating objects, and you can also make subclasses of them for specialized behavior (like implementing physics or collisions), but those specialized behaviors are *usually* best handled in game logic and not in the Layer itself. 

Before explicitly discussing layers, it's important to note that just like the engine has an `EngineContext`, Layers have a `LayerContext` object that sandboxes time, speed, and other resources for objects in the layer. Note that there are no protections for adding objects to multiple layers, and that is undefined behavior. 

Here's what layers primarily do:
- Manage all the `Sprites` (internall called `Sprites`) and update both their frames and their motion.
- Manage all the `animations`, by storing the normalized render vertices in memory containers for quick access and reference to by `Sprites`.
- `Banners` › Handles the display of text and UI elements both on the `canvas resolution` and on the scale of the `scene_resolution` via wrangling the `LayerContext` for everything it has. `Banners` take in `Widget` objects to form the cohesive menu. This makes it much easier to form things like dialouge boxes or complex UI elements with images that is fully left up to user control.
- `Cells` › Cells are a debug class that can be used to display normalized coordinates of a texture directly to the canvas, on top of everything else in the layer. Very primitive.
- `Events` › Every Layer also has it's own event queue that is made up of three separate queues that behave differently. Events activate once their internal timer hit zero:
  - `Relative Queue` › Only the frontmost event tick down in time, and all events further back in the queue must wait for the one in front to finish before beginning
  - `Absolute Queue` › An event in the absolute queue will execute in the time specified, regardless of other events in the queue (so it's not really a queue)
  - `Epoch Queue` › An event queue that blocks all other events from ticking down in time or activating until the entire `Epoch Queue` is empty. Good for cutscenes.
- `Font` › Layers must also internall manage all their own fonts. The engine always has access to a low-resolution `sys-font`, but any other fonts to be used in a layer must be explicitly declared and prepped by the user.
- `Initializer` › Very chunky and very annoying class. The initializer preps the Layer to properly be set up for use. When the user calls the initializer, the initializer essentially queues up all the assets to be converted to textures and preps`Font`, `Animation` and `Tileset` caches to be converted into their final form. Then, when `engine.compose_layer(Layer)` is called, the `Renderer` swoops in and renders all the textures in one fell swoop and stores the results in the Layer's `TextureRegistry`. 
- `Morphs` › morphs are effects that you can apply to `Sprites` `Particles` and `Banners` that modify their display upon rendering. For example, the `Morph` `gfx::Stretch` when applied to `Sprites` stretches the texture in the direction they are moving. Although not explicitlt enforced, `Morphs` should not update `position`.
- `Particles` › Particle effects are incredibly versitile but also very low level, such that if a user wants to create a particle effect, they must specify the exact vertices on the screen where that particle must be rendered. They create a `ParticleGroup` object and then define how each particle in that `ParticleGroup` will be rendered. However, a wonderful consequence of this is that particles are incredibly efficient, even on a web environment. Generating `30000` particles per second is possible while retaining 60fps on a web browser. 
- `Physics` › Unimplemented, but it'll be a pain
- `Routes` › Routes are a counterpart to `Morphs` that do not affect rendering, but change the `position` of an object by sending it towards a target. Routes self-destruct once they reach their destination, and cannot exist without something to move. However you can make things look real pretty with a route + particle combination.
- › `Tilesets` › unimplemented.

Now this is a lot for one `Layer` object to handle, and seems a lot to get your head around, but the nice thing is that most of this is nicely abstracted away from you such that if you want to create a piece of text, all you do is do something like:

```
Banner b = new Banner({0,0}, 40, 40); //pos, width and height
Text t = new Text("Hello World!\n^c:red^How's it goin?", layer->get_font (MY_FONT)); //makes text with the folling font, with "How's it goin?" being colored red with custom command
b->add_widget(text, Align::CENTER); // Adds a widget with center alignment.
layer->add_banner(b);
```

If this seems like a lot to you just to render a piece of text, DO NOT FRET. The `Layer Library` comes to the rescue. If you find yourself making banners, sprites, or particles with similar structures often, you can do the following:

```
MyCustomSubclass jerry;
layer->lib.archive<MyCustomSubclass>(jerry, "my boy");
```
Now, whenever you want your subclass back (or want it immediately rendered), you can do:
```
// gets a copied object
auto astroBoy = layer->lib.get<MyCustomSubclass>("my boy");
layer->add_sprite(astroBoy,);
// immediately adds to layer
layer->add_from_lib<MyCustomSubclass>("my boy");
```

### General Other Stuff
There's some other things that don't fit neatly into the discussion above. 
#### Namespacing
The whole engine is namespaced with `namespace geng`, although there are two exceptions:
- Non-essential engine objects (like custom `Routes` or `Morphs`) are prefaced with `gfx` instead of `geng`, to distinguish them from vital engine behavior. For example, the `gfx::Stretch` or `gfx::Shake` `Morphs` are nice to have but not necessary. The same occurs with `Routes`, like `gfx::Balloon`, which sends a target on a balloon-like trajectory.
- Debug functonally like logging is gated behind `geng::debug`. This is to make it explicit when using debug features in the engine. 
#### Custom Object Types

There's a couple other important object types not discussed above:
1. `FPos2D` is the object the engine uses for all positioning. x and y represent the object's **center** on the canvas, and the `z` determines rendering order and mouse-selection.
2. `Gear` is the fundamental unit the renderer renders apart from `Tilesets.` `Sprites`, `Particles`, and `Banners` are all `Gears` so you see generic functions like `attach_particle` contian a `Gear*` instead of one of them specifically.
3. `Transform2D` is the fundamental object that Gears have that keep track of width, height, scale, rotation, ect. 
4. `SparseVector` is a custom object that is a vector but with pointers that go null when you erase elements. This means that you can retain ordering with `O(1)` removal anywhere as long as you know the index. It's very situational. 
5. `Pos2D` `Dim2D` `Box2D` `Directional2D` are all simple structs that help pass around information while retaining readability. For example, a `Pos2D` is just two integers, x & y. `Dim2D` is the same thing with shorts w & h. a `Box2D` is basically just a `Pos2D` and `Box2D` that has real handy `to_vertex` functions that automatically calculate the FPos2D points if you were to split it up into two quads. `Directional2D` is just a box but the variable names are `top, bottom, left,` and `right` and they're all shorts too. 
6. `Quad` a quad is an enhanced `Box2D` that has even better functions to help convert simple coordinates into `Fonts`, `Frames`, ect. 
7. `Texture` is a texture contains a pointer to the texture object in memory (a `SDL_Texture*`), and an `IMG_Info` object, which contains the width, height, and filename of the texture. 
### Engine Quirks
Making an engine poses several design challenges, and a couple sacrifices had to made along the way that makes wrangling certain aspects of the engine difficult.
- `Banner` texture atlasing: all elements in a `Banner` must come from the same source `.png`. This usually isn't an issue, but say for example your font and your image in a banner are in two separate files. You cannot make a banner with both, and instead have to make two banners. This is intentional. Switching between different textures frequently is bad for rendering because each time you switch a texture in a rendering iteration, it is a call to the JavaScript backend of Emscripten + WebAssembly, which is slow. 
- `Texture loading` › There's only support for .png right now. 
- `I change your texture` › For every texture in the engine, I automatically scale each dimension to the next power of two. This helps keep rendering consistent on a wide variety of textures. However, however, however, most *egregiously* is that I append one white pixel to the end of every single texture, and you can imagine that is not good. But let me tell you why it's good: this way, I can ensure that every texture has one *completely white point*, so that means say you want to render a particle effect of a single color. Instead of switching to the `null texture` to render it, I can keep the render call in the same texture. Thus, no matter how many particels you render or where you render them in the pipeline, particle effects _do not switch textures_, so essentially every texture is automatically atlased for every particle effect. 
- `Loading spritesheets` › There's no support for loading spritesheets right now, so essentially what you have to do is construct any spritesheet manually in a `.hpp` file yourself. This is horrible and will be changed, but there's some nice `FrameTable` constructor functions that make this easier for the time being. 