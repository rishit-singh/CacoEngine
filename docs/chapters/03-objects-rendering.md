# Chapter 3: Objects & Rendering

## Introduction

CacoEngine's object system forms the foundation of game development, providing a flexible hierarchy for creating and managing game entities. This chapter covers the complete object system, from basic objects to complex meshes, and the rendering pipeline that brings them to life.

## Object System Overview

### Object Hierarchy

```
Object (Base Class)
├── Triangle
├── Rectangle
│   └── Sprite
├── Circle
└── Mesh (Custom Vertex Objects)
```

### Core Object Components

Every object in CacoEngine is composed of several key components:

```cpp
class Object {
public:
    int ID;                    // Unique identifier
    Vector2Df Position;        // World position
    Texture mTexture;          // Texture data
    RGBA FillColor;            // Color information
    RasterizeMode FillMode;    // Rendering strategy
    Mesh ObjectMesh;           // Geometry data
    
    // Core methods
    void AddVertex(Vertex2Df vertex);
    void Translate(Vector2Df offset);
    void SetFillColor(RGBA color);
};
```

## Vertex System & Math

### Vector Mathematics

#### Vector2D (Integer Precision)
```cpp
struct Vector2D {
    int X, Y;
    
    // Arithmetic operations
    Vector2D operator+(Vector2D other);
    Vector2D operator-(Vector2D other);
    Vector2D operator*(Vector2D other);
    Vector2D operator/(Vector2D other);
    
    // Scalar operations
    Vector2D operator*(int scalar);
    Vector2D operator/(int scalar);
    
    // Utility
    int DistanceFrom(Vector2D other);
    bool operator==(Vector2D other);
};
```

#### Vector2Df (Float Precision)
```cpp
struct Vector2Df {
    double X, Y;
    
    // High precision arithmetic
    Vector2Df operator+(Vector2Df other);
    Vector2Df operator-(Vector2Df other);
    Vector2Df operator*(Vector2Df other);
    Vector2Df operator/(Vector2Df other);
    
    // Scalar operations
    Vector2Df operator*(double scalar);
    Vector2Df operator/(double scalar);
    
    // Utility
    double DistanceFrom(Vector2Df other);
    bool Equals(Vector2Df other);
};
```

### Vertex Structures

#### Vertex2D (Integer Vertex)
```cpp
struct Vertex2D {
    Vector2D Position;
    Vector2D TextureCoordinates;
    RGBA Color;
    
    SDL_Vertex GetSDLVertex();
    SDL_Point GetSDLPoint();
    bool operator==(Vertex2D other);
};
```

#### Vertex2Df (Float Vertex)
```cpp
struct Vertex2Df {
    Vector2Df Position;
    Vector2Df TextureCoordinates;
    RGBA Color;
    
    SDL_Vertex GetSDLVertex();
    SDL_FPoint GetSDLPoint();
    bool operator==(Vertex2Df other);
};
```

### Color System

#### RGBA Structure
```cpp
struct RGBA {
    int R, G, B, A;  // 0-255 range
    
    RGBA(int r = 0, int g = 0, int b = 0, int a = 255);
    bool operator==(RGBA other);
};
```

#### Predefined Colors
```cpp
enum class Color {
    Red,
    Blue,
    Green,
    White,
    Black
};

extern RGBA Colors[5];  // Predefined color array
```

## Mesh System

### Mesh Class Architecture

```cpp
class Mesh {
protected:
    void Initialize();
    
public:
    std::vector<Vertex2Df> Vertices;
    
    void AddTriangle(Vertex2Df v1, Vertex2Df v2, Vertex2Df v3);
    std::vector<SDL_Vertex> GetVertexBuffer();
    std::vector<SDL_FPoint> GetPoints();
    
    bool Equals(Mesh& other);
    bool operator==(Mesh& other);
};
```

### Creating Custom Meshes

#### Basic Triangle Mesh
```cpp
CacoEngine::Mesh CreateTriangleMesh() {
    CacoEngine::Mesh mesh;
    
    // Define triangle vertices
    auto v1 = CacoEngine::Vertex2Df(CacoEngine::Vector2Df(0, 0), CacoEngine::Colors[(int)CacoEngine::Color::Red]);
    auto v2 = CacoEngine::Vertex2Df(CacoEngine::Vector2Df(50, 0), CacoEngine::Colors[(int)CacoEngine::Color::Green]);
    auto v3 = CacoEngine::Vertex2Df(CacoEngine::Vector2Df(25, 50), CacoEngine::Colors[(int)CacoEngine::Color::Blue]);
    
    mesh.AddTriangle(v1, v2, v3);
    return mesh;
}
```

#### Complex Mesh Creation
```cpp
CacoEngine::Mesh CreateStarMesh(float radius, int points) {
    CacoEngine::Mesh mesh;
    
    float angleStep = 2.0f * M_PI / points;
    CacoEngine::Vector2Df center(0, 0);
    
    for (int i = 0; i < points; ++i) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        // Outer vertices
        auto outer1 = CacoEngine::Vector2Df(radius * cos(angle1), radius * sin(angle1));
        auto outer2 = CacoEngine::Vector2Df(radius * cos(angle2), radius * sin(angle2));
        
        // Inner vertex
        auto inner = CacoEngine::Vector2Df(
            (radius * 0.5f) * cos(angle1 + angleStep * 0.5f),
            (radius * 0.5f) * sin(angle1 + angleStep * 0.5f)
        );
        
        // Create triangles
        mesh.AddTriangle(
            CacoEngine::Vertex2Df(center, CacoEngine::Colors[(int)CacoEngine::Color::White]),
            CacoEngine::Vertex2Df(outer1, CacoEngine::Colors[(int)CacoEngine::Color::Red]),
            CacoEngine::Vertex2Df(inner, CacoEngine::Colors[(int)CacoEngine::Color::Blue])
        );
        
        mesh.AddTriangle(
            CacoEngine::Vertex2Df(center, CacoEngine::Colors[(int)CacoEngine::Color::White]),
            CacoEngine::Vertex2Df(inner, CacoEngine::Colors[(int)CacoEngine::Color::Blue]),
            CacoEngine::Vertex2Df(outer2, CacoEngine::Colors[(int)CacoEngine::Color::Red])
        );
    }
    
    return mesh;
}
```

## Built-in Object Types

### 1. Triangle Objects

```cpp
class Triangle : public Object {
public:
    Triangle(Vertex2Df v1, Vertex2Df v2, Vertex2Df v3, RGBA color);
    Triangle(const Triangle& other);
    
    Triangle& operator=(const Triangle& other);
    virtual ~Triangle();
};
```

#### Creating Triangles
```cpp
auto triangle = std::make_shared<CacoEngine::Triangle>(
    CacoEngine::Vertex2Df(CacoEngine::Vector2Df(100, 100)),
    CacoEngine::Vertex2Df(CacoEngine::Vector2Df(200, 100)),
    CacoEngine::Vertex2Df(CacoEngine::Vector2Df(150, 200)),
    CacoEngine::Colors[(int)CacoEngine::Color::Red]
);
```

### 2. Rectangle Objects

```cpp
class Rectangle : public Object {
public:
    Vector2Df Dimensions;
    
    Rectangle(Vector2Df position, Vector2Df size, RGBA color = Colors[(int)Color::White], Texture texture = Texture());
    virtual ~Rectangle();
};
```

#### Creating Rectangles
```cpp
auto rectangle = std::make_shared<CacoEngine::Rectangle>(
    CacoEngine::Vector2Df(100, 100),  // Position
    CacoEngine::Vector2Df(200, 150),  // Size
    CacoEngine::Colors[(int)CacoEngine::Color::Blue]
);
```

### 3. Circle Objects

```cpp
class Circle : public Object {
protected:
    void GeneratePoints();
    double Radius;
    
public:
    double GetRadius();
    Vector2Df GetCenter();
    void SetRadius(double radius);
    
    bool CollidesWith(Circle& other);
    
    Circle(Vector2Df center, double radius = 1.0);
    virtual ~Circle();
};
```

#### Creating Circles
```cpp
auto circle = std::make_shared<CacoEngine::Circle>(
    CacoEngine::Vector2Df(300, 300),  // Center
    75.0                              // Radius
);
circle->SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::Green]);
```

### 4. Sprite Objects

```cpp
class Sprite : public Rectangle {
public:
    Sprite(Texture texture, Vector2Df position = Vector2Df(), Vector2Df size = Vector2Df());
    virtual ~Sprite();
};
```

#### Creating Sprites
```cpp
// Load texture
auto texture = CacoEngine::TextureManager::CreateTexture("player.png", renderer);

// Create sprite
auto sprite = std::make_shared<CacoEngine::Sprite>(
    texture,
    CacoEngine::Vector2Df(100, 100),  // Position
    CacoEngine::Vector2Df(64, 64)     // Size
);
```

## Rendering System

### Rendering Modes

```cpp
enum class RasterizeMode {
    Points,     // Render vertices as points
    WireFrame,  // Render edges only
    SolidColor, // Render filled with solid color
    Texture     // Render with texture applied
};
```

### Renderer Architecture

```cpp
class Renderer {
private:
    SDL_Renderer* Instance;
    
public:
    RGBA Color;
    
    void Clear(RGBA color = RGBA());
    void SetColor(RGBA color);
    SDL_Renderer* GetInstance();
    
    Renderer(SDL_Window* window = nullptr);
    ~Renderer();
};
```

### Rendering Pipeline

#### 1. Object Rendering
```cpp
void Engine::Render(SDL_Renderer* renderer, std::vector<std::shared_ptr<Object>>& objects) {
    for (auto& obj : objects) {
        switch (obj->FillMode) {
            case RasterizeMode::Points:
                RenderPoints(renderer, obj);
                break;
            case RasterizeMode::WireFrame:
                RenderWireframe(renderer, obj);
                break;
            case RasterizeMode::SolidColor:
                RenderSolidColor(renderer, obj);
                break;
            case RasterizeMode::Texture:
                RenderTextured(renderer, obj);
                break;
        }
    }
}
```

#### 2. Mesh Rendering
```cpp
void RenderMesh(SDL_Renderer* renderer, const Mesh& mesh, RasterizeMode mode) {
    auto vertices = mesh.GetVertexBuffer();
    
    switch (mode) {
        case RasterizeMode::Points: {
            auto points = mesh.GetPoints();
            SDL_RenderDrawPointsF(renderer, points.data(), points.size());
            break;
        }
        case RasterizeMode::WireFrame: {
            // Render edges
            for (size_t i = 0; i < vertices.size(); i += 3) {
                SDL_RenderDrawLineF(renderer, 
                    vertices[i].position.x, vertices[i].position.y,
                    vertices[i+1].position.x, vertices[i+1].position.y);
                SDL_RenderDrawLineF(renderer, 
                    vertices[i+1].position.x, vertices[i+1].position.y,
                    vertices[i+2].position.x, vertices[i+2].position.y);
                SDL_RenderDrawLineF(renderer, 
                    vertices[i+2].position.x, vertices[i+2].position.y,
                    vertices[i].position.x, vertices[i].position.y);
            }
            break;
        }
        case RasterizeMode::SolidColor:
        case RasterizeMode::Texture: {
            SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
            break;
        }
    }
}
```

## Texture System

### Texture Management

```cpp
class Texture {
public:
    int ID;
    SDL_Texture* mTexture;
    
    Texture(int id = 0, SDL_Texture* texture = nullptr);
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    ~Texture();
};
```

### TextureManager

```cpp
class TextureManager {
public:
    static std::vector<Texture> Textures;
    
    static Texture CreateTexture(std::string_view path, Renderer& renderer) {
        Surface surface = Surface(IMG_Load(path.data()));
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetInstance(), surface.Screen);
        
        int id = Textures.size();
        Texture newTexture(id, texture);
        Textures.push_back(newTexture);
        
        return newTexture;
    }
    
    static Texture GetTexture(int id) {
        if (id >= 0 && id < Textures.size()) {
            return Textures[id];
        }
        return Texture();
    }
};
```

### Texture Application

#### Basic Texture Usage
```cpp
void ApplyTextureToObject(std::shared_ptr<Object> obj, const std::string& texturePath, Renderer& renderer) {
    // Load texture
    auto texture = TextureManager::CreateTexture(texturePath, renderer);
    
    // Apply to object
    obj->mTexture = texture;
    obj->FillMode = RasterizeMode::Texture;
    
    // Set texture coordinates for vertices
    for (auto& vertex : obj->ObjectMesh.Vertices) {
        vertex.TextureCoordinates = Vector2Df(
            vertex.Position.X / obj->ObjectMesh.Vertices.size(),
            vertex.Position.Y / obj->ObjectMesh.Vertices.size()
        );
    }
}
```

#### Advanced Texture Mapping
```cpp
void SetTextureCoordinates(Object& obj, const std::vector<Vector2Df>& texCoords) {
    if (texCoords.size() != obj.ObjectMesh.Vertices.size()) {
        throw std::invalid_argument("Texture coordinates count must match vertex count");
    }
    
    for (size_t i = 0; i < obj.ObjectMesh.Vertices.size(); ++i) {
        obj.ObjectMesh.Vertices[i].TextureCoordinates = texCoords[i];
    }
}
```

## Object Lifecycle Management

### Object Creation Patterns

#### Factory Pattern for Objects
```cpp
class ObjectFactory {
public:
    static std::shared_ptr<Object> CreateTriangle(Vector2Df pos, float size, RGBA color) {
        auto v1 = Vertex2Df(pos, color);
        auto v2 = Vertex2Df(pos + Vector2Df(size, 0), color);
        auto v3 = Vertex2Df(pos + Vector2Df(size/2, size), color);
        
        return std::make_shared<Triangle>(v1, v2, v3, color);
    }
    
    static std::shared_ptr<Object> CreateRectangle(Vector2Df pos, Vector2Df size, RGBA color) {
        return std::make_shared<Rectangle>(pos, size, color);
    }
    
    static std::shared_ptr<Object> CreateCircle(Vector2Df pos, double radius, RGBA color) {
        auto circle = std::make_shared<Circle>(pos, radius);
        circle->SetFillColor(color);
        return circle;
    }
};
```

#### Builder Pattern for Complex Objects
```cpp
class ObjectBuilder {
private:
    std::shared_ptr<Object> obj;
    
public:
    ObjectBuilder() : obj(std::make_shared<Object>()) {}
    
    ObjectBuilder& SetPosition(Vector2Df pos) {
        obj->Position = pos;
        return *this;
    }
    
    ObjectBuilder& SetColor(RGBA color) {
        obj->SetFillColor(color);
        return *this;
    }
    
    ObjectBuilder& SetRenderMode(RasterizeMode mode) {
        obj->FillMode = mode;
        return *this;
    }
    
    ObjectBuilder& AddVertex(Vertex2Df vertex) {
        obj->AddVertex(vertex);
        return *this;
    }
    
    ObjectBuilder& SetTexture(const Texture& texture) {
        obj->mTexture = texture;
        return *this;
    }
    
    std::shared_ptr<Object> Build() {
        return obj;
    }
};

// Usage
auto customObject = ObjectBuilder()
    .SetPosition(Vector2Df(100, 100))
    .SetColor(Colors[(int)Color::Red])
    .SetRenderMode(RasterizeMode::SolidColor)
    .AddVertex(Vertex2Df(Vector2Df(0, 0)))
    .AddVertex(Vertex2Df(Vector2Df(50, 0)))
    .AddVertex(Vertex2Df(Vector2Df(25, 50)))
    .Build();
```

### Object Transformation

#### Translation
```cpp
void Object::Translate(Vector2Df offset) {
    Position += offset;
    
    // Update mesh vertices
    for (auto& vertex : ObjectMesh.Vertices) {
        vertex.Position += offset;
    }
}
```

#### Rotation
```cpp
void RotateObject(Object& obj, double angle, Vector2Df pivot = Vector2Df(0, 0)) {
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    
    for (auto& vertex : obj.ObjectMesh.Vertices) {
        // Translate to pivot
        Vector2Df translated = vertex.Position - pivot;
        
        // Rotate
        Vector2Df rotated(
            translated.X * cos_a - translated.Y * sin_a,
            translated.X * sin_a + translated.Y * cos_a
        );
        
        // Translate back
        vertex.Position = rotated + pivot;
    }
}
```

#### Scaling
```cpp
void ScaleObject(Object& obj, Vector2Df scale, Vector2Df pivot = Vector2Df(0, 0)) {
    for (auto& vertex : obj.ObjectMesh.Vertices) {
        Vector2Df translated = vertex.Position - pivot;
        Vector2Df scaled(translated.X * scale.X, translated.Y * scale.Y);
        vertex.Position = scaled + pivot;
    }
}
```

## Advanced Rendering Techniques

### Batch Rendering
```cpp
class BatchRenderer {
private:
    std::vector<SDL_Vertex> vertexBuffer;
    std::vector<int> indexBuffer;
    size_t maxBatchSize;
    
public:
    BatchRenderer(size_t maxSize = 1000) : maxBatchSize(maxSize) {
        vertexBuffer.reserve(maxSize);
        indexBuffer.reserve(maxSize);
    }
    
    void AddObject(const Object& obj) {
        auto vertices = obj.ObjectMesh.GetVertexBuffer();
        
        if (vertexBuffer.size() + vertices.size() > maxBatchSize) {
            Flush();
        }
        
        int baseIndex = vertexBuffer.size();
        vertexBuffer.insert(vertexBuffer.end(), vertices.begin(), vertices.end());
        
        // Generate indices for triangles
        for (size_t i = 0; i < vertices.size(); i += 3) {
            indexBuffer.push_back(baseIndex + i);
            indexBuffer.push_back(baseIndex + i + 1);
            indexBuffer.push_back(baseIndex + i + 2);
        }
    }
    
    void Flush(SDL_Renderer* renderer) {
        if (!vertexBuffer.empty()) {
            SDL_RenderGeometry(renderer, nullptr, 
                vertexBuffer.data(), vertexBuffer.size(),
                indexBuffer.data(), indexBuffer.size());
            
            vertexBuffer.clear();
            indexBuffer.clear();
        }
    }
};
```

### Frustum Culling
```cpp
class FrustumCuller {
private:
    Vector2Df viewportMin, viewportMax;
    
public:
    void SetViewport(Vector2Df min, Vector2Df max) {
        viewportMin = min;
        viewportMax = max;
    }
    
    bool IsVisible(const Object& obj) {
        // Simple AABB test
        Vector2Df objMin = obj.Position;
        Vector2Df objMax = obj.Position;
        
        // Find object bounds
        for (const auto& vertex : obj.ObjectMesh.Vertices) {
            Vector2Df pos = obj.Position + vertex.Position;
            objMin.X = std::min(objMin.X, pos.X);
            objMin.Y = std::min(objMin.Y, pos.Y);
            objMax.X = std::max(objMax.X, pos.X);
            objMax.Y = std::max(objMax.Y, pos.Y);
        }
        
        // Check intersection with viewport
        return !(objMax.X < viewportMin.X || objMin.X > viewportMax.X ||
                 objMax.Y < viewportMin.Y || objMin.Y > viewportMax.Y);
    }
};
```

## Practical Examples

### Complete Game Object System

```cpp
class GameObjectManager {
private:
    std::vector<std::shared_ptr<Object>> objects;
    BatchRenderer batchRenderer;
    FrustumCuller culler;
    
public:
    void Initialize(Vector2Df viewportSize) {
        culler.SetViewport(Vector2Df(0, 0), viewportSize);
    }
    
    void AddObject(std::shared_ptr<Object> obj) {
        objects.push_back(obj);
    }
    
    void Update(double deltaTime) {
        // Update object logic here
        for (auto& obj : objects) {
            // Custom update logic per object type
        }
    }
    
    void Render(SDL_Renderer* renderer) {
        for (auto& obj : objects) {
            if (culler.IsVisible(*obj)) {
                batchRenderer.AddObject(*obj);
            }
        }
        batchRenderer.Flush(renderer);
    }
    
    void Clear() {
        objects.clear();
    }
};
```

### Creating a Complete Scene

```cpp
void CreateGameScene(GameObjectManager& manager, Renderer& renderer) {
    // Create background
    auto background = std::make_shared<Rectangle>(
        Vector2Df(0, 0),
        Vector2Df(800, 600),
        Colors[(int)Color::Blue]
    );
    manager.AddObject(background);
    
    // Create player sprite
    auto playerTexture = TextureManager::CreateTexture("player.png", renderer);
    auto player = std::make_shared<Sprite>(
        playerTexture,
        Vector2Df(100, 400),
        Vector2Df(64, 64)
    );
    manager.AddObject(player);
    
    // Create obstacles
    for (int i = 0; i < 5; ++i) {
        auto obstacle = std::make_shared<Circle>(
            Vector2Df(150 + i * 120, 300),
            30.0
        );
        obstacle->SetFillColor(Colors[(int)Color::Red]);
        manager.AddObject(obstacle);
    }
    
    // Create star-shaped power-ups
    for (int i = 0; i < 3; ++i) {
        auto star = std::make_shared<Object>();
        star->ObjectMesh = CreateStarMesh(25.0f, 5);
        star->Position = Vector2Df(200 + i * 200, 200);
        star->SetFillColor(Colors[(int)Color::White]);
        star->FillMode = RasterizeMode::SolidColor;
        manager.AddObject(star);
    }
}
```

## Next Steps

Understanding objects and rendering prepares you for:

- **[Chapter 4: Physics System](04-physics.md)** - Adding physics to your objects
- **[Chapter 5: Input & Events](05-input-events.md)** - Making objects interactive
- **[Chapter 6: Assets & Textures](06-assets-textures.md)** - Advanced texture management

## Summary

CacoEngine's object and rendering system provides:

- **Flexible Object Hierarchy**: From simple shapes to complex meshes
- **Powerful Vertex System**: Precise control over geometry
- **Multiple Rendering Modes**: Points, wireframe, solid, and textured
- **Efficient Rendering**: Batch rendering and frustum culling
- **Easy Texture Integration**: Simple texture loading and application
- **Extensible Design**: Easy to create custom object types

The system is designed for both simplicity and power, allowing beginners to create basic objects quickly while providing advanced features for complex game requirements.

---

**Next:** [Chapter 4: Physics System](04-physics.md) 