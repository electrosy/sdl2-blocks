# Directory for object and dependency files
OBJDIR = ./build
SRCDIR = ./src

# OBJS defines files to compile as part of the project
OBJS = $(OBJDIR)/sdl2-blocks.o $(OBJDIR)/src/gfx/Video.o $(OBJDIR)/src/Input.o $(OBJDIR)/src/Sprite.o $(OBJDIR)/src/Block.o $(OBJDIR)/src/GameModel.o \
       $(OBJDIR)/src/Clock.o $(OBJDIR)/src/gfx/Renderable.o $(OBJDIR)/src/gfx/Renderables.o $(OBJDIR)/src/GameController.o \
       $(OBJDIR)/src/Timer.o $(OBJDIR)/src/Textures.o $(OBJDIR)/src/gfx/Font.o $(OBJDIR)/src/Rand_int.o $(OBJDIR)/src/UI/UIElement.o $(OBJDIR)/src/UI/UIMenu.o \
       $(OBJDIR)/src/HighScores.o $(OBJDIR)/src/Audio.o $(OBJDIR)/src/RectContainer.o $(OBJDIR)/src/ProgressBar.o $(OBJDIR)/src/State/GameState.o \
       $(OBJDIR)/src/State/MenuState.o $(OBJDIR)/src/State/OptionMenuState.o $(OBJDIR)/src/State/HighScoresMenuState.o $(OBJDIR)/src/State/PlayState.o \
       $(OBJDIR)/src/State/PauseState.o $(OBJDIR)/src/State/IntroState.o $(OBJDIR)/src/State/GameStateMachine.o \
       $(OBJDIR)/src/State/GameOverState.o $(OBJDIR)/src/State/CreditsState.o $(OBJDIR)/src/State/KeyboardOptionsState.o $(OBJDIR)/src/State/LanguageOptionsState.o \
       $(OBJDIR)/src/TextEntry.o $(OBJDIR)/src/Fader.o $(OBJDIR)/src/Config.o $(OBJDIR)/src/Board.o $(OBJDIR)/src/LanguageModel.o \
       $(OBJDIR)/src/State/BlockEditorState.o $(OBJDIR)/src/UI/UI_Tile.o $(OBJDIR)/src/Layout.o $(OBJDIR)/src/UI/UIWidget.o

# OBJ_NAME name of executable
OBJ_NAME = sdl2-blocks

# Compiler options
CXX = g++
CXXFLAGS = -std=gnu++1z -g -fPIE -w -MMD -MP
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -pie

# Detect the operating system and append to CXXFLAGS for Windows
ifeq ($(OS),Windows_NT)
    # Append Windows-specific flags
    CXXFLAGS += -DSDL_MAIN_HANDLED
else
    # Optionally, you can add logic for non-Windows (e.g., Linux)
endif

# Define build type (default to minimal if not specified)
BUILD_TYPE ?= minimal

# Marker file to track build type
BUILD_CONFIG = $(OBJDIR)/build_config

# Conditional flags based on BUILD_TYPE
ifneq ($(BUILD_TYPE),minimal)
    CXXFLAGS += -DFULL_ASSETS
else
    #CXXFLAGS += -DMINIMAL_ASSETS
endif

# Directories to create
DIRS = $(OBJDIR) $(OBJDIR)/src $(OBJDIR)/src/State $(OBJDIR)/src/UI $(OBJDIR)/src/gfx

# Default target
all: $(DIRS) $(BUILD_CONFIG) $(OBJ_NAME)

# Create necessary directories only once
$(DIRS):
	mkdir -p $(DIRS)

# Create or update build_config with the current BUILD_TYPE
$(BUILD_CONFIG): $(OBJDIR) FORCE
	@echo "Checking build type: $(BUILD_TYPE)"
	@if [ ! -f $(BUILD_CONFIG) ] || [ "$$(cat $(BUILD_CONFIG))" != "$(BUILD_TYPE)" ]; then \
		echo "$(BUILD_TYPE)" > $(BUILD_CONFIG); \
		echo "Updated $(BUILD_CONFIG) to $(BUILD_TYPE)"; \
	fi

# Linking step
$(OBJ_NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compilation step for each source file, depends on build_config
$(OBJDIR)/%.o: %.cpp $(BUILD_CONFIG) | $(DIRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/src/%.o: src/%.cpp $(BUILD_CONFIG) | $(DIRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include all generated dependency files
DEPS = $(OBJS:.o=.d)
-include $(DEPS)

# Clean target to remove compiled files, executable, dependency files, and build_config
clean:
	rm -rf $(OBJDIR) $(OBJ_NAME)

# Phony targets
.PHONY: all clean FORCE
FORCE: