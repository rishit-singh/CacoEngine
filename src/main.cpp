#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>
#include <functional>
#include <vector>

#include "engine.hpp"
#include "objects.hpp"
#include "rigidbody.hpp"
#include "rigidsprite.hpp"
#include "rigidobject.hpp"
#include "texture.hpp"
#include "tools.hpp"
#include "rigidsprite.hpp"
#include "random.hpp"
#include "box.hpp"
#include "vertex.hpp"
#include "collider.hpp"

namespace GameConstants {
    // Physics constants
    constexpr float DEFAULT_CIRCLE_RADIUS = 50.0f;
    constexpr float MOVEMENT_FORCE = 100.0f;
    constexpr float JUMP_FORCE = 700.0f;
    constexpr float BOUNDARY_FORCE = 200.0f;
    constexpr float WORLD_BOUNDARY = 1000.0f;
    
    // Rendering constants
    constexpr float DEFAULT_SPRITE_SIZE = 100.0f;
    constexpr float LARGE_SPRITE_SIZE = 600.0f;
    
    // Initial positions
    constexpr float INITIAL_X = 200.0f;
    constexpr float INITIAL_Y = 200.0f;
    
    // Physics simulation
    constexpr float GRAVITY_FORCE = 50.0f;
    constexpr float INITIAL_VELOCITY = 50.0f;
    
    // Color cycling
    constexpr int MAX_COLOR_INDEX = 4;
}

namespace TextureNames {
    constexpr std::string_view CACODEMON = "cacodemon";
    constexpr std::string_view CACODEMON_LEFT = "cacodemon_left";
    constexpr std::string_view CACODEMON_RIGHT = "cacodemon_right";
}

// Command pattern for input handling
class InputCommand {
public:
    virtual ~InputCommand() = default;
    virtual void execute() = 0;
};

class MoveCommand : public InputCommand {
private:
    std::vector<std::shared_ptr<CacoEngine::RigidObject2D>>& rigidObjects;
    CacoEngine::Vector2Df direction;
    
public:
    MoveCommand(std::vector<std::shared_ptr<CacoEngine::RigidObject2D>>& objects, CacoEngine::Vector2Df dir)
        : rigidObjects(objects), direction(dir) {}
    
    void execute() override {
        if (!rigidObjects.empty()) {
            rigidObjects.back()->RigidBody.AddForce(direction);
        }
    }
};

class JumpCommand : public InputCommand {
private:
    std::vector<std::shared_ptr<CacoEngine::Object>>& objects;
    size_t selectedIndex;
    
public:
    JumpCommand(std::vector<std::shared_ptr<CacoEngine::Object>>& objs, size_t index)
        : objects(objs), selectedIndex(index) {}
    
    void execute() override {
        if (selectedIndex < objects.size()) {
            objects[selectedIndex]->Translate(CacoEngine::Vector2Df(0, GameConstants::JUMP_FORCE));
        }
    }
};

class TextureManager {
private:
    std::unordered_map<std::string, CacoEngine::Texture> textureCache;
    
public:
    void loadTexture(const std::string& name, const std::string& filename, CacoEngine::Renderer& renderer) {
        textureCache[name] = CacoEngine::TextureManager::CreateTexture(filename, renderer);
    }
    
    const CacoEngine::Texture& getTexture(const std::string& name) const {
        auto it = textureCache.find(name);
        if (it != textureCache.end()) {
            return it->second;
        }
        throw std::runtime_error("Texture not found: " + name);
    }
    
    bool hasTexture(const std::string& name) const {
        return textureCache.find(name) != textureCache.end();
    }
};

class GameObjectManager {
private:
    std::vector<std::shared_ptr<CacoEngine::Object>> objects;
    std::vector<std::shared_ptr<CacoEngine::RigidObject2D>> rigidObjects;
    std::vector<CacoEngine::RigidCircle> rigidCircles;
    size_t selectedIndex = 0;
    
public:
    void addObject(std::shared_ptr<CacoEngine::Object> obj) {
        objects.push_back(std::move(obj));
    }
    
    void addRigidObject(std::shared_ptr<CacoEngine::RigidObject2D> obj) {
        rigidObjects.push_back(std::move(obj));
    }
    
    void addRigidCircle(const CacoEngine::RigidCircle& circle) {
        rigidCircles.push_back(circle);
    }
    
    CacoEngine::Object& getSelectedObject() {
        const size_t totalObjects = objects.size() + rigidObjects.size();
        if (totalObjects == 0) {
            throw std::runtime_error("No objects available");
        }
        
        if (selectedIndex < objects.size()) {
            return *objects[selectedIndex];
        } else {
            const size_t rigidIndex = selectedIndex - objects.size();
            if (rigidIndex < rigidObjects.size()) {
                return *rigidObjects[rigidIndex];
            }
        }
        
        // Fallback to last rigid object if index is out of bounds
        return *rigidObjects.back();
    }
    
    void selectNext() {
        const size_t totalObjects = objects.size() + rigidObjects.size();
        if (totalObjects > 0) {
            selectedIndex = (selectedIndex + 1) % totalObjects;
        }
    }
    
    void selectPrevious() {
        const size_t totalObjects = objects.size() + rigidObjects.size();
        if (totalObjects > 0) {
            selectedIndex = (selectedIndex == 0) ? totalObjects - 1 : selectedIndex - 1;
        }
    }
    
    size_t getSelectedIndex() const { return selectedIndex; }
    void incrementSelectedIndex() { selectedIndex++; }
    
    // Getters for engine integration
    std::vector<std::shared_ptr<CacoEngine::Object>>& getObjects() { return objects; }
    std::vector<std::shared_ptr<CacoEngine::RigidObject2D>>& getRigidObjects() { return rigidObjects; }
    std::vector<CacoEngine::RigidCircle>& getRigidCircles() { return rigidCircles; }
};

class Application : public CacoEngine::Engine {
private:
    int tintIndex = 0;
    CacoEngine::RGBA tintColor;
    
    std::unique_ptr<TextureManager> textureManager;
    std::unique_ptr<GameObjectManager> objectManager;
    
    // Input command mapping
    std::unordered_map<SDL_Keycode, std::unique_ptr<InputCommand>> keyCommands;
    
    void initializeTextures() {
        textureManager->loadTexture(std::string(TextureNames::CACODEMON), "cacodemon.png", EngineRenderer);
        textureManager->loadTexture(std::string(TextureNames::CACODEMON_LEFT), "cacodemon_left.png", EngineRenderer);
        textureManager->loadTexture(std::string(TextureNames::CACODEMON_RIGHT), "cacodemon_right.png", EngineRenderer);
    }
    
    void initializeObjects() {
        // Create initial sprite
        auto initialSprite = std::make_shared<CacoEngine::Object>(
            CacoEngine::Sprite(
                textureManager->getTexture(std::string(TextureNames::CACODEMON)),
                CacoEngine::Vector2Df(GameConstants::INITIAL_X, GameConstants::INITIAL_Y),
                CacoEngine::Vector2Df(GameConstants::LARGE_SPRITE_SIZE, GameConstants::LARGE_SPRITE_SIZE)
            )
        );
        objectManager->addObject(initialSprite);
        
        // Create initial rigid circle
        auto initialCircle = std::make_shared<CacoEngine::RigidCircle>(
            CacoEngine::Vector2Df(GameConstants::INITIAL_X, GameConstants::INITIAL_Y),
            GameConstants::DEFAULT_CIRCLE_RADIUS
        );
        
        objectManager->addRigidObject(initialCircle);
        objectManager->addRigidCircle(*initialCircle);
    }
    
    void initializeInputCommands() {
        keyCommands[SDLK_LEFT] = std::make_unique<MoveCommand>(
            objectManager->getRigidObjects(),
            CacoEngine::Vector2Df(-GameConstants::MOVEMENT_FORCE, 0)
        );
        
        keyCommands[SDLK_RIGHT] = std::make_unique<MoveCommand>(
            objectManager->getRigidObjects(),
            CacoEngine::Vector2Df(GameConstants::MOVEMENT_FORCE, 0)
        );
        
        keyCommands[SDLK_UP] = std::make_unique<MoveCommand>(
            objectManager->getRigidObjects(),
            CacoEngine::Vector2Df(0, -GameConstants::MOVEMENT_FORCE)
        );
        
        keyCommands[SDLK_DOWN] = std::make_unique<MoveCommand>(
            objectManager->getRigidObjects(),
            CacoEngine::Vector2Df(0, GameConstants::MOVEMENT_FORCE)
        );
        
        keyCommands[SDLK_SPACE] = std::make_unique<JumpCommand>(
            objectManager->getObjects(),
            objectManager->getSelectedIndex()
        );
    }
    
    void handlePhysics() {
        if (objectManager->getRigidObjects().empty()) return;
        
        auto& lastObject = *objectManager->getRigidObjects().back();
        
        // Apply boundary forces
        if (lastObject.Position.X > GameConstants::WORLD_BOUNDARY) {
            lastObject.RigidBody.AddForce(CacoEngine::Vector2Df(-GameConstants::BOUNDARY_FORCE, 0));
        }
    }
    
    void handleCollisions() {
        if (objectManager->getRigidObjects().empty()) return;
        
        CacoEngine::RigidCircleCollider collider(
            objectManager->getRigidCircles(),
            objectManager->getRigidObjects()
        );
        
        collider.Handle();
    }
    
public:
    Application(std::string_view appName, CacoEngine::Vector2Df windowDimensions)
        : Engine(appName, windowDimensions),
          textureManager(std::make_unique<TextureManager>()),
          objectManager(std::make_unique<GameObjectManager>()) {
        tintColor = CacoEngine::Colors[tintIndex];
    }
    
    void OnInitialize() override {
        initializeTextures();
        initializeObjects();
        initializeInputCommands();
        
                 // Set up engine object references
         for (auto& obj : objectManager->getObjects()) {
             this->AddObject(obj);
         }
         for (auto& obj : objectManager->getRigidObjects()) {
             this->AddObject(std::static_pointer_cast<CacoEngine::RigidObject2D>(obj));
         }
    }
    
    void OnUpdate(double frame) override {
        EngineRenderer.SetColor(CacoEngine::Colors[static_cast<int>(CacoEngine::Color::White)]);
        
        // Debug output (should be replaced with proper logging)
        std::cout << "Cursor: (" << CursorPosition.X << ", " << CursorPosition.Y << ")" << std::endl;
        
        handlePhysics();
        handleCollisions();
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        // Create new circle at cursor position
        auto newCircle = CacoEngine::RigidCircle(
            CacoEngine::Vector2Df(CursorPosition.X, CursorPosition.Y),
            GameConstants::DEFAULT_CIRCLE_RADIUS
        );
        
        objectManager->addRigidCircle(newCircle);
        
                 auto rigidObject = std::make_shared<CacoEngine::RigidCircle>(newCircle);
         objectManager->addRigidObject(rigidObject);
         this->AddObject(std::static_pointer_cast<CacoEngine::RigidObject2D>(rigidObject));
        
        objectManager->incrementSelectedIndex();
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        // Handle movement commands
        auto commandIt = keyCommands.find(event.keysym.sym);
        if (commandIt != keyCommands.end()) {
            commandIt->second->execute();
            return;
        }
        
        // Handle special commands
        switch (event.keysym.sym) {
            case SDLK_s:
                createNewRigidCircle();
                break;
                
            case SDLK_w:
                toggleWireframeMode();
                break;
                
            case SDLK_c:
                objectManager->selectNext();
                break;
                
            case SDLK_d:
                objectManager->selectPrevious();
                break;
                
            default:
                break;
        }
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        if (event.y > 0) {
            tintIndex = (tintIndex + 1) % (GameConstants::MAX_COLOR_INDEX + 1);
        } else if (event.y < 0) {
            tintIndex = (tintIndex == 0) ? GameConstants::MAX_COLOR_INDEX : tintIndex - 1;
        }
        
        tintColor = CacoEngine::Colors[tintIndex];
    }
    
private:
    void createNewRigidCircle() {
        auto newCircle = std::make_shared<CacoEngine::RigidCircle>(
            CacoEngine::Vector2Df(0, 0),
            GameConstants::DEFAULT_CIRCLE_RADIUS
        );
        
                 objectManager->addRigidObject(newCircle);
         this->AddObject(std::static_pointer_cast<CacoEngine::RigidObject2D>(newCircle));
        objectManager->incrementSelectedIndex();
    }
    
    void toggleWireframeMode() {
        try {
            auto& selectedObject = objectManager->getSelectedObject();
            selectedObject.FillMode = (selectedObject.FillMode == CacoEngine::RasterizeMode::WireFrame)
                ? CacoEngine::RasterizeMode::Texture
                : CacoEngine::RasterizeMode::WireFrame;
        } catch (const std::exception& e) {
            std::cerr << "Error toggling wireframe mode: " << e.what() << std::endl;
        }
    }
};

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <app_name> <window_width> <window_height>" << std::endl;
        return 1;
    }
    
    try {
        Application app(argv[1], CacoEngine::Vector2Df(std::stoi(argv[2]), std::stoi(argv[3])));
        app.Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}


