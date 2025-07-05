# CacoEngine Documentation

![CacoEngine Logo](../assets/logo.png)

**CacoEngine** is a powerful 2D game engine built with C++ and SDL2, designed for rapid game development with robust physics simulation, flexible rendering, and intuitive object management.

## Table of Contents

### Getting Started
- [Quick Start Guide](QUICK_START.md) - Get running in 5 minutes
- [Installation & Setup](installation.md) - Complete installation guide
- [Your First Game](first-game.md) - Step-by-step tutorial

### Core Documentation
- [Chapter 1: Engine Overview](chapters/01-engine-overview.md)
- [Chapter 2: Architecture & Design](chapters/02-architecture.md)
- [Chapter 3: Objects & Rendering](chapters/03-objects-rendering.md)
- [Chapter 4: Physics System](chapters/04-physics.md)
- [Chapter 5: Input & Events](chapters/05-input-events.md)
- [Chapter 6: Assets & Textures](chapters/06-assets-textures.md)
- [Chapter 7: Collision Detection](chapters/07-collision.md)
- [Chapter 8: Advanced Features](chapters/08-advanced.md)

### Reference
- [API Reference](api-reference.md) - Complete API documentation
- [Configuration](configuration.md) - Engine configuration options
- [Troubleshooting](troubleshooting.md) - Common issues and solutions

### Examples & Tutorials
- [Game Examples](examples/README.md) - Complete game examples
- [Code Samples](code-samples.md) - Useful code snippets
- [Best Practices](best-practices.md) - Development guidelines

## Key Features

### **Flexible Rendering System**
- Hardware-accelerated 2D graphics with SDL2
- Multiple rendering modes (points, wireframe, solid, textured)
- Mesh-based object system with vertex manipulation
- Sprite system with automatic texture management

### **Physics Engine**
- Rigid body dynamics with force-based movement
- Circle and box collision shapes
- Collision detection and response system
- Customizable physics parameters (mass, velocity, acceleration)

### **Event-Driven Architecture**
- Flexible input handling system
- Mouse and keyboard event management
- Extensible event system for custom game logic
- Frame-based update cycles

### **Object Management**
- Hierarchical object system with inheritance
- Automatic memory management with smart pointers
- Component-based design for extensibility
- Efficient object rendering and updates

### **Game Development Ready**
- Complete game loop implementation
- Delta time calculations for smooth gameplay
- Asset loading and management
- Cross-platform compatibility

## Quick Example

```cpp
#include "engine.hpp"
#include "objects.hpp"
#include "sprite.hpp"

class MyGame : public CacoEngine::Engine {
public:
    MyGame() : Engine("My Game", CacoEngine::Vector2Df(800, 600)) {}
    
    void OnInitialize() override {
        // Load texture and create sprite
        auto texture = CacoEngine::TextureManager::CreateTexture("player.png", EngineRenderer);
        auto sprite = std::make_shared<CacoEngine::Sprite>(texture, 
            CacoEngine::Vector2Df(400, 300), 
            CacoEngine::Vector2Df(64, 64));
        
        AddObject(sprite);
    }
    
    void OnUpdate(double deltaTime) override {
        // Game logic here
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        // Handle input
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        // Handle mouse input
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Handle scroll input
    }
};

int main() {
    MyGame game;
    game.Run();
    return 0;
}
```

## What's Included

### Core Engine Components
- **Engine**: Main engine class with game loop and lifecycle management
- **Renderer**: Hardware-accelerated 2D rendering system
- **Objects**: Flexible object system with meshes, sprites, and shapes
- **Physics**: Rigid body physics with collision detection
- **Input**: Event-driven input handling system
- **Assets**: Texture loading and management

### Built-in Objects
- **Sprite**: Textured rectangular objects
- **Circle**: Circular geometric objects with collision
- **Rectangle**: Rectangular shapes with customizable properties
- **Triangle**: Basic triangular shapes
- **Mesh**: Custom vertex-based objects

### Physics Objects
- **RigidObject2D**: Physics-enabled base objects
- **RigidCircle**: Physics circles with collision
- **Box2D**: Physics rectangles with collision
- **RigidBody2D**: Physics component with force simulation

## Who Should Use CacoEngine?

### **Game Developers**
- Indie developers creating 2D games
- Students learning game development
- Rapid prototyping of game ideas
- Educational game development projects

### **Engine Developers**
- Learning game engine architecture
- Studying physics simulation systems
- Understanding rendering pipelines
- Exploring C++ game development

### **Educators**
- Teaching game development concepts
- Demonstrating physics principles
- Computer graphics education
- Object-oriented programming examples

## Technical Requirements

### Minimum Requirements
- **Compiler**: C++17 compatible (GCC 7+, Clang 5+, MSVC 2017+)
- **SDL2**: Development libraries installed
- **CMake**: Version 3.10 or higher
- **Platform**: Windows, macOS, Linux

### Recommended Setup
- **IDE**: Visual Studio Code, CLion, or Visual Studio
- **Build System**: CMake with Ninja or Make
- **Debugger**: GDB or LLDB for debugging
- **Version Control**: Git for project management

## Community & Support

### Documentation
- Comprehensive API reference
- Step-by-step tutorials
- Code examples and samples
- Best practices guide

### Issues & Bugs
- GitHub issue tracker
- Community forums
- Development roadmap
- Feature requests

### Contributing
- Contribution guidelines
- Code style standards
- Testing requirements
- Documentation updates

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments

CacoEngine is built on top of the excellent SDL2 library and incorporates modern C++ practices for game development. Special thanks to the open-source community for their continued support and contributions. 