##sdl2-blocks
##Copyright (C) 2020 Steven Philley

#OBJS defines files to compile as part of the project
OBJS = sdl2-blocks.cpp Video.cpp Input.cpp Sprite.cpp Block.cpp GameModel.cpp \
		Clock.cpp Renderable.cpp Renderables.cpp GameController.cpp \
		Timer.cpp Textures.cpp Font.cpp Rand_int.cpp UIElement.cpp UIMenu.cpp \
		HighScores.cpp Audio.cpp ./src/RectContainer.cpp ./src/ProgressBar.cpp ./src/State/GameState.cpp ./src/State/MenuState.cpp \
		./src/State/OptionMenuState.cpp ./src/State/HighScoresMenuState.cpp ./src/State/PlayState.cpp \
		./src/State/PauseState.cpp ./src/State/IntroState.cpp ./src/State/GameStateMachine.cpp \
		./src/State/GameOverState.cpp ./src/State/CreditsState.cpp
		

#OBJ_NAME name of executable
OBJ_NAME = sdl2-blocks

#
all : $(OBJS)
	g++ $(OBJS) -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o $(OBJ_NAME)
