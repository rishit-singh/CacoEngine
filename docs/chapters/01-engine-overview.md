# Chapter 1: Engine Overview

## Introduction

CacoEngine is a modern 2D game engine built with C++ and SDL2, designed to provide game developers with a powerful yet intuitive framework for creating 2D games. This chapter introduces the core concepts, architecture, and philosophy behind CacoEngine.

## 🎯 Design Philosophy

### Simplicity First
CacoEngine prioritizes ease of use without sacrificing power. The engine provides sensible defaults while allowing deep customization when needed.

### Performance Oriented
Built on SDL2 for hardware-accelerated graphics, CacoEngine is designed for smooth 60+ FPS gameplay with efficient memory management.

### Extensible Architecture
The engine uses inheritance and composition patterns, making it easy to extend and customize for specific game requirements.

## 🏗️ Core Architecture

### Engine Class Hierarchy

```
CacoEngine::Engine (Abstract Base Class)
├── Game Loop Management
├── Window & Renderer Control
├── Event System
├── Object Management
└── Physics Integration
```

### Key Components

#### 1. **Engine Core** (`engine.hpp`)
The main engine class that manages the game lifecycle:

```cpp
class Engine {
protected:
    std::vector<Extension> Extensions;
    SDL_Event Event;
    Renderer EngineRenderer;
    std::vector<std::shared_ptr<Object>> Objects;
    std::vector<std::shared_ptr<RigidObject2D>> RigidObjects;
    
public:
    void Run();
    virtual void OnInitialize() = 0;
    virtual void OnUpdate(double deltaTime) = 0;
    virtual void OnKeyPress(SDL_KeyboardEvent& event) = 0;
    virtual void OnMouseClick(SDL_MouseButtonEvent& event) = 0;
    virtual void OnMouseScroll(SDL_MouseWheelEvent& event) = 0;
};
```

#### 2. **Rendering System** (`renderer.hpp`)
Hardware-accelerated 2D rendering:

```cpp
class Renderer {
public:
    RGBA Color;
    void Clear(RGBA color = RGBA());
    void SetColor(RGBA color);
    SDL_Renderer* GetInstance();
};
```

#### 3. **Object System** (`objects.hpp`)
Flexible object hierarchy for game entities:

```cpp
class Object {
public:
    Vector2Df Position;
    Texture mTexture;
    RGBA FillColor;
    RasterizeMode FillMode;
    Mesh ObjectMesh;
    
    void Translate(Vector2Df offset);
    void SetFillColor(RGBA color);
};
```

#### 4. **Physics System** (`rigidbody.hpp`, `rigidobject.hpp`)
Real-time physics simulation:

```cpp
class RigidBody2D {
public:
    Vector2Df Velocity;
    Vector2Df Acceleration;
    Vector2Df Force;
    double Mass;
    
    void AddForce(Vector2Df force);
    void UpdateAcceleration();
};
```

## 🔄 Engine Lifecycle

### 1. Initialization Phase
```cpp
void Engine::Initialize() {
    // SDL initialization
    // Window creation
    // Renderer setup
    // Extension loading
    // Call OnInitialize()
}
```

### 2. Game Loop
```cpp
void Engine::Run() {
    while (IsRunning) {
        // Handle events
        // Update physics
        // Call OnUpdate()
        // Render objects
        // Calculate delta time
    }
}
```

### 3. Event Processing
```cpp
// Input events are dispatched to virtual methods
OnKeyPress(keyboardEvent);
OnMouseClick(mouseEvent);
OnMouseScroll(scrollEvent);
```

### 4. Rendering Pipeline
```cpp
// Clear screen
// Render all objects
// Render rigid objects
// Present frame
```

## 🎮 Creating Your First Engine

### Basic Engine Implementation

```cpp
#include "engine.hpp"
#include "sprite.hpp"

class MyGame : public CacoEngine::Engine {
private:
    std::shared_ptr<CacoEngine::Sprite> player;
    
public:
    MyGame() : Engine("My Game", CacoEngine::Vector2Df(800, 600)) {}
    
    void OnInitialize() override {
        // Load assets
        auto texture = CacoEngine::TextureManager::CreateTexture("player.png", EngineRenderer);
        
        // Create player sprite
        player = std::make_shared<CacoEngine::Sprite>(
            texture,
            CacoEngine::Vector2Df(400, 300),  // Position
            CacoEngine::Vector2Df(64, 64)     // Size
        );
        
        // Add to engine
        AddObject(player);
    }
    
    void OnUpdate(double deltaTime) override {
        // Game logic here
        // Objects are automatically rendered
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        switch (event.keysym.sym) {
            case SDLK_LEFT:
                player->Translate(CacoEngine::Vector2Df(-100 * deltaTime, 0));
                break;
            case SDLK_RIGHT:
                player->Translate(CacoEngine::Vector2Df(100 * deltaTime, 0));
                break;
        }
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        // Handle mouse clicks
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Handle scroll events
    }
};
```

### Running Your Engine

```cpp
int main() {
    MyGame game;
    game.Run();  // Starts the game loop
    return 0;
}
```

## 📊 Performance Characteristics

### Memory Management
- **Smart Pointers**: Automatic memory management with `std::shared_ptr`
- **Object Pooling**: Reuse objects to reduce allocations
- **Efficient Rendering**: Hardware-accelerated graphics with SDL2

### Frame Rate Optimization
- **Delta Time**: Smooth animation independent of frame rate
- **Efficient Updates**: Only update changed objects
- **Culling**: Automatic off-screen object culling

### Physics Performance
- **Broad Phase**: Efficient collision detection algorithms
- **Narrow Phase**: Precise collision resolution
- **Force Integration**: Stable physics simulation

## 🛠️ Extension System

### Available Extensions

```cpp
enum class Extension {
    Video = SDL_INIT_VIDEO,
    Audio = SDL_INIT_AUDIO
};

// Add extensions to your engine
void OnInitialize() override {
    AddExtension(Extension::Video);
    AddExtension(Extension::Audio);
}
```

### Custom Extensions
You can extend the engine with custom functionality:

```cpp
class MyGameEngine : public CacoEngine::Engine {
private:
    AudioManager audioManager;
    NetworkManager networkManager;
    
public:
    void OnInitialize() override {
        // Initialize custom systems
        audioManager.Initialize();
        networkManager.Connect();
    }
};
```

## 🎨 Rendering Modes

### Available Render Modes

```cpp
enum class RasterizeMode {
    Points,     // Render as points
    WireFrame,  // Render as wireframe
    SolidColor, // Render as solid color
    Texture     // Render with texture
};

// Set render mode
object->FillMode = RasterizeMode::Texture;
```

### Rendering Pipeline
1. **Object Sorting**: Objects sorted by depth/priority
2. **Culling**: Off-screen objects are culled
3. **Batch Rendering**: Similar objects rendered together
4. **Texture Binding**: Textures applied efficiently

## 🔧 Configuration Options

### Engine Configuration

```cpp
// Engine constructor parameters
Engine(
    std::string_view title = "CacoEngine App",
    Vector2Df resolution = Vector2Df(800, 600),
    bool fullscreen = false
);
```

### Window Properties

```cpp
// Access window properties
std::string_view Title;
Vector2Df Resolution;
bool IsRunning;
Vector2D CursorPosition;
SDL_Window* Window;
```

## 🚀 Next Steps

Now that you understand the engine overview, let's dive deeper into specific systems:

- **[Chapter 2: Architecture & Design](02-architecture.md)** - Detailed architecture patterns
- **[Chapter 3: Objects & Rendering](03-objects-rendering.md)** - Object system deep dive
- **[Chapter 4: Physics System](04-physics.md)** - Physics and collision systems

## 📝 Summary

CacoEngine provides:
- **Simple API**: Easy to learn, powerful to use
- **Performance**: Hardware-accelerated graphics with efficient algorithms
- **Flexibility**: Extensible architecture for custom game requirements
- **Completeness**: Full game development framework with physics, rendering, and input

The engine follows modern C++ practices with automatic memory management, event-driven architecture, and component-based design, making it suitable for both beginners and experienced developers.

---

**Next:** [Chapter 2: Architecture & Design](02-architecture.md) 