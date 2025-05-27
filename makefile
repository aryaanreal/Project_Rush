# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Source files
SRCS = main.cpp Player.cpp Enemy.cpp Bullet.cpp BulletManager.cpp EnemyBullet.cpp \
       AudioManager.cpp CollisionManager.cpp PowerUp.cpp Entity.cpp LevelGenerator.cpp \
       UIManager.cpp PlayerUpgradeManager.cpp scoremanager.cpp

# Output binary
TARGET = Project_Rush

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
