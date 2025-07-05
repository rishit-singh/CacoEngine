# Chapter 2: Architecture & Design

## Introduction

This chapter provides an in-depth look at CacoEngine's architecture, design patterns, and the reasoning behind key architectural decisions. Understanding these concepts will help you build more efficient games and extend the engine effectively.

## Architectural Overview

### System Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                        Game Application                         │
├─────────────────────────────────────────────────────────────────┤
│                     CacoEngine::Engine                          │
├─────────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐ │
│  │   Objects   │  │   Physics   │  │   Renderer  │  │    Input    │ │
│  │   System    │  │   System    │  │   System    │  │   System    │ │
│  └─────────────┘  └─────────────┘  └─────────────┘  └─────────────┘ │
├─────────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐ │
│  │   Vertex    │  │   Texture   │  │   Surface   │  │    Tools    │ │
│  │   Math      │  │   Manager   │  │   Manager   │  │   Utilities │ │
│  └─────────────┘  └─────────────┘  └─────────────┘  └─────────────┘ │
├─────────────────────────────────────────────────────────────────┤
│                            SDL2                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Design Patterns Used

#### 1. **Template Method Pattern**
The `Engine` class uses the Template Method pattern for the game loop:

```cpp
class Engine {
public:
    void Run() {
        Initialize();
        while (IsRunning) {
            HandleEvents();        // Framework handles this
            OnUpdate(DeltaTime);   // Game implements this
            UpdatePhysics();       // Framework handles this
            Render();              // Framework handles this
        }
        Cleanup();
    }
    
    // Abstract methods for game-specific logic
    virtual void OnInitialize() = 0;
    virtual void OnUpdate(double deltaTime) = 0;
    virtual void OnKeyPress(SDL_KeyboardEvent& event) = 0;
    virtual void OnMouseClick(SDL_MouseButtonEvent& event) = 0;
    virtual void OnMouseScroll(SDL_MouseWheelEvent& event) = 0;
};
```

#### 2. **Strategy Pattern**
Rendering modes use the Strategy pattern:

```cpp
enum class RasterizeMode {
    Points,     // Different rendering strategies
    WireFrame,
    SolidColor,
    Texture
};

class Object {
public:
    RasterizeMode FillMode;  // Strategy selection
    // Rendering behavior changes based on strategy
};
```

#### 3. **Observer Pattern**
Event handling implements the Observer pattern:

```cpp
class Engine {
private:
    void ProcessEvents() {
        while (SDL_PollEvent(&Event)) {
            switch (Event.type) {
                case SDL_KEYDOWN:
                    OnKeyPress(Event.key);     // Notify observers
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    OnMouseClick(Event.button); // Notify observers
                    break;
            }
        }
    }
};
```

#### 4. **Component Pattern**
Objects use composition for flexibility:

```cpp
class Object {
public:
    Vector2Df Position;      // Transform component
    Texture mTexture;        // Texture component
    RGBA FillColor;          // Color component
    RasterizeMode FillMode;  // Rendering component
    Mesh ObjectMesh;         // Geometry component
};
```

## Core Systems Architecture

### 1. Object Management System

#### Object Hierarchy
```
Object (Base Class)
├── Triangle
├── Rectangle
│   └── Sprite
├── Circle
└── Mesh
```

#### RigidObject Hierarchy
```
RigidObject2D (Physics-Enabled Objects)
├── RigidCircle
└── Box2D
```

#### Object Lifecycle Management

```cpp
class Engine {
private:
    std::vector<std::shared_ptr<Object>> Objects;
    std::vector<std::shared_ptr<RigidObject2D>> RigidObjects;
    
public:
    Object& AddObject(std::shared_ptr<Object> object) {
        Objects.push_back(object);
        return *object;
    }
    
    RigidObject2D& AddObject(std::shared_ptr<RigidObject2D> object) {
        RigidObjects.push_back(object);
        return *object;
    }
};
```

### 2. Rendering System Architecture

#### Rendering Pipeline
```cpp
void Engine::Render() {
    EngineRenderer.Clear();
    
    // Render static objects
    for (auto& obj : Objects) {
        RenderObject(obj);
    }
    
    // Render physics objects
    for (auto& obj : RigidObjects) {
        RenderRigidObject(obj);
    }
    
    SDL_RenderPresent(EngineRenderer.GetInstance());
}
```

#### Mesh-Based Rendering
```cpp
class Mesh {
public:
    std::vector<Vertex2Df> Vertices;
    
    void AddTriangle(Vertex2Df v1, Vertex2Df v2, Vertex2Df v3) {
        Vertices.push_back(v1);
        Vertices.push_back(v2);
        Vertices.push_back(v3);
    }
    
    std::vector<SDL_Vertex> GetVertexBuffer() {
        std::vector<SDL_Vertex> buffer;
        for (const auto& vertex : Vertices) {
            buffer.push_back(vertex.GetSDLVertex());
        }
        return buffer;
    }
};
```

### 3. Physics System Architecture

#### Physics Integration
```cpp
class RigidBody2D {
public:
    Vector2Df Velocity;
    Vector2Df Acceleration;
    Vector2Df Force;
    double Mass;
    
    void AddForce(Vector2Df force) {
        Force += force;
    }
    
    void UpdateAcceleration() {
        Acceleration = Force / Mass;
    }
};
```

#### Physics Update Loop
```cpp
void Engine::UpdatePhysics() {
    for (auto& obj : RigidObjects) {
        obj->RigidBody.UpdateAcceleration();
        obj->RigidBody.Velocity += obj->RigidBody.Acceleration * DeltaTime;
        obj->Position += obj->RigidBody.Velocity * DeltaTime;
        obj->RigidBody.Force = Vector2Df(0, 0);  // Reset forces
    }
}
```

### 4. Input System Architecture

#### Event-Driven Input
```cpp
class Engine {
private:
    uint8_t* KeyStates;
    std::unordered_map<SDL_Keycode, Key> KeyMap;
    
    void MapKey(SDL_KeyboardEvent& event) {
        KeyMap[event.keysym.sym] = Key(event.keysym.scancode, event.state);
    }
    
public:
    Key GetKeyState(SDL_Scancode scancode) {
        return KeyMap[scancode];
    }
};
```

## Memory Management

### Smart Pointer Usage

#### Object Ownership
```cpp
// Engine owns objects through shared_ptr
std::vector<std::shared_ptr<Object>> Objects;
std::vector<std::shared_ptr<RigidObject2D>> RigidObjects;

// Multiple systems can reference same object
auto sprite = std::make_shared<Sprite>(texture, position, size);
AddObject(sprite);                    // Engine holds reference
someOtherSystem.AddSprite(sprite);    // Other system holds reference
```

#### RAII Pattern
```cpp
class Texture {
private:
    SDL_Texture* mTexture;
    
public:
    ~Texture() {
        if (mTexture) {
            SDL_DestroyTexture(mTexture);  // Automatic cleanup
        }
    }
};
```

### Memory Layout Optimization

#### Cache-Friendly Data Structures
```cpp
class Mesh {
private:
    std::vector<Vertex2Df> Vertices;  // Contiguous memory layout
    
public:
    std::vector<SDL_Vertex> GetVertexBuffer() {
        // Efficient batch conversion
        std::vector<SDL_Vertex> buffer;
        buffer.reserve(Vertices.size());
        
        for (const auto& vertex : Vertices) {
            buffer.push_back(vertex.GetSDLVertex());
        }
        return buffer;
    }
};
```

## System Interactions

### Cross-System Communication

#### Physics-Rendering Integration
```cpp
class RigidObject2D : public Object {
public:
    RigidBody2D RigidBody;
    
    void UpdateTransform() {
        // Physics system updates position
        Position += RigidBody.Velocity * deltaTime;
        
        // Rendering system uses updated position
        // No direct coupling - data-driven communication
    }
};
```

#### Event-Driven Communication
```cpp
void Engine::ProcessEvents() {
    while (SDL_PollEvent(&Event)) {
        switch (Event.type) {
            case SDL_KEYDOWN:
                MapKey(Event.key);
                OnKeyPress(Event.key);  // Notify game logic
                break;
        }
    }
}
```

### Data Flow Architecture

```
Input Events → Engine → Game Logic → Physics → Rendering → Display
     ↑                                   ↓
     └─────────── Delta Time ←───────────┘
```

## Extensibility Design

### Inheritance-Based Extension

#### Custom Objects
```cpp
class CustomGameObject : public CacoEngine::Object {
private:
    float health;
    float speed;
    
public:
    CustomGameObject(float h, float s) : health(h), speed(s) {}
    
    void Update(double deltaTime) {
        // Custom update logic
        if (health <= 0) {
            // Handle destruction
        }
    }
};
```

#### Custom Physics Objects
```cpp
class CustomPhysicsObject : public CacoEngine::RigidObject2D {
private:
    float bounceCoefficient;
    
public:
    bool CollidesWith(RigidObject2D& other) override {
        bool collision = RigidObject2D::CollidesWith(other);
        if (collision) {
            // Custom collision response
            RigidBody.Velocity *= -bounceCoefficient;
        }
        return collision;
    }
};
```

### Composition-Based Extension

#### Component System
```cpp
class GameObject {
private:
    std::shared_ptr<CacoEngine::Object> renderComponent;
    std::shared_ptr<CacoEngine::RigidObject2D> physicsComponent;
    std::shared_ptr<AudioComponent> audioComponent;
    std::shared_ptr<AIComponent> aiComponent;
    
public:
    void Update(double deltaTime) {
        if (physicsComponent) physicsComponent->Update(deltaTime);
        if (audioComponent) audioComponent->Update(deltaTime);
        if (aiComponent) aiComponent->Update(deltaTime);
    }
};
```

## Performance Considerations

### Rendering Optimization

#### Batch Rendering
```cpp
class BatchRenderer {
private:
    std::vector<SDL_Vertex> vertexBuffer;
    std::vector<int> indexBuffer;
    
public:
    void BatchRender(const std::vector<std::shared_ptr<Object>>& objects) {
        // Collect all vertices
        for (const auto& obj : objects) {
            auto vertices = obj->ObjectMesh.GetVertexBuffer();
            vertexBuffer.insert(vertexBuffer.end(), vertices.begin(), vertices.end());
        }
        
        // Single render call
        SDL_RenderGeometry(renderer, texture, vertexBuffer.data(), vertexBuffer.size(), 
                          indexBuffer.data(), indexBuffer.size());
    }
};
```

### Physics Optimization

#### Spatial Partitioning
```cpp
class PhysicsWorld {
private:
    struct SpatialGrid {
        std::vector<std::vector<std::shared_ptr<RigidObject2D>>> cells;
        float cellSize;
        
        void Insert(std::shared_ptr<RigidObject2D> obj) {
            int x = static_cast<int>(obj->Position.X / cellSize);
            int y = static_cast<int>(obj->Position.Y / cellSize);
            cells[y * width + x].push_back(obj);
        }
    };
    
    SpatialGrid grid;
    
public:
    void UpdateCollisions() {
        // Only check collisions within same cell
        for (auto& cell : grid.cells) {
            for (size_t i = 0; i < cell.size(); ++i) {
                for (size_t j = i + 1; j < cell.size(); ++j) {
                    if (cell[i]->CollidesWith(*cell[j])) {
                        // Handle collision
                    }
                }
            }
        }
    }
};
```

## Configuration Architecture

### Engine Configuration
```cpp
struct EngineConfig {
    std::string title = "CacoEngine App";
    Vector2Df resolution = Vector2Df(800, 600);
    bool fullscreen = false;
    bool vsync = true;
    int maxFPS = 60;
    
    struct Physics {
        float gravity = 9.81f;
        float timeStep = 1.0f / 60.0f;
        int velocityIterations = 8;
        int positionIterations = 3;
    } physics;
    
    struct Rendering {
        bool enableBatching = true;
        bool enableCulling = true;
        int maxBatchSize = 1000;
    } rendering;
};
```

### Runtime Configuration
```cpp
class Engine {
private:
    EngineConfig config;
    
public:
    Engine(const EngineConfig& cfg = EngineConfig()) : config(cfg) {}
    
    void SetConfig(const EngineConfig& cfg) {
        config = cfg;
        // Apply configuration changes
        ApplyConfig();
    }
};
```

## Testing Architecture

### Unit Testing Support
```cpp
class TestableEngine : public Engine {
public:
    // Expose protected methods for testing
    void TestUpdatePhysics() { UpdatePhysics(); }
    void TestRender() { Render(); }
    
    // Mock dependencies for testing
    void SetMockRenderer(std::shared_ptr<MockRenderer> renderer) {
        mockRenderer = renderer;
    }
};
```

### Integration Testing
```cpp
class EngineIntegrationTest {
private:
    std::unique_ptr<Engine> engine;
    
public:
    void SetUp() {
        engine = std::make_unique<TestableEngine>();
        engine->Initialize();
    }
    
    void TestCompleteGameLoop() {
        engine->OnInitialize();
        for (int i = 0; i < 60; ++i) {  // Test 1 second at 60 FPS
            engine->OnUpdate(1.0 / 60.0);
        }
        // Assert expected behavior
    }
};
```

## Performance Metrics

### Profiling Integration
```cpp
class PerformanceProfiler {
private:
    std::unordered_map<std::string, double> timings;
    
public:
    void StartTimer(const std::string& name) {
        startTimes[name] = GetCurrentTime();
    }
    
    void EndTimer(const std::string& name) {
        timings[name] = GetCurrentTime() - startTimes[name];
    }
    
    void PrintStats() {
        for (const auto& [name, time] : timings) {
            std::cout << name << ": " << time << "ms\n";
        }
    }
};
```

## Next Steps

Now that you understand the architecture, let's explore specific systems:

- **[Chapter 3: Objects & Rendering](03-objects-rendering.md)** - Deep dive into the object system
- **[Chapter 4: Physics System](04-physics.md)** - Physics and collision mechanics
- **[Chapter 5: Input & Events](05-input-events.md)** - Event handling and input management

## Summary

CacoEngine's architecture provides:

- **Modular Design**: Clear separation of concerns between systems
- **Extensibility**: Multiple extension points for custom functionality
- **Performance**: Optimized data structures and algorithms
- **Maintainability**: Clean interfaces and well-defined responsibilities
- **Testability**: Mockable components and clear dependencies

Understanding this architecture will help you build better games and contribute to the engine's development.

---

**Next:** [Chapter 3: Objects & Rendering](03-objects-rendering.md) 