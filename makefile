# Directory for object and dependency files
OBJDIR = ./build
SRCDIR = ./src

# OBJS defines files to compile as part of the project
OBJS = $(OBJDIR)/sdl2-blocks.o $(OBJDIR)/Video.o $(OBJDIR)/Input.o $(OBJDIR)/Sprite.o $(OBJDIR)/Block.o $(OBJDIR)/GameModel.o \
       $(OBJDIR)/Clock.o $(OBJDIR)/Renderable.o $(OBJDIR)/Renderables.o $(OBJDIR)/GameController.o \
       $(OBJDIR)/Timer.o $(OBJDIR)/Textures.o $(OBJDIR)/Font.o $(OBJDIR)/Rand_int.o $(OBJDIR)/UIElement.o $(OBJDIR)/UIMenu.o \
       $(OBJDIR)/HighScores.o $(OBJDIR)/Audio.o $(OBJDIR)/src/RectContainer.o $(OBJDIR)/src/ProgressBar.o $(OBJDIR)/src/State/GameState.o \
       $(OBJDIR)/src/State/MenuState.o $(OBJDIR)/src/State/OptionMenuState.o $(OBJDIR)/src/State/HighScoresMenuState.o $(OBJDIR)/src/State/PlayState.o \
       $(OBJDIR)/src/State/PauseState.o $(OBJDIR)/src/State/IntroState.o $(OBJDIR)/src/State/GameStateMachine.o \
       $(OBJDIR)/src/State/GameOverState.o $(OBJDIR)/src/State/CreditsState.o $(OBJDIR)/src/State/KeyboardOptionsState.o \
       $(OBJDIR)/src/TextEntry.o $(OBJDIR)/src/Fader.o $(OBJDIR)/src/Config.o $(OBJDIR)/src/Board.o

# OBJ_NAME name of executable
OBJ_NAME = sdl2-blocks

# Compiler options
CXX = g++
CXXFLAGS = -std=gnu++1z -g -fPIE -w -MMD -MP
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -pie

# Directories to create
DIRS = $(OBJDIR) $(OBJDIR)/src $(OBJDIR)/src/State

# Default target
all: $(DIRS) $(OBJ_NAME)

# Create necessary directories only once
$(DIRS):
	mkdir -p $(DIRS)

# Linking step
$(OBJ_NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compilation step for each source file, generates object and dependency files
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# This pattern will generate the `.d` files alongside `.o` files
$(OBJDIR)/%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MP -MT '$(@:.d=.o)' -MF $@ $<

$(OBJDIR)/src/%.d: src/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MP -MT '$(@:.d=.o)' -MF $@ $<

# Include all generated dependency files
DEPS = $(OBJS:.o=.d)
-include $(DEPS)

# Clean target to remove compiled files, executable, and dependency files
clean:
	rm -rf $(OBJDIR) $(OBJ_NAME)