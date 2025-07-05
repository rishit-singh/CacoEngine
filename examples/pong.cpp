/**
 * Pong Game - CacoEngine Example
 * 
 * Features Demonstrated:
 * - Basic collision detection
 * - Input handling (keyboard)
 * - Physics simulation (ball movement)
 * - Score tracking and display
 * - Object management (paddles, ball)
 * - Boundary collision detection
 * - Game state management
 */

#include "../include/engine.hpp"
#include "../include/objects.hpp"
#include "../include/rigidbody.hpp"
#include "../include/rigidobject.hpp"
#include "../include/collider.hpp"
#include <iostream>
#include <sstream>
#include <string>

class PongGame : public CacoEngine::Engine {
private:
    // Game objects
    std::shared_ptr<CacoEngine::Rectangle> leftPaddle;
    std::shared_ptr<CacoEngine::Rectangle> rightPaddle;
    std::shared_ptr<CacoEngine::RigidCircle> ball;
    
    // Game state
    int leftScore = 0;
    int rightScore = 0;
    
    // Paddle movement
    bool leftPaddleUp = false;
    bool leftPaddleDown = false;
    bool rightPaddleUp = false;
    bool rightPaddleDown = false;
    
    // Game constants
    const float PADDLE_SPEED = 400.0f;
    const float PADDLE_WIDTH = 20.0f;
    const float PADDLE_HEIGHT = 100.0f;
    const float BALL_RADIUS = 10.0f;
    const float BALL_SPEED = 300.0f;
    const float WINDOW_WIDTH = 800.0f;
    const float WINDOW_HEIGHT = 600.0f;
    
    // Game settings
    const int WIN_SCORE = 5;
    bool gameOver = false;
    std::string winner = "";

public:
    PongGame() : Engine("Pong - CacoEngine Example", CacoEngine::Vector2Df(WINDOW_WIDTH, WINDOW_HEIGHT)) {}
    
    void OnInitialize() override {
        std::cout << "=== PONG GAME ===" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "Player 1 (Left): W/S keys" << std::endl;
        std::cout << "Player 2 (Right): Up/Down arrow keys" << std::endl;
        std::cout << "First to " << WIN_SCORE << " points wins!" << std::endl;
        std::cout << "Press R to restart after game over" << std::endl;
        
        InitializeGame();
    }
    
    void InitializeGame() {
        // Clear existing objects
        Objects.clear();
        RigidObjects.clear();
        
        // Reset game state
        leftScore = 0;
        rightScore = 0;
        gameOver = false;
        winner = "";
        
        // Create left paddle
        leftPaddle = std::make_shared<CacoEngine::Rectangle>(
            CacoEngine::Vector2Df(30, WINDOW_HEIGHT/2 - PADDLE_HEIGHT/2),
            CacoEngine::Vector2Df(PADDLE_WIDTH, PADDLE_HEIGHT),
            CacoEngine::Colors[(int)CacoEngine::Color::White]
        );
        AddObject(leftPaddle);
        
        // Create right paddle
        rightPaddle = std::make_shared<CacoEngine::Rectangle>(
            CacoEngine::Vector2Df(WINDOW_WIDTH - 30 - PADDLE_WIDTH, WINDOW_HEIGHT/2 - PADDLE_HEIGHT/2),
            CacoEngine::Vector2Df(PADDLE_WIDTH, PADDLE_HEIGHT),
            CacoEngine::Colors[(int)CacoEngine::Color::White]
        );
        AddObject(rightPaddle);
        
        // Create ball
        ball = std::make_shared<CacoEngine::RigidCircle>(
            CacoEngine::Vector2Df(WINDOW_WIDTH/2, WINDOW_HEIGHT/2),
            BALL_RADIUS,
            CacoEngine::Colors[(int)CacoEngine::Color::Red]
        );
        
        // Set initial ball velocity
        ResetBall();
        AddObject(ball);
    }
    
    void ResetBall() {
        ball->Position = CacoEngine::Vector2Df(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
        
        // Random direction
        float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;
        float angle = (rand() % 60 - 30) * M_PI / 180.0f; // -30 to 30 degrees
        
        ball->Physics.Velocity = CacoEngine::Vector2Df(
            direction * BALL_SPEED * cos(angle),
            BALL_SPEED * sin(angle)
        );
    }
    
    void OnUpdate(double deltaTime) override {
        if (gameOver) return;
        
        // Update paddle positions based on input
        UpdatePaddles(deltaTime);
        
        // Check ball collisions
        CheckBallCollisions();
        
        // Check scoring
        CheckScoring();
        
        // Update console display
        UpdateDisplay();
    }
    
    void UpdatePaddles(double deltaTime) {
        // Left paddle movement
        if (leftPaddleUp && leftPaddle->Position.Y > 0) {
            leftPaddle->Position.Y -= PADDLE_SPEED * deltaTime;
        }
        if (leftPaddleDown && leftPaddle->Position.Y < WINDOW_HEIGHT - PADDLE_HEIGHT) {
            leftPaddle->Position.Y += PADDLE_SPEED * deltaTime;
        }
        
        // Right paddle movement
        if (rightPaddleUp && rightPaddle->Position.Y > 0) {
            rightPaddle->Position.Y -= PADDLE_SPEED * deltaTime;
        }
        if (rightPaddleDown && rightPaddle->Position.Y < WINDOW_HEIGHT - PADDLE_HEIGHT) {
            rightPaddle->Position.Y += PADDLE_SPEED * deltaTime;
        }
    }
    
    void CheckBallCollisions() {
        // Ball-wall collisions (top and bottom)
        if (ball->Position.Y - BALL_RADIUS <= 0 || ball->Position.Y + BALL_RADIUS >= WINDOW_HEIGHT) {
            ball->Physics.Velocity.Y *= -1;
            
            // Keep ball in bounds
            if (ball->Position.Y - BALL_RADIUS <= 0) {
                ball->Position.Y = BALL_RADIUS;
            }
            if (ball->Position.Y + BALL_RADIUS >= WINDOW_HEIGHT) {
                ball->Position.Y = WINDOW_HEIGHT - BALL_RADIUS;
            }
        }
        
        // Ball-paddle collisions
        CheckPaddleCollision(leftPaddle);
        CheckPaddleCollision(rightPaddle);
    }
    
    void CheckPaddleCollision(std::shared_ptr<CacoEngine::Rectangle> paddle) {
        // Simple AABB collision detection
        float ballLeft = ball->Position.X - BALL_RADIUS;
        float ballRight = ball->Position.X + BALL_RADIUS;
        float ballTop = ball->Position.Y - BALL_RADIUS;
        float ballBottom = ball->Position.Y + BALL_RADIUS;
        
        float paddleLeft = paddle->Position.X;
        float paddleRight = paddle->Position.X + PADDLE_WIDTH;
        float paddleTop = paddle->Position.Y;
        float paddleBottom = paddle->Position.Y + PADDLE_HEIGHT;
        
        // Check collision
        if (ballRight >= paddleLeft && ballLeft <= paddleRight &&
            ballBottom >= paddleTop && ballTop <= paddleBottom) {
            
            // Reverse ball X velocity
            ball->Physics.Velocity.X *= -1;
            
            // Add some variation based on where the ball hits the paddle
            float hitPosition = (ball->Position.Y - (paddle->Position.Y + PADDLE_HEIGHT/2)) / (PADDLE_HEIGHT/2);
            ball->Physics.Velocity.Y += hitPosition * 100.0f;
            
            // Prevent ball from getting stuck inside paddle
            if (ball->Position.X < paddle->Position.X + PADDLE_WIDTH/2) {
                ball->Position.X = paddleLeft - BALL_RADIUS;
            } else {
                ball->Position.X = paddleRight + BALL_RADIUS;
            }
        }
    }
    
    void CheckScoring() {
        // Ball went off left side (right player scores)
        if (ball->Position.X < 0) {
            rightScore++;
            std::cout << "Right player scores! Score: " << leftScore << " - " << rightScore << std::endl;
            
            if (rightScore >= WIN_SCORE) {
                gameOver = true;
                winner = "Right Player";
                std::cout << "=== GAME OVER ===" << std::endl;
                std::cout << "Winner: " << winner << std::endl;
                std::cout << "Press R to restart" << std::endl;
            } else {
                ResetBall();
            }
        }
        
        // Ball went off right side (left player scores)
        if (ball->Position.X > WINDOW_WIDTH) {
            leftScore++;
            std::cout << "Left player scores! Score: " << leftScore << " - " << rightScore << std::endl;
            
            if (leftScore >= WIN_SCORE) {
                gameOver = true;
                winner = "Left Player";
                std::cout << "=== GAME OVER ===" << std::endl;
                std::cout << "Winner: " << winner << std::endl;
                std::cout << "Press R to restart" << std::endl;
            } else {
                ResetBall();
            }
        }
    }
    
    void UpdateDisplay() {
        // This would be where you'd render score text if the engine supported text rendering
        // For now, we use console output
        static int displayCounter = 0;
        displayCounter++;
        
        if (displayCounter % 60 == 0) { // Update every 60 frames (about 1 second)
            std::cout << "Score: " << leftScore << " - " << rightScore << std::endl;
        }
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        switch (event.keysym.sym) {
            // Left paddle controls
            case SDLK_w:
                leftPaddleUp = true;
                break;
            case SDLK_s:
                leftPaddleDown = true;
                break;
                
            // Right paddle controls
            case SDLK_UP:
                rightPaddleUp = true;
                break;
            case SDLK_DOWN:
                rightPaddleDown = true;
                break;
                
            // Restart game
            case SDLK_r:
                if (gameOver) {
                    std::cout << "Restarting game..." << std::endl;
                    InitializeGame();
                }
                break;
                
            // Quit game
            case SDLK_ESCAPE:
                std::cout << "Thanks for playing Pong!" << std::endl;
                IsRunning = false;
                break;
        }
    }
    
    void OnKeyRelease(SDL_KeyboardEvent& event) {
        switch (event.keysym.sym) {
            // Left paddle controls
            case SDLK_w:
                leftPaddleUp = false;
                break;
            case SDLK_s:
                leftPaddleDown = false;
                break;
                
            // Right paddle controls
            case SDLK_UP:
                rightPaddleUp = false;
                break;
            case SDLK_DOWN:
                rightPaddleDown = false;
                break;
        }
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        // Not used in Pong
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Not used in Pong
    }
};

int main() {
    srand(time(nullptr)); // Initialize random seed
    
    PongGame game;
    game.Run();
    
    return 0;
} 