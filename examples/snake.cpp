/**
 * Snake Game - CacoEngine Example
 * 
 * Features Demonstrated:
 * - Grid-based movement system
 * - Dynamic object creation and destruction
 * - Collision detection with self and walls
 * - Game state management and scoring
 * - Timed updates and game loops
 * - Food generation and collection
 * - Growing snake mechanics
 * - High score tracking
 */

#include "../include/engine.hpp"
#include "../include/objects.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <ctime>
#include <algorithm>

class SnakeGame : public CacoEngine::Engine {
private:
    struct GridPosition {
        int x, y;
        GridPosition(int x = 0, int y = 0) : x(x), y(y) {}
        bool operator==(const GridPosition& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    enum Direction {
        UP, DOWN, LEFT, RIGHT
    };
    
    // Game constants
    const int GRID_WIDTH = 25;
    const int GRID_HEIGHT = 20;
    const int CELL_SIZE = 25;
    const float WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
    const float WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;
    const double MOVE_INTERVAL = 0.2; // Move every 0.2 seconds
    
    // Snake data
    std::deque<GridPosition> snake;
    Direction currentDirection = RIGHT;
    Direction nextDirection = RIGHT;
    
    // Food
    GridPosition food;
    bool foodExists = false;
    
    // Game state
    int score = 0;
    int highScore = 0;
    bool gameOver = false;
    bool paused = false;
    
    // Timing
    double lastMoveTime = 0.0;
    
    // Visual objects
    std::vector<std::shared_ptr<CacoEngine::Rectangle>> snakeSegments;
    std::shared_ptr<CacoEngine::Rectangle> foodRect;
    
    // Game settings
    bool speedIncrease = true;
    double currentSpeed = MOVE_INTERVAL;

public:
    SnakeGame() : Engine("Snake - CacoEngine Example", CacoEngine::Vector2Df(WINDOW_WIDTH, WINDOW_HEIGHT)) {}
    
    void OnInitialize() override {
        std::cout << "=== SNAKE GAME ===" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "Arrow Keys / WASD - Move snake" << std::endl;
        std::cout << "Space - Pause/Resume" << std::endl;
        std::cout << "R - Restart (when game over)" << std::endl;
        std::cout << "Escape - Quit" << std::endl;
        std::cout << "Collect food to grow and increase score!" << std::endl;
        
        InitializeGame();
    }
    
    void InitializeGame() {
        // Clear existing objects
        Objects.clear();
        snakeSegments.clear();
        
        // Reset game state
        score = 0;
        gameOver = false;
        paused = false;
        currentSpeed = MOVE_INTERVAL;
        currentDirection = RIGHT;
        nextDirection = RIGHT;
        
        // Initialize snake in center
        snake.clear();
        snake.push_back(GridPosition(GRID_WIDTH / 2, GRID_HEIGHT / 2));
        snake.push_back(GridPosition(GRID_WIDTH / 2 - 1, GRID_HEIGHT / 2));
        snake.push_back(GridPosition(GRID_WIDTH / 2 - 2, GRID_HEIGHT / 2));
        
        // Create visual snake segments
        CreateSnakeVisuals();
        
        // Generate first food
        GenerateFood();
        
        lastMoveTime = GetTime();
        
        std::cout << "New game started! Current high score: " << highScore << std::endl;
    }
    
    void CreateSnakeVisuals() {
        snakeSegments.clear();
        
        for (size_t i = 0; i < snake.size(); i++) {
            auto segment = std::make_shared<CacoEngine::Rectangle>(
                CacoEngine::Vector2Df(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE),
                CacoEngine::Vector2Df(CELL_SIZE - 1, CELL_SIZE - 1), // -1 for grid lines
                i == 0 ? CacoEngine::Colors[(int)CacoEngine::Color::Green] : // Head is green
                         CacoEngine::Colors[(int)CacoEngine::Color::White]   // Body is white
            );
            snakeSegments.push_back(segment);
            AddObject(segment);
        }
    }
    
    void GenerateFood() {
        if (foodExists) {
            // Remove old food
            auto it = std::find(Objects.begin(), Objects.end(), foodRect);
            if (it != Objects.end()) {
                Objects.erase(it);
            }
        }
        
        // Generate food position that's not on snake
        do {
            food.x = rand() % GRID_WIDTH;
            food.y = rand() % GRID_HEIGHT;
        } while (IsPositionOnSnake(food));
        
        // Create food visual
        foodRect = std::make_shared<CacoEngine::Rectangle>(
            CacoEngine::Vector2Df(food.x * CELL_SIZE, food.y * CELL_SIZE),
            CacoEngine::Vector2Df(CELL_SIZE - 1, CELL_SIZE - 1),
            CacoEngine::Colors[(int)CacoEngine::Color::Red]
        );
        AddObject(foodRect);
        
        foodExists = true;
    }
    
    bool IsPositionOnSnake(const GridPosition& pos) {
        for (const auto& segment : snake) {
            if (segment == pos) return true;
        }
        return false;
    }
    
    void OnUpdate(double deltaTime) override {
        if (gameOver || paused) return;
        
        double currentTime = GetTime();
        
        // Move snake at regular intervals
        if (currentTime - lastMoveTime >= currentSpeed) {
            MoveSnake();
            lastMoveTime = currentTime;
        }
        
        // Update display info
        UpdateDisplay();
    }
    
    void MoveSnake() {
        // Update direction
        currentDirection = nextDirection;
        
        // Calculate new head position
        GridPosition newHead = snake.front();
        switch (currentDirection) {
            case UP:    newHead.y--; break;
            case DOWN:  newHead.y++; break;
            case LEFT:  newHead.x--; break;
            case RIGHT: newHead.x++; break;
        }
        
        // Check wall collision
        if (newHead.x < 0 || newHead.x >= GRID_WIDTH || 
            newHead.y < 0 || newHead.y >= GRID_HEIGHT) {
            GameOver();
            return;
        }
        
        // Check self collision
        if (IsPositionOnSnake(newHead)) {
            GameOver();
            return;
        }
        
        // Add new head
        snake.push_front(newHead);
        
        // Check food collision
        if (newHead == food) {
            score += 10;
            std::cout << "Food eaten! Score: " << score << std::endl;
            
            // Increase speed slightly
            if (speedIncrease) {
                currentSpeed = std::max(0.05, currentSpeed - 0.005);
            }
            
            GenerateFood();
        } else {
            // Remove tail if no food eaten
            snake.pop_back();
        }
        
        // Update visuals
        UpdateSnakeVisuals();
    }
    
    void UpdateSnakeVisuals() {
        // Clear old segments
        for (auto& segment : snakeSegments) {
            auto it = std::find(Objects.begin(), Objects.end(), segment);
            if (it != Objects.end()) {
                Objects.erase(it);
            }
        }
        
        // Create new segments
        CreateSnakeVisuals();
    }
    
    void GameOver() {
        gameOver = true;
        
        if (score > highScore) {
            highScore = score;
            std::cout << "NEW HIGH SCORE: " << highScore << "!" << std::endl;
        }
        
        std::cout << "=== GAME OVER ===" << std::endl;
        std::cout << "Final Score: " << score << std::endl;
        std::cout << "Snake Length: " << snake.size() << std::endl;
        std::cout << "High Score: " << highScore << std::endl;
        std::cout << "Press R to restart" << std::endl;
    }
    
    void UpdateDisplay() {
        static int displayCounter = 0;
        displayCounter++;
        
        if (displayCounter % 120 == 0) { // Update every 2 seconds
            std::cout << "Score: " << score << " | Length: " << snake.size() << " | Speed: " << (1.0/currentSpeed) << " moves/sec" << std::endl;
        }
    }
    
    double GetTime() {
        return SDL_GetTicks() / 1000.0;
    }
    
    void OnKeyPress(SDL_KeyboardEvent& event) override {
        if (gameOver) {
            if (event.keysym.sym == SDLK_r) {
                std::cout << "Restarting game..." << std::endl;
                InitializeGame();
            }
            return;
        }
        
        switch (event.keysym.sym) {
            // Movement controls - prevent reverse direction
            case SDLK_UP:
            case SDLK_w:
                if (currentDirection != DOWN) nextDirection = UP;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                if (currentDirection != UP) nextDirection = DOWN;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                if (currentDirection != RIGHT) nextDirection = LEFT;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                if (currentDirection != LEFT) nextDirection = RIGHT;
                break;
                
            // Pause/Resume
            case SDLK_SPACE:
                paused = !paused;
                std::cout << (paused ? "Game Paused" : "Game Resumed") << std::endl;
                break;
                
            // Restart
            case SDLK_r:
                std::cout << "Restarting game..." << std::endl;
                InitializeGame();
                break;
                
            // Quit
            case SDLK_ESCAPE:
                std::cout << "Thanks for playing Snake!" << std::endl;
                std::cout << "Final High Score: " << highScore << std::endl;
                IsRunning = false;
                break;
                
            // Cheat codes for testing
            case SDLK_PLUS:
            case SDLK_EQUALS:
                if (currentSpeed > 0.05) {
                    currentSpeed -= 0.02;
                    std::cout << "Speed increased!" << std::endl;
                }
                break;
            case SDLK_MINUS:
                if (currentSpeed < 0.5) {
                    currentSpeed += 0.02;
                    std::cout << "Speed decreased!" << std::endl;
                }
                break;
        }
    }
    
    void OnKeyRelease(SDL_KeyboardEvent& event) {
        // Not used in Snake
    }
    
    void OnMouseClick(SDL_MouseButtonEvent& event) override {
        // Click to pause/resume
        if (!gameOver) {
            paused = !paused;
            std::cout << (paused ? "Game Paused" : "Game Resumed") << std::endl;
        }
    }
    
    void OnMouseScroll(SDL_MouseWheelEvent& event) override {
        // Not used in Snake
    }
};

int main() {
    srand(time(nullptr));
    
    SnakeGame game;
    game.Run();
    
    return 0;
} 