# CacoEngine Examples

This directory contains complete game examples that demonstrate various features and capabilities of CacoEngine. Each example is a fully playable game that showcases different aspects of the engine.

## Examples Overview

### 1. Pong (`pong.cpp`)
**Classic paddle tennis game**

**Features Demonstrated:**
- Basic collision detection (ball-paddle, ball-walls)
- Input handling (keyboard controls)
- Physics simulation (ball movement)
- Score tracking and display
- Object management (paddles, ball)
- Game state management
- Boundary collision detection

**Controls:**
- Player 1: W/S keys
- Player 2: Up/Down arrow keys
- R: Restart game
- Escape: Quit

**Learning Focus:** Perfect for beginners to understand basic game mechanics, collision detection, and input handling.

### 2. Asteroids (`asteroids.cpp`)
**Space shooter with rotating ship and destructible asteroids**

**Features Demonstrated:**
- Object rotation and directional movement
- Projectile system with bullet management
- Multiple object collision detection
- Dynamic object creation and destruction
- Advanced physics (thrust, rotation, momentum)
- Screen wrapping mechanics
- Game state management with multiple levels
- Custom mesh creation for ship and asteroids

**Controls:**
- A/D: Rotate left/right
- W: Thrust
- Space: Fire bullets
- R: Restart (when game over)
- Escape: Quit

**Learning Focus:** Intermediate level showcasing advanced physics, projectile systems, and complex object interactions.

### 3. Snake (`snake.cpp`)
**Classic snake game with growing mechanics**

**Features Demonstrated:**
- Grid-based movement system
- Dynamic object creation and destruction
- Collision detection with self and walls
- Game state management and scoring
- Timed updates and game loops
- Food generation and collection
- Growing snake mechanics
- High score tracking

**Controls:**
- Arrow Keys / WASD: Move snake
- Space: Pause/Resume
- R: Restart
- +/-: Adjust speed (cheat codes)
- Escape: Quit

**Learning Focus:** Grid-based games, timed updates, and self-collision detection.

### 4. Breakout (`breakout.cpp`)
**Block destruction game with physics-based ball**

**Features Demonstrated:**
- Block destruction mechanics
- Ball physics with realistic bouncing
- Collision detection with multiple objects
- Level progression and block layouts
- Lives system and game over conditions
- Different block types with multiple hit points
- Paddle-based ball control

**Controls:**
- A/D or Arrow Keys: Move paddle
- Space: Launch ball
- R: Restart (when game over)
- Escape: Quit

**Learning Focus:** Multi-object collision systems, level design, and physics-based gameplay.

### 5. Particle System Demo (`particle_demo.cpp`)
**Visual effects showcase with multiple particle types**

**Features Demonstrated:**
- Custom mesh creation and manipulation
- Particle systems with physics
- Advanced rendering techniques
- Mouse interaction for particle generation
- Dynamic object management
- Visual effects and animations
- Performance considerations with many objects
- Multiple particle effect types

**Controls:**
- Click: Create particles at mouse position
- 1-4: Switch particle modes (Explosions, Fireworks, Trails, Attractors)
- Space: Clear all particles
- A: Auto-generate effects
- Mouse Wheel: Change modes
- Escape: Quit

**Learning Focus:** Advanced rendering, particle systems, and performance optimization.

## Compilation Instructions

### Prerequisites
- C++17 compatible compiler
- SDL2 development libraries
- CMake (optional, for build system)

### Manual Compilation

For each example, use the following command structure:

```bash
# From the project root directory
g++ -std=c++17 -I./include -L./lib examples/[game_name].cpp src/*.cpp -lSDL2 -lSDL2_image -o [game_name]
```

**Individual Examples:**
```bash
# Pong
g++ -std=c++17 -I./include examples/pong.cpp src/*.cpp -lSDL2 -lSDL2_image -o pong

# Asteroids
g++ -std=c++17 -I./include examples/asteroids.cpp src/*.cpp -lSDL2 -lSDL2_image -o asteroids

# Snake
g++ -std=c++17 -I./include examples/snake.cpp src/*.cpp -lSDL2 -lSDL2_image -o snake

# Breakout
g++ -std=c++17 -I./include examples/breakout.cpp src/*.cpp -lSDL2 -lSDL2_image -o breakout

# Particle Demo
g++ -std=c++17 -I./include examples/particle_demo.cpp src/*.cpp -lSDL2 -lSDL2_image -o particle_demo
```

### Using the Makefile
```bash
# Compile all examples
make examples

# Compile specific example
make pong
make asteroids
make snake
make breakout
make particle_demo

# Clean compiled examples
make clean-examples
```

## Running the Examples

After compilation, run the examples from the project root directory:

```bash
./pong
./asteroids
./snake
./breakout
./particle_demo
```

## Code Structure

Each example follows a similar structure:

1. **Game Class**: Inherits from `CacoEngine::Engine`
2. **Custom Objects**: Game-specific objects (Ship, Bullet, Block, etc.)
3. **Game Logic**: Update loops, collision detection, scoring
4. **Input Handling**: Keyboard and mouse event processing
5. **State Management**: Game over, restart, level progression

## Learning Path

**Recommended order for learning:**

1. **Start with Pong** - Learn basic concepts
2. **Move to Snake** - Understand grid-based movement
3. **Try Breakout** - Explore multi-object collision
4. **Challenge Asteroids** - Master advanced physics
5. **Experiment with Particle Demo** - Create visual effects

## Key Engine Features Showcased

### Physics System
- RigidBody2D for realistic movement
- Collision detection and response
- Gravity and acceleration
- Velocity and momentum

### Rendering System
- Multiple render modes (Points, WireFrame, SolidColor, Texture)
- Custom mesh creation
- Object transforms and positioning
- Alpha blending and transparency

### Object Management
- Dynamic object creation/destruction
- Smart pointer memory management
- Object hierarchies and inheritance
- Efficient object updates

### Input System
- Keyboard event handling
- Mouse interaction
- Real-time input processing
- Multiple input schemes

## Performance Tips

1. **Limit Object Count**: Keep particle counts reasonable (< 1000)
2. **Use Object Pooling**: Reuse objects when possible
3. **Optimize Collision**: Use spatial partitioning for many objects
4. **Smart Updates**: Only update objects that need it
5. **Memory Management**: Use smart pointers correctly

## Extending the Examples

### Adding New Features
- **Audio**: Add sound effects using SDL2_mixer
- **Textures**: Replace solid colors with sprite graphics
- **UI**: Add menus, HUD elements, and text rendering
- **Networking**: Create multiplayer versions
- **Save System**: Add high score persistence
- **Level Editor**: Create custom level design tools

### Creating New Games
Use these examples as templates for your own games:
- **Platformer**: Use collision detection from Breakout
- **Top-down Shooter**: Extend Asteroids mechanics
- **Puzzle Game**: Adapt Snake's grid system
- **Racing Game**: Use physics from Asteroids

## Troubleshooting

### Common Issues
1. **Compilation Errors**: Ensure SDL2 is properly installed
2. **Linking Issues**: Check library paths and names
3. **Runtime Crashes**: Verify asset paths and memory management
4. **Performance Issues**: Reduce object counts or optimize update loops

### Debug Tips
- Use console output for debugging game state
- Add visual indicators for collision boxes
- Monitor frame rate and object counts
- Use debugger for step-through analysis

## Contributing

Feel free to:
- Add new example games
- Improve existing examples
- Fix bugs or optimize performance
- Add better documentation
- Create tutorials based on these examples

## License

These examples are provided under the same license as CacoEngine. Use them freely for learning and development. 