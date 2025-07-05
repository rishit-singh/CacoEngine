# CacoEngine Quick Start Guide

## 🚀 Get Up and Running in 5 Minutes

### Prerequisites
- **C++17 compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **SDL2 development libraries** installed
- **CMake 3.10+** for building
- **SDL2_image** for texture loading

### Installation

#### 1. Clone and Build
```bash
git clone <repository-url>
cd CacoEngine
mkdir build && cd build
cmake .. && make
```

#### 2. Run the Example
```bash
./CacoEngine "My Game" 800 600
```

## 🎮 Your First Game in 60 Seconds

### Basic Game Template

```cpp
#include "engine.hpp"
#include "objects.hpp"
#include "sprite.hpp"

class MyGame : public CacoEngine::Engine {
private:
    std::shared_ptr<CacoEngine::Circle> player;
    
public:
    MyGame() : Engine("My First Game", CacoEngine::Vector2Df(800, 600)) {}
    
    void OnInitialize() override {
        // Create a player circle
        player = std::make_shared<CacoEngine::Circle>(
            CacoEngine::Vector2Df(400, 300),  // Center position
            30.0                              // Radius
        );
        player->SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::Blue]);
        
        // Add to engine
        AddObject(player);
    }
    
    void OnUpdate(double deltaTime) override {
        // Game logic goes here
        // Objects are automatically rendered
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        const float speed = 200.0f;
        switch (event.keysym.sym) {
            case SDLK_UP:
                player->Translate(CacoEngine::Vector2Df(0, -speed * GetDeltaTime()));
                break;
            case SDLK_DOWN:
                player->Translate(CacoEngine::Vector2Df(0, speed * GetDeltaTime()));
                break;
            case SDLK_LEFT:
                player->Translate(CacoEngine::Vector2Df(-speed * GetDeltaTime(), 0));
                break;
            case SDLK_RIGHT:
                player->Translate(CacoEngine::Vector2Df(speed * GetDeltaTime(), 0));
                break;
        }
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        // Create a new circle at mouse position
        auto newCircle = std::make_shared<CacoEngine::Circle>(
            CacoEngine::Vector2Df(event.x, event.y),
            20.0
        );
        newCircle->SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::Red]);
        AddObject(newCircle);
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Resize player based on scroll
        double currentRadius = player->GetRadius();
        player->SetRadius(currentRadius + (event.y * 5));
    }
};

int main() {
    MyGame game;
    game.Run();
    return 0;
}
```

### Build and Run

```bash
# Save as my_game.cpp
g++ -std=c++17 my_game.cpp -o my_game -lSDL2 -lSDL2_image -lCacoEngine
./my_game
```

## 🎯 Core Concepts

### 1. **Engine Class**
Your game inherits from `CacoEngine::Engine` and implements virtual methods:
- `OnInitialize()` - Setup your game world
- `OnUpdate(deltaTime)` - Game logic each frame
- `OnKeyPress()` - Handle keyboard input
- `OnMouseClick()` - Handle mouse input
- `OnMouseScroll()` - Handle scroll events

### 2. **Objects**
Everything visual inherits from `CacoEngine::Object`:
- `Circle` - Circular shapes
- `Rectangle` - Rectangular shapes
- `Triangle` - Triangular shapes
- `Sprite` - Textured rectangles

### 3. **Rendering**
Objects have different rendering modes:
- `RasterizeMode::Points` - Show as points
- `RasterizeMode::WireFrame` - Show as wireframe
- `RasterizeMode::SolidColor` - Show filled
- `RasterizeMode::Texture` - Show with texture

### 4. **Textures**
Load and apply textures to objects:
```cpp
auto texture = CacoEngine::TextureManager::CreateTexture("image.png", EngineRenderer);
auto sprite = std::make_shared<CacoEngine::Sprite>(texture, position, size);
```

## 🎨 Common Tasks

### Creating Different Objects

```cpp
// Circle
auto circle = std::make_shared<CacoEngine::Circle>(
    CacoEngine::Vector2Df(100, 100), 50.0);

// Rectangle
auto rect = std::make_shared<CacoEngine::Rectangle>(
    CacoEngine::Vector2Df(200, 200), 
    CacoEngine::Vector2Df(100, 80));

// Triangle
auto triangle = std::make_shared<CacoEngine::Triangle>(
    CacoEngine::Vertex2Df(CacoEngine::Vector2Df(0, 0)),
    CacoEngine::Vertex2Df(CacoEngine::Vector2Df(50, 0)),
    CacoEngine::Vertex2Df(CacoEngine::Vector2Df(25, 50)),
    CacoEngine::Colors[(int)CacoEngine::Color::Green]
);
```

### Applying Colors

```cpp
// Use predefined colors
object->SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::Red]);

// Create custom colors
CacoEngine::RGBA customColor(255, 128, 0, 255);  // Orange
object->SetFillColor(customColor);
```

### Loading Textures

```cpp
void OnInitialize() override {
    // Load texture
    auto texture = CacoEngine::TextureManager::CreateTexture("player.png", EngineRenderer);
    
    // Create sprite
    auto sprite = std::make_shared<CacoEngine::Sprite>(
        texture,
        CacoEngine::Vector2Df(100, 100),
        CacoEngine::Vector2Df(64, 64)
    );
    
    AddObject(sprite);
}
```

### Handling Input

```cpp
void OnKeyPress(SDL_KeyboardEvent& event) override {
    switch (event.keysym.sym) {
        case SDLK_SPACE:
            // Jump action
            break;
        case SDLK_ESCAPE:
            // Exit game
            IsRunning = false;
            break;
    }
}

void OnMouseClick(SDL_MouseButtonEvent& event) override {
    if (event.button == SDL_BUTTON_LEFT) {
        // Left click action
        CacoEngine::Vector2Df mousePos(event.x, event.y);
        // Do something at mouse position
    }
}
```

## 🔥 Advanced Features

### Physics Objects

```cpp
#include "rigidobject.hpp"

class MyPhysicsGame : public CacoEngine::Engine {
private:
    std::shared_ptr<CacoEngine::RigidCircle> physicsCircle;
    
public:
    void OnInitialize() override {
        // Create physics-enabled circle
        physicsCircle = std::make_shared<CacoEngine::RigidCircle>(
            CacoEngine::Vector2Df(400, 100),
            30.0
        );
        physicsCircle->RigidBody.Mass = 1.0;
        AddObject(physicsCircle);
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        if (event.keysym.sym == SDLK_SPACE) {
            // Apply upward force
            physicsCircle->RigidBody.AddForce(CacoEngine::Vector2Df(0, -500));
        }
    }
};
```

### Custom Meshes

```cpp
void CreateCustomShape() {
    auto customObject = std::make_shared<CacoEngine::Object>();
    
    // Create a star shape
    auto& mesh = customObject->ObjectMesh;
    CacoEngine::Vector2Df center(0, 0);
    
    for (int i = 0; i < 5; ++i) {
        float angle = i * 2.0f * M_PI / 5.0f;
        auto vertex = CacoEngine::Vertex2Df(
            center + CacoEngine::Vector2Df(50 * cos(angle), 50 * sin(angle)),
            CacoEngine::Colors[(int)CacoEngine::Color::White]
        );
        mesh.Vertices.push_back(vertex);
    }
    
    customObject->Position = CacoEngine::Vector2Df(400, 300);
    customObject->FillMode = CacoEngine::RasterizeMode::SolidColor;
    AddObject(customObject);
}
```

## 🎮 Complete Example: Pong Game

```cpp
#include "engine.hpp"
#include "objects.hpp"

class PongGame : public CacoEngine::Engine {
private:
    std::shared_ptr<CacoEngine::Rectangle> paddle1, paddle2, ball;
    CacoEngine::Vector2Df ballVelocity;
    
public:
    PongGame() : Engine("Pong", CacoEngine::Vector2Df(800, 600)) {
        ballVelocity = CacoEngine::Vector2Df(200, 150);
    }
    
    void OnInitialize() override {
        // Create paddles
        paddle1 = std::make_shared<CacoEngine::Rectangle>(
            CacoEngine::Vector2Df(50, 250),
            CacoEngine::Vector2Df(20, 100)
        );
        paddle2 = std::make_shared<CacoEngine::Rectangle>(
            CacoEngine::Vector2Df(730, 250),
            CacoEngine::Vector2Df(20, 100)
        );
        
        // Create ball
        ball = std::make_shared<CacoEngine::Rectangle>(
            CacoEngine::Vector2Df(390, 290),
            CacoEngine::Vector2Df(20, 20)
        );
        
        AddObject(paddle1);
        AddObject(paddle2);
        AddObject(ball);
    }
    
    void OnUpdate(double deltaTime) override {
        // Move ball
        ball->Translate(ballVelocity * deltaTime);
        
        // Ball collision with top/bottom
        if (ball->Position.Y <= 0 || ball->Position.Y >= 580) {
            ballVelocity.Y *= -1;
        }
        
        // Ball collision with paddles (simplified)
        // Add your collision logic here
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        const float speed = 300.0f;
        switch (event.keysym.sym) {
            case SDLK_w:
                paddle1->Translate(CacoEngine::Vector2Df(0, -speed * DeltaTime));
                break;
            case SDLK_s:
                paddle1->Translate(CacoEngine::Vector2Df(0, speed * DeltaTime));
                break;
            case SDLK_UP:
                paddle2->Translate(CacoEngine::Vector2Df(0, -speed * DeltaTime));
                break;
            case SDLK_DOWN:
                paddle2->Translate(CacoEngine::Vector2Df(0, speed * DeltaTime));
                break;
        }
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {}
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {}
};

int main() {
    PongGame game;
    game.Run();
    return 0;
}
```

## 🚀 Next Steps

Now that you've got the basics down:

1. **[Chapter 1: Engine Overview](chapters/01-engine-overview.md)** - Understand the engine architecture
2. **[Chapter 2: Architecture & Design](chapters/02-architecture.md)** - Learn design patterns
3. **[Chapter 3: Objects & Rendering](chapters/03-objects-rendering.md)** - Master the object system
4. **[Chapter 4: Physics System](chapters/04-physics.md)** - Add physics to your games

## 💡 Tips for Success

### Performance
- Use `std::shared_ptr` for object management
- Batch similar objects for better rendering performance
- Consider object pooling for frequently created/destroyed objects

### Organization
- Keep your game logic in `OnUpdate()`
- Use `OnInitialize()` for setup
- Separate rendering and game logic

### Debugging
- Use different `RasterizeMode` values to visualize objects
- Check `DeltaTime` for smooth animations
- Use `IsRunning` to control game state

## 🎯 Common Patterns

### Game State Management
```cpp
enum class GameState { Menu, Playing, Paused, GameOver };

class MyGame : public CacoEngine::Engine {
private:
    GameState currentState = GameState::Menu;
    
public:
    void OnUpdate(double deltaTime) override {
        switch (currentState) {
            case GameState::Menu:
                UpdateMenu(deltaTime);
                break;
            case GameState::Playing:
                UpdateGame(deltaTime);
                break;
            // ... other states
        }
    }
};
```

### Simple Animation
```cpp
class AnimatedObject {
private:
    float animationTime = 0.0f;
    std::shared_ptr<CacoEngine::Object> object;
    
public:
    void Update(double deltaTime) {
        animationTime += deltaTime;
        
        // Bounce animation
        float offset = sin(animationTime * 3.0f) * 10.0f;
        object->Position.Y = baseY + offset;
    }
};
```

Happy coding! 🎮✨

---

**Ready to dive deeper?** Check out the [full documentation](README.md) for advanced features and comprehensive guides. 