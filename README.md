# CacoEngine

![CacoEngine Logo](assets/logo.png)

**CacoEngine** is a powerful 2D game engine built with modern C++ and SDL2, designed for rapid game development with robust physics simulation, flexible rendering, and intuitive object management.

## Features

- **Hardware-Accelerated 2D Graphics**: SDL2-powered rendering with multiple modes (points, wireframe, solid, textured)
- **Physics Simulation**: Rigid body dynamics with collision detection and response
- **Flexible Object System**: Hierarchical object management with mesh-based rendering
- **Event-Driven Architecture**: Complete input handling system for mouse and keyboard
- **Cross-Platform**: Support for Windows, macOS, and Linux
- **Modern C++**: Clean, maintainable code using C++17 features and smart pointers

## Quick Start

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- SDL2 development libraries
- CMake 3.10 or higher

### Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/CacoEngine.git
cd CacoEngine
```

2. Install SDL2 development libraries:

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl2-dev
```

**macOS:**
```bash
brew install sdl2
```

**Windows:**
Download SDL2 development libraries from [libsdl.org](https://www.libsdl.org/download-2.0.php)

3. Build the project:
```bash
cmake -Bbuild
cd build
make
```

### Basic Usage

```cpp
#include "engine.hpp"
#include "objects.hpp"

class MyGame : public CacoEngine::Engine {
public:
    MyGame() : Engine("My Game", CacoEngine::Vector2Df(800, 600)) {}
    
    void OnInitialize() override {
        // Create a circle object
        auto circle = std::make_shared<CacoEngine::Circle>(
            CacoEngine::Vector2Df(400, 300),  // Position
            50,                                // Radius
            CacoEngine::Colors[(int)CacoEngine::Color::Red]  // Color
        );
        
        AddObject(circle);
    }
    
    void OnUpdate(double deltaTime) override {
        // Game logic here
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        // Handle keyboard input
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        // Handle mouse clicks
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Handle mouse wheel
    }
};

int main() {
    MyGame game;
    game.Run();
    return 0;
}
```

## Architecture

CacoEngine follows a clean, modular architecture:

- **Engine Core**: Main game loop, event handling, and lifecycle management
- **Object System**: Hierarchical object management with built-in shapes and custom meshes
- **Rendering System**: Hardware-accelerated 2D rendering with multiple modes
- **Physics System**: Rigid body dynamics with collision detection
- **Input System**: Event-driven input handling for keyboard and mouse
- **Asset Management**: Texture loading and management system

## Built-in Objects

### Basic Objects
- **Triangle**: Simple triangular shapes
- **Rectangle**: Rectangular objects with customizable dimensions
- **Circle**: Circular objects with radius control
- **Sprite**: Textured rectangular objects
- **Mesh**: Custom vertex-based objects

### Physics Objects
- **RigidObject2D**: Base class for physics-enabled objects
- **RigidCircle**: Physics-enabled circles with collision
- **Box2D**: Physics-enabled rectangles with collision

## Rendering Modes

CacoEngine supports multiple rendering modes for each object:

- **Points**: Render only vertices as points
- **WireFrame**: Render object outlines
- **SolidColor**: Fill objects with solid colors
- **Texture**: Apply textures to objects

## Physics System

The physics system provides:

- **Rigid Body Dynamics**: Force-based movement with mass, velocity, and acceleration
- **Collision Detection**: Efficient collision detection between objects
- **Collision Response**: Customizable collision response callbacks
- **Physics Integration**: Automatic integration with the rendering system

## Documentation

Comprehensive documentation is available in the `docs/` directory:

- [Quick Start Guide](docs/QUICK_START.md) - Get running in 5 minutes
- [Engine Overview](docs/chapters/01-engine-overview.md) - Core concepts and design
- [Architecture & Design](docs/chapters/02-architecture.md) - System architecture and patterns
- [Objects & Rendering](docs/chapters/03-objects-rendering.md) - Complete object system guide

## Examples

The `src/main.cpp` file contains a complete example application demonstrating:

- Basic engine setup and configuration
- Object creation and management
- Input handling and event processing
- Physics simulation and collision detection

## Contributing

Contributions are welcome! Please read the contributing guidelines and submit pull requests for any improvements.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Built on top of the excellent [SDL2](https://www.libsdl.org/) library
- Inspired by modern game engine architecture principles
- Thanks to the open-source community for feedback and contributions

## Support

If you encounter any issues or have questions:

- Check the documentation in the `docs/` directory
- Review the example code in `src/main.cpp`
- Open an issue on the GitHub repository
- Join our community discussions

---

**CacoEngine** - Simple, Powerful, Modern 2D Game Development

### Compilation

```bash
cmake -Bbuild;
cd build;
make;
```

​	

