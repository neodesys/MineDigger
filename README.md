MineDigger
==========

**Contents**

1. [About the game](#about-the-game)
2. [Build the game](#build-the-game)
3. [Run the game](#run-the-game)
4. [Source code organization](#source-code-organization)

--------------------------------------------------------------------------------

About the game
--------------

MineDigger is a small prototype of a classical 2D sprite-based board game for
desktop computers (Linux and Windows).

It's an hypercasual match-3 game like Bejeweled or CandyCrush in its simplest
expression (no metagame, no levels, no evolution, just a single and simple core
main loop).

First challenge was to create in 5 days a full-featured game engine from scratch
for the core main loop using SDL2 and C++.

Then, extra features like sound, fonts, path-based animations and score have
been added to make the game more complete compared to a real game.

The full game code is available here and released under the GPL-3 license in the
hope that it may be useful to anybody starting with SDL2, sprites manipulation
or game crafting.

Associated game resources have been downloaded from the Internet with free
licenses for non-commercial usage. If you are the owner of any of these
resources and you changed the license type or you just want to be credited for
it, please contact me at contact@neodesys.com

--------------------------------------------------------------------------------

Build the game
--------------

### Prerequirements

First of all, clone the repository:

```
$ git clone https://github.com/neodesys/MineDigger.git
$ cd MineDigger
```

You can build the game on Linux using GNU make and g++, or on Windows using
either MinGW (under an environment providing GNU make like Cygwin) or Microsoft
Visual Studio.

In all cases you will need a full-featured C++11 compiler and its tool suite:

- [g++](https://gcc.gnu.org/) (version 4.8 and above)
- [MinGW-w64](http://mingw-w64.org/) (version 3.0 and above)
- [Visual Studio](https://www.visualstudio.com/) (version 2015 and above)

To build the game under Linux, you will need to install SDL2 libraries:

- libsdl2-dev
- libsdl2-image-dev
- libsdl2-mixer-dev
- libsdl2-ttf-dev

To build the game under Windows using MinGW, you will need to install
[Cygwin](https://www.cygwin.com/) with, at least, the following packages:

- make
- ncurses
- mingw64-i686-gcc-g++
- mingw64-x86_64-gcc-g++ (only if you have a 64 bits system)

To build the game under Windows using Microsoft C++ compiler, you just need to
open the Visual Studio solution within */ide/msvc/MineDigger.sln*.

Under Windows, with MinGW or Microsoft C++ compiler, all dependencies are
already provided within the */ext* directory.

### Building from an IDE

Project files are provided for:

- [Visual Studio](https://www.visualstudio.com/) within */ide/msvc* folder
- [Netbeans](https://netbeans.org/) within */ide/nbproject* folder

Just open the project file in your favorite IDE and launch the build command.  
Configurations are provided for debug and release builds in 32 and 64 bits.

### Building from the command line

You can cross-build for Linux and Windows (using MinGW) from a Linux host, or
just build for Windows (using MinGW) from a Windows host using Cygwin.  
All build instructions are contained in the *Makefile* file.

Make options are:

- `BUILD` defines the build mode which can be `debug` (default) or `release`
- `PLATFORM` defines the compiler suite which can be `linux` (default) or
  `mingw`
- `ARCH` defines the architecture which can be `64` (default) or `32`

Thus, you can call the following commands to build:

- for Linux 64 bits

```
$ make PLATFORM=linux ARCH=64 BUILD=debug
$ make PLATFORM=linux ARCH=64 BUILD=release
```

- for Linux 32 bits

```
$ make PLATFORM=linux ARCH=32 BUILD=debug
$ make PLATFORM=linux ARCH=32 BUILD=release
```

- for Windows 64 bits (using MinGW)

```
$ make PLATFORM=mingw ARCH=64 BUILD=debug
$ make PLATFORM=mingw ARCH=64 BUILD=release
```

- for Windows 32 bits (using MinGW)

```
$ make PLATFORM=mingw ARCH=32 BUILD=debug
$ make PLATFORM=mingw ARCH=32 BUILD=release
```

### Build output

Builds intermediate files are produced within a */build/* sub-directory
named against the build configuration:
`(linux|mingw|msc)(32|64)_(debug|release)`.

Builds output files are produced within a */bin/* sub-directory named against
the targeted system: `(linux|mingw|msc)(32|64)` and the executable file is
called `MineDigger_debug(.exe)` or `MineDigger_release(.exe)` depending on the
build flavor.

All files needed for runtime (resources and shared libraries) are automatically
copied into the right sub-directory during the build process.

Calling `make clean-all` cleans all builds intermediate and output files for
all configurations.

--------------------------------------------------------------------------------

Run the game
------------

As stated above, under Windows all runtime prerequisites are already copied into
the executable folder, so you just have to launch the executable file to run the
game.

Under Linux, you must install SDL2 libraries before running the game: libsdl2,
libsdl2-image, libsdl2-mixer, libsdl2-ttf and extra libraries for loading
resources: libvorbis, libjpeg and libpng.

The game includes a log system with 3 levels of verbosity: *info*, *warning*
and *critical*.  
By default, debug versions of the game are configured with the *info* level and
release versions with the *critical* level.  
You can change the default verbosity from the command line using the `-l` or
`--log` option with one of the following arguments:

- `info` for logging all messages
- `warn` or `warning` for logging *warning* and *critical* messages only
- `crit` or `critical` for logging *critical* messages only

--------------------------------------------------------------------------------

Source code organization
------------------------

### Folders

Repository root folder contains the following sub-directories:

- */ext* contains any external library source code and/or binaries. Each
  third-party library is provided in its own sub-directory containing its
  version information and license, *bin*, *include* and *lib* sub-directories.
  All Windows shared libraries (DLL) included into *bin* sub-directories are
  automatically copied into the right output directory during the build
  process.

- */ide* contains project files for Microsoft Visual Studio IDE in */ide/msvc*
  and Netbeans IDE in */ide/nbproject*.

- */res* contains runtime game resources. All these resources are automatically
  copied into the right output directory during the build process.

- */res-src* contains resources sources used to make the start and score game
  screens. These sources have been created using
  [The Gimp](http://www.gimp.org/).

- */src* contains code sources.

- root directory contains this *README.md* file and the *Makefile* file.

*/bin* and */build* folders are produced during the build process and can be
safely deleted when not needed anymore. They are automatically deleted when
calling `make clean-all` command.

### Source code

The source code is organized around 3 main namespaces: *sys*, *app* and *game*
representing the application layers:

- the lowest system layer *sys* is in charge of all system objects, all SDL2
  calls are exclusively made from this layer,

- the intermediate generic application layer *app* uses the system layer to
  provide application basic mechanisms like application main loop, sprites and
  animations,

- the upper game layer *game* uses the application and system layers to
  implement the game itself. It defines game specific behaviors through 3
  nested namespaces implementing each one a different game screen: *start*,
  *play* and *score*.

```
      _______________________________________
     |             |            |            |
     |    start    |    play    |   score    |
     |_____________|____________|____________|
     |                                       |
     |                  game                 |
     |_______________________________________|
      _______________________________   _____
     |                               | |     |
     |               app             | |     |
     |_______________________________| |     |
      _________________________________/     |
     |                                       |
     |                  sys                  |
     |_______________________________________| game code
 --------------------------------------------------------------------
      _______________________________________  third-party libraries
     |                                       |
     |                 SDL2                  |
     |_______________________________________|
      _________   _______   _________   _____
     |         | |       | |         | |     |
     | Windows | | Linux | | Android | | iOS |
     |_________| |_______| |_________| |_____|
```

All namespaces are organized hierarchically into sub-folders named against the
namespace.

**The *sys* namespace**

The *sys* namespace contains all elements in relation with the system using
SDL2 for portability.

One of the most important classes is `GameEngine` in charge of the system
initialization at the very beginning and system cleanup at the end of the game.

This `GameEngine` initializes and keeps track of sub-systems:

- `ResLoader` which provides low-level loading of game assets
- `AudioMixer` which provides access to sound samples and music
- `Renderer` which provides access to graphics

In parallel with these sub-systems, the *sys* namespace defines resources
managing classes:

- `AudioSample` and `Music` for sounds effects and music
- `Texture` for surface drawing
- `Font` for text drawing

Eventually, the *sys* namespace implements the logging system through the
`Logger` class and the base interfaces needed by the upper layer to specify
user inputs: `IMouseListener`, drawing: `IDrawable` and animations: `IAnimated`.

The `GameEngine`, sub-systems and all resources are created using auto-factory
patterns to avoid C++ exceptions overhead and provide better control on
resources allocation for future improvements (memory pools, etc...).

**The *app* namespace**

The *app* namespace uses the *sys* namespace to provide application basic
mechanisms.

It implements the application main loop `app::run(IGame& game)` within
*app.cpp*, using only the `IGame`, `IGameScreen` and `IResHolder` interfaces.

Any game must implement the `IGame` interface to be entirely managed by the
*app* system.

A game is composed of one or more game screens, implemented through the
`IGameScreen` interface.

A game screen is mainly a self-contained game level which resources are loaded
before running the level and cleaned up after.  
No extra resource can be loaded, nor cleaned up, during the game level
execution.

All game resources (shared from the `IGame` object or specific to an
`IGameScreen` level), are loaded and cleaned up through the `IResHolder`
interface.  
This latter interface allows sequential loading of resources while displaying
a default loading drawable provided by the `IGame` object.

Here is the complete game lifecycle management provided by the *app* system:

* `IGame` initialization:

    - Loading drawable creation through createLoadingDrawable(engine). This
      method must be simple and return quickly. It should create the loading
      `sys::IDrawable` that will be displayed during resources loading phases of
      the lifecycle.

    - Loading drawable recuperation through getLoadingDrawable(). This method
      is called only once to get a pointer to the loading `sys::IDrawable`
      object. This pointer is stored for further use whenever a loading
      drawable needs to be displayed. If returning nullptr (no specific loading
      drawable), the default blank screen (a black screen) will be used.

    - Shared game resources loading: if getResState(nullptr) returns READY, the
      system goes directly to the game main loop (no shared resources to load),
      else if it returns LOADING, the loading drawable is displayed and
      getResState(pEngine) is looped over until it returns READY.

* `IGame` main loop:

    - onGameStart(mixer) is called, all game states and audio volumes should be
      reset here. All shared resources are guaranteed to be initialized.

    - The game main loop itself: getCurrentGameScreen() is called at the
      beginning of each iteration, switching active game screen on change
      (see `IGameScreen` lifecycle hereafter).

    - onGameEnd(mixer) is called, all game states cleanup and stopping/pausing
      audio samples/music should be performed here if necessary.

* `IGame` cleanup:

    - Shared game resources destruction through cleanRes(true). This method
      call must be transitive and also call all embedded game screens and other
      `IResHolder` objects cleanRes(true) method.

    - Loading drawable destruction through destroyLoadingDrawable().

Each time the active game screen changes during the `IGame` main loop, it
follows this full lifecycle:

* `IGameScreen` local resources loading: if getResState(nullptr) returns READY,
  the system goes directly to screen main loop (no resources to load), else if
  it returns LOADING, the game loading drawable is displayed and
  getResState(pEngine) is looped over until it returns READY.

* `IGameScreen` main loop:

    - onGameScreenStart(mixer) is called, all screen states and audio volumes
      should be reset here. All screen local resources (as well as game shared
      resources) are guaranteed to be initialized.

    - The screen main loop itself, processing at each iteration:

        + inputs (through `sys::IMouseListener` methods)
        + animations (through `sys::IAnimated` method)
        + drawing (through `sys::IDrawable` method)

    - onGameScreenEnd(mixer) is called, all screen states cleanup and
      stopping/pausing audio samples/music should be performed here if
      necessary.

* `IGameScreen` cleanup: local resources may be destroyed here through
  cleanRes(false). It's up to the game screen to decide if it destroys its
  local resources here or not. In all cases, game screen local resources will
  be destroyed during the `IGame` cleanup phase when game is terminating. The
  method call must be transitive and also call all embedded `IResHolder`
  objects cleanRes(false) method.

*Note*: full-level game screens with heavy local resources should always
destroy its local resources on cleanup phase, while transition screens
displayed regularly (menu, options, etc...) with few resources may not.  
Indeed, transition screens may be pre-loaded during game shared resources
loading phase in order to allow smooth transitions.

In parallel with the game and game screens lifecycles management, the *app*
namespace provides generic game objects with basic behaviors.  
Those objects are organized around 2 main categories: sprites and drawers.

A sprite is in charge of the game object position:

- `Sprite` base class provides static position and scale.

- `DynSprite` child class provides dynamic position using a time corrected
  Verlet integration.

- `PathSprite` child class provides animated position and scale interpolated
  along a key-frame path (using linear or Catmull-Rom interpolation).

Sprites are drawn using a delegate implementing the `ISpriteDrawer` interface:

- `TextureDrawer` provides mechanisms to draw a `sys::Texture` resource using
  clipping (for texture atlas) and sub-images (for texture animation).

- `NumberDrawer` provides mechanisms to draw a number with a shadow using a
  `NumberStamp` resource.

Eventually, the `BackBoard` class provides mechanisms to draw a simple
static background using a `sys::Texture` resource or not.

**The *game* namespace**

The *game* namespace implements the game itself.

It is organized around 3 sub-namespaces, each of them implementing a specific
game screen: *start*, *play* and *score* screens.

Each screen sub-namespace includes a main class implementing the
`app::IGameScreen` interface: `start::StartScreen`, `play::PlayScreen` and
`score::ScoreScreen`.

Those latter classes are in charge of managing screen local resources and
specific game objects.

The game itself, implementing the `app::IGame` interface, is defined in the
`MineDigger` class.  
This class is in charge of managing game shared resources and cycling between
game screens: start -> play -> score.

In order to simplify game configuration, all game parameters have been
centralized within the `MineDigger` class and are defined in a separate file:
*MineDigger.cfg* included statically in the *MineDigger.o* compilation unit.

**The *game::start* and *game::score* screens**

Apart from the main class implementing the `app::IGameScreen` interface, both
screen sub-namespaces include game objects with specific behaviors.

- `start::StartSprite` is an `app::DynSprite` embedding an `app::TextureDrawer`.

- `start::StartBoard` is a `start::StartSprite` embedding a second
  `app::TextureDrawer` in order to draw an overlay when mouse goes over the
  start button or clicks on it.

Both objects behavior is to go out of the screen when user clicks on start
button.

- `score::ScoreSprite` is an `app::Sprite` embedding an `app::TextureDrawer` and
  an `app::NumberDrawer` to draw and animate the text "Score: [user score]".

- `score::BackButton` is an `app::Sprite` embedding two `app::TextureDrawer`
  objects in order to draw the back button and an overlay when mouse goes over
  or clicks on the back button. On back button click, the game starts again.

**The *game::play* screen**

This namespace implements the game main screen.

Apart from `PlayScreen` main class in charge of managing screen local resources,
the namespace includes game objects with specific behaviors:

- `SparkSprite` is an `app::PathSprite` embedding an `app::TextureDrawer` in
  order to draw the spark climbing up towards the dynamite.

- `ScoreDisplay` is an `app::Sprite` embedding an `app::NumberDrawer` in order
  to draw the user current score.

- `Countdown` is an `app::Sprite` embedding an `app::NumberDrawer` in order to
  draw the playtime countdown.

All other namespace objects implement the gem board game.  
This board is based on a Model-View-Controller pattern.

The model is implemented by the `GemBoardModel` class, offering 3 simple
commands:

- *resetBoard* to initialize a new game board,
- *swapGems* to try to swap two gems,
- *tryCollapse* to detect contiguous gems and process gems elimination if
  required.

The view is implemented by the `GemBoardView` class.  
This object listens to model changes through different callbacks:
*onBoardReset*, *onCancelSwap*, *onValidateSwap*, *onGemCreated*,
*onGemDestroyed* and *onGemFallen*.

The view maintains a visual state of the model through an array of `GemSprite`
sprites.

The controller behavior is shared between the `GemBoardView` and the
`GemSprite` objects.

Each `GemSprite` is an `app::DynSprite` embedding an `app::TextureDrawer` to
draw a particular gem.  
It maintains an internal state: *RECYCLED*, *FALLING*, *IN_PLACE*,
*SPRING_ATTACHED* or *THROWN_OUT*, and adapts its behavior according to its
current state.  
It also keeps track of the model row and column and of a target position
corresponding to its place in the view.

* A *RECYCLED* gem sprite does nothing, it is free to be used and placed into
  the view. This is the initial state of all `GemSprite` objects.

* When the board is reset (onBoardReset callback), the view initializes gem
  sprites for all model rows and columns.  
  For each model slot, it sets corresponding sprite target position, texture
  (according to the model gem type) and sprite state to *FALLING*
  (`GemSprite::initGem`).

* While in *FALLING* state, gem sprite falls down under gravity until it reaches
  its target position.  
  Then it stops, switches state to *IN_PLACE* and triggers model *tryCollapse*
  command to check if this movement can trigger gems elimination.

* While in *IN_PLACE* state, gem sprite can be selected by user and moved
  around.  
  When a sprite position is set away from its target position, its state
  switches to *SPRING_ATTACHED* and then it tries to get back to its target
  position by simulating a damped spring.  
  When reaching again its target position after a *SPRING_ATTACHED* state, the
  sprite state switches back to *IN_PLACE*.

* When a gem sprite is moved above another gem slot or two contiguous gems are
  selected, the view triggers a "try-swap" procedure:

    - target positions of swapped sprites A and B are exchanged but each gem
      sprite keeps its tracking model row and column (`GemSprite::trySwapGem`);

    - as swapped sprites A and B are now away from their new target positions,
      they automatically switch to *SPRING_ATTACHED* state and try to reach
      their target positions using the damped spring simulation;

    - when the first sprite reaches its new target position, it calls the model
      *swapGems* command;

    - the model processes the swap and emits a *CancelSwap* or *ValidateSwap*
      event received by the view through its *onCancelSwap* or *onValidateSwap*
      callback;

    - if the view receives a *CancelSwap* event, it just restores sprites A and
      B target positions (`GemSprite::cancelMove`) and both sprites will move
      back to their previous positions using the damped spring simulation;

    - if the view receives a *ValidateSwap* event, it confirms swap to sprites
      A and B (`GemSprite::validateMove`) and exchanges model rows and columns
      between A and B.  
      When sprites A and B finally reach their new target positions, they both
      trigger model *tryCollapse* command to process gems elimination.

* When the model processes the *tryCollapse* command, it can emit 3 different
  events: *GemDestroyed*, *GemFallen* and *GemCreated*. These events are
  received by the view through its corresponding callbacks.

* When a gem is destroyed (onGemDestroyed callback), the view throws out the
  corresponding gem sprite (`GemSprite::throwGemOut`).  
  Then the sprite switches to *THROWN_OUT* state and "jumps out" of the screen
  under gravity.  
  When getting out of the screen, the sprite switches back to *RECYCLED* state
  and is ready for another cycle.

* When a gem has to fall down to a lower row (onGemFallen callback), the view
  sets the corresponding sprite new target position and switches its state to
  *FALLING* (`GemSprite::fallGem`).  
  Then, the sprite falls down to its lower position as explained hereabove
  (this movement will trigger another *tryCollapse* command when the sprite
  reaches its target position).

* When a new gem is created (onGemCreated callback), the view takes a free
  sprite from the *RECYCLED* pool and initializes a new gem
  (`GemSprite::initGem`).  
  It does exactly the same thing as during the *onBoardReset* callback but for
  only one particular sprite and not for all game board slots.

It is guaranteed that, for each distinct column, the model will always emit
*GemDestroyed* events before *GemFallen* events and before *GemCreated* events.
So, created and fallen gems always take place to an empty slot.

--------------------------------------------------------------------------------
