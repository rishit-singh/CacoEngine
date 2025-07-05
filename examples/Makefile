# CacoEngine Examples Makefile
# Compiles all example games

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -I../include
LIBS = -lSDL2 -lSDL2_image -lm

# Source files
ENGINE_SOURCES = ../src/engine.cpp ../src/renderer.cpp ../src/objects.cpp ../src/rigidbody.cpp \
                 ../src/rigidobject.cpp ../src/vertex.cpp ../src/texture.cpp ../src/sprite.cpp \
                 ../src/collider.cpp ../src/surface.cpp ../src/tools.cpp ../src/key.cpp \
                 ../src/rigidsprite.cpp ../src/box.cpp

# Example targets
EXAMPLES = pong asteroids snake breakout particle_demo

# Default target
all: $(EXAMPLES)

# Individual example targets
pong: pong.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(ENGINE_SOURCES) $(LIBS)

asteroids: asteroids.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(ENGINE_SOURCES) $(LIBS)

snake: snake.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(ENGINE_SOURCES) $(LIBS)

breakout: breakout.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(ENGINE_SOURCES) $(LIBS)

particle_demo: particle_demo.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(ENGINE_SOURCES) $(LIBS)

# Convenience targets
examples: $(EXAMPLES)
	@echo "All examples compiled successfully!"

# Clean targets
clean:
	rm -f $(EXAMPLES)
	@echo "Cleaned example executables"

clean-examples: clean

# Debug builds
debug: CXXFLAGS += -g -DDEBUG
debug: $(EXAMPLES)

# Test run targets (useful for quick testing)
test-pong: pong
	./pong

test-asteroids: asteroids
	./asteroids

test-snake: snake
	./snake

test-breakout: breakout
	./breakout

test-particle: particle_demo
	./particle_demo

# Help target
help:
	@echo "CacoEngine Examples Makefile"
	@echo "Usage:"
	@echo "  make              - Compile all examples"
	@echo "  make examples     - Compile all examples"
	@echo "  make [game]       - Compile specific game (pong, asteroids, snake, breakout, particle_demo)"
	@echo "  make debug        - Compile with debug flags"
	@echo "  make clean        - Remove compiled executables"
	@echo "  make test-[game]  - Compile and run specific game"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Available games:"
	@echo "  pong         - Classic paddle tennis game"
	@echo "  asteroids    - Space shooter with rotating ship"
	@echo "  snake        - Classic snake game"
	@echo "  breakout     - Block destruction game"
	@echo "  particle_demo - Particle system demonstration"

# Make targets phony
.PHONY: all examples clean clean-examples debug help test-pong test-asteroids test-snake test-breakout test-particle 