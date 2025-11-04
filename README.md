# Engine
A multiplatform, data-driven 2D game engine written in C++ and scriptable in Lua.
### Supported platforms
* Linux
* Windows
* MacOS
* Android

### Freatures
* Text rendering (highly performant, font atlas based) - uses TrueType vectorized fonts
  * Font rasterization takes place in a background thread
* PNG texture loading
* Sprite rendering
* Tilemap loading (Tiled json format)
* Touchscreen support on Android (multitouch)
* Keyboard support
* Mouse support (currently supports only click events)
* Multithreaded asset loading
* Shader-based OpenGL/OpenGL ES rendering pipeline
* Lua API that supports most of the above features

### Requirements
* Graphics card compatible with one of the following:
  * OpenGL 3.0+
  * OpenGL 2.1 with GL_EXT_framebuffer_object extension
  * OpenGL ES 2.0

  The graphics API is detected automatically on launch.

### Building
* An example project that uses the engine as a submodule can be found [here](https://github.com/Mandeson/Engine-Demo.git)
* The project uses CMake
* All required libraries are either found in the system (default on GNU/Linux) or downloaded from GitHub
  * You can manually choose how to obtain libraries by defining the followind CMake build options to ON or OFF:
    * **ENGINE_USE_SYSTEM_LUA_LIB** - must be manually set to OFF on Debian due to different Lua header directories
    * **ENGINE_USE_SYSTEM_GLM_LIB**
    * **ENGINE_USE_SYSTEM_FREETYPE_LIB**
    * **ENGINE_USE_SYSTEM_NLOHMANN_JSON_LIB**
* The main script, **game.lua**, must be placed in assets/scripts/ directory