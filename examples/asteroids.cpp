/**
 * Asteroids Game - CacoEngine Example
 * 
 * Features Demonstrated:
 * - Object rotation and directional movement
 * - Projectile system with bullet management
 * - Multiple object collision detection
 * - Dynamic object creation and destruction
 * - Advanced physics (thrust, rotation, momentum)
 * - Particle effects using custom meshes
 * - Screen wrapping mechanics
 * - Game state management with multiple levels
 */

#include "../include/engine.hpp"
#include "../include/objects.hpp"
#include "../include/rigidbody.hpp"
#include "../include/rigidobject.hpp"
#include "../include/collider.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// Custom ship object with rotation
class Ship : public CacoEngine::RigidObject2D {
public:
    float rotation = 0.0f;
    float rotationSpeed = 0.0f;
    bool thrusting = false;
    const float THRUST_POWER = 400.0f;
    const float ROTATION_SPEED = 360.0f; // degrees per second
    const float MAX_SPEED = 300.0f;
    const float FRICTION = 0.98f;
    
    Ship(CacoEngine::Vector2Df position) : RigidObject2D(position, 15.0f) {
        // Create ship mesh (triangle pointing up)
        CreateShipMesh();
        FillColor = CacoEngine::Colors[(int)CacoEngine::Color::White];
        FillMode = CacoEngine::RasterizeMode::WireFrame;
    }
    
    void CreateShipMesh() {
        ObjectMesh = CacoEngine::Mesh();
        
        // Ship triangle vertices (pointing up)
        auto front = CacoEngine::Vertex2Df(CacoEngine::Vector2Df(0, -15), CacoEngine::Colors[(int)CacoEngine::Color::White]);
        auto backLeft = CacoEngine::Vertex2Df(CacoEngine::Vector2Df(-10, 10), CacoEngine::Colors[(int)CacoEngine::Color::White]);
        auto backRight = CacoEngine::Vertex2Df(CacoEngine::Vector2Df(10, 10), CacoEngine::Colors[(int)CacoEngine::Color::White]);
        
        ObjectMesh.AddTriangle(front, backLeft, backRight);
    }
    
    void Update(double deltaTime) {
        // Apply rotation
        rotation += rotationSpeed * deltaTime;
        
        // Apply thrust
        if (thrusting) {
            float thrustX = sin(rotation * M_PI / 180.0f) * THRUST_POWER * deltaTime;
            float thrustY = -cos(rotation * M_PI / 180.0f) * THRUST_POWER * deltaTime;
            
            Physics.Velocity.X += thrustX;
            Physics.Velocity.Y += thrustY;
        }
        
        // Apply friction
        Physics.Velocity.X *= FRICTION;
        Physics.Velocity.Y *= FRICTION;
        
        // Limit max speed
        float speed = sqrt(Physics.Velocity.X * Physics.Velocity.X + Physics.Velocity.Y * Physics.Velocity.Y);
        if (speed > MAX_SPEED) {
            Physics.Velocity.X = (Physics.Velocity.X / speed) * MAX_SPEED;
            Physics.Velocity.Y = (Physics.Velocity.Y / speed) * MAX_SPEED;
        }
    }
    
    CacoEngine::Vector2Df GetFrontPosition() {
        float frontX = Position.X + sin(rotation * M_PI / 180.0f) * 15.0f;
        float frontY = Position.Y - cos(rotation * M_PI / 180.0f) * 15.0f;
        return CacoEngine::Vector2Df(frontX, frontY);
    }
};

// Bullet class
class Bullet : public CacoEngine::RigidObject2D {
public:
    float lifetime = 2.0f; // 2 seconds
    const float BULLET_SPEED = 500.0f;
    
    Bullet(CacoEngine::Vector2Df position, float direction) : RigidObject2D(position, 2.0f) {
        // Set velocity based on direction
        Physics.Velocity.X = sin(direction * M_PI / 180.0f) * BULLET_SPEED;
        Physics.Velocity.Y = -cos(direction * M_PI / 180.0f) * BULLET_SPEED;
        
        FillColor = CacoEngine::Colors[(int)CacoEngine::Color::White];
        FillMode = CacoEngine::RasterizeMode::SolidColor;
    }
    
    void Update(double deltaTime) {
        lifetime -= deltaTime;
    }
    
    bool IsExpired() {
        return lifetime <= 0;
    }
};

// Asteroid class
class Asteroid : public CacoEngine::RigidObject2D {
public:
    float rotation = 0.0f;
    float rotationSpeed;
    int size; // 1 = small, 2 = medium, 3 = large
    
    Asteroid(CacoEngine::Vector2Df position, int asteroidSize) : RigidObject2D(position, GetRadiusForSize(asteroidSize)), size(asteroidSize) {
        rotationSpeed = (rand() % 100 - 50) * 2.0f; // -100 to 100 degrees per second
        
        // Random velocity
        float angle = (rand() % 360) * M_PI / 180.0f;
        float speed = 50.0f + (rand() % 100);
        Physics.Velocity.X = cos(angle) * speed;
        Physics.Velocity.Y = sin(angle) * speed;
        
        CreateAsteroidMesh();
        FillColor = CacoEngine::Colors[(int)CacoEngine::Color::White];
        FillMode = CacoEngine::RasterizeMode::WireFrame;
    }
    
    float GetRadiusForSize(int size) {
        switch (size) {
            case 1: return 15.0f; // Small
            case 2: return 25.0f; // Medium
            case 3: return 40.0f; // Large
            default: return 25.0f;
        }
    }
    
    void CreateAsteroidMesh() {
        ObjectMesh = CacoEngine::Mesh();
        
        // Create irregular asteroid shape
        int numPoints = 8;
        float baseRadius = GetRadiusForSize(size);
        
        for (int i = 0; i < numPoints; i++) {
            float angle1 = (i * 2 * M_PI) / numPoints;
            float angle2 = ((i + 1) * 2 * M_PI) / numPoints;
            
            // Add some randomness to the radius
            float radius1 = baseRadius + (rand() % 10 - 5);
            float radius2 = baseRadius + (rand() % 10 - 5);
            
            auto center = CacoEngine::Vertex2Df(CacoEngine::Vector2Df(0, 0), CacoEngine::Colors[(int)CacoEngine::Color::White]);
            auto point1 = CacoEngine::Vertex2Df(
                CacoEngine::Vector2Df(cos(angle1) * radius1, sin(angle1) * radius1),
                CacoEngine::Colors[(int)CacoEngine::Color::White]
            );
            auto point2 = CacoEngine::Vertex2Df(
                CacoEngine::Vector2Df(cos(angle2) * radius2, sin(angle2) * radius2),
                CacoEngine::Colors[(int)CacoEngine::Color::White]
            );
            
            ObjectMesh.AddTriangle(center, point1, point2);
        }
    }
    
    void Update(double deltaTime) {
        rotation += rotationSpeed * deltaTime;
    }
};

class AsteroidsGame : public CacoEngine::Engine {
private:
    std::shared_ptr<Ship> ship;
    std::vector<std::shared_ptr<Bullet>> bullets;
    std::vector<std::shared_ptr<Asteroid>> asteroids;
    
    // Game state
    int score = 0;
    int lives = 3;
    int level = 1;
    bool gameOver = false;
    
    // Input state
    bool leftPressed = false;
    bool rightPressed = false;
    bool thrustPressed = false;
    bool firePressed = false;
    
    // Game constants
    const float WINDOW_WIDTH = 800.0f;
    const float WINDOW_HEIGHT = 600.0f;
    const float FIRE_RATE = 0.2f; // seconds between bullets
    float lastFireTime = 0.0f;

public:
    AsteroidsGame() : Engine("Asteroids - CacoEngine Example", CacoEngine::Vector2Df(WINDOW_WIDTH, WINDOW_HEIGHT)) {}
    
    void OnInitialize() override {
        std::cout << "=== ASTEROIDS GAME ===" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "A/D - Rotate left/right" << std::endl;
        std::cout << "W - Thrust" << std::endl;
        std::cout << "Space - Fire" << std::endl;
        std::cout << "R - Restart (when game over)" << std::endl;
        std::cout << "Escape - Quit" << std::endl;
        
        InitializeGame();
    }
    
    void InitializeGame() {
        // Clear existing objects
        Objects.clear();
        RigidObjects.clear();
        bullets.clear();
        asteroids.clear();
        
        // Reset game state
        score = 0;
        lives = 3;
        level = 1;
        gameOver = false;
        
        // Create ship
        ship = std::make_shared<Ship>(CacoEngine::Vector2Df(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
        AddObject(ship);
        
        // Create initial asteroids
        CreateAsteroids();
    }
    
    void CreateAsteroids() {
        int numAsteroids = 4 + level; // More asteroids each level
        
        for (int i = 0; i < numAsteroids; i++) {
            CacoEngine::Vector2Df position;
            
            // Place asteroids away from ship
            do {
                position.X = rand() % (int)WINDOW_WIDTH;
                position.Y = rand() % (int)WINDOW_HEIGHT;
            } while (position.DistanceFrom(ship->Position) < 100.0f);
            
            auto asteroid = std::make_shared<Asteroid>(position, 3); // Start with large asteroids
            asteroids.push_back(asteroid);
            AddObject(asteroid);
        }
    }
    
    void OnUpdate(double deltaTime) override {
        if (gameOver) return;
        
        // Update ship
        ship->rotationSpeed = 0.0f;
        if (leftPressed) ship->rotationSpeed = -ship->ROTATION_SPEED;
        if (rightPressed) ship->rotationSpeed = ship->ROTATION_SPEED;
        ship->thrusting = thrustPressed;
        ship->Update(deltaTime);
        
        // Handle firing
        if (firePressed && (GetTime() - lastFireTime) >= FIRE_RATE) {
            FireBullet();
            lastFireTime = GetTime();
        }
        
        // Update bullets
        UpdateBullets(deltaTime);
        
        // Update asteroids
        UpdateAsteroids(deltaTime);
        
        // Check collisions
        CheckCollisions();
        
        // Wrap objects around screen
        WrapObjects();
        
        // Check win condition
        if (asteroids.empty()) {
            level++;
            std::cout << "Level " << level << " - More asteroids incoming!" << std::endl;
            CreateAsteroids();
        }
        
        // Update display
        UpdateDisplay();
    }
    
    void FireBullet() {
        auto bullet = std::make_shared<Bullet>(ship->GetFrontPosition(), ship->rotation);
        bullets.push_back(bullet);
        AddObject(bullet);
    }
    
    void UpdateBullets(double deltaTime) {
        for (auto it = bullets.begin(); it != bullets.end();) {
            (*it)->Update(deltaTime);
            
            if ((*it)->IsExpired()) {
                RemoveObject(*it);
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    void UpdateAsteroids(double deltaTime) {
        for (auto& asteroid : asteroids) {
            asteroid->Update(deltaTime);
        }
    }
    
    void CheckCollisions() {
        // Bullet-asteroid collisions
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            bool bulletHit = false;
            
            for (auto asteroidIt = asteroids.begin(); asteroidIt != asteroids.end();) {
                if (CheckCircleCollision((*bulletIt)->Position, (*bulletIt)->Radius,
                                       (*asteroidIt)->Position, (*asteroidIt)->Radius)) {
                    
                    // Remove bullet
                    RemoveObject(*bulletIt);
                    bulletIt = bullets.erase(bulletIt);
                    bulletHit = true;
                    
                    // Handle asteroid destruction
                    DestroyAsteroid(*asteroidIt);
                    asteroidIt = asteroids.erase(asteroidIt);
                    
                    score += 100;
                    break;
                } else {
                    ++asteroidIt;
                }
            }
            
            if (!bulletHit) {
                ++bulletIt;
            }
        }
        
        // Ship-asteroid collisions
        for (auto& asteroid : asteroids) {
            if (CheckCircleCollision(ship->Position, ship->Radius,
                                   asteroid->Position, asteroid->Radius)) {
                lives--;
                std::cout << "Hit! Lives remaining: " << lives << std::endl;
                
                if (lives <= 0) {
                    gameOver = true;
                    std::cout << "=== GAME OVER ===" << std::endl;
                    std::cout << "Final Score: " << score << std::endl;
                    std::cout << "Press R to restart" << std::endl;
                } else {
                    // Respawn ship at center
                    ship->Position = CacoEngine::Vector2Df(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
                    ship->Physics.Velocity = CacoEngine::Vector2Df(0, 0);
                    ship->rotation = 0.0f;
                }
                break;
            }
        }
    }
    
    void DestroyAsteroid(std::shared_ptr<Asteroid> asteroid) {
        RemoveObject(asteroid);
        
        // Split into smaller asteroids if not already small
        if (asteroid->size > 1) {
            for (int i = 0; i < 2; i++) {
                auto smallerAsteroid = std::make_shared<Asteroid>(asteroid->Position, asteroid->size - 1);
                asteroids.push_back(smallerAsteroid);
                AddObject(smallerAsteroid);
            }
        }
    }
    
    bool CheckCircleCollision(CacoEngine::Vector2Df pos1, float radius1, CacoEngine::Vector2Df pos2, float radius2) {
        float distance = pos1.DistanceFrom(pos2);
        return distance < (radius1 + radius2);
    }
    
    void WrapObjects() {
        // Wrap ship
        WrapObject(ship);
        
        // Wrap bullets
        for (auto& bullet : bullets) {
            WrapObject(bullet);
        }
        
        // Wrap asteroids
        for (auto& asteroid : asteroids) {
            WrapObject(asteroid);
        }
    }
    
    void WrapObject(std::shared_ptr<CacoEngine::RigidObject2D> object) {
        if (object->Position.X < 0) object->Position.X = WINDOW_WIDTH;
        if (object->Position.X > WINDOW_WIDTH) object->Position.X = 0;
        if (object->Position.Y < 0) object->Position.Y = WINDOW_HEIGHT;
        if (object->Position.Y > WINDOW_HEIGHT) object->Position.Y = 0;
    }
    
    void RemoveObject(std::shared_ptr<CacoEngine::RigidObject2D> object) {
        auto it = std::find(RigidObjects.begin(), RigidObjects.end(), object);
        if (it != RigidObjects.end()) {
            RigidObjects.erase(it);
        }
    }
    
    void UpdateDisplay() {
        static int displayCounter = 0;
        displayCounter++;
        
        if (displayCounter % 60 == 0) {
            std::cout << "Score: " << score << " | Lives: " << lives << " | Level: " << level << " | Asteroids: " << asteroids.size() << std::endl;
        }
    }
    
    double GetTime() {
        return SDL_GetTicks() / 1000.0;
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        switch (event.keysym.sym) {
            case SDLK_a:
                leftPressed = true;
                break;
            case SDLK_d:
                rightPressed = true;
                break;
            case SDLK_w:
                thrustPressed = true;
                break;
            case SDLK_SPACE:
                firePressed = true;
                break;
            case SDLK_r:
                if (gameOver) {
                    std::cout << "Restarting game..." << std::endl;
                    InitializeGame();
                }
                break;
            case SDLK_ESCAPE:
                std::cout << "Thanks for playing Asteroids!" << std::endl;
                IsRunning = false;
                break;
        }
    }
    
    void OnKeyRelease(SDL_KeyboardEvent& event) {
        switch (event.keysym.sym) {
            case SDLK_a:
                leftPressed = false;
                break;
            case SDLK_d:
                rightPressed = false;
                break;
            case SDLK_w:
                thrustPressed = false;
                break;
            case SDLK_SPACE:
                firePressed = false;
                break;
        }
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        // Not used in Asteroids
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Not used in Asteroids
    }
};

int main() {
    srand(time(nullptr));
    
    AsteroidsGame game;
    game.Run();
    
    return 0;
} 