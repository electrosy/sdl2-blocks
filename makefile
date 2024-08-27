# OBJS defines files to compile as part of the project
OBJS = sdl2-blocks.o Video.o Input.o Sprite.o Block.o GameModel.o \
       Clock.o Renderable.o Renderables.o GameController.o \
       Timer.o Textures.o Font.o Rand_int.o UIElement.o UIMenu.o \
       HighScores.o Audio.o ./src/RectContainer.o ./src/ProgressBar.o ./src/State/GameState.o \
       ./src/State/MenuState.o ./src/State/OptionMenuState.o ./src/State/HighScoresMenuState.o ./src/State/PlayState.o \
       ./src/State/PauseState.o ./src/State/IntroState.o ./src/State/GameStateMachine.o \
       ./src/State/GameOverState.o ./src/State/CreditsState.o ./src/State/KeyboardOptionsState.o \
       ./src/TextEntry.o ./src/Fader.o ./src/Config.o ./src/Board.o

# OBJ_NAME name of executable
OBJ_NAME = sdl2-blocks

# Compiler options
CXX = g++
CXXFLAGS = -std=gnu++1z -g -fPIE -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LDFLAGS = -pie

# Default target
all: $(OBJ_NAME)

# Linking step
$(OBJ_NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CXXFLAGS)

# Compilation step for each source file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove compiled files
clean:
	rm -f $(OBJS) $(OBJ_NAME)

