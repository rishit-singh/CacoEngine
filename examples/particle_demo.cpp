/**
 * Particle System Demo - CacoEngine Example
 * 
 * Features Demonstrated:
 * - Custom mesh creation and manipulation
 * - Particle systems with physics
 * - Advanced rendering techniques
 * - Mouse interaction for particle generation
 * - Dynamic object management
 * - Visual effects and animations
 * - Performance considerations with many objects
 */

#include "../include/engine.hpp"
#include "../include/objects.hpp"
#include "../include/rigidbody.hpp"
#include "../include/rigidobject.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

class Particle : public CacoEngine::RigidObject2D {
public:
    float lifetime = 2.0f;
    float maxLifetime = 2.0f;
    CacoEngine::RGBA originalColor;
    
    Particle(CacoEngine::Vector2Df pos, CacoEngine::Vector2Df vel, CacoEngine::RGBA color, float life = 2.0f)
        : RigidObject2D(pos, 2.0f), maxLifetime(life), lifetime(life), originalColor(color) {
        Physics.Velocity = vel;
        Physics.Acceleration = CacoEngine::Vector2Df(0, 100); // Gravity
        FillColor = color;
        FillMode = CacoEngine::RasterizeMode::SolidColor;
    }
    
    void Update(double deltaTime) {
        lifetime -= deltaTime;
        
        // Fade out over time
        float alpha = (lifetime / maxLifetime) * 255.0f;
        FillColor.A = std::max(0, (int)alpha);
        
        // Apply some friction
        Physics.Velocity.X *= 0.99f;
        Physics.Velocity.Y *= 0.99f;
    }
    
    bool IsExpired() {
        return lifetime <= 0;
    }
};

class ParticleSystem {
private:
    std::vector<std::shared_ptr<Particle>> particles;
    std::mt19937 rng;
    
public:
    ParticleSystem() : rng(std::random_device{}()) {}
    
    void CreateExplosion(CacoEngine::Vector2Df position, int count = 50) {
        std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
        std::uniform_real_distribution<float> speedDist(50.0f, 200.0f);
        std::uniform_real_distribution<float> lifeDist(1.0f, 3.0f);
        std::uniform_int_distribution<int> colorDist(0, 4);
        
        for (int i = 0; i < count; i++) {
            float angle = angleDist(rng);
            float speed = speedDist(rng);
            float life = lifeDist(rng);
            
            CacoEngine::Vector2Df velocity(
                cos(angle) * speed,
                sin(angle) * speed
            );
            
            auto particle = std::make_shared<Particle>(
                position,
                velocity,
                CacoEngine::Colors[colorDist(rng)],
                life
            );
            
            particles.push_back(particle);
        }
    }
    
    void CreateFirework(CacoEngine::Vector2Df position) {
        std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
        std::uniform_real_distribution<float> speedDist(100.0f, 300.0f);
        
        // Create circular pattern
        for (int i = 0; i < 30; i++) {
            float angle = (i / 30.0f) * 2.0f * M_PI;
            float speed = speedDist(rng);
            
            CacoEngine::Vector2Df velocity(
                cos(angle) * speed,
                sin(angle) * speed
            );
            
            auto particle = std::make_shared<Particle>(
                position,
                velocity,
                CacoEngine::Colors[(i % 5)],
                2.5f
            );
            
            particles.push_back(particle);
        }
    }
    
    void CreateTrail(CacoEngine::Vector2Df position, CacoEngine::Vector2Df direction) {
        std::uniform_real_distribution<float> spreadDist(-0.5f, 0.5f);
        std::uniform_real_distribution<float> speedDist(20.0f, 80.0f);
        
        for (int i = 0; i < 10; i++) {
            CacoEngine::Vector2Df velocity = direction;
            velocity.X += spreadDist(rng) * 50.0f;
            velocity.Y += spreadDist(rng) * 50.0f;
            
            float speed = speedDist(rng);
            velocity = velocity * (speed / velocity.DistanceFrom(CacoEngine::Vector2Df(0, 0)));
            
            auto particle = std::make_shared<Particle>(
                position,
                velocity,
                CacoEngine::Colors[(int)CacoEngine::Color::White],
                1.0f
            );
            
            particles.push_back(particle);
        }
    }
    
    void Update(double deltaTime) {
        for (auto it = particles.begin(); it != particles.end();) {
            (*it)->Update(deltaTime);
            
            if ((*it)->IsExpired()) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    std::vector<std::shared_ptr<Particle>>& GetParticles() {
        return particles;
    }
    
    void Clear() {
        particles.clear();
    }
    
    size_t GetParticleCount() {
        return particles.size();
    }
};

class ParticleDemo : public CacoEngine::Engine {
private:
    ParticleSystem particleSystem;
    
    // Demo objects
    std::vector<std::shared_ptr<CacoEngine::Circle>> attractors;
    std::vector<std::shared_ptr<CacoEngine::Rectangle>> emitters;
    
    // Mouse tracking
    CacoEngine::Vector2Df mousePos;
    CacoEngine::Vector2Df lastMousePos;
    bool mousePressed = false;
    
    // Demo settings
    enum DemoMode {
        EXPLOSIONS,
        FIREWORKS,
        TRAILS,
        ATTRACTORS
    } currentMode = EXPLOSIONS;
    
    // Timing
    double lastAutoEffect = 0.0;
    const double AUTO_EFFECT_INTERVAL = 2.0;
    
    const float WINDOW_WIDTH = 800.0f;
    const float WINDOW_HEIGHT = 600.0f;

public:
    ParticleDemo() : Engine("Particle System Demo - CacoEngine", CacoEngine::Vector2Df(WINDOW_WIDTH, WINDOW_HEIGHT)) {}
    
    void OnInitialize() override {
        std::cout << "=== PARTICLE SYSTEM DEMO ===" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "Click - Create particles at mouse position" << std::endl;
        std::cout << "1-4 - Switch demo modes" << std::endl;
        std::cout << "  1: Explosions" << std::endl;
        std::cout << "  2: Fireworks" << std::endl;
        std::cout << "  3: Trails" << std::endl;
        std::cout << "  4: Attractors" << std::endl;
        std::cout << "Space - Clear all particles" << std::endl;
        std::cout << "A - Auto-generate effects" << std::endl;
        std::cout << "Escape - Quit" << std::endl;
        
        SetupDemo();
    }
    
    void SetupDemo() {
        // Create some attractor points
        for (int i = 0; i < 3; i++) {
            auto attractor = std::make_shared<CacoEngine::Circle>(
                CacoEngine::Vector2Df(
                    (i + 1) * WINDOW_WIDTH / 4,
                    WINDOW_HEIGHT / 2
                ),
                20.0f,
                CacoEngine::Colors[(int)CacoEngine::Color::Blue]
            );
            attractor->FillMode = CacoEngine::RasterizeMode::WireFrame;
            attractors.push_back(attractor);
            AddObject(attractor);
        }
        
        // Create some particle emitters
        for (int i = 0; i < 2; i++) {
            auto emitter = std::make_shared<CacoEngine::Rectangle>(
                CacoEngine::Vector2Df(
                    (i + 1) * WINDOW_WIDTH / 3,
                    WINDOW_HEIGHT - 50
                ),
                CacoEngine::Vector2Df(20, 20),
                CacoEngine::Colors[(int)CacoEngine::Color::Green]
            );
            emitter->FillMode = CacoEngine::RasterizeMode::WireFrame;
            emitters.push_back(emitter);
            AddObject(emitter);
        }
        
        std::cout << "Current Mode: Explosions" << std::endl;
    }
    
    void OnUpdate(double deltaTime) override {
        // Update particle system
        particleSystem.Update(deltaTime);
        
        // Update visual objects from particle system
        UpdateParticleVisuals();
        
        // Auto-generate effects
        if (GetTime() - lastAutoEffect > AUTO_EFFECT_INTERVAL) {
            AutoGenerateEffect();
            lastAutoEffect = GetTime();
        }
        
        // Update display
        UpdateDisplay();
    }
    
    void UpdateParticleVisuals() {
        // Remove old particle visuals
        RigidObjects.erase(
            std::remove_if(RigidObjects.begin(), RigidObjects.end(),
                [](const std::shared_ptr<CacoEngine::RigidObject2D>& obj) {
                    return std::dynamic_pointer_cast<Particle>(obj) != nullptr;
                }),
            RigidObjects.end()
        );
        
        // Add current particles
        for (auto& particle : particleSystem.GetParticles()) {
            RigidObjects.push_back(particle);
        }
    }
    
    void AutoGenerateEffect() {
        if (particleSystem.GetParticleCount() > 500) return; // Limit particles
        
        CacoEngine::Vector2Df position(
            rand() % (int)WINDOW_WIDTH,
            rand() % (int)WINDOW_HEIGHT
        );
        
        CreateEffectAtPosition(position);
    }
    
    void CreateEffectAtPosition(CacoEngine::Vector2Df position) {
        switch (currentMode) {
            case EXPLOSIONS:
                particleSystem.CreateExplosion(position, 30);
                break;
            case FIREWORKS:
                particleSystem.CreateFirework(position);
                break;
            case TRAILS:
                {
                    CacoEngine::Vector2Df direction(
                        (rand() % 200 - 100) / 100.0f,
                        (rand() % 200 - 100) / 100.0f
                    );
                    particleSystem.CreateTrail(position, direction);
                }
                break;
            case ATTRACTORS:
                particleSystem.CreateExplosion(position, 20);
                break;
        }
    }
    
    void UpdateDisplay() {
        static int displayCounter = 0;
        displayCounter++;
        
        if (displayCounter % 60 == 0) {
            std::cout << "Particles: " << particleSystem.GetParticleCount() << " | Mode: " << GetModeString() << std::endl;
        }
    }
    
    std::string GetModeString() {
        switch (currentMode) {
            case EXPLOSIONS: return "Explosions";
            case FIREWORKS: return "Fireworks";
            case TRAILS: return "Trails";
            case ATTRACTORS: return "Attractors";
            default: return "Unknown";
        }
    }
    
    double GetTime() {
        return SDL_GetTicks() / 1000.0;
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        switch (event.keysym.sym) {
            case SDLK_1:
                currentMode = EXPLOSIONS;
                std::cout << "Mode: Explosions" << std::endl;
                break;
            case SDLK_2:
                currentMode = FIREWORKS;
                std::cout << "Mode: Fireworks" << std::endl;
                break;
            case SDLK_3:
                currentMode = TRAILS;
                std::cout << "Mode: Trails" << std::endl;
                break;
            case SDLK_4:
                currentMode = ATTRACTORS;
                std::cout << "Mode: Attractors" << std::endl;
                break;
            case SDLK_SPACE:
                particleSystem.Clear();
                std::cout << "Particles cleared!" << std::endl;
                break;
            case SDLK_a:
                for (int i = 0; i < 5; i++) {
                    AutoGenerateEffect();
                }
                break;
            case SDLK_ESCAPE:
                std::cout << "Thanks for watching the particle demo!" << std::endl;
                IsRunning = false;
                break;
        }
    }
    
    void OnKeyRelease(SDL_KeyboardEvent& event) {
        // Not used
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        mousePos = CacoEngine::Vector2Df(event.x, event.y);
        mousePressed = true;
        
        CreateEffectAtPosition(mousePos);
        
        std::cout << "Created " << GetModeString() << " at (" << mousePos.X << ", " << mousePos.Y << ")" << std::endl;
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Scroll to change modes
        if (event.y > 0) {
            currentMode = (DemoMode)((currentMode + 1) % 4);
        } else {
            currentMode = (DemoMode)((currentMode + 3) % 4);
        }
        std::cout << "Mode: " << GetModeString() << std::endl;
    }
};

int main() {
    srand(time(nullptr));
    
    ParticleDemo demo;
    demo.Run();
    
    return 0;
} 