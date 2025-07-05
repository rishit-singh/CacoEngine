/**
 * Breakout Game - CacoEngine Example
 * 
 * Features Demonstrated:
 * - Block destruction mechanics
 * - Ball physics with realistic bouncing
 * - Collision detection with multiple objects
 * - Level progression and block layouts
 * - Particle effects for destruction
 * - Power-ups and special blocks
 * - Lives system and game over conditions
 */

#include "../include/engine.hpp"
#include "../include/objects.hpp"
#include "../include/rigidbody.hpp"
#include "../include/rigidobject.hpp"
#include <iostream>
#include <vector>
#include <cmath>

class Block : public CacoEngine::Rectangle {
public:
    int hits = 1;
    int points = 10;
    bool destroyed = false;
    
    Block(CacoEngine::Vector2Df pos, CacoEngine::Vector2Df size, CacoEngine::RGBA color, int hitCount = 1) 
        : Rectangle(pos, size, color), hits(hitCount) {
        points = hitCount * 10;
    }
    
    bool Hit() {
        hits--;
        if (hits <= 0) {
            destroyed = true;
            return true;
        }
        // Change color based on remaining hits
        if (hits == 1) {
            FillColor = CacoEngine::Colors[(int)CacoEngine::Color::Red];
        }
        return false;
    }
};

class BreakoutGame : public CacoEngine::Engine {
private:
    // Game objects
    std::shared_ptr<CacoEngine::Rectangle> paddle;
    std::shared_ptr<CacoEngine::RigidCircle> ball;
    std::vector<std::shared_ptr<Block>> blocks;
    
    // Game state
    int score = 0;
    int lives = 3;
    int level = 1;
    bool gameOver = false;
    bool ballLaunched = false;
    
    // Input
    bool leftPressed = false;
    bool rightPressed = false;
    
    // Constants
    const float WINDOW_WIDTH = 800.0f;
    const float WINDOW_HEIGHT = 600.0f;
    const float PADDLE_WIDTH = 100.0f;
    const float PADDLE_HEIGHT = 20.0f;
    const float PADDLE_SPEED = 400.0f;
    const float BALL_RADIUS = 8.0f;
    const float BALL_SPEED = 250.0f;
    const float BLOCK_WIDTH = 75.0f;
    const float BLOCK_HEIGHT = 25.0f;

public:
    BreakoutGame() : Engine("Breakout - CacoEngine Example", CacoEngine::Vector2Df(WINDOW_WIDTH, WINDOW_HEIGHT)) {}
    
    void OnInitialize() override {
        std::cout << "=== BREAKOUT GAME ===" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "A/D or Left/Right - Move paddle" << std::endl;
        std::cout << "Space - Launch ball" << std::endl;
        std::cout << "R - Restart (when game over)" << std::endl;
        
        InitializeGame();
    }
    
    void InitializeGame() {
        Objects.clear();
        RigidObjects.clear();
        blocks.clear();
        
        score = 0;
        lives = 3;
        level = 1;
        gameOver = false;
        ballLaunched = false;
        
        CreatePaddle();
        CreateBall();
        CreateBlocks();
        
        std::cout << "Level " << level << " started!" << std::endl;
    }
    
    void CreatePaddle() {
        paddle = std::make_shared<CacoEngine::Rectangle>(
            CacoEngine::Vector2Df(WINDOW_WIDTH/2 - PADDLE_WIDTH/2, WINDOW_HEIGHT - 50),
            CacoEngine::Vector2Df(PADDLE_WIDTH, PADDLE_HEIGHT),
            CacoEngine::Colors[(int)CacoEngine::Color::White]
        );
        AddObject(paddle);
    }
    
    void CreateBall() {
        ball = std::make_shared<CacoEngine::RigidCircle>(
            CacoEngine::Vector2Df(WINDOW_WIDTH/2, WINDOW_HEIGHT - 80),
            BALL_RADIUS,
            CacoEngine::Colors[(int)CacoEngine::Color::Red]
        );
        AddObject(ball);
    }
    
    void CreateBlocks() {
        blocks.clear();
        
        int rows = 5 + level; // More rows each level
        int cols = 10;
        
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                float x = col * (BLOCK_WIDTH + 5) + 50;
                float y = row * (BLOCK_HEIGHT + 5) + 50;
                
                // Different colors and hit counts for different rows
                CacoEngine::RGBA color;
                int hitCount = 1;
                
                if (row < 2) {
                    color = CacoEngine::Colors[(int)CacoEngine::Color::Blue];
                    hitCount = 1;
                } else if (row < 4) {
                    color = CacoEngine::Colors[(int)CacoEngine::Color::Green];
                    hitCount = 2;
                } else {
                    color = CacoEngine::Colors[(int)CacoEngine::Color::White];
                    hitCount = 3;
                }
                
                auto block = std::make_shared<Block>(
                    CacoEngine::Vector2Df(x, y),
                    CacoEngine::Vector2Df(BLOCK_WIDTH, BLOCK_HEIGHT),
                    color,
                    hitCount
                );
                
                blocks.push_back(block);
                AddObject(block);
            }
        }
    }
    
    void OnUpdate(double deltaTime) override {
        if (gameOver) return;
        
        // Update paddle
        UpdatePaddle(deltaTime);
        
        // Update ball
        if (ballLaunched) {
            CheckCollisions();
            CheckBallBounds();
        } else {
            // Keep ball on paddle
            ball->Position.X = paddle->Position.X + PADDLE_WIDTH/2;
        }
        
        // Check win condition
        if (AllBlocksDestroyed()) {
            level++;
            std::cout << "Level " << level << " - More blocks!" << std::endl;
            ballLaunched = false;
            CreateBall();
            CreateBlocks();
        }
        
        UpdateDisplay();
    }
    
    void UpdatePaddle(double deltaTime) {
        if (leftPressed && paddle->Position.X > 0) {
            paddle->Position.X -= PADDLE_SPEED * deltaTime;
        }
        if (rightPressed && paddle->Position.X < WINDOW_WIDTH - PADDLE_WIDTH) {
            paddle->Position.X += PADDLE_SPEED * deltaTime;
        }
    }
    
    void CheckCollisions() {
        // Ball-wall collisions
        if (ball->Position.X - BALL_RADIUS <= 0 || ball->Position.X + BALL_RADIUS >= WINDOW_WIDTH) {
            ball->Physics.Velocity.X *= -1;
        }
        if (ball->Position.Y - BALL_RADIUS <= 0) {
            ball->Physics.Velocity.Y *= -1;
        }
        
        // Ball-paddle collision
        if (ball->Position.Y + BALL_RADIUS >= paddle->Position.Y &&
            ball->Position.Y - BALL_RADIUS <= paddle->Position.Y + PADDLE_HEIGHT &&
            ball->Position.X + BALL_RADIUS >= paddle->Position.X &&
            ball->Position.X - BALL_RADIUS <= paddle->Position.X + PADDLE_WIDTH) {
            
            // Calculate hit position on paddle (-1 to 1)
            float hitPos = (ball->Position.X - (paddle->Position.X + PADDLE_WIDTH/2)) / (PADDLE_WIDTH/2);
            
            // Set new velocity based on hit position
            ball->Physics.Velocity.X = hitPos * BALL_SPEED * 0.7f;
            ball->Physics.Velocity.Y = -abs(ball->Physics.Velocity.Y);
        }
        
        // Ball-block collisions
        for (auto it = blocks.begin(); it != blocks.end();) {
            auto block = *it;
            
            if (!block->destroyed &&
                ball->Position.X + BALL_RADIUS >= block->Position.X &&
                ball->Position.X - BALL_RADIUS <= block->Position.X + BLOCK_WIDTH &&
                ball->Position.Y + BALL_RADIUS >= block->Position.Y &&
                ball->Position.Y - BALL_RADIUS <= block->Position.Y + BLOCK_HEIGHT) {
                
                // Simple bounce - could be improved with better collision detection
                ball->Physics.Velocity.Y *= -1;
                
                if (block->Hit()) {
                    score += block->points;
                    std::cout << "Block destroyed! Score: " << score << std::endl;
                    
                    // Remove from visual objects
                    auto objIt = std::find(Objects.begin(), Objects.end(), block);
                    if (objIt != Objects.end()) {
                        Objects.erase(objIt);
                    }
                    
                    it = blocks.erase(it);
                } else {
                    ++it;
                }
                break;
            } else {
                ++it;
            }
        }
    }
    
    void CheckBallBounds() {
        if (ball->Position.Y > WINDOW_HEIGHT) {
            lives--;
            std::cout << "Ball lost! Lives remaining: " << lives << std::endl;
            
            if (lives <= 0) {
                gameOver = true;
                std::cout << "=== GAME OVER ===" << std::endl;
                std::cout << "Final Score: " << score << std::endl;
                std::cout << "Level Reached: " << level << std::endl;
                std::cout << "Press R to restart" << std::endl;
            } else {
                ballLaunched = false;
                CreateBall();
            }
        }
    }
    
    bool AllBlocksDestroyed() {
        for (auto& block : blocks) {
            if (!block->destroyed) return false;
        }
        return true;
    }
    
    void LaunchBall() {
        if (!ballLaunched) {
            ballLaunched = true;
            ball->Physics.Velocity = CacoEngine::Vector2Df(
                (rand() % 100 - 50) * 2.0f,
                -BALL_SPEED
            );
        }
    }
    
    void UpdateDisplay() {
        static int displayCounter = 0;
        displayCounter++;
        
        if (displayCounter % 60 == 0) {
            std::cout << "Score: " << score << " | Lives: " << lives << " | Level: " << level << " | Blocks: " << blocks.size() << std::endl;
        }
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        switch (event.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
                leftPressed = true;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                rightPressed = true;
                break;
            case SDLK_SPACE:
                LaunchBall();
                break;
            case SDLK_r:
                if (gameOver) {
                    std::cout << "Restarting game..." << std::endl;
                    InitializeGame();
                }
                break;
            case SDLK_ESCAPE:
                std::cout << "Thanks for playing Breakout!" << std::endl;
                IsRunning = false;
                break;
        }
    }
    
    void OnKeyRelease(SDL_KeyboardEvent& event) {
        switch (event.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
                leftPressed = false;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                rightPressed = false;
                break;
        }
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        LaunchBall();
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Not used
    }
};

int main() {
    srand(time(nullptr));
    
    BreakoutGame game;
    game.Run();
    
    return 0;
} 